#include <iostream>
#include <cstdlib>
#include <sys/random.h>
#include <uchar.h>

using namespace std;

int main(int argc, char* argv[]){

	setlocale(LC_ALL, "");

	if (argv[1] == NULL || argv[2] == NULL){
		cout << "chargen START END [CHARS]" << endl << "START: hex value of start of range codepoint" << endl << "  END: hex value of end of range codepoint " << endl << "CHARS: amount of chars to create" << endl;
		return 1;
	}
	int startRange = stoi(argv[1],0,16);
	int endRange = stoi(argv[2],0,16);
	int charCount;
	if (argv[3] != NULL){
		charCount = stoi(argv[3]);
	} else{
		charCount = 1;
	}
	if (endRange < startRange){
		cout << "END must be greater than START" << endl;
		return 1;
	}
	
	int theCodepoint;
	wchar_t theChar;
	unsigned int codepointRandomness;
	
	// Actually do stuff
	for (int i=0;i<charCount;i++){
		getrandom(&codepointRandomness, sizeof(unsigned int), GRND_NONBLOCK);
		theCodepoint = codepointRandomness%(endRange-startRange+1)+startRange;
		theChar = theCodepoint;
		wcout << theChar;
	}
	cout << endl;

	
	return 0;
}
