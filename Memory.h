//
// Created by timur on 10.09.2021.
//

#ifndef VM_5_MEMORY_H
#define VM_5_MEMORY_H
#include <vector>
#include "Types.h"


class Memory {
  std::vector<word> memory;

public:
    Memory();

    ~Memory();

    Memory(const Memory &) = delete;

    Memory(const Memory &&) = delete;

    Memory &operator=(const Memory &) = delete;

    Memory &operator=(const Memory &&) = delete;

  word &operator[](address_t address);
};

#endif //VM_5_MEMORY_H
