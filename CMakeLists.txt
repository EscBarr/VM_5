cmake_minimum_required(VERSION 3.20)
project(VM_5)

set(CMAKE_CXX_STANDARD 20)
file(GLOB SOURCES
        *.h
        *.cpp
        )
add_executable(VM_5 VM_5.cpp CPU.cpp CPU.h Memory.cpp Memory.h PSW.cpp PSW.h Types.h Commands.h commands/MOV.cpp commands/MOV.h Types.h commands/IO.h commands/IO.cpp commands/Jump.cpp commands/Jump.h commands/IMath.cpp commands/IMath.h commands/FMath.cpp commands/FMath.h commands/Compare.cpp commands/Compare.h commands/Bit_Operations.h)
