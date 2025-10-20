#include <iostream>
#include <vector>
#include <algorithm>
#include <signal.h>
#include <cstring>
#include <termios.h> // rip windows users
#include <unistd.h>
#include <codecvt>
#include <locale>

using namespace std;//Yes ik this isnt considered ideal practice but i'm not really using external libs


wstring_convert<std::codecvt_utf8<char16_t>, char16_t> converter;
u16string theWord;
string theWord8;

void revealOnSigint(int thingy){
    cout << "sigint received, quitting" << endl << "the word was: " << converter.to_bytes(theWord) << endl;
    exit(thingy);
}

int main(int argc, char* argv[]){
    if (argc > 3) clog << "ignoring excess arguments" << endl;
    if (argv[1]) if (string(argv[1]) == "--help" || string(argv[1]) == "-h"){
        cerr << "usage: " << string(argv[0]) << " [WORD] [LIVES]" << endl
        << "[WORD] (string): word or phraseg" << endl
        << "[LIVES] (unsigned int): number of lives, 0 for infinite" << endl
        << "multiple letter guesses are interpreted as whole word guesses" << endl;
        return 1;
    }
    
    // set up word
    if (argv[1]) {
    	theWord8 = argv[1];
    	theWord = converter.from_bytes(theWord8);
    }
    else{
        cout << "whats the word (won't echo)" << endl;

        termios normalTerminal;
        tcgetattr(STDIN_FILENO, &normalTerminal);
        termios hiddenTerminal = normalTerminal;
        hiddenTerminal.c_lflag &= ~ECHO;
        tcsetattr(STDIN_FILENO, TCSANOW, &hiddenTerminal);

        getline(cin,theWord8);
    	theWord = converter.from_bytes(theWord8);

        tcsetattr(STDIN_FILENO, TCSANOW, &normalTerminal);
    }
    vector<char16_t> wordv_char(theWord.begin(),theWord.end());
    vector<u16string> wordv;
    for (char16_t c : wordv_char){
        u16string wordvcharToPush(1,c);
        wordv.push_back(wordvcharToPush);
    }
    unsigned int letters;
    letters = theWord.length();
    for (u16string s : wordv)
        if (s == u" ")
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
    }
    if (lives == 0){
        invincible = true;
        lives = 4294967295;//just in case
    }
    if (invincible) cout << "invincible mode" << endl;
    else cout << lives << " lives" << endl;

    // set up vectors for guesses
    vector<u16string> guessedv;
    vector<u16string> triedStuff;
    for (u16string s : wordv){
        if (s == u" ") guessedv.push_back(u" ");
        else guessedv.push_back(u"_");
    }

    cout << "start guessing" << endl;
    unsigned int guessNo = 0;
    bool correctGuess;
    bool wholeWordCorrectGuess;
    bool alreadyGuessed;
    
        for (u16string s : guessedv) cout << converter.to_bytes(s);
        cout << endl;

    u16string guess;
    string guess8;
    while (guessedv != wordv || lives > 0){ // start da gaem
        correctGuess = false;
        alreadyGuessed = false;
        cout << "> ";
        getline(cin, guess8);
        guess = converter.from_bytes(guess8);
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
                cerr << "skill issue" << endl << "the word was: " << theWord8 << endl;
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
        for (u16string s : guessedv) cout << converter.to_bytes(s);
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
