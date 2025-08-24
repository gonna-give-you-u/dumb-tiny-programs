#include <iostream>
#include <math.h>

void convert(short direction, float h, float m, float s, float ms){
	float factor;
	float tinms;
	float toutms;
	
	if (direction == 0){ // sexagesimal to decimal
		tinms = 3600000*h + 60000*m + 1000*s + ms;
		toutms = 125*(tinms/108);
		factor = 100;
	} else if (direction == 1){ // decimal to sexagesimal
		tinms = 10000000*h + 100000*m + 1000*s + ms;
		toutms = 0.864*tinms;
		factor = 60;
	}

	float outmsr = fmodf(toutms,1000);
	float outs = (toutms-outmsr)/1000;
	float outsr = fmodf(outs,factor);
	float outm = (outs-outsr)/factor;
	float outmr = fmodf(outm,factor);
	float outh = (outm-outmr)/factor;

	std::cout << std::to_string(static_cast<int>(round(outh)))+"h "
				+std::to_string(static_cast<int>(round(outmr)))+"m "
				+std::to_string(static_cast<int>(round(outsr)))+"s "
				+std::to_string(static_cast<int>(round(outmsr)))+"ms\n";
}

void printHelp(std::string execName){
			std::cout << "usage: "+
			execName+
			" OPTION HR [MIN] [SEC] [MSEC]\nOPTION:\n       -s: convert to sexagecimal ('normal' time)\n       -d: convert to decimal time\nConverting current time is not currently supported\n";
}

int main(int argc, char* argv[]){
	std::string execName = std::string(argv[0]);
	short direction;
	if (argc > 1 && argc <= 6){
		if (std::string(argv[1]) == "-d"){
			direction = 0;
		} else if (std::string(argv[1]) == "-s"){
			direction = 1;
		} else {
			std::cout << "Unknown option: "+std::string(argv[1])+"\n";
			printHelp(execName);
		}
		
		if (argc == 6){
			convert(direction, atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), atoi(argv[5]));
		} else if (argc == 5){
			convert(direction, atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), 0);
		} else if (argc == 4){
			convert(direction, atoi(argv[2]), atoi(argv[3]), 0, 0);
		} else if (argc == 3){
			convert(direction, atoi(argv[2]), 0, 0, 0);
		}
	}
	else {
		//using current time not yet implemented
		if (argc > 6){
			std::cout << "Too many arguments\n";
		}
		printHelp(execName);
	}
	
	return 0;
}
