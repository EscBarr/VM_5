//
// Created by timur on 10.09.2021.
//

#ifndef VM_5_MEMORY_H
#define VM_5_MEMORY_H

#include "Types.h"
#include "vector"

class Memory {
    std::vector<mem_word_t> memory;

public:
    Memory();

    ~Memory();

    Memory(const Memory &) = delete;

    Memory(const Memory &&) = delete;

    Memory &operator=(const Memory &) = delete;

    Memory &operator=(const Memory &&) = delete;

    mem_word_t &operator[](address_t address);
};

#endif //VM_5_MEMORY_H
