#pragma once

#include "../Commands.h"
#include "../CPU.h"
#include <functional>
//=========================================================
//================Вещественная арифметика=================

class FloatArithmetic : public Command //Главный прототип команды с выставление флагов
{
public:
    explicit FloatArithmetic(std::function<double(double, double)> operation) : run(operation) {}

    void operator()(CPU &Cpu) noexcept;

protected:
    std::function<double(double, double)> run;

    void change_flags(CPU &Cpu, double result) noexcept;

    void set_ZF(CPU &Cpu, double result) noexcept;

    void set_OF(CPU &Cpu, double result) noexcept;

    void set_SF(CPU &Cpu, double result) noexcept;
};

class FAdd : public FloatArithmetic {
public:
    double run(double left, double right) noexcept {
        return left + right;
    }

    void operator()(CPU &Cpu) noexcept {
        double result = run(Cpu.RAM[Cpu.get_adr_r2()].data.f, Cpu.RAM[Cpu.get_adr_r3()].data.f);
        Cpu.RAM[Cpu.get_adr_r1()].data.f = result;
        change_flags(Cpu, result);
    }
};

class FSub : public FloatArithmetic {
public:
    double run(double left, double right) noexcept {
        return left - right;
    }

    virtual void operator()(CPU &Cpu) noexcept //Для сравнений
    {
        double result = run(Cpu.RAM[Cpu.get_adr_r2()].data.f, Cpu.RAM[Cpu.get_adr_r3()].data.f);
        Cpu.RAM[Cpu.get_adr_r1()].data.f = result;
        change_flags(Cpu, result);
    }
};

class FMult : public FloatArithmetic {
public:
    double run(double left, double right) noexcept {
        return left * right;
    }

    void operator()(CPU &Cpu) noexcept {
        double result = run(Cpu.RAM[Cpu.get_adr_r2()].data.f, Cpu.RAM[Cpu.get_adr_r3()].data.f);
        Cpu.RAM[Cpu.get_adr_r1()].data.f = result;
        change_flags(Cpu, result);
    }
};

class FDiv : public FloatArithmetic {
public:
    double run(double left, double right) noexcept {
        return left / right;
    }

    void operator()(CPU &Cpu) noexcept {
        double result = run(Cpu.RAM[Cpu.get_adr_r2()].data.f, Cpu.RAM[Cpu.get_adr_r3()].data.f);
        Cpu.RAM[Cpu.get_adr_r1()].data.f = result;
        change_flags(Cpu, result);
    }
};