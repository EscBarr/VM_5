cmake_minimum_required(VERSION 3.20)
project(VM_5)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXE_LINKER_FLAGS "-static")
file(GLOB SOURCES
        *.h
        *.cpp
        )
add_executable(VM_5 VM_5.cpp CPU.cpp CPU.h Memory.cpp Memory.h PSW.cpp PSW.h Types.h Commands.h commands/MOV.cpp commands/MOV.h Types.h commands/IO.h commands/IO.cpp commands/Jump.cpp commands/Jump.h commands/IMath.cpp commands/IMath.h commands/FMath.cpp commands/FMath.h commands/Compare.cpp commands/Compare.h commands/Bit_Operations.h Loaders/LoaderFromText.cpp Loaders/LoaderFromText.h HeadersExtensions.h Assembly/AssemblyTypes.h Assembly/Assembly.cpp Assembly/Assembly.h Automats/IntegerParser.cpp Automats/IntegerParser.h Automats/FloatParser.cpp Automats/FloatParser.h Parsers/MathParser.cpp Parsers/MathParser.h Parsers/IndirectParser.cpp Parsers/IndirectParser.h commands/Bit_Operations.cpp Loaders/LoaderBinFile.cpp Loaders/LoaderBinFile.h)
