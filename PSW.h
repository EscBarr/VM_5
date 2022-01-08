//
// Created by timur on 10.09.2021.
//

#ifndef VM_5_PSW_H
#define VM_5_PSW_H

#include "Types.h"

class PSW {
  address_t IP{0}; //Instruction Pointer
  uint16_t ZF: 1;  //Флаг нуля
  uint16_t SF: 1;  //Флаг знака
  uint16_t OF: 1; //Флаг переполнения
 public:
  //==Геттеры==
  address_t get_IP();

  uint16_t get_ZF();

  uint16_t get_SF();

  uint16_t get_OF();


    //==Сеттеры==
    void set_IP(address_t Ip);

    void set_ZF(uint16_t ZF);

    void set_SF(uint16_t SF);

    void set_OF(uint16_t OF);

};


#endif //VM_5_PSW_H
