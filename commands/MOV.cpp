//
// Created by timur on 14.09.2021.
//

#include "MOV.h"
//Пересылка в любой адресный регистр - адреса ячейки памяти

void MOV_reg_reg::operator()(CPU &Cpu) noexcept {
    //Проверяем оба регистра т.к пересылка между ними
    Cpu.Check_reg(true);
    Cpu.Check_reg(false);
    int16_t operand_size = Cpu.cur_command.Cmd.s;//Для размера операнда
    switch (operand_size) {
        case 0://Для обмена 16 битных регистров
        {
            Cpu.RCU.RCU_16[Cpu.cur_command.Cmd.r1] = Cpu.RCU.RCU_16[Cpu.cur_command.Cmd.r2];
        }
        case 1://Для обмена 32 битных регистров
        {
            Cpu.RCU.RCU_32[Cpu.cur_command.Cmd.r1] = Cpu.RCU.RCU_32[Cpu.cur_command.Cmd.r2];
            break;
        }
    }
}

void MOV_reg_mem::operator()(CPU &Cpu) noexcept {
    //Проверяем r1 т.к пересылка из него по адресу памяти
    Cpu.Check_reg(true);
    int16_t operand_size = Cpu.cur_command.Cmd.s;//Для размера операнда
    switch (operand_size) {
        case 0://Для обмена 16 битного регистра с 16 битной ячейкой памяти
        {
            switch (Cpu.cur_command.Cmd.r2)//так как в одной ячейке могут лежать два 16 битных числа либо 1 32 битное по r2 определяем в какую из ячеек класть 0,1
            {
                case 1: {
                    Cpu.RAM[Cpu.cur_command.offset].word.d16_t[1] = Cpu.RCU.RCU_16[Cpu.cur_command.Cmd.r1];
                }
                    break;
                default://В случае иного значения кладем в 0
                {
                    Cpu.RAM[Cpu.cur_command.offset].word.d16_t[0] = Cpu.RCU.RCU_16[Cpu.cur_command.Cmd.r1];
                }
                    break;
            }
        }
        case 1://Для обмена 32 битного регистра с 32 битной ячейкой памяти
        {
            Cpu.RAM[Cpu.cur_command.offset].word.d32_t = Cpu.RCU.RCU_32[Cpu.cur_command.Cmd.r1];
        }
            break;
    }
}

void MOV_mem_reg::operator()(CPU &Cpu) noexcept {
    //Проверяем r1 т.к пересылка в него из адреса памяти
    Cpu.Check_reg(true);
    int16_t operand_size = Cpu.cur_command.Cmd.s;//Для размера операнда
    switch (operand_size) {
        case 0://Для обмена 16 битного регистра с 16 битной ячейкой памяти
        {
            switch (Cpu.cur_command.Cmd.r2)//так как в одной ячейке могут лежать два 16 битных числа либо 1 32 битное по r2 определяем из какой из ячеек брать 0,1
            {
                case 1: {
                    Cpu.RCU.RCU_16[Cpu.cur_command.Cmd.r1] = Cpu.RAM[Cpu.cur_command.offset].word.d16_t[1];
                }
                    break;
                default://В случае иного значения кладем в 0
                {
                    Cpu.RCU.RCU_16[Cpu.cur_command.Cmd.r1] = Cpu.RAM[Cpu.cur_command.offset].word.d16_t[0];
                }
                    break;
            }
        }
        case 1://Для обмена 32 битного регистра с 32 битной ячейкой памяти
        {
            Cpu.RAM[Cpu.cur_command.offset].word.d32_t = Cpu.RCU.RCU_32[Cpu.cur_command.Cmd.r1];
            break;
        }
    }
}
