//
// Created by timur on 08.10.2021.
//

#include "IMath.h"

void IntArithmetic::set_ZF(CPU &Cpu, uint64_t result) noexcept {
    if (result == 0) {
        Cpu.psw.set_ZF(1);
    } else {
        Cpu.psw.set_ZF(0);
    }
}

void IntArithmetic::set_SF(CPU &Cpu, uint64_t result) noexcept {
    if (result < 0) {
        Cpu.psw.set_SF(1);
    } else {
        Cpu.psw.set_SF(0);
    }
}

void IntArithmetic::set_OF(CPU &Cpu, uint64_t result) noexcept {
    if (result > std::numeric_limits<uint32_t>::max() || result > std::numeric_limits<int32_t>::max()) {
        Cpu.psw.set_OF(1);
    } else {
        Cpu.psw.set_OF(0);
    }
}

void IntArithmetic::set_CF(CPU &Cpu, uint64_t result) noexcept {                 //2^33 в 2
    if (result & 0b100000000000000000000000000000000) {
        Cpu.psw.set_CF(1);
    } else {
        Cpu.psw.set_CF(0);
    }
}

void IntArithmetic::change_flags(CPU &Cpu, uint64_t result) noexcept {
    switch (Cpu.cur_command.Cmd.code) {
        case CPU::idiv: {
            set_ZF(Cpu, result);
            set_SF(Cpu, result);
            break;
        }
        case CPU::imult: {
            set_ZF(Cpu, result);
            set_SF(Cpu, result);
            set_OF(Cpu, result);
            break;
        }
        default: {
            set_ZF(Cpu, result);
            set_SF(Cpu, result);
            set_OF(Cpu, result);
            set_CF(Cpu, result);
            break;
        }
    }
}

void IntArithmetic::operator()(CPU &Cpu) noexcept {
    int16_t Arithmetic_type = Cpu.cur_command.Cmd.s;// берем размер операндов
    switch (Arithmetic_type) {
        case 0: // Короткая
        {
            uint64_t result = run(Cpu.RCU.RCU_16.at(Cpu.cur_command.Cmd.r1).ui16,
                                  Cpu.RCU.RCU_16.at(Cpu.cur_command.Cmd.r2).ui16);
            change_flags(Cpu, result);
            Cpu.RCU.RCU_16.at(Cpu.cur_command.Cmd.r1).ui16 = result;
            break;
        }
        default://длинная
            uint64_t result = run(Cpu.RCU.RCU_32.at(Cpu.cur_command.Cmd.r1).u32,
                                  Cpu.RCU.RCU_32.at(Cpu.cur_command.Cmd.r2).u32);
            change_flags(Cpu, result);
            Cpu.RCU.RCU_32.at(Cpu.cur_command.Cmd.r1).u32 = result;
            break;
    }
}
