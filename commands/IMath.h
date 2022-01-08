//
// Created by timur on 08.10.2021.
//

#ifndef VM_5_IMATH_H
#define VM_5_IMATH_H

#include "../Types.h"
#include "../Commands.h"
#include "../CPU.h"
#include <functional>
#include <utility>
//=========================================================
//================Целочисленная арифметика=================

//=================Знаковая,безнаковая=====================

class IntArithmetic : public Command //Главный прототип команды с выставлением флагов
{
public:
    explicit IntArithmetic(std::function<int64_t(int64_t, int64_t)> operation) : run(std::move(operation)) {}

    void operator()(CPU &Cpu) noexcept;

protected:
    std::function<int64_t(int64_t, int64_t)> run;

    void change_flags(CPU &Cpu, uint64_t result) noexcept;

    void set_ZF(CPU &Cpu, uint64_t result) noexcept;

    void set_SF(CPU &Cpu, uint64_t result) noexcept;

    void set_OF(CPU &Cpu, uint64_t result) noexcept;


};

//class IntAdd : public IntArithmetic
//        {
//        public:
//            IntAdd(std::function<int64_t (int64_t ,int64_t)> operation) : IntArithmetic(std::move(operation)) {}
//            void operator()(CPU& Cpu) noexcept
//            {
//                int16_t Arithmetic_type = Cpu.cur_command.Cmd.code;//
//                switch (Arithmetic_type)
//                {
//                    case CPU::iaddshort: // Короткая
//                    {
//                        uint64_t result = run(Cpu.RCU.RCU_16.at(Cpu.cur_command.Cmd.r1).ui16, Cpu.RCU.RCU_16.at(Cpu.cur_command.Cmd.r2).ui16);
//                        change_flags(Cpu, result);
//                        Cpu.RCU.RCU_16.at(Cpu.cur_command.Cmd.r1).ui16 = result;
//                        break;
//                    }
//                    default:
//                        uint64_t result = run(Cpu.RCU.RCU_32.at(Cpu.cur_command.Cmd.r1).u32, Cpu.RCU.RCU_32.at(Cpu.cur_command.Cmd.r2).u32);
//                        change_flags(Cpu, result);
//                        Cpu.RCU.RCU_32.at(Cpu.cur_command.Cmd.r1).u32 = result;
//                        break;
//                }
//            }
//        };

//class IntSub : public IntArithmetic
//        {
//    IntSub(std::function<int64_t (int64_t ,int64_t)> operation) : IntArithmetic(std::move(operation)) {}
//        public:
//            virtual void operator()(CPU& Cpu) noexcept//Для сравнений
//            {
//                int16_t Arithmetic_type = Cpu.cur_command.Cmd.code;//
//                switch (Arithmetic_type)
//                {
//                    case CPU::isubshort: // Короткая
//                    {
//                        uint64_t result = run(Cpu.RCU.RCU_16.at(Cpu.cur_command.Cmd.r1).ui16, Cpu.RCU.RCU_16.at(Cpu.cur_command.Cmd.r2).ui16);
//                        change_flags(Cpu, result);
//                        Cpu.RCU.RCU_16.at(Cpu.cur_command.Cmd.r1).ui16 = result;
//                        break;
//                    }
//                    default:
//                        uint64_t result = run(Cpu.RCU.RCU_32.at(Cpu.cur_command.Cmd.r1).u32, Cpu.RCU.RCU_32.at(Cpu.cur_command.Cmd.r2).u32);
//                        change_flags(Cpu, result);
//                        Cpu.RCU.RCU_32.at(Cpu.cur_command.Cmd.r1).u32 = result;
//                        break;
//                }
//            }
//        };

//class IntMult : public IntArithmetic
//        {
//    IntMult(std::function<int64_t (int64_t ,int64_t)> operation) : IntArithmetic(std::move(operation)) {}
//        public:
//            void operator()(CPU& Cpu) noexcept
//            {
//                int16_t Arithmetic_type = Cpu.cur_command.Cmd.code;//
//                switch (Arithmetic_type)
//                {
//                    case CPU::imultshort: // Короткая
//                    {
//                        uint64_t result = run(Cpu.RCU.RCU_16.at(Cpu.cur_command.Cmd.r1).ui16, Cpu.RCU.RCU_16.at(Cpu.cur_command.Cmd.r2).ui16);
//                        change_flags(Cpu, result);
//                        Cpu.RCU.RCU_16.at(Cpu.cur_command.Cmd.r1).ui16 = result;
//                        break;
//                    }
//                    default:
//                        uint64_t result = run(Cpu.RCU.RCU_32.at(Cpu.cur_command.Cmd.r1).u32, Cpu.RCU.RCU_32.at(Cpu.cur_command.Cmd.r2).u32);
//                        change_flags(Cpu, result);
//                        Cpu.RCU.RCU_32.at(Cpu.cur_command.Cmd.r1).u32 = result;
//                        break;
//                }
//            }
//        };

//class IntDiv : public IntArithmetic
//        {
//    IntDiv(std::function<int64_t (int64_t ,int64_t)> operation) : IntArithmetic(std::move(operation)) {}
//        public:
//            void operator()(CPU& Cpu) noexcept
//            {
//                int16_t Arithmetic_type = Cpu.cur_command.code;//
//                switch (Arithmetic_type)
//                {
//                    case CPU::idivshort: // Короткая
//                    {
//                        uint64_t result = run(Cpu.get_adr_r1(), Cpu.get_adr_r3());
//                        change_flags(Cpu, result);
//                        Cpu.adr_registers[Cpu.cur_command.r1] = result;
//                        break;
//                    }
//                    default:
//                        uint64_t result = run(Cpu.RAM[Cpu.get_adr_r2()].data.ui32, Cpu.RAM[Cpu.get_adr_r3()].data.ui32);
//                        change_flags(Cpu, result);
//                        Cpu.RAM[Cpu.get_adr_r1()].data.ui32 = result;
//                        break;
//                }
//            }
//        };

//class IntMod : public IntArithmetic
//        {
//        public:
//            uint64_t run(uint32_t left, uint32_t right)
//            {
//                return left % right;
//            }
//            void operator()(CPU& Cpu) noexcept
//            {
//                int16_t Arithmetic_type = Cpu.cur_command.code;//
//                switch (Arithmetic_type)
//                {
//                    case CPU::imodshort: // Короткая
//                    {
//                        uint64_t result = run(Cpu.get_adr_r1(), Cpu.get_adr_r3());
//                        change_flags(Cpu, result);
//                        Cpu.adr_registers[Cpu.cur_command.r1] = result;
//                        break;
//                    }
//                    default:
//                        uint64_t result = run(Cpu.RAM[Cpu.get_adr_r2()].data.ui32, Cpu.RAM[Cpu.get_adr_r3()].data.ui32);
//                        change_flags(Cpu, result);
//                        Cpu.RAM[Cpu.get_adr_r1()].data.ui32 = result;
//                        break;
//                }
//            }
//        };

#endif //VM_5_IMATH_H
