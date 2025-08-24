#include <iostream>

int main(int argc, char* argv[]){
	std::string input;
	for (int i=1; i<argc; i++){
		input += std::string(argv[i]);
		if (i < argc-1){
			input += " ";
		}
	}
	if (argc > 1){
		std::cout << "Yes, '"+input+ "' rhymes with Grug.\n";
	} else{
		std::cout <<"Yes, that rhymes with Grug.\n";
	}

	return 0;
}
