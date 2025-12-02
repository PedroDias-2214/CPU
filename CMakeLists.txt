cmake_minimum_required(VERSION 4.0)
project(CPU)

set(CMAKE_CXX_STANDARD 20)

add_executable(CPU main.cpp
        MMU.cpp
        CPU.cpp
        Barramento.cpp
        registradores.cpp
        Decoder.cpp
        Memoria.cpp)
