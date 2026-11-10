#include <iostream>
#include "AudioConnect.h"
#include "Sound.h"

int main()
{
	std::cout << "Programm started..." << std::endl;
	try {
		AudioConnect a; // подключаемся к устройству
	}
	catch (const std::runtime_error& e) { // выводим текст ошибки
		std::cerr << e.what() << std::endl;
	}
}
