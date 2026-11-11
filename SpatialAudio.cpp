#include <iostream>
#include <limits>
#include "AudioConnect.h"
#include "Sound.h"

void clearInput() { // сброс ошибки cin и остатка строки
	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int main()
{
	std::cout << "Programm started..." << std::endl;
	try {
		AudioConnect a; // подключаемся к устройству
		Sound s; 	   // загружаем Deep.wav, создаём буфер и источник

		bool work = true;
		while (work) {
			std::cout << std::endl << "1 play  2 stop  3 volume  4 left-right  5 circle  6 exit" << std::endl << "> ";

			int choice = 0;
			if (!(std::cin >> choice)) { // ввели не число
				clearInput();
				std::cout << "Wrong type, enter a number" << std::endl;
				continue;
			}
			clearInput(); // убираем остаток строки, например в 3abc

			switch (choice) {
				case 1:
					s.play();
					std::cout << "Playing" << std::endl;
					break;
				case 2:
					s.stop();
					std::cout << "Stopped" << std::endl;
					break;
				case 3: { // скобки нужны из-за переменных внутри case
					std::cout << "Volume 0-100: ";
					float vol = 0;
					if (!(std::cin >> vol)) {
						clearInput();
						std::cout << "Wrong type, enter a number" << std::endl;
						break;
					}
					clearInput();
					float gain = s.volume(vol); // volume возвращает громкость от 0 до 1
					std::cout << "Volume " << gain * 100 << std::endl;
					break;
				}
				case 4:
					std::cout << "Moving left-right..." << std::endl;
					s.playOX(); // меню ждёт, пока движение закончится
					std::cout << "Done" << std::endl;
					break;
				case 5:
					std::cout << "Moving around..." << std::endl;
					s.aroundcircle();
					std::cout << "Done" << std::endl;
					break;
				case 6:
					work = false; // выходим из цикла, потом сработают деструкторы
					break;
				default:
					std::cout << "Unknown command" << std::endl;
			}
		}
	}
	catch (const std::runtime_error& e) { // выводим текст ошибки
		std::cerr << e.what() << std::endl;
	}
}
