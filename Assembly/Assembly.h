//
// Created by timur on 07.01.2022.
//

#pragma once
#include "AssemblyTypes.h"
class Assembly {
  std::unordered_map<std::string, std::tuple<int, int>>
	  Commands = { // Команда асм, код операции в интерпретаторе и тип команды
	  {"movRR", std::tuple<int, int>(CPU::mov_reg_reg, CommandList::move_reg_reg)},
	  {"movRM", std::tuple<int, int>(CPU::mov_reg_mem, CommandList::move_reg_mem)},
	  {"movMR", std::tuple<int, int>(CPU::mov_mem_reg, CommandList::move_mem_reg)},
	  {"movАR", std::tuple<int, int>(CPU::mov_adr_reg, CommandList::move_adr_reg)},
//  	{"lea",std::tuple<int,int>(CPU::move,CommandList::Lea)},
	  {"add", std::tuple<int, int>(CPU::iadd, CommandList::math)},
	  {"sub", std::tuple<int, int>(CPU::isub, CommandList::math)},
	  {"mult", std::tuple<int, int>(CPU::imult, CommandList::math)},
	  {"div", std::tuple<int, int>(CPU::idiv, CommandList::math)},
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
  std::unordered_map<std::string, uint16_t> RCU32 = {{"lr1", 1}, {"lr2", 2}, {"lr3", 3}, {"lr4", 4},
													 {"lr5", 5}, {"lr6", 6}, {"lr7", 7}, {"lr8", 8}};
  // 0,если не существует
  std::unordered_map<std::string, uint16_t> RCU16 = {{"r1", 1}, {"r2", 2}, {"r3", 3}, {"r4", 4},
													 {"r5", 5}, {"r6", 6}, {"r7", 7}, {"r8", 8}, {"r9", 9}, {"r10", 10},
													 {"r11", 11}, {"r12", 12}, {"r13", 13}, {"r14", 14}, {"r15", 15},
													 {"r16", 16}};
  uint32_t globalAddress = 0; // Глобальный адрес для трансляции имен
  uint32_t startLabel = 0; // Адрес стартового входа в программу
  std::unordered_map<std::string, NameTableCell> NameTable; // Таблица имен
  std::vector<word_t> TranslatorMemory; // Буфер трансляции программы
  std::vector<uint16_t> tableMovingName; // Таблица перемещающихся имен
  // Запись в буфер
  void WriteMem(std::vector<word_t> value) {
	for (int i = 0; i < value.size(); ++i) {
	  TranslatorMemory.push_back(value[i]);
	  globalAddress++;
	}
  }
  //Запись перемещающегося имени
  void AddMovingName(size_t address) { tableMovingName.push_back(address); }
  //Тип операнда
  Operands Type(std::string str);
  // Парсинг операндов
  std::vector<std::string> ParseArguments(std::string str);
  // Загрузка адреса
  void Lea(uint16_t code, std::string arguments);
  // Перемещение
  void Move(uint16_t code, std::string arguments);
  // Переходы
  void Jump(uint16_t code, std::string arguments);
  //Арифметика
  void Math(uint16_t code, std::string arguments);
  // Ввод-вывод
  void IO(uint16_t code, std::string str);
  // Парсингш команды
  void ParseCommand(std::tuple<int, int> cmd, std::string arguments);
  // Первый проход
  void FirstTranslate(std::stringstream &istr);
  // Первый проход секции кода
  void ParseCodeSection(std::stringstream &istr);
  // Первый проход в секции данных
  void ParseDataSection(std::stringstream &istr);
  // Запись буфера в бинарнвй файл
  void WriteBinFile(std::string filename);
  // Парсинг одной записи(команды)
  void Parse(std::stringstream &istr);
 public:
  // Трансляция програмы
  void Translate(std::string filename);
  Assembly() {}
  ~Assembly() {}

};


