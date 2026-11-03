#include <iostream>
#include "AudioConnect.h"

int main()
{
	std::cout << "Programm started..." << std::endl;
	try {
		AudioConnect a;
	}
	catch (const std::runtime_error& e) {
		std::cerr << e.what() << std::endl;
	}
}
