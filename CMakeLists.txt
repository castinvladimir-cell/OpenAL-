cmake_minimum_required(VERSION 3.20)
project(SpatialAudio VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)

add_executable(SpatialAudio SpatialAudio.cpp AudioConnect.cpp)

find_package(OpenAL CONFIG REQUIRED)

find_package(SndFile CONFIG REQUIRED)

target_link_libraries(SpatialAudio PRIVATE OpenAL::OpenAL SndFile::sndfile)

add_custom_command(TARGET SpatialAudio POST_BUILD # после сборки копируем Deep.wav к exe
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/Deep.wav $<TARGET_FILE_DIR:SpatialAudio>)
