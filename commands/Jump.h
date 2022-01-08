//
// Created by timur on 10.09.2021.
//

#ifndef VM_5_JUMP_H
#define VM_5_JUMP_H


#include "../Commands.h"
#include "../CPU.h"

class JumpDirect : public Command {
public:
    void operator()(CPU &Cpu) noexcept override;

    void set_ip(CPU &Cpu);
};

class Coditional_jump : public JumpDirect {
    static bool Equal(PSW Flags);

    static bool Lesser(PSW Flags);

public:
    void operator()(CPU &Cpu) noexcept override;
};

class Call : public Command {
public:
    void operator()(CPU &Cpu) noexcept override;
};

class Ret : public Command {
public:
    void operator()(CPU &Cpu) noexcept override;
};

#endif //VM_5_JUMP_H
