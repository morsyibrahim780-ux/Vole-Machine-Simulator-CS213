cmake_minimum_required(VERSION 4.3)
project(Vole_Machine_Simulator_CS213)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Vole_Machine_Simulator_CS213
        controlUnit.h
        main.cpp
        ProgramLoader.h
        RegisterFile+ALU
        Screen.h
        "Vole machine code."
        "Vole machine code")
