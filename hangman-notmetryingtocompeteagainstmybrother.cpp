#include <iostream>
#include <vector>
#include <algorithm>
#include <signal.h>
#include <cstring>

using namespace std;

string theWord;
void revealOnSigint(int thingy){
    cout << "sigint received, quitting" << endl << "the word was: " << theWord << endl;
    exit(thingy);
}

int main(int argc, char* argv[]){
    if (argc > 3) clog << "ignoring excess arguments" << endl;
    if (argv[1]) if (string(argv[1]) == "--help" || string(argv[1]) == "-h"){
        cerr << "usage: " << string(argv[0]) << " [WORD] [LIVES]" << endl
        << "[WORD] (string): ASCII word or phrase, non-ASCII not yet supported because splitting it is weird and i gave up trying" << endl
        << "[LIVES] (unsigned int): number of lives, 0 for infinite" << endl
        << "multiple letter guesses are interpreted as whole word guesses" << endl;
        return 1;
    }
    
    // set up word
    if (argv[1]) theWord = argv[1];
    else{
        cout << "whats the word" << endl;
        getline(cin,theWord);
        // clear the word from the terminal
        cout << "\x1b[1A\x1b[2K\x1b[1A\x1b[2K";
    }
    vector<char> wordv_char(theWord.begin(),theWord.end());
    vector<string> wordv;
    for (char c : wordv_char){
        string wordvcharToPush(1,c);
        wordv.push_back(wordvcharToPush);
    }
    unsigned int letters;
    letters = theWord.length();
    for (string s : wordv)
        if (s == " ")
            letters--;
    cout << letters << " letters" << endl;
    signal(SIGINT, revealOnSigint);

    // set up lives
    unsigned int lives;
    bool invincible = false;
    if (argv[1] && argv[2]){
        try { lives = stoi(argv[2]); }
        catch (...){
            cerr << "PUT A PROPER LIVES NUMBER (1st arg) NOT WHATEVER THAT IS" << endl;
            return 1;
        }
    } else{
        cout << "how many lives" << endl;
        string livesS;
        getline(cin,livesS);
        try { lives = stoi(livesS); }
        catch (...){
            cerr << "PUT A PROPER LIVES NUMBER (1st arg) NOT WHATEVER THAT IS" << endl;
            return 1;
        }
        // clear the question from the terminal
        cout << "\x1b[1A\x1b[2K\x1b[1A\x1b[2K";
    }
    if (lives == 0){
        invincible = true;
        lives = 4294967295;//just in case
    }
    if (lives == 0) cout << "invincible mode" << endl;
    else cout << lives << " lives" << endl;

    // set up vectors for guesses
    vector<string> guessedv;
    vector<string> triedStuff;
    for (string s : wordv){
        if (s == " ") guessedv.push_back(" ");
        else guessedv.push_back("_");
    }

    cout << "start guessing" << endl;
    unsigned int guessNo = 0;
    bool correctGuess;
    bool wholeWordCorrectGuess;
    bool alreadyGuessed;
    string guessPrompt;
    
        for (string s : guessedv) cout << s;
        cout << endl;
    
    if (getenv("PS2") != NULL) guessPrompt = getenv("PS2"); //it seems that bash doesn't normally 'export' this one
    else guessPrompt = "> ";

    while (guessedv != wordv || lives > 0){
        correctGuess = false;
        alreadyGuessed = false;
        string guess;
        cout << guessPrompt;
        getline(cin,guess);
        if (count(triedStuff.begin(),triedStuff.end(),guess) > 0){
            alreadyGuessed = true;
            cerr << "you already guessed that" << endl;
        } else guessNo++;
        if (guess == theWord){
            correctGuess = true;
            wholeWordCorrectGuess = true;
            break;
        } else if (guess.length() == 1 && !alreadyGuessed){
            for (long unsigned int i=0;i<wordv.size();i++){
                if (guess == wordv[i]){
                    correctGuess = true;
                    cout << "yes" << endl;
                    guessedv[i] = guess;
                    triedStuff.push_back(guess);
                }
            }
        }
        if ((!correctGuess && !alreadyGuessed) && guess.length() > 0){
            if (!invincible) lives--;
            if (lives <= 0){
                cerr << "skill issue" << endl << "the word was: " << theWord << endl;
                return 2;
            }
            triedStuff.push_back(guess);
            cout << "nuh uh" << endl;
            if (!invincible){
                cout << "you have " << lives;
                if (lives == 1) cout << " life";
                else cout << " lives";
                cout << " left" << endl;
            }
        }
        for (string s : guessedv) cout << s;
        cout << endl;
        if (guessedv == wordv) break;
    }

    if (guessedv == wordv || wholeWordCorrectGuess){
        cout << "cratulations that took you " << guessNo;
        if (guessNo == 1) cout << " guess";
        else cout << " guesses";
        if (!invincible){
            cout << " with " << lives;
            if (lives == 1) cout << " life";
            else cout << " lives";
            cout << " remaining";
        }

        cout << endl;
    }

    return 0;
}
