#include <fstream>
#include <iostream>
#include <sys/random.h>

int main(int argc, char* argv[]){
	std::string helptext = "-r (random) or -i (incrementing) or -c (charcount; but more like bytecount)\nuse -m to set max volume \033[;0;41m(WARNING: VALUES ABOVE 100% GET VERY LOUD VERY QUICK!!!!!)\033[0m\nBtw this requires pactl at the moment\n"; //contains final newline, don't use std::endl after it (unless you actually want 2 newlines); <--mandatory semicolon in comment
	unsigned int vol;
	unsigned int volMax = 100;
	int argv_offset = 0;
	if (argv[1] == NULL){
		std::cerr << helptext;
		return 1;
	}
	if (std::string(argv[1]) == "--max" || std::string(argv[1]) == "-m"){//max volume
		argv_offset = 2;
		if (argv[2] == NULL){
			std::cerr << "\033[;31mput an int after '"+std::string(argv[1])+"'!!!!1\033[0m" << std::endl;
			return 1;
		}
		try{
			volMax = atoi(argv[2]);
		} catch(...){
			std::cerr << "\033[;31mare you sure there's a valid int after '" + std::string(argv[1]) + "'?\033[0m" << std::endl;
			return 1;
		}
		if (atoi(argv[2]) < 0){
			std::cerr << "\033[;31mmax volume can not be a negative number (it's an unsigned int and causes overflows to ear-shattering levels so i blocked it, use a high positive int instead if you want to destroy your ears)\033[0m" << std::endl;
			return 1;
		}
		if (argv[3] == NULL){
			std::cerr << "\033[;31mput something after the number!!!\033[0m" << std::endl << std::endl;
			std::cout << helptext;
			return 1;
		}
	}
	if (std::string(argv[1+argv_offset]) == "-i" || std::string(argv[1+argv_offset]) == "--incrementing"){//incrementing
		std::system("pactl get-sink-volume @DEFAULT_SINK@ |awk 'NR==1{print $5}' |head -c -2 >/tmp/pwvolumeThing; echo >> /tmp/pwvolumeThing");
		std::ifstream volFileI;
		volFileI.open("/tmp/pwvolumeThing");
		if(volFileI.is_open()){
			std::string line;
			while(getline(volFileI, line)){
				vol = std::stoi(line);
			}
		} else {
			std::cout << "\033[;33mthe /tmp/pwvolumeThing file is ohio\033[0m" << std::endl;
		}
		volFileI.close();
		if (vol < volMax){
			vol++;
		} else if (vol == volMax){
			vol = 0;
		} else {
			std::cerr << "\033[;31mwhy is volume = " + std::to_string(vol)+"?\033[0m" << std::endl;
			return 1;
		}
	} else if (std::string(argv[1+argv_offset]) == "-r" || std::string(argv[1+argv_offset]) == "--random"){//random
		unsigned int randomThing;
		getrandom(&randomThing, sizeof(unsigned int), GRND_NONBLOCK);
		vol = randomThing%(volMax+1);
	} else if (std::string(argv[1+argv_offset]) == "--charcount" || std::string(argv[1+argv_offset]) == "-c" || std::string(argv[1+argv_offset]) == "--bytecount"){
		std::string charstring = std::string(argv[2+argv_offset]);
		vol = charstring.length();
	} else{
		std::cerr << "\033[;31munknown option: " << std::string(argv[1+argv_offset]) << "\033[0m" << std::endl << std::endl;
		std::cout << helptext;
		return 1;
	}
	if (vol > volMax){
		vol = volMax;
	}
	system(("pactl set-sink-volume @DEFAULT_SINK@ "+std::to_string(vol)+"%").c_str());
	if (vol > 100){
		std::cout << "The volume is now \033[;0;41m" + std::to_string(vol) +"%!!!\033[0m" << std::endl;
	} else{
		std::cout << "The volume is now " + std::to_string(vol) +"%" << std::endl;
	}

	return 0;
}
