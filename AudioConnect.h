#pragma once
#include <iostream>
#include <AL/alc.h>
#include <stdexcept>
#include <string>

class AudioConnect
{
private:
	ALCdevice* device_;
	ALCcontext* context_;

	void legitcheck_(int function = 0) {

			if (device_ == nullptr && function == 1) {
				std::cout << "Ошибка 1 (" << std::endl;
				throw std::runtime_error("Error: there is no device connected");
			}

			if (context_ == nullptr && function == 2) {
				alcCloseDevice(device_);
				std::cout << "Ошибка 2 (" << std::endl;
				throw std::runtime_error("Error: there is no context created");
			}

			if (alcGetCurrentContext() != context_ && function == 3) {
				alcDestroyContext(context_);
				alcCloseDevice(device_); 
				std::cout << "Ошибка 3 (" << std::endl;
				throw std::runtime_error("Error: context is not current");
			} 
	}

public:

	AudioConnect() {

		device_ = alcOpenDevice(nullptr);
		legitcheck_(1);

		context_ = alcCreateContext(device_, nullptr);
		legitcheck_(2);

		alcMakeContextCurrent(context_);
		legitcheck_(3);

		std::string devicename = alcGetString(device_, ALC_DEVICE_SPECIFIER);

		std::cout << devicename << std::endl;

		/*if (legitcheck_(1) > 0) {
			legitcheck_(1); 
		} else if (legitcheck_(1) == 0)  { 
			context_ = alcCreateContext(device_, nullptr);
			if (legitcheck_(2) > 0) {
				legitcheck_(2);
			} else if (legitcheck_(2) == 0) {
				alcMakeContextCurrent(context_);
				if (legitcheck_(3) > 0) {
					legitcheck_(3);
				}
				else {
					std::cout << alcGetString(device_, ALC_DEVICE_SPECIFIER) << std::endl;
				}
			}
		}*/
	};

	~AudioConnect() {
		if (alcGetCurrentContext() == context_) {
			alcMakeContextCurrent(nullptr);
			alcDestroyContext(context_); 
			alcCloseDevice(device_);
		} else {
			alcDestroyContext(context_);
			alcCloseDevice(device_);
		}
	};
};
