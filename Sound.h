# pragma once

#include <iostream>
#include <string>
#include <AL/al.h>
#include <stdexcept>
#include <sndfile.h>
#include <vector> 
#include <thread> 
#include <chrono> 
#include <cmath> 

class Sound {
private: 
    ALuint TapeRec_ = 0; // источник (магнитофон)
    ALuint Buffer_ = 0; // буфер (кассета)
    float volumein = 0.5;
    float x = 0, y = 0, z = 0;

    void lEar(float y, float z, int N) {
        for (int i = 0; i < 16; i++) { //Смещаем в левое ухо
            x += -0.25;
            alSource3f(TapeRec_, AL_POSITION, x, y, z);
            std::this_thread::sleep_for(std::chrono::milliseconds(N));  
        } 
    }

    void rEar(int y, int z, int N) {
        for (int i = 0; i < 16; i++) { // Смещаем в правое ухо
            x += 0.25;
            alSource3f(TapeRec_, AL_POSITION, x, y, z);
            std::this_thread::sleep_for(std::chrono::milliseconds(N)); 
        }  
    }

    void nuller() {
        x = 0, y = 0, z = 0;
        alSource3f(TapeRec_, AL_POSITION, x, y, z);
    }

public: 

    void aroundcircle(int N = 100) {
        int Radius = 3; 
        float Radian = 0.25; 
        float Radik = 0;
        for (int i = 0; i < 57; i++) {
            x = Radius * cos(Radik += Radian);
            Radik -= Radian;
            z = Radius * sin(Radik += Radian);
            alSource3f(TapeRec_, AL_POSITION, x, y, z);
            std::this_thread::sleep_for(std::chrono::milliseconds(N));
        }
        nuller();
    }

    float volume(float volume) { // проценты в громкость от 0 до 1
        volumein = volume;
        if (volumein > 100) {
            volumein = 1.0; // max vol.
        } else if (volumein < 0) {
            volumein = 0;  // min vol. 
        } else {
            volumein = volumein / 100; 
        }
        alSourcef(TapeRec_, AL_GAIN, volumein);
        return volumein;
    }

    void playOX() {
        for (int y = 0, z = -2, N = 50, i = 0; i < 2; i++ ) {
            x = 0; // Начинаем с нуля и идем в левое ухо
            lEar(y, z, N); //Перешли в сторону (л) и вернули
            rEar(y, z, N);
        
            rEar(y, z, N); //Перешли в сторону (р) и вернули
            lEar(y, z, N); 
            
        }
        nuller();
    }

    void play() {
        alSourcePlay(TapeRec_);
    }

    void stop() {
        alSourceStop(TapeRec_);
    }

    Sound() {             //default, здесь происходит логика преобразований над звуком для работы с ним
        SF_INFO info {}; // сведения о файле, поля обнулены
        SNDFILE* curfile = sf_open("Deep.wav", SFM_READ, &info);  
        if (curfile == nullptr) {
            throw std::runtime_error("Error: file is not exist"); 
        }
        std::vector<short> squanch(info.frames * info.channels); // кадры умножить на каналы
        short* ptr = squanch.data();                            // Лежит звук
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

        // Ага. Ухты, ничего себе
        alGenSources(1, &TapeRec_);
        alSourcei(TapeRec_, AL_BUFFER, Buffer_);

        if (alGetError()) { // Проверка создания источника и привязки к нему буфера
            alDeleteSources(1, &TapeRec_);
            alDeleteBuffers(1, &Buffer_);
            throw std::runtime_error("Error: something wrong");
        }
        // Громкость
        alSourcef(TapeRec_, AL_GAIN, volumein);
    } 

    ~Sound() {
        alDeleteSources(1, &TapeRec_);
        alDeleteBuffers(1, &Buffer_);
    }
};