#include "Compare.h"

void Compare::operator()(CPU &Cpu) noexcept//Целочисленное сравнение.
{
    int16_t Arithmetic_type = Cpu.cur_command.Cmd.s;// берем размер операндов
    switch (Arithmetic_type) {
        case 0: // Короткая
        {
            uint16_t temp = Cpu.RCU.RCU_16.at(Cpu.cur_command.Cmd.r1).ui16;
            IntArithmetic t([](int64_t a, int64_t b) { return (a - b); });
            t.operator()(Cpu);
            Cpu.RCU.RCU_16.at(Cpu.cur_command.Cmd.r1).ui16 = temp;//возвращаем исходное состояние
            Cpu.psw.set_IP(Cpu.psw.get_IP() + 2);
            break;
        }
        default://длинная
        {
            uint32_t temp = Cpu.RCU.RCU_32.at(Cpu.cur_command.Cmd.r1).u32;
            IntArithmetic t([](int64_t a, int64_t b) { return (a - b); });
            t.operator()(Cpu);
            Cpu.RCU.RCU_32.at(Cpu.cur_command.Cmd.r1).u32 = temp;//возвращаем исходное состояние
            Cpu.psw.set_IP(Cpu.psw.get_IP() + 4);
            break;
        }
    }
}

void CompareF::operator()(CPU &Cpu) noexcept {
    float temp = Cpu.RCU.RCU_32.at(Cpu.cur_command.Cmd.r1).f;
    FloatArithmetic t([](double a, double b) { return (a - b); });
    t.operator()(Cpu);
    Cpu.RCU.RCU_32.at(Cpu.cur_command.Cmd.r1).f = temp;//возвращаем исходное состояние
    Cpu.psw.set_IP(Cpu.psw.get_IP() + 4);
}