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
    uint16_t CF: 1; //Флаг переноса(остатка)
public:
    //==Геттеры==
    const address_t &get_IP() const;

    const uint16_t &get_ZF() const;

    const uint16_t &get_SF() const;

    const uint16_t &get_OF() const;

    const uint16_t &get_CF() const;

    //==Сеттеры==
    void set_IP(address_t Ip);

    void set_ZF(uint16_t ZF);

    void set_SF(uint16_t SF);

    void set_OF(uint16_t OF);

    void set_CF(uint16_t CF);
};


#endif //VM_5_PSW_H
