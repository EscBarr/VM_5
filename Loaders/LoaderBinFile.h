//
// Created by timur on 10.01.2022.
//

#pragma once
#include "../HeadersExtensions.h"
namespace file_s = std::filesystem;
class LoaderBinFile {
public:
    std::vector<std::string> All_txt_founded;
    void Scan_file_dir();
    bool Display_All_Bin();
    bool Load(CPU &Cpu,uint32_t startAdr, int Num_File);
};



