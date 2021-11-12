//
// Created by timur on 10.09.2021.
//
#include "PSW.h"

const address_t &PSW::get_IP() const {
    return IP;
}

const uint16_t &PSW::get_ZF() const {
    return ZF;
}

const uint16_t &PSW::get_SF() const {
    return SF;
}

const uint16_t &PSW::get_OF() const {
    return OF;
}

const uint16_t &PSW::get_CF() const {
    return CF;
}

void PSW::set_IP(address_t Ip) {
    IP = Ip;
}

void PSW::set_ZF(uint16_t Zf) {
    ZF = Zf;
}

void PSW::set_SF(uint16_t Sf) {
    SF = Sf;
}

void PSW::set_OF(uint16_t Of) {
    OF = Of;
}

void PSW::set_CF(uint16_t Cf) {
    CF = Cf;
}

