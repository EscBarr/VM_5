//
// Created by timur on 10.09.2021.
//

#include "CPU.h"
#include "commands/MOV.h"
#include "commands/IMath.h"
#include "commands/FMath.h"
#include "commands/Jump.h"
#include "commands/Compare.h"
#include "commands/IO.h"
#include "commands/Bit_Operations.h"
#include "CPU.h"

void CPU::Initialize_Comands() {
    //==Команда остановки==
    commands[stop] = nullptr;
    //==Команды перессылки==
    commands[mov_mem_reg] = new class MOV_reg_reg();
    commands[mov_reg_mem] = new class MOV_reg_mem();
    commands[mov_mem_reg] = new class MOV_mem_reg();
    //==Команды целой арифметики==//
    commands[iadd] = new class IntArithmetic([](int64_t a, int64_t b) { return (a + b); });
    commands[isub] = new class IntArithmetic([](int64_t a, int64_t b) { return (a - b); });
    commands[imult] = new class IntArithmetic([](int64_t a, int64_t b) { return (a * b); });
    commands[idiv] = new class IntArithmetic([](int64_t a, int64_t b) { return (a / b); });
    commands[imod] = new class IntArithmetic([](int64_t a, int64_t b) { return (a % b); });
    //==Битовые операции==//
    commands[Bitwise_AND] = new class Bitwise_op([](int64_t a, int64_t b) { return (a & b); });
    commands[Bitwise_OR] = new class Bitwise_op([](int64_t a, int64_t b) { return (a || b); });
    commands[Bitwise_XOR] = new class Bitwise_op([](int64_t a, int64_t b) { return (a ^ b); });
    commands[Bitwise_NOT] = new class Bitwise_op([](int64_t a, int64_t b) { return (~a); });
    commands[Bitwise_LEFT] = new class Bitwise_op([](int64_t a, int64_t b) { return (a << b); });
    commands[Bitwise_RIGHT] = new class Bitwise_op([](int64_t a, int64_t b) { return (a >> b); });
    //==Команды дробной арифметики==//
    commands[fadd] = new class FloatArithmetic([](double a, double b) { return (a + b); });
    commands[fsub] = new class FloatArithmetic([](double a, double b) { return (a + b); });
    commands[fmul] = new class FloatArithmetic([](double a, double b) { return (a + b); });
    commands[fdiv] = new class FloatArithmetic([](double a, double b) { return (a + b); });
    //==Команды Сравнения==//
    commands[cmp] = new class Compare();
    commands[cmpF] = new class CompareF();
    //==Команды Ввода/Вывода==//
    commands[input] = new class Input();
    commands[out] = new class Out();
    //==Команды перехода==//
    commands[jmp] = new class JumpDirect();
    commands[jmpE] = new class Coditional_jump();
    commands[jmpNE] = new class Coditional_jump();
    commands[jmpG] = new class Coditional_jump();
    commands[jmpG_E] = new class Coditional_jump();
    commands[jmpA] = new class Coditional_jump();
    commands[jmpA_E] = new class Coditional_jump();
    commands[jmpL] = new class Coditional_jump();
    commands[jmpL_E] = new class Coditional_jump();
    commands[jmpB] = new class Coditional_jump();
    commands[jmpB_E] = new class Coditional_jump();
    commands[jmpsf] = new class Coditional_jump();
    commands[jmpNsf] = new class Coditional_jump();
    commands[call] = new class Call();
    commands[Return] = new class Ret();

}

CPU::CPU() {
    psw.set_IP(0);
    Initialize_Comands();
}

void CPU::restart() {
    psw.set_IP(0);
    psw.set_OF(0), psw.set_SF(0), psw.set_ZF(0), psw.set_CF(0);
    std::fill(RCU.RCU_16.begin(), RCU.RCU_16.end(), 0);
    std::fill(RCU.RCU_32.begin(), RCU.RCU_32.end(), 0);
}

/*void CPU::Load_cmd()//Если не объединять ячейки в юнион, то только сдвигами плюс проблема с пересылками из памяти в регистры, пока не будет использоваться.
{
    //RAM[psw.get_IP()] &= RAM[psw.get_IP()] - 1;
    //cur_command.Cmd = (RAM[psw.get_IP()] << 8) | RAM[psw.get_IP()+1];//ПРОЧИТАТЬ ПРО БИТОВЫЕ МАСКИ
    cur_command.Cmd.s = RAM[psw.get_IP()] & -RAM[psw.get_IP()];//Извлекаем последний бит т.к в памяти лежит 8 битное значение
    cur_command.Cmd.code = RAM[psw.get_IP()] & RAM[psw.get_IP()] - 1;//убираем извлеченный бит и записываем 7 битный КОП
    cur_command.Cmd.r2 = RAM[*//**//*psw.get_IP()+1] & 0xF;//Извлекаем последние 4 бита
    cur_command.Cmd.r1 = RAM[psw.get_IP()] & RAM[psw.get_IP()] - 4;//убираем извлеченные 4 бита и записываем последние 4
    psw.set_IP(psw.get_IP() + 2);
    if(cur_command.Cmd.s == 1)
    {
        cur_command.offset = (RAM[psw.get_IP()] << 8) | RAM[psw.get_IP()+1];//Объединение двух ячеек памяти
        psw.set_IP(psw.get_IP() + 2);
    }
}*/
void CPU::start() {
    cur_command = RAM[psw.get_IP()].cmd;
    while (cur_command.Cmd.code != stop) {
        commands[cur_command.Cmd.code]->operator()(*this);//Вызов оператора для исполнения команды
        psw.set_IP(psw.get_IP() + 1);
        cur_command = RAM[psw.get_IP()].cmd;
    }
}

void CPU::Check_reg(const bool &r1_or_r2) {
    switch (cur_command.Cmd.s) {
        case 0: {
            try {
                if (r1_or_r2) { RCU.RCU_16.at(cur_command.Cmd.r1); }
                else { RCU.RCU_16.at(cur_command.Cmd.r2); }
            }
            catch (const std::out_of_range &exception) {
                r1_or_r2 ? std::cerr << "Ошибка, неверно указан номер 16 битного регистра r1 " << '\n' : std::cerr
                        << "Ошибка, неверно указан номер 16 битного регистра r2 " << '\n';
            }
        }
            break;
        case 1: {
            try {
                if (r1_or_r2) { RCU.RCU_32.at(cur_command.Cmd.r1); }
                else { RCU.RCU_32.at(cur_command.Cmd.r2); }
            }
            catch (const std::out_of_range &exception) {
                r1_or_r2 ? std::cerr << "Ошибка, неверно указан номер 32 битного регистра r1 " << '\n' : std::cerr
                        << "Ошибка, неверно указан номер 32 битного регистра r2 " << '\n';
            }
        }

    }


}
/* i 32444
     datatype32 d
     d = 32444
     int i;
     if(s==0)
     {
     i = 2
     }
     else
     {
     i = 4
     }
     for(i<4)
     CPU.memory[IP+i]=d.d32[i]*/