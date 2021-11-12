//
// Created by timur on 10.09.2021.
//

#ifndef
VM_5_TYPES_H
#define
VM_5_TYPES_H

#include
"stdint.h"
#include
"cctype"
#include
"array"
using address_t = uint16_t;
using word = uint8_t; //байт памяти

//Короткая команда
struct base_short_command
{
// Результат в r1
uint8_t code: 7;       // Код операции
uint8_t s: 1;       // Размер целого операнда
uint8_t r1: 4;         // Регистр №1
uint8_t r2: 4;            // Регистр №2
};

// Длинная команда
struct base_command_t
{
base_short_command Cmd;
uint16_t offset;         // Смещение
};


#pragma
pack(pop)
union datatype16//2
{
int16_t i16;
uint16_t ui16;
word d16[2];
};
union datatype32//4
{
int32_t i32;
uint32_t u32;
float f;
word d32[4];
};
union word_t//ячейка памяти хранит в себе либо 2 16 битных числа либо 1 32 битное
{
datatype16 d16_t[2];
datatype32 d32_t;
};

union registers
{
std::array<datatype16, 16>RCU_16;
std::array<datatype32, 8>RCU_32;
};

union mem_word_t
{
word_t word;
base_command_t cmd;
};
#endif //VM_5_TYPES_H
