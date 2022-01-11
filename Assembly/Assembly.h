//
// Created by timur on 07.01.2022.
//

#pragma once
#include "AssemblyTypes.h"
#include "../Parsers/MathParser.h"
#include "../Automats/FloatParser.h"
class Assembly {
  std::unordered_map<std::string, std::tuple<int, int>>
	  Commands = { // Команда ассемблера, код операции в интерпретаторе и тип команды по аргументам
	  {"movRR", std::tuple<int, int>(CPU::mov_reg_reg, CommandList::move_reg_reg)},
	  {"movRM", std::tuple<int, int>(CPU::mov_reg_mem, CommandList::move_reg_mem)},
	  {"movMR", std::tuple<int, int>(CPU::mov_mem_reg, CommandList::move_mem_reg)},
	  {"movAR", std::tuple<int, int>(CPU::mov_adr_reg, CommandList::move_adr_reg)},
	  {"add", std::tuple<int, int>(CPU::iadd, CommandList::math)},
	  {"sub", std::tuple<int, int>(CPU::isub, CommandList::math)},
	  {"mult", std::tuple<int, int>(CPU::imult, CommandList::math)},
	  {"div", std::tuple<int, int>(CPU::idiv, CommandList::math)},
	  {"mod", std::tuple<int, int>(CPU::imod, CommandList::math)},
	  {"fadd", std::tuple<int, int>(CPU::fadd, CommandList::math)},
	  {"fsub", std::tuple<int, int>(CPU::fsub, CommandList::math)},
	  {"fmult", std::tuple<int, int>(CPU::fmul, CommandList::math)},
	  {"fdiv", std::tuple<int, int>(CPU::fdiv, CommandList::math)},
	  {"and", std::tuple<int, int>(CPU::Bitwise_AND, CommandList::math)},
	  {"or", std::tuple<int, int>(CPU::Bitwise_OR, CommandList::math)},
	  {"not", std::tuple<int, int>(CPU::Bitwise_NOT, CommandList::math)},
	  {"xor", std::tuple<int, int>(CPU::Bitwise_XOR, CommandList::math)},
	  {"shl", std::tuple<int, int>(CPU::Bitwise_LEFT, CommandList::math)},
	  {"shr", std::tuple<int, int>(CPU::Bitwise_RIGHT, CommandList::math)},
	  {"input", std::tuple<int, int>(CPU::input, CommandList::io)},
	  {"output", std::tuple<int, int>(CPU::out, CommandList::io)},
	  {"cmp", std::tuple<int, int>(CPU::cmp, CommandList::cmp)},
	  {"fcmp", std::tuple<int, int>(CPU::cmpF, CommandList::cmp)},
	  {"jmp", std::tuple<int, int>(CPU::jmp, CommandList::jmp)},
	  {"jmpe", std::tuple<int, int>(CPU::jmpE, CommandList::jmp)},
	  {"jmpne", std::tuple<int, int>(CPU::jmpNE, CommandList::jmp)},
	  {"jmpl", std::tuple<int, int>(CPU::jmpL, CommandList::jmp)},
	  {"jmple", std::tuple<int, int>(CPU::jmpL_E, CommandList::jmp)},
	  {"jmpg", std::tuple<int, int>(CPU::jmpG, CommandList::jmp)},
	  {"jmpge", std::tuple<int, int>(CPU::jmpG_E, CommandList::jmp)},
	  {"call", std::tuple<int, int>(CPU::call, CommandList::call)},
	  {"ret", std::tuple<int, int>(CPU::Return, CommandList::other)}
  };

  // 0,если не существует
  std::unordered_map<std::string, uint16_t> RCU16 = {{"r1", 0}, {"r2", 1}, {"r3", 2}, {"r4", 3},
                                                     {"r5", 4}, {"r6", 5}, {"r7", 6}, {"r8", 7}, {"r9", 8}, {"r10", 9},
                                                     {"r11", 10}, {"r12", 11}, {"r13", 12}, {"r14", 13}, {"r15", 14},
                                                     {"r16", 15}};

  // 0,если не существует
  std::unordered_map<std::string, uint32_t> RCU32 = {{"lr1", 0}, {"lr2", 1}, {"lr3", 2}, {"lr4", 3},
                                                     {"lr5", 4}, {"lr6", 5}, {"lr7", 6}, {"lr8", 7}};
  uint32_t globalAddress = 0; // Глобальный счетчик адреса для трансляции имен
  uint32_t startLabel = 0; // Адрес начала трансляции команд для интерпретатора
  uint32_t CntrData = 0;//Счетчик байт для секции данных
  uint32_t CntrCode = 0;//Счетчик байт для секции кода
  std::unordered_map<std::string, NameTableCell> Table_Of_Names; // Таблица имен названия переменных
  std::vector<word> InterpreterMemory; // Буфер трансляции команд программы в интерпретатор
  std::vector<uint16_t> tableMovingName; // Таблица перемещающихся имен
  std::string PathToDirectory;
  // Запись в буфер
  void WriteMem(std::vector<word> value)
  {
	for (int i = 0; i < value.size(); ++i)
	{
	  InterpreterMemory.push_back(value[i]);
	  globalAddress++;
	}
  }
  //Запись перемещающегося имени
  void AddMovingName(uint16_t address) { tableMovingName.push_back(address); }
  //Тип операнда
  Operands Type(std::string str);
  // Парсинг операндов
  std::vector<std::string> ParseArguments(std::string str);
  // Перемещение (Регистр - регистр)
  void MoveRR(uint16_t code, std::string arguments);
  // Перемещение (Регистр - память)
  void MoveRM(uint16_t code, std::string arguments);
  // Перемещение (Регистр - память)
  void MoveMR(uint16_t code, std::string arguments);
  // Перемещение (Загрузка адреса в регистр)
  void MoveAR(uint16_t code, std::string arguments);
  // Переходы
  void Jump(uint16_t code, std::string arguments);
  //Арифметика
  void Math(uint16_t code, std::string arguments);
  // Ввод-вывод
  void IO(uint16_t code, std::string str);
  // Сравнение
  void Cmp(uint16_t code, std::string arguments);
  // Вызов подпрограммы
  void Call(uint16_t code, std::string arguments);
  // Вычленение типа команды и вызов соответствующей функции для записи в буфер памяти команд
  void ParseCommand(std::tuple<int, int> cmd, std::string arguments);
  // Первый проход
  void FirstPass(std::stringstream &istr);
  // Первый проход секции кода
  void ParseCodeSection(std::stringstream &istr);
  // Первый проход в секции данных
  void ParseDataSection(std::stringstream &istr);
  // Запись буфера в бинарный/текстовый файл
  void WriteBufferFile(std::string filename);
  // Парсинг одной записи(команды)
  void SecondPass(std::stringstream &istr);
  //Проверка являются ли операнды одним типом
  bool Check_Operands(std::vector<std::string> args);
  //Проверка размера регистров
  bool Check_RegSize( std::vector<std::string> args);
  //Проверка размера регистра
  bool Check_RegSizeSingle( std::vector<std::string> args,bool first_second);
 public:
  // Трансляция программы
  void Translate(std::string filename);
  Assembly() {}
  ~Assembly() {}

};


