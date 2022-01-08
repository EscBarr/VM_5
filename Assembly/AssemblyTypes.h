//
// Created by timur on 07.01.2022.
//
#include "../HeadersExtensions.h"
#pragma once
enum CommandList {
  io,
  math,
  move_reg_reg,
  move_reg_mem,
  move_mem_reg,
  move_adr_reg,
  lea,
  cmp,
  jmp,
  call,
  other
};
enum Operands {
  cnst,
  reg,
  var,
  inderect
};
struct NameTableCell {
  uint16_t Op_Type; //Тип операнда
  uint16_t Address; //Расположение в памяти
  datatype32 Val;
};