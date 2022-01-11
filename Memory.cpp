//
// Created by timur on 10.09.2021.
//

#include "Memory.h"
#include "Memory.h"

// 65565 / 4 дает нам 16 391 ячейку в одной ячейке либо 2 16 битных числа, либо 1 32 битное (ну и команды 32 бита), только как достать 16 битное
Memory::Memory() {
    memory.resize(UINT16_MAX);
}

Memory::~Memory() {
    std::destroy(memory.begin(), memory.end());
}

word &Memory::operator[](address_t address) {
  return memory.at(address);
}