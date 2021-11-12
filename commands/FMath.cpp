#include "FMath.h"

void FloatArithmetic::change_flags(CPU &Cpu, double result) noexcept {
    set_ZF(Cpu, result);
    set_OF(Cpu, result);
    set_SF(Cpu, result);
}

void FloatArithmetic::set_ZF(CPU &Cpu, double result) noexcept {
    if (result < std::numeric_limits<float>::epsilon()) {
        Cpu.psw.set_ZF(1);
    } else {
        Cpu.psw.set_ZF(0);
    }
}

void FloatArithmetic::set_OF(CPU &Cpu, double result) noexcept {
    if (result > std::numeric_limits<float>::max()) {
        Cpu.psw.set_OF(1);
    } else {
        Cpu.psw.set_OF(0);
    }
}

void FloatArithmetic::set_SF(CPU &Cpu, double result) noexcept {
    if (result < 0.0) {
        Cpu.psw.set_SF(1);
    } else {
        Cpu.psw.set_SF(0);
    }
}

void FloatArithmetic::operator()(CPU &Cpu) noexcept {
    double result = run(Cpu.RCU.RCU_32.at(Cpu.cur_command.Cmd.r1).f, Cpu.RCU.RCU_32.at(Cpu.cur_command.Cmd.r2).f);
    change_flags(Cpu, result);
    Cpu.RCU.RCU_32.at(Cpu.cur_command.Cmd.r1).f = result;

}
