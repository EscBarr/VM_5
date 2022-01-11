//
// Created by timur on 14.09.2021.
//

#include "MOV.h"
//Пересылка в любой регистр - адреса ячейки памяти

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
    Cpu.psw.set_IP(Cpu.psw.get_IP() + 2);
}

void MOV_reg_mem::operator()(CPU &Cpu) noexcept {

    //Проверяем r1 т.к пересылка в него из адреса памяти
    Cpu.Check_reg(true);
    int16_t operand_size = Cpu.cur_command.Cmd.s;//Для размера операнда
    switch (operand_size) {
        case 0://Для обмена 16 битного регистра с 16 битной ячейкой памяти
        {
            datatype16 Temp; //копируем значение ячеек памяти в объединение из двух байтного массива
            Temp.d16[0] = Cpu.RAM[Cpu.cur_command.offset];
            Temp.d16[1] = Cpu.RAM[Cpu.cur_command.offset + 1];
            Cpu.RCU.RCU_16[Cpu.cur_command.Cmd.r1] =
                    Temp; //копируем значение регистра в объединение из двух байтного массива
                    break;
        }
        case 1://Для обмена 32 битного регистра с 32 битной ячейкой памяти
        {
            datatype32 Temp; //копируем значение ячеек памяти в объединение из двух байтного массива
            Temp.d32[0] = Cpu.RAM[Cpu.cur_command.offset];
            Temp.d32[1] = Cpu.RAM[Cpu.cur_command.offset + 1];
            Temp.d32[2] = Cpu.RAM[Cpu.cur_command.offset + 2];
            Temp.d32[3] = Cpu.RAM[Cpu.cur_command.offset + 3];
            Cpu.RCU.RCU_32[Cpu.cur_command.Cmd.r1] =
                    Temp; //копируем значение регистра в объединение из двух байтного массива
                    break;
        }
    }
    Cpu.psw.set_IP(Cpu.psw.get_IP() + 4);
}

void MOV_mem_reg::operator()(CPU &Cpu) noexcept {
    //Проверяем r1 т.к пересылка из него по адресу памяти
    Cpu.Check_reg(true);
    int16_t operand_size = Cpu.cur_command.Cmd.s;//Для размера операнда
    switch (operand_size) {
        case 0://Для обмена 16 битного регистра с 16 битной ячейкой памяти
        {
            datatype16 Temp =
                    Cpu.RCU.RCU_16[Cpu.cur_command.Cmd.r1]; //копируем значение регистра в объединение из двух байтного массива
                    Cpu.RAM[Cpu.cur_command.offset] = Temp.d16[0];
                    Cpu.RAM[Cpu.cur_command.offset + 1] = Temp.d16[1];
                    /* switch (Cpu.cur_command.Cmd.r2)//так как в одной ячейке могут лежать два 16 битных числа либо 1 32 битное по r2 определяем в какую из ячеек класть 0,1
                       {
                           case 1: {
                               Cpu.RAM[Cpu.cur_command.offset].word.d16_t[1] =
                           }
                               break;
                           default://В случае иного значения кладем в 0
                           {
                               Cpu.RAM[Cpu.cur_command.offset].word.d16_t[0] = Cpu.RCU.RCU_16[Cpu.cur_command.Cmd.r1];
                           }
                               break;
                       }*/
                    break;
        }
        case 1://Для обмена 32 битного регистра с 32 битной ячейкой памяти
        {
            datatype32 TempFourBytes = Cpu.RCU.RCU_32[Cpu.cur_command.Cmd.r1];
            Cpu.RAM[Cpu.cur_command.offset] = TempFourBytes.d32[0];
            Cpu.RAM[Cpu.cur_command.offset + 1] = TempFourBytes.d32[1];
            Cpu.RAM[Cpu.cur_command.offset + 2] = TempFourBytes.d32[2];
            Cpu.RAM[Cpu.cur_command.offset + 3] = TempFourBytes.d32[3];
            break;
        }
    }
    Cpu.psw.set_IP(Cpu.psw.get_IP() + 4);
}
void MOV_adr_reg::operator()(CPU &Cpu) noexcept {
  //Проверяем r1 т.к пересылка в него смещения из команды(адреса метки или переменной)
  Cpu.Check_reg(true);
  Cpu.RCU.RCU_16[Cpu.cur_command.Cmd.r1].ui16 =
	  Cpu.cur_command.offset; //копируем значение регистра в объединение из двух байтного массива
	  Cpu.psw.set_IP(Cpu.psw.get_IP() + 4);
}
