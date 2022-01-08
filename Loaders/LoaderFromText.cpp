//
// Created by timur on 06.01.2022.
//

#include "LoaderFromText.h"
void LoaderFromText::Scan_file_dir() {

  for (const auto &entry : file_s::directory_iterator(file_s::current_path())) {
	if (entry.path().extension() == ".txt") {
	  All_txt_founded.push_back(entry.path().filename().string());
	}
  }

}

bool check_command_length(std::string words) {
  std::string word;
  uint16_t count{0};
  std::istringstream temp(words);
  while (temp >> word) {
	count++;
  }
  return count > 3 ? true : false;
}

bool LoaderFromText::Display_All_txt() {
  if (All_txt_founded.size() == 0) {
	std::cout << "\nВ директории не найдены текстовые файлы для загрузки.\nЗавершение работы" << std::endl;
	return false;
  }
  std::cout << "\nВыберите файл для выполнения" << std::endl;
  for (size_t i = 0; i < All_txt_founded.size(); i++) {
	std::cout << i + 1 << "." << All_txt_founded[i] << std::endl;
  }

}
bool LoaderFromText::Load(CPU &Cpu, int Num_File) {
  std::ifstream Program_list(All_txt_founded[Num_File - 1]);
  if (!Program_list || Program_list.peek() == EOF) {
	std::cout << "Файл не найден или в нем нет элементов" << std::endl;
	return false;
  }
  int32_t Count_Lines{1};
  std::string Cur_Command;
  address_t start_point{0};
  bool start_was_used = false;
  char Token = ' ';
  while (std::getline(Program_list, Cur_Command)) {
	Cur_Command = Cur_Command.substr(0, Cur_Command.find(';'));//удаление комментария
	std::istringstream SplitTokens(Cur_Command);
	SplitTokens >> Token;
	switch (Token) {
	  case 's': {
		SplitTokens >> start_point;
		break;
	  }
	  case 'f': {
		datatype32 Value;
		SplitTokens >> Value.f;
		for (int i = 0; i < 4; ++i) {
		  Cpu.RAM[start_point] = Value.d32[i];
		  start_point++;
		}
		break;
	  }
	  case 'u': {
		char Size = ' ';
		SplitTokens >> Size;
		if (Size == 'l') {
		  datatype32 Value;
		  SplitTokens >> Value.u32;
		  for (int i = 0; i < 4; ++i) {
			Cpu.RAM[start_point] = Value.d32[i];
			start_point++;
		  }
		} else if (Size == 's') {
		  datatype16 Value;
		  SplitTokens >> Value.ui16;
		  for (int i = 0; i < 2; ++i) {
			Cpu.RAM[start_point] = Value.d16[i];
			start_point++;
		  }

		} else {
		  throw std::invalid_argument("Неизвестная операция в строке номер " + std::to_string(Count_Lines));
		}
		break;
	  }
	  case 'i': {
		char Size = ' ';
		SplitTokens >> Size;
		if (Size == 'l') {
		  datatype32 Value;
		  SplitTokens >> Value.i32;
		  for (int i = 0; i < 4; ++i) {
			Cpu.RAM[start_point] = Value.d32[i];
			start_point++;
		  }
		} else if (Size == 's') {
		  datatype16 Value;
		  SplitTokens >> Value.i16;
		  for (int i = 0; i < 2; ++i) {
			Cpu.RAM[start_point] = Value.d16[i];
			start_point++;
		  }

		} else {
		  throw std::invalid_argument("Неизвестная операция в строке номер " + std::to_string(Count_Lines));
		}
		break;
	  }
	  case 'c': {
		Command_register Temp{};
		uint16_t r1, r2, code, s;
		std::string temp = SplitTokens.str();//копия строки с командой
		SplitTokens >> code >> s >> r1 >> r2;
		Temp.Cmd.code = code;
		Temp.Cmd.s = s;
		Temp.Cmd.r1 = r1;
		Temp.Cmd.r2 = r2;
		for (int i = 0; i < 2; ++i)//запись первых двух байтов
		{
		  Cpu.RAM[start_point] = Temp.data.d32[i];
		  start_point++;
		}
		if (check_command_length(temp)) {
		  uint16_t off;
		  SplitTokens >> off;
		  Temp.offset = off;
		  for (int i = 2; i < 4; ++i)//запись последних двух байтов со смещением
		  {
			Cpu.RAM[start_point] = Temp.data.d32[i];
			start_point++;
		  }
		}
		break;
	  }
	  default: {
		throw std::invalid_argument("Неизвестная операция в строке номер " + std::to_string(Count_Lines));
		break;
	  }
	}
	Count_Lines++;
  }
  return false;
}
