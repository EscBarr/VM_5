//
// Created by timur on 06.01.2022.
//

#pragma once
#include "../HeadersExtensions.h"
namespace file_s = std::filesystem;
class LoaderFromText {
  std::vector<std::string> All_txt_founded;
  void Scan_file_dir();
  bool Display_All_txt();
  bool Load(CPU &Cpu, int Num_File);

};


