//
// Created by timur on 10.09.2021.
//

#ifndef VM_5_CPU_H
#define VM_5_CPU_H


#pragma once

#include "Types.h"
#include "Memory.h"
#include "PSW.h"
#include "Commands.h"

class CPU {
public:
    base_command_t cur_command;//Текущая выполняемая команда
    PSW psw; // PSW-IP + FLAGS
    Memory RAM;// Память
    registers RCU;// Регистры общего назначения
    CPU();

    CPU(const CPU &) = delete;

    CPU(const CPU &&) = delete;

    CPU &operator=(const CPU &) = delete;

    CPU &operator=(const CPU &&) = delete;

    void Load_cmd();

    void Check_reg(const bool &r1_or_r2);//проверка номера регистра для текущей выполняемой команды true - проаеряем r1
    void restart();//Перезагрузка
    void start();//Старт
    enum Op_codes : uint8_t {
        stop, //0
        mov_reg_reg, //1 пересылка регистр-регистр
        mov_reg_mem,//2 пересылка регистр-память
        mov_mem_reg,//3 пересылка память-регистр
        ///
        ///
        ///
        iadd,//4 сложение
        isub,//5 вычитание
        imult,//6 умножение
        idiv,//7 деление
        imod,//8 деление с остатком
        ///
        ///
        ///
        fadd,//9
        fsub,//10
        fmul,//12
        fdiv,//13
        ///
        input,//14
        out,//15
        ///
        ///
        ///
        cmp,//Сравнение целочисленных -16
        cmpF,//Сравнение вещественных-17
        ////
        jmp,//Прямой переход - 18
        jmpE,// если равно - 19
        jmpNE,// если не равно - 20
        jmpG,// если больше - 21
        jmpG_E,// если больше или равно - 22
        jmpA, //если больше (беззнаковое) - 23
        jmpA_E, //если больше или равно (беззнаковое) - 24
        jmpL,// если меньше - 25
        jmpL_E,// если меньше или равно - 26
        jmpB, // если меньше (беззнаковое) - 27
        jmpB_E, // если меньше или равно (беззнаковое) - 28
        jmpsf,//отрицательное - 29
        jmpNsf,//не отрицательное - 30
        call,//Заход в подпрограмму - 31
        Return,// возврат из подпрограммы в адрес лежащий в 255 адресном регистре - 32
        //
        //
//                iaddshort,//Короткое сложение - 33
//                isubshort,//Короткое вычитание -34
//                imultshort,//Короткое умножение - 35
//                idivshort,//Короткое деление - 36
//                imodshort,//Короткое деление с остатком - 37
        //
        //
        Bitwise_AND,//Битовое И - 38
        Bitwise_OR,//Битовое ИЛИ - 39
        Bitwise_XOR,//Битовое СЛОЖЕНИЕ ПО МОДУЛЮ 2 - 40
        Bitwise_NOT,//Битовое ОТРИЦАНИЕ - 41
        Bitwise_LEFT,//Битовый сдвиг влево на r2 бит - 42
        Bitwise_RIGHT,//Битовый сдвиг вправо на r2 бит - 43
        end
    };
private:
    void Initialize_Comands();

    Command *commands[end];//Массив указателей на команды
};


#endif //VM_5_CPU_H
