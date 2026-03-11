#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>

using namespace std;

int main(int argc, char* argv[]){
    int ao = 0; //arg_offset
    bool isExclude = false;
    bool fullLine = false;
    int matchMode = 1; // 0=and; 1=or
    string theText;
    vector<string> theLines;
    vector<string> tagsToFind;
    vector<string> emptyStringVector;
    
    if (!argv[1]){
        cerr << "usage: " << string(argv[0]) << " [-x|-f] TAGFILE TAGS..." << endl
        << "tag format example: /path/to/file.bin::foo,bar" << endl
        << "each file on its own line" << endl
        << "ONLY one instance of a tag per path otherwise breakage (incorrect matching), i.e. no repeated tags (e.g. the line '/path/example::foo,bar,foo' is bad)" << endl
        << "-x: find all files WITHOUT the tag instead" << endl
        << "-f: return entire line in TAGFILE instead of just the path" << endl
        << "this binary searches for files with EITHER tag, i haven't been able to figure out how to make an 'and' match mode, but you can emulate with -f and piping it into another tagsearch command using - as the file" << endl
        //<< "MATCHING:" << endl
        //<< "\t-a (--and): find/exclude files that have all specified tags (default)" << endl // implementation too hard
        //<< "\t-o (--or): find/exclude files with any tag specified" << endl
        //<< "searching for multiple tags not yet (directly) supported by this binary" << endl
        //<< "currently -x and other opts must be separate and in this order" << endl
        ;
        return 1;
    }
    
    bool debug = false;
    if (string(argv[1]) == "--debug"){ // increases exec time
        ao++;
        debug = true;
    }
    if (string(argv[1+ao]) == "-f"){
        ao++;
        fullLine = true;
        if (!argv[2+ao]){
            cerr << "put something after the option!!!!1" << endl;
            return 1;
        }
    }
    if (string(argv[1+ao]) == "-x"){
        ao++;
        isExclude = true;
        if (!argv[2+ao]){
            cerr << "put something after the option!!!!1" << endl;
            return 1;
        }
    }
    if ((string(argv[1+ao]) == "-fx" || string(argv[1+ao]) == "-xf") && (!fullLine && !isExclude)){ //Very crappy implementation ik
        ao++;
        isExclude = true;
        fullLine = true;
        if (!argv[2+ao]){
            cerr << "put something after the option!!!!1" << endl;
            return 1;
        }
    }
    
    if (string(argv[1+ao]) == "-o" || string(argv[1+ao]) == "--or"){
        ao++;
        matchMode = 1;
    }/* else
    if (string(argv[1+ao]) == "-a" || string(argv[1+ao]) == "--and"){ //currently doesn't work (you can use pipes and the stdin file "-" to emulate this)
        ao++;
    }*/ 
    for (int i=2+ao;i<argc; i++){
        tagsToFind.push_back(string(argv[i]));
    }
    
    string filename = string(argv[1+ao]);
    if (filename == "-"){ // stdin pipe
        while (getline(cin,theText)){
            if (theText.substr(0,2) != "##"){ // use ## to make a single-line comment in tagfiles
                theLines.push_back(theText);
            }
        }
    } else{ // not stdin pipe
        ifstream theFile(filename);
        while (getline (theFile, theText)){
            if (theText.substr(0,2) != "##"){ // use ## to make a single-line comment in tagfiles
                theLines.push_back(theText);
            }
        }
        theFile.close();
    }
    
    vector<string> thePaths;
    vector<vector<string>> theTags;
    vector<string> tags1d;
    for (int i=0;i<theLines.size();i++){
        thePaths.push_back(theLines[i].substr(0,theLines[i].find("::")));
    }
    for (int i=0;i<theLines.size();i++){
        tags1d.push_back(theLines[i].substr(theLines[i].find("::")+2));
    }
    string tagToPush;
    for (int i=0;i<theLines.size();i++){
        vector<string> tagsToAdd;
        stringstream tagsss(tags1d[i]);
        while (getline(tagsss,tagToPush,',')){
            tagsToAdd.push_back(tagToPush);
        }
        theTags.push_back(tagsToAdd);
    }
    /*
    for (int i=0;i<thePaths.size();i++){
        for (int j=0;j<theTags[i].size();j++){
            if (string(argv[2+ao]) == theTags[i][j] && !isExclude){ // not exclude
                cout << thePaths[i] << endl;
            } else if (string(argv[2+ao]) == theTags[i][j] && isExclude){ // exclude
                thePaths.erase(thePaths.begin()+i);
                theTags.erase(theTags.begin()+i);
            }
        }
    }*/
    vector<string> alreadyChecked;
    int found;
    int isPathChecked;
    for (int i=0;i<thePaths.size();i++){
        for (int j=0;j<theTags[i].size();j++){
            if (matchMode == 1){ // matchmode is 'or'
                for (string s : tagsToFind){
                    found = count(theTags[i].begin(), theTags[i].end(), s);
                    isPathChecked = count(alreadyChecked.begin(), alreadyChecked.end(), thePaths[i]);
                    if (found > 0 && isPathChecked == 0 && !isExclude){
                        if (fullLine) cout << theLines[i] << endl;
                        else cout << thePaths[i] << endl;
                        alreadyChecked.push_back(thePaths[i]);
                    } else if (found > 0 && isPathChecked == 0 && isExclude){
                        if (debug)
                            cout << "\033[;37mDEBUG: setting to empty string: thePaths[i]: index " << i << ": " << thePaths[i] << "\033[0m" << endl;
                        
                        thePaths[i] = "";
                        
                        if (debug){
                            cout << "\033[;37mDEBUG: setting to empty string vector: theTags[i] index " << i << ": ";
                            for (string s : theTags[i]){
                                cout << s << ",";
                            }
                            cout << "\033[0m" << endl;
                        }
                        theTags[i] = emptyStringVector;
                        
                        if (fullLine) {
                            if (debug) cout << "\033[;37mDEBUG: setting to empty string: theLines[i]: index " << i << "\033[0m" << endl;
                            theLines[i] = "";
                        }
                    }
                }
            }

        }
    }
    
    if (isExclude){ // Fixed an annoying bug that was once here
        for (int i=0;i<thePaths.size();i++){
            if (fullLine && theLines[i] != "") cout << theLines[i] << endl;
            else if (thePaths[i] != "" && theTags[i] != emptyStringVector) cout << thePaths[i] << endl;
        }
    }
    
    return 0;
}
