//
// Created by timur on 14.09.2021.
//

#include "IO.h"

void Input::operator()(CPU &Cpu) noexcept {
    int16_t operand_size = Cpu.cur_command.Cmd.s;//Для размера операнда
    Cpu.Check_reg(true);//Проверим индекс r1 из команды
    // Загружаем значение в память, по адресу на который указывает r1
    if (operand_size == 0) {
        std::cin >> Cpu.RCU.RCU_16.at(Cpu.cur_command.Cmd.r1).ui16;
        Cpu.psw.set_IP(Cpu.psw.get_IP() + 2);
    } else {
        std::cin >> Cpu.RCU.RCU_32.at(Cpu.cur_command.Cmd.r1).u32;
        Cpu.psw.set_IP(Cpu.psw.get_IP() + 4);
    }

}

void Out::operator()(CPU &Cpu) noexcept {
    int16_t operand_size = Cpu.cur_command.Cmd.s;//Для размера операнда
    Cpu.Check_reg(true);//Проверим индекс r1 из команды
    int16_t type_for_display = Cpu.cur_command.Cmd.r2;//Для корректности отображения данных в r2 лежит тип данных 0 - беззнаковый,1 - знаковый,2 - вещественный,3
    switch (operand_size)//Загружаем значение в память, по адресу на который указывает r2
    {
        case 0: {
            if (type_for_display == 1) {
                std::cout << Cpu.RCU.RCU_16.at(Cpu.cur_command.Cmd.r1).i16;
            } else {
                std::cout << Cpu.RCU.RCU_16.at(Cpu.cur_command.Cmd.r1).ui16;
            }
            Cpu.psw.set_IP(Cpu.psw.get_IP() + 2);
            break;
        }
        case 1: {
            switch (type_for_display) {
                case 1: {
                    std::cout << Cpu.RCU.RCU_32.at(Cpu.cur_command.Cmd.r1).i32;
                    break;
                }
                case 2: {
                    std::cout << Cpu.RCU.RCU_32.at(Cpu.cur_command.Cmd.r1).f;
                    break;
                }
                default: {
                    std::cout << Cpu.RCU.RCU_32.at(Cpu.cur_command.Cmd.r1).u32;
                    break;
                }
            }
            Cpu.psw.set_IP(Cpu.psw.get_IP() + 4);
            break;
        }
        default: {
            break;
        }
    }
}