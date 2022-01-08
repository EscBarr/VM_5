//
// Created by timur on 10.09.2021.
//

#ifndef VM_5_TYPES_H
#define VM_5_TYPES_H

#include <stdint.h>
#include <cctype>
#include <array>
using address_t = uint16_t;
using word = uint8_t; //байт памяти


//2
union datatype16 {
  int16_t i16;
  uint16_t ui16;
  word d16[2];
};
//4
union datatype32 {
  int32_t i32;
  uint32_t u32;
  float f;
  word d32[4];
};

union word_t {
  word d32[4];
};

union registers {
  std::array<datatype16, 16> RCU_16;
  std::array<datatype32, 8> RCU_32;
};
//Короткая команда
#pragma pack(push, 1)
struct base_short_command {
  // Результат в r1
  uint8_t code: 7;       // Код операции
  uint8_t s: 1;       // Размер целого операнда
  uint8_t r1: 4;         // Регистр №1
  uint8_t r2: 4;            // Регистр №2
};
#pragma pack(pop)
union Command_register {
  //datatype16 dataTwoBytes;
  word_t data;
  // Длинная команда
  struct {
	base_short_command Cmd;
	uint16_t offset; // Смещение
  };
};

#endif //VM_5_TYPES_H
//    base_command_t(word_t word) {
//      Cmd.s = word.d32[0] & -word.d32[0];//Извлекаем последний бит т.к в памяти лежит 8 битное значение
//      Cmd.code =  word.d32[0] &  word.d32[0] - 1;//убираем извлеченный бит и записываем 7 битный КОП
//      Cmd.r2 =  word.d32[1] & 0xF;//Извлекаем последние 4 бита
//      Cmd.r1 =  word.d32[1] &  word.d32[1] - 4;//убираем извлеченные 4 бита и записываем последние 4
//      if(Cmd.s == 1)
//      {
//        offset = ( word.d32[2] << 8) |  word.d32[3];//Объединение двух ячеек памяти
//      }
//    }

//union mem_word_t {
//  datatype16 d16_t[2];
//  datatype32 d32_t;
//  base_command_t base_command;
//};

//union word_t//ячейка памяти хранит в себе либо 2 16 битных числа либо 1 32 битное
//{
//  datatype16 d16_t[2];
//  datatype32 d32_t;
//};