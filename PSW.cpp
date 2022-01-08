//
// Created by timur on 10.09.2021.
//
#include "PSW.h"

address_t PSW::get_IP() {
  return IP;
}

uint16_t PSW::get_ZF() {
  return ZF;
}

uint16_t PSW::get_SF() {
  return SF;
}

uint16_t PSW::get_OF() {
  return OF;
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


