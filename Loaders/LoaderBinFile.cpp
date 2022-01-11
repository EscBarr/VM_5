//
// Created by timur on 10.01.2022.
//

#include "LoaderBinFile.h"

bool LoaderBinFile::Load(CPU &Cpu,uint32_t startAdr, int Num_File)
{
    std::ifstream Program_list(All_txt_founded[Num_File - 1]);
    if (!Program_list || Program_list.peek() == EOF) {
        std::cout << "Файл не найден или в нем нет элементов" << std::endl;
        return false;
    }
    uint32_t countData, movingTable, addr;
    Program_list.read((char*)&movingTable, sizeof(movingTable));
    Program_list.read((char*)&countData, sizeof(countData));
    Program_list.read((char*)&addr, sizeof(addr));
    Cpu.psw.set_IP(addr + startAdr);
    word data;
    uint32_t currAdress = startAdr;
    for (; currAdress < countData; currAdress += 1)
    {
        Program_list.read((char*)&data, sizeof(data));
        Cpu.RAM[currAdress] = data;
    }
    Command_register code;
    for (size_t i = countData; i < movingTable; i+=2)
    {
        Program_list.read((char*)&code.data.d32[0], sizeof(data));
        Program_list.read((char*)&code.data.d32[1], sizeof(data));
        for (int j = 0; j < 2; ++j)
        {
            Cpu.RAM[currAdress] = code.data.d32[j];
            currAdress++;
        }
        if (code.Cmd.s == 1 || (code.Cmd.code >= CPU::jmp && code.Cmd.code <= CPU::jmpNsf) || code.Cmd.code == CPU::mov_reg_mem || code.Cmd.code == CPU::mov_mem_reg || code.Cmd.code == CPU::mov_adr_reg)
        {
            Program_list.read((char*)&code.data.d32[2], sizeof(data));
            Program_list.read((char*)&code.data.d32[3], sizeof(data));
            for (int j = 2; j < 4; ++j)
            {
                Cpu.RAM[currAdress] = code.data.d32[j];
                currAdress++;
            }
            i+=2;
        }

    }
    code.Cmd.code = 0;
    for (int j = 0; j < 2; ++j)
    {
        Cpu.RAM[currAdress] = code.data.d32[j];
        currAdress++;
    }
    uint16_t movingName;
    while (Program_list.read((char*)&movingName, sizeof(movingName)))
    {
        Cpu.RAM[movingName + startAdr] += startAdr;
    }
    return true;

}

void LoaderBinFile::Scan_file_dir()
{
    for (const auto &entry : file_s::directory_iterator(file_s::current_path())) {
        if (entry.path().extension() == ".vm5") {
            All_txt_founded.push_back(entry.path().filename().string());
        }
    }


}

bool LoaderBinFile::Display_All_Bin() {
    if (All_txt_founded.size() == 0) {
        std::cout << "\nВ директории не найдены бинарные файлы для загрузки.\nЗавершение работы" << std::endl;
        return false;
    }
    std::cout << "\nВыберите файл для выполнения" << std::endl;
    for (size_t i = 0; i < All_txt_founded.size(); i++) {
        std::cout << i + 1 << "." << All_txt_founded[i] << std::endl;
    }
    return true;
}
