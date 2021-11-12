//
// Created by timur on 10.09.2021.
//

#include "Memory.h"
#include "Memory.h"

// 65565 / 4 дает нам 16 391 ячейку в одной ячейке либо 2 16 битных числа, либо 1 32 битное (ну и команды 32 бита), только как достать 16 битное
Memory::Memory() {
    memory.reserve(UINT16_MAX / sizeof(mem_word_t));
}

Memory::~Memory() {
    std::destroy(memory.begin(), memory.end());
}

mem_word_t &Memory::operator[](address_t address) {
    return memory[address];
}