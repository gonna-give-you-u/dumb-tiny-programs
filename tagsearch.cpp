#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <string>

using namespace std;

int main(int argc, char* argv[]){ // 1 giant main function veri good programming 12/10
    int ao = 0; //arg_offset
    bool isExclude = false;
    bool fullLine = false;
    int matchMode = 1; // 0=and; 1=or
    string theText;
    vector<string> tagsToFind;
    
    if (!argv[1]){
        cerr << "usage: " << string(argv[0]) << " [-x|-f] [matchMode] TAGFILE TAGS..." << endl
        << "tag format example: /path/to/file.bin::foo,bar" << endl
        << "each file on its own line" << endl
        << "ONLY one instance of a tag per path otherwise breakage (incorrect matching), i.e. no repeated tags (e.g. the line '/path/example::foo,bar,foo' is bad)" << endl
        << "-x: find all files WITHOUT the tag instead" << endl
        << "-f: return entire line in TAGFILE instead of just the path" << endl
        << "this binary searches for files with EITHER tag, i haven't been able to figure out how to make an 'and' match mode, but you can emulate with -f and piping it into another tagsearch command using - as the file" << endl
        << "MATCHING:" << endl
        << "\t-a (--and): find/exclude files that have all specified tags (default)" << endl // finally
        << "\t-o (--or): find/exclude files with any tag specified" << endl
        << "currently -x and other opts must be separate and in this order" << endl
        << "also hint: if using a $() in bash, set `IFS=$'\\n';` before the 'for' to make spaces and special chars not break the $()" << endl
        ;
        return 1;
    }

    if (string(argv[1]) == "-f"){
        ao++;
        fullLine = true;
        if (!argv[2]){
            cerr << "put something after the option!!!!1" << endl;
            return 1;
        }
    }
    if (string(argv[1+ao]) == "-x"){
        ao++;
        isExclude = true;
        if (!argv[1+ao]){
            cerr << "put something after the option!!!!1" << endl;
            return 1;
        }
    }
    if ((string(argv[1+ao]) == "-fx" || string(argv[1+ao]) == "-xf") && !(fullLine || isExclude)){ //Very crappy options implementation ik
        ao++;
        isExclude = true;
        fullLine = true;
        if (!argv[1+ao]){
            cerr << "put something after the option!!!!1" << endl;
            return 1;
        }
    }
    
    if (string(argv[1+ao]) == "-o" || string(argv[1+ao]) == "--or"){
        ao++;
        matchMode = 1;
        if (!argv[1+ao]){
            cerr << "put something after the option!!!!1" << endl;
            return 1;
        }
    } else
    if (string(argv[1+ao]) == "-a" || string(argv[1+ao]) == "--and"){ //works now (but you can still use pipes and the stdin file "-" to emulate this)
        ao++;
        matchMode = 0;
        if (!argv[1+ao]){
            cerr << "put something after the option!!!!1" << endl;
            return 1;
        }
    }
    for (int i=2+ao;i<argc; i++){
        tagsToFind.push_back(string(argv[i]));
    }
    
    string filename = string(argv[1+ao]);

    istream *theInputStream;
    ifstream theFile;
    if (filename == "-")
        theInputStream = &cin;
    else{
        theFile.open(filename);
        theInputStream = &theFile;
    }

    if(!*theInputStream){
        cerr << "uh oh cant open the file: " << filename << endl
		<< "is the file normal, does it exist, and do you have permission to read it" << endl
        ;
        return 1;
    }

	{
	    vector<string> alreadyChecked;
	    int found;
	    string currentLine;
	    string currentTagLine;
	    string currentPath;
	    string tagToPush;
	    int matchedTags = 0;
	    bool shouldBreakOut = false;

	    while (getline(*theInputStream,currentLine)){

	        if (currentLine.substr(0,2) == "##") continue; //skip comments
	        currentPath = currentLine.substr(0,currentLine.find("::"));
	        currentTagLine = currentLine.substr(currentLine.find("::")+2);
	        stringstream tagsss(currentTagLine);
	        vector<string> currentTags;
	        while (getline(tagsss,tagToPush,',')){
	            currentTags.push_back(tagToPush);
	        }

	        matchedTags = 0;

	        shouldBreakOut = false;
	        for (string funniString : currentTags){
	            if (shouldBreakOut) break;
	            for (string s : tagsToFind){ // basically for each tag it searches for it, in OR mode if nothing found it moves onto the next tag
	                found = count(currentTags.begin(), currentTags.end(), s);
	                
	                if (matchMode == 1){ // matchmode is 'or'
	                    if ((found > 0 && !isExclude) || (found <= 0 && isExclude)){
	                        if (fullLine) cout << currentLine << endl;
	                        else cout << currentPath << endl;
	                        shouldBreakOut = true; // once 1 tag matches, break out of 2 loops to avoid needless continuing
	                        break;
	                    }
	                }
	                else if (matchMode == 0){ // matchmode is 'and' (but not 'exclude')
	                    if (!isExclude){
	                        if (found > 0){
	                            if (matchedTags >= tagsToFind.size()){
	                                if (fullLine) cout << currentLine << endl;
	                                else cout << currentPath << endl;
	                                shouldBreakOut = true;
	                                break;
	                            } else{
	                                matchedTags++;
	                                continue;
	                            }
	                        } else{
	                            shouldBreakOut = true;
	                            break;
	                        }
	                    } else{
	                        if (found > 0){
	                            matchedTags++;
	                        }
	                    }
	                }
	            }
	            if (matchMode == 0 && isExclude && matchedTags < tagsToFind.size()){ // matchmode is 'and' and 'exclude'
	                if (fullLine) cout << currentLine << endl;
	                else cout << currentPath << endl;
	            }
	        }
	    }
	}

    if (theFile)
        theFile.close();
    
    return 0;
}
