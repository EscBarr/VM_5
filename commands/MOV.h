//
// Created by timur on 14.09.2021.
//

#ifndef VM_5_MOV_H
#define VM_5_MOV_H


#include "../Commands.h"
#include "../CPU.h"

class MOV_reg_reg : public Command {
    void operator()(CPU &Cpu) noexcept override;
};

class MOV_reg_mem : public MOV_reg_reg {
  void operator()(CPU &Cpu) noexcept override;
};

class MOV_mem_reg : public MOV_reg_reg {
  void operator()(CPU &Cpu) noexcept override;
};

class MOV_adr_reg : public MOV_reg_reg {
  void operator()(CPU &Cpu) noexcept override;
};

#endif //VM_5_MOV_H

