#include <iostream>
#include <vector>

using namespace std;

int main(int argc, char* argv[]){
    string stuff;
    int modeNo;

    try{
        if (string(argv[1]) == "-r"){
            modeNo=0;
        } else if (string(argv[1]) == "-a"){
            modeNo=1;
        }
    } catch(logic_error){
        cout << "i think you forgot to put something there?\n";
        cout << "usage: spongebob-case options text\noptions:\n    -r : random\n    -a : alternating\n";
        exit(1);
    }
    if (string(argv[1]) != "-r" && string(argv[1]) != "-a"){
            cout << "usage: spongebob-case options text\noptions:\n    -r : random\n    -a : alternating\n";
            exit(1);
    }
    cout << "put your text here: ";
    getline(cin,stuff);
    char stuffArray[stuff.length()+1];
    auto daFirst = stuff.begin();
    auto daLast = stuff.end();
    copy(daFirst, daLast, stuffArray);
    stuffArray[stuff.length()]='\0';
    vector<char> outStuff;
    char newChar;
    srand(time(NULL));//random generators are weird

    //does not work with nonlatin chars
    //TODO fix that somehow idk
    if (modeNo == 0){
        for (int i=0;i<stuff.length();i++){
            if (rand()%2 == 0){
                newChar = toupper(stuffArray[i]);
            } else {
                newChar = tolower(stuffArray[i]);
            }
            outStuff.push_back(newChar);
        }
    } else if (modeNo == 1){//non-letters are not skipped and i cant be asked to fix that atm
        for (int i=0;i<stuff.length();i++){
            if (i%2==0){
                newChar = toupper(stuffArray[i]);
            } else{
                newChar = tolower(stuffArray[i]);
            }
            outStuff.push_back(newChar);
        }
    } else{
        cout << "idk something went wrong (modeNo isn't 0 or 1)";
    }
    cout << "\n";
    for (int i=0;i<outStuff.size();i++){
        cout << outStuff[i];
    }
    cout << '\n';

    return 0;
}
