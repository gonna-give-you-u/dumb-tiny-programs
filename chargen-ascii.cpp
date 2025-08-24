#include <iostream>
#include <cstdlib>

using namespace std;

int main(int argc, char* argv[]){

	setlocale(LC_ALL, "");

	if (argv[1] == NULL || argv[2] == NULL){
		cout << "chargen START END [CHARS] [SEED]\nSTART: decimal value of start of range codepoint\n  END: decimal value of end of range codepoint\nCHARS: amount of chars to create\n SEED: seed to use for the random generator";
		return 1;
	}
	int startRange = atoi(argv[1]);
	int endRange = atoi(argv[2]);
	int charCount;
	if (argv[3] != NULL){
		charCount = atoi(argv[3]);
	} else{
		charCount = 1;
	}
	if (endRange < startRange){
		cout << "END must be greater than START\n";
		return 1;
	}
	if (argv[4] != NULL){
		srand(atoi(argv[4]));
	}
	else{
		srand(time(NULL));
	}
	
	int theCodepoint;
	char theChar;
	
	// Actually do stuff
	for (int i=0;i<charCount;i++){
		theCodepoint = rand()%(endRange-startRange+1)+startRange;
		theChar = theCodepoint;
		cout << theChar;
	}
	cout << "\n";

	
	return 0;
}
