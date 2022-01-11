//
// Created by timur on 10.09.2021.
//

#include "Jump.h"

void JumpDirect::operator()(CPU &Cpu) noexcept {
    set_ip(Cpu);
}

void JumpDirect::set_ip(CPU &Cpu) {
  int8_t r1_temp = Cpu.cur_command.Cmd.r1;// по r1 тип перехода а по s прямой или относительный
  switch (r1_temp) {
	case 0://Безусловный прямой/относительный переход
	{
	  if (Cpu.cur_command.Cmd.s == 0) {
		Cpu.psw.set_IP(Cpu.cur_command.offset); // Для прямого перехода используется смещение в команде
	  } else {
		address_t New_adr = Cpu.psw.get_IP() + Cpu.cur_command.offset;
		Cpu.psw.set_IP(New_adr);
	  }
	  break;
	}
	case 1:// Безусловный косвенный
	{
	  Cpu.Check_reg(false);
	  if (Cpu.cur_command.Cmd.s == 0) {
		Cpu.psw.set_IP(Cpu.RCU.RCU_16[Cpu.cur_command.Cmd.r2].ui16); // Для прямого косвенного перехода адрес находится во втором регистре команды
	  } else {
		address_t New_adr = Cpu.psw.get_IP() + Cpu.RCU.RCU_16[Cpu.cur_command.Cmd.r2].ui16;
		Cpu.psw.set_IP(New_adr);
	  }
	  break;
	}
	  /*   case 2://относительный берем смещение из команды по r2 смотрим вперед или назад относительно текущего адреса
		 {
			 //r2 = 1 значит вычитаем из текущего адреса иначе прибавляем
			 address_t New_adr =
					 Cpu.cur_command.Cmd.r2 == 1 ? Cpu.psw.get_IP() - Cpu.cur_command.offset : Cpu.psw.get_IP() +
																							   Cpu.cur_command.offset;
			 Cpu.psw.set_IP(New_adr);
			 break;
		 }*/
    }
}

bool Coditional_jump::Equal(PSW Flags) {
    return Flags.get_ZF();
}

bool Coditional_jump::Lesser(PSW Flags) {
    return (Flags.get_SF() != Flags.get_OF());
}

void Coditional_jump::operator()(CPU &Cpu) noexcept {
    uint8_t code_temp = Cpu.cur_command.Cmd.code;
    switch (code_temp) {
        case CPU::jmpE: {
            if (Equal(Cpu.psw)) {
                set_ip(Cpu);
            }
            else
            {
                Cpu.psw.set_IP(Cpu.psw.get_IP() + 4);
            }
            break;
        }
        case CPU::jmpNE: {
            if (!Equal(Cpu.psw)) {
                set_ip(Cpu);
            }
            else
            {
                Cpu.psw.set_IP(Cpu.psw.get_IP() + 4);
            }
            break;
        }
        case CPU::jmpG: {
            if (!(Lesser(Cpu.psw) || Equal(Cpu.psw))) {
                set_ip(Cpu);
            }
            else
            {
                Cpu.psw.set_IP(Cpu.psw.get_IP() + 4);
            }
            break;
        }
        case CPU::jmpG_E: {
            if (!Lesser(Cpu.psw) || Equal(Cpu.psw)) {
                set_ip(Cpu);
            }
            else
            {
                Cpu.psw.set_IP(Cpu.psw.get_IP() + 4);
            }
            break;
        }
        case CPU::jmpL: {
            if (Lesser(Cpu.psw)) {
                set_ip(Cpu);
            }
            else
            {
                Cpu.psw.set_IP(Cpu.psw.get_IP() + 4);
            }
            break;
        }
        case CPU::jmpL_E: {
            if (Lesser(Cpu.psw) || Equal(Cpu.psw)) {
                set_ip(Cpu);
            }
            else
            {
                Cpu.psw.set_IP(Cpu.psw.get_IP() + 4);
            }
            break;
        }
        case CPU::jmpsf: {
            if (Cpu.psw.get_SF()) {
                set_ip(Cpu);
            }
            else
            {
                Cpu.psw.set_IP(Cpu.psw.get_IP() + 4);
            }
            break;
        }
        case CPU::jmpNsf: {
            if (!(Cpu.psw.get_SF())) {
                set_ip(Cpu);
            }
            else
            {
                Cpu.psw.set_IP(Cpu.psw.get_IP() + 4);
            }
            break;
        }
    }
}

void Call::operator()(CPU &Cpu) noexcept {
    Cpu.RCU.RCU_16.at(15).ui16 =
            Cpu.psw.get_IP()+2;//Сохраняем адрес после захода в подпрограмму в 16 регистр общего назначения
    Cpu.Check_reg(false);//проверяем на правильность поле с вторым адресным регистром
    //В смещении команды хранится адрес начала подпрограммы
    Cpu.psw.set_IP(Cpu.cur_command.offset);
}

void Ret::operator()(CPU &Cpu) noexcept {
  Cpu.psw.set_IP(Cpu.RCU.RCU_16.at(15).ui16);//В 16 регистре общего назначения лежит точка возврата
}