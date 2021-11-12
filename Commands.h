//
// Created by timur on 10.09.2021.
//

#ifndef VM_5_COMMANDS_H
#define VM_5_COMMANDS_H

#include "stdexcept"
#include "iostream"

class CPU;

class Command {
public:
    Command() = default;

    virtual void operator()(CPU &Cpu) = 0;

    virtual ~Command() = default;
};


#endif //VM_5_COMMANDS_H
