//
// Created by timur on 14.09.2021.
//

#ifndef VM_5_IO_H
#define VM_5_IO_H

#include "../Commands.h"
#include "../CPU.h"

//Ввод и вывод только в регистры
class Input : public Command {
    void operator()(CPU &Cpu) noexcept override;
};

class Out : public Command {
    void operator()(CPU &Cpu) noexcept override;
};

#endif //VM_5_IO_H
