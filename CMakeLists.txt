cmake_minimum_required(VERSION 3.20)
project(SpatialAudio VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

add_executable(SpatialAudio SpatialAudio.cpp AudioConnect.cpp)

find_package(OpenAL CONFIG REQUIRED)

find_package(SndFile CONFIG REQUIRED)

target_link_libraries(SpatialAudio PRIVATE OpenAL::OpenAL SndFile::sndfile)