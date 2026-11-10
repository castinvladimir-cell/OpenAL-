# pragma once

#include <iostream>
#include <string>
#include <AL/al.h>
#include <stdexcept>
#include <sndfile.h>
#include <vector> 

class Sound {
private: 
    ALuint TapeRec_; // источник (магнитофон)
    ALuint Buffer_; // буфер (кассета)

public: 
    double_t volumein = 0.5; 

    double volume(double volume) { // проценты в громкость от 0 до 1
        volumein = volume;
        if (volumein > 100) {
            volumein = 1.0; // max vol.
        } else if (volumein < 0) {
            volumein = 0;  // min vol. 
        } else {
            volumein = volumein / 100; 
        }
        return volumein;
    }

    Sound() { //default 
        SF_INFO info {}; // сведения о файле, поля обнулены
        SNDFILE* curfile = sf_open("Deep.wav", SFM_READ, &info);  
        if (curfile == nullptr) {
            throw std::runtime_error("Error: file is not exist"); 
        }
        std::vector<short> squanch(info.frames * info.channels); // кадры умножить на каналы
        short* ptr = squanch.data(); // Лежит звук
        sf_count_t howmuch = info.frames; 

        sf_count_t count = sf_readf_short(curfile, ptr, howmuch); // сколько кадров прочитали
        sf_close(curfile);

        if (count < 1) {
            throw std::runtime_error("Error: cannot read file");
        }

        ALenum curForm = AL_FORMAT_MONO16;
        switch (info.channels) {  // проверка формата на моно или стерео
            case 1: 
                curForm = AL_FORMAT_MONO16;
                break;
            case 2: 
                curForm = AL_FORMAT_STEREO16;
                break;
            default: 
                throw std::runtime_error("Error: too much "); 
        }

        alGenBuffers(1, &Buffer_); // создаём буфер
        ALsizei ctByte = (count * info.channels) * sizeof(short);
        alBufferData(Buffer_, curForm, ptr, ctByte, info.samplerate); // кладём звук в буфер

        if (alGetError()) { // Проверяем, получилось ли положить звук AL_NO_ERROR = 0
            alDeleteBuffers(1, &Buffer_);
            throw std::runtime_error("Error: audio not in buffer");
        }
    }

    ~Sound() {

    }
};