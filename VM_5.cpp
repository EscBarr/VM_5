#include <iostream>
#include "array"
#include "./Parsers/MathParser.h"
#include "CPU.h"
#include "./Assembly/Assembly.h"
#include <Windows.h>
#include "./Loaders/LoaderBinFile.h"
//int errors;
//void test(const char* input, double expected) {
//    try {
//        MathParser p;
//        auto result =  p.evalExpression(input);;
//        if (result == expected) return;
//        std::cout << input << " = " << expected << " : error, got " << result << '\n';
//    }
//    catch (std::exception& e) {
//        std::cout << input << " : exception: " << e.what() << '\n';
//    }
//    ++errors;
//}
//    base_command_t temp;
//    temp.Cmd.code = 2;
//    temp.Cmd.s = 1;
//    temp.Cmd.r1 = 0;
//    temp.Cmd.r2 = 15;
//    std::cout << sizeof(registers);
//    test("1", 1);
//    test("(1)", 1);
//test("2*(1+20)", 42);
namespace file_s = std::filesystem;
std::vector<std::string> Scan_file_dir()
{
    std::vector<std::string> All_txt_founded;
    for (const auto& entry : file_s::directory_iterator(file_s::current_path()))
    {

        if (entry.path().extension() == ".txt")
        {
            All_txt_founded.push_back(entry.path().filename().string());
        }
    }
    return All_txt_founded;
};
bool Display_All_txt(std::vector<std::string> txt_list)
{
    if (txt_list.size()==0)
    {
        std::cout << "\nВ директории не найдены файлы для загрузки.\nЗавершение работы" << std::endl;
        return false;
    }
    std::cout << "\nВыберите файл для выполнения" << std::endl;
    for (size_t i = 0; i < txt_list.size(); i++)
    {
        std::cout << i + 1 << "." << txt_list[i] << std::endl;
    }
    return true;
}

int main()
{

    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);


    CPU Cpu;
    std::vector<std::string> All_txt_founded = Scan_file_dir();
    LoaderBinFile Temp;
    int16_t program_name;
    Assembly temp;

    try
    {
        Display_All_txt(All_txt_founded);
        std::cin >> program_name;
        temp.Translate(All_txt_founded[program_name-1]);
        Temp.Scan_file_dir();
        Temp.Display_All_Bin();
        std::cin >> program_name;
        Temp.Load(Cpu,0,program_name);
        Cpu.start();
    }
    catch (const std::exception& ex)
    {
        std::cout << ex.what() << std::endl;
    }



    std::cout<<"\n";
    system("pause");
    return 0;
}
