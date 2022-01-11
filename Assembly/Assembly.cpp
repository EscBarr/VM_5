//
// Created by timur on 07.01.2022.
//

#include "Assembly.h"

void RemoveComment(std::string& str)
{
    str = str.substr(0, str.find(';'));//удаление комментария
}


void Assembly::ParseDataSection(std::stringstream &istrstream)
{

    std::unordered_map<std::string, int> type = {
            {"int16", Int16},{"int32", Int32}, {"float", Float} };

    std::streampos position;
    std::string str;
    std::getline(istrstream, str, '\n');
    RemoveComment(str);

    while (str.find(".code") == std::string::npos && !istrstream.eof())
    {
        std::stringstream tempIStr(str);
        std::vector<std::string> tempDataArray;
        //Разбор строки на элементы
        for(size_t i = 0; !tempIStr.eof(); ++i)
        {
            std::string tempStr;
            tempIStr >> tempStr;
            if (!tempStr.empty())
                tempDataArray.push_back(tempStr);
        }
        if (tempDataArray.size() == 3)
        {
            if (Type(tempDataArray[0]) != Operands::var)
                throw std::logic_error(tempDataArray[0] + " неверный формат имени переменной");
            if (NameTable.find(tempDataArray[0]) != NameTable.end())
                throw std::logic_error(tempDataArray[0] + " уже определена");
            if (type.find(tempDataArray[1]) == type.end())
                throw std::logic_error("Нераспознанный тип");
            NameTableCell variable = { static_cast<uint16_t>(type[tempDataArray[1]]), static_cast<uint16_t>(globalAddress)  };
            if (type[tempDataArray[1]] == DataType::Int16)//Тип = Целый короткий
            {
                IntegerParser myAtoi;
                int tempResult;
                if (myAtoi.InitializeNum(tempDataArray[2], tempResult))
                {
                    datatype16 data;
                    data.ui16 = tempResult;
                    variable.Val16 = data;
                    globalAddress += 2;
                    CntrData += 2;
                }
                else
                    throw std::logic_error(tempDataArray[2] + " Нераспознанное значение");
            }
            else if (type[tempDataArray[1]] == DataType::Int32)
            {
                IntegerParser myAtoi;
                int tempResult;
                if (myAtoi.InitializeNum(tempDataArray[2], tempResult))
                {
                    datatype32 data;
                    data.u32 = tempResult;
                    variable.Val32 = data;
                    globalAddress += 4;
                    CntrData += 4;
                }
                else
                    throw std::logic_error(tempDataArray[2] + " Нераспознанное значение");
            }
            else if (type[tempDataArray[1]] == DataType::Float)
            {//Тип = Дробное
                datatype32 data;
                FloatParser myAtoi;
                data.f = atof(tempDataArray[2].c_str());
                variable.Val32 = data;
                globalAddress += 4;
                CntrData += 4;
            }

            NameTable.insert(std::make_pair(tempDataArray[0], variable)); // Добавить метку в таблицу имен
        }
        else
            throw std::logic_error("Ожидалось минимум 3 аргумента");
        position = istrstream.tellg();
        std::getline(istrstream, str, '\n');
        RemoveComment(str);
    }
    if(str.find(".code") != std::string::npos)
        istrstream.seekg(position);
}

Operands Assembly::Type(std::string str) {
    std::string action = "+-/*";
    if (RCU16.find(str) != RCU16.end())
    {
        return Operands::reg;
    }
    if (RCU32.find(str) != RCU32.end())
    {
        return Operands::lreg;
    }
    // Косвенное обращение [Регистр + смещение]
    if (str.find('[') != std::string::npos and str.find(']') != std::string::npos)
    {
        return Operands::indirect;
    }
    // Косвенное обращение,без [] -- Регистр + смещение
    int indexReg = -1;
    indexReg = str.find("lr");
    if (indexReg != std::string::npos)
    {
        if (RCU32.find("lr" + str[indexReg + 1]) != RCU32.end()) // Проверка существует ли такой 32 битный регистр
            {
            if (str.find('+') != std::string::npos or
            str.find('-') != std::string::npos)
            {
                return Operands::indirect;
            }
            }
        else
        {
            throw std::invalid_argument("Регистра с таким названием не существует");
        }
    }
    else
    {
        indexReg = str.find("r");
        if (indexReg != std::string::npos)
        {
            if (RCU16.find("r" + str[indexReg + 1]) != RCU16.end()) // Проверка существует ли такой 16 битный регистр
                {
                if (str.find('+') != std::string::npos or
                str.find('-') != std::string::npos)
                {
                    return Operands::indirect;
                }
                }
            else
            {
                throw std::invalid_argument("Регистра с таким названием не существует");
            }
        }
    }
    // Арифметика
    for(char ch : action)
    {
        if (str.find(ch) != std::string::npos)
        {
            return Operands::cnst;
        }
    }
    // Числовая константа
    int resTemp;
    IntegerParser intPar;
    if (intPar.InitializeNum(str, resTemp))
    {
        return Operands::cnst;
    }
    // Переменная
    bool variable = true;
    if (isdigit(str[0])) variable = false;
    for (size_t i = 0; i < str.length() && variable; i++)
    {
        if (!isalpha(str[i]) && !isdigit(str[i]) && str[i] != '_')
            variable = false;
    }
    //if (NameTable.find(str) == NameTable.end()) // Проверка присутствует ли такая переменная
    if (variable) return var;
    if(str.find('#') != std::string::npos) return special;
    throw std::logic_error( str + " Неизвестный тип аргумента");

}

std::vector<std::string> Assembly::ParseArguments(std::string str) {
    str.erase(remove_if(str.begin(), str.end(), isspace), str.end()); // Удаление пробелов
    std::vector<std::string> args = split(str, ','); // Разделить строку по ','
    return args;
}

void Assembly::ParseCodeSection(std::stringstream &istr) {
    bool isstartLabel = false;
    std::string nameStartLabel;
    std::string str;
    istr >> str;
    while (str != ".data" && !istr.eof())
    {
        if (str.find(';') != std::string::npos) // Пропуск комментария
            {
            std::getline(istr, str,'\n');
            }
        else
        {
                int indexEndLabel = str.find(':'); // Метка
                if (indexEndLabel != std::string::npos) // Добавление метки в таблицу имен
                    {
                    str = str.substr(0, indexEndLabel);
                    NameTableCell label = { 0,static_cast<uint16_t>(globalAddress) };
                    if (NameTable.find(str) != NameTable.end())
                        throw std::logic_error(str + "метка с таким именем уже определена");
                    NameTable.insert(std::make_pair(str, label));
                    }
                else // Если текущая строка содержит код,считать адрес
                {
                    std::string line;
                    std::getline(istr, line, '\n');
                    RemoveComment(line);
                    std::vector<std::string> args = ParseArguments(line);
                    for (size_t i = 0; i < args.size(); i++)
                    {
                        int typeOp = Type(args[i]);
                        if (typeOp == Operands::lreg || typeOp == Operands::var || typeOp == Operands::cnst)// Типы операндов, которые занимают дополнительную ячейку памяти ВМ
                            {
                            CntrCode +=2;
                            globalAddress+=2;
                            break; // В команде не может быть более 1 адреса
                            }
                    }
                    CntrCode +=2;
                    globalAddress+=2;
                }
        }
        istr >> str;
    }
    if(NameTable.find("main") == NameTable.end())
    {
        throw std::logic_error("Стартовая метка не найдена");
    }
    else if(NameTable["main"].Op_Type != 0)
    {
        throw std::logic_error("Стартовая метка не найдена");
    }
    startLabel = NameTable["main"].Address;
//    if(!isstartLabel) throw std::logic_error("Стартовая метка не найдена");
//    if (NameTable.find(nameStartLabel) != NameTable.end())
//    {
//        if (NameTable[nameStartLabel].Op_Type != 0) throw std::logic_error(nameStartLabel + "не является меткой");
//        startLabel = NameTable[nameStartLabel].Address;
//    }
//    else throw std::logic_error("Метка " + nameStartLabel + " не определена");

}

void Assembly::ParseCommand(std::tuple<int, int> cmd, std::string arguments) {
    switch (std::get<1>(cmd))
    {
        case CommandList::move_reg_reg:
        {
            MoveRR(std::get<0>(cmd), arguments); break;
        }
        case CommandList::move_reg_mem:
        {
            MoveRM(std::get<0>(cmd), arguments); break;
        }
        case CommandList::move_mem_reg:
        {
            MoveMR(std::get<0>(cmd), arguments); break;
        }
        case CommandList::move_adr_reg:
        {
            MoveАR(std::get<0>(cmd), arguments); break;
        }
        case CommandList::jmp:
        {
            Jump(std::get<0>(cmd), arguments); break;
        }
        case CommandList::math:
        {
            Math(std::get<0>(cmd), arguments); break;
        }
        case CommandList::io:
        {
            IO(std::get<0>(cmd), arguments); break;
        }
        case CommandList::cmp:
        {
            Cmp(std::get<0>(cmd), arguments);
            break;
        }
        case CommandList::call:
        {
            Call(std::get<0>(cmd), arguments);
            break;
        }
        case CommandList::other:
        {
            Command_register temp;
            temp.Cmd.r1 = 15;
            temp.Cmd.s = 0;
            temp.Cmd.code = CPU::Return;
            std::vector<word> value;
            for (int i = 0; i < 2; ++i)
            {
                value.push_back(temp.data.d32[i]);
            }
            WriteMem(value);
            break;
        }
        default:
            throw std::logic_error("Неизвестная команда");
            break;
    }

}

void Assembly::FirstTranslate(std::stringstream &istr) {
    bool dataSection = false;
    bool codeSection = false;
    std::string str;
    istr >> str;
    str.c_str();
    while (!istr.eof())
    {
        RemoveComment(str);
        if (str == ".data")
        {
            std::getline(istr, str, '\n');
            if (codeSection) throw std::logic_error("Секция данных после секции кода");
            if (dataSection) throw std::logic_error("Опеределение секции данных уже встречалось");
            dataSection = true;
            ParseDataSection(istr); // Сбор имен переменных
        }
        else if (str == ".code")
        {
            if (codeSection) throw std::logic_error("Опеределение секции кода уже встречалось");
            codeSection = true;
            ParseCodeSection(istr); // Сбор имен меток и метки start
        }
        else
        {
            std::getline(istr, str, '\n');
        }
        istr >> str;
    }
    if(!dataSection) throw std::logic_error("Отсутствует секция данных");
    if(!codeSection) throw std::logic_error("Отсутствует секция кода");

}
// Парсинг одной записи(команды)
void Assembly::Parse(std::stringstream &istr)
{
    std::string str;
    do
    {
        std::getline(istr, str, '\n');
        RemoveComment(str);
    } while (str.find(".code") == std::string::npos);

    istr >> str;
    while (!istr.eof())// Транслируем секцию кода
        {
        if (str.find(";") != std::string::npos) // Пропуск комменатриев,т.к считываение идет по слову из строки
            {
            std::getline(istr, str, '\n');
            }
        else if (str == "main")
        {
            istr >> str;
        }
        else if (str.find(":") == std::string::npos) // Пропуск всех меток И проверка является ли слово мнемоникой команды
            {
            if(Commands.find(str) == Commands.end())
            {
                throw std::logic_error("Команда " + str + " не определена");
            }
            std::string arguments;
            std::getline(istr, arguments, '\n');// Получить строчку аргументов
            RemoveComment(arguments);// Очищаем строку от комментария
            try
            {
                ParseCommand(Commands[str], arguments); // Парсинг команды
            }
            catch (const std::exception& e)
            {
                throw std::logic_error(str + " "+  arguments + "\n" + e.what());
            }
            }
        istr >> str;
        }
    WriteBufferFile(PathToDirectory + "test1.vm5"); // Запись в бинарный файл

}

void Assembly::WriteBufferFile(std::string filename) {
    // Адрес таблицы tableMovingName = Данные(по 2/4 байта) + код(по 2 байта)
    uint32_t countData{0};
    std::vector<NameTableCell> correctData;
    for (auto el: NameTable)
    {
        if(el.second.Op_Type != 0)
            correctData.push_back(el.second);
    }

    for (std::pair<std::string, NameTableCell> elem : NameTable)
    {
        if (elem.second.Op_Type != 0)
        {
            elem.second.Op_Type == DataType::Int16 ? countData += 2 : countData += 4;
        }
    }
    uint32_t movingTable = countData + TranslatorMemory.size();// Расположение таблицы перемещающих имен
    std::ofstream file(filename, std::ios::binary);
    if(!file)
    {
        throw std::logic_error("Файл не найден");
    }
    file.write((char*)&movingTable, sizeof(movingTable));
    file.write((char*)&countData, sizeof(countData));
    file.write((char*)&startLabel, sizeof(startLabel)); // Стартовое IP
    std::sort(correctData.begin(), correctData.end(), [](const NameTableCell& lhs, const NameTableCell rhs) {
        return lhs.Address < rhs.Address;
    });

    for (auto elem : correctData) // Запись данных
        {
        if (elem.Op_Type != 0)
        {
            if (elem.Op_Type == DataType::Int16)
            {
                file.write((char*)&elem.Val16, sizeof(elem.Val16.ui16));
            } else
            {
                file.write((char*)&elem.Val32, sizeof(elem.Val32.u32));
            }
        }
        }
    for (word code : TranslatorMemory) // Запись команд
        {
        file.write((char*)&code, sizeof(code));
        }
    for (uint16_t movingName : tableMovingName) // Таблица перемещающихся имен
        {
        file.write((char*)&movingName, sizeof(movingName));
        }
    file.close();

}


void Assembly::Jump(uint16_t code, std::string arguments) {

}
//Проверка размера регистров
bool Assembly::Check_RegSize( std::vector<std::string> args)
{
    bool Flag_LongReg = false;
    for (size_t i = 0; i < args.size(); i++)
    {
        if (Type(args[i]) != Operands::reg) Flag_LongReg = true;
    }
    if (Flag_LongReg)//работа с 32 битными регистрами
        {
        for (size_t i = 0; i < args.size(); i++)
        {
            if (Type(args[i]) != Operands::lreg) throw std::logic_error("Арифметические команды возможны только на регистрах");;
        }
        }
    return Flag_LongReg;
}

void Assembly::Math(uint16_t code, std::string arguments)
{
    std::vector<std::string> args = ParseArguments(arguments);
    Check_Operands(args);//проверка одного ли типа регистры 16 - 16 или 32- 32
    bool Flag_LongReg = Check_RegSize(args); //если в команде встречены не короткие регистры
    if (code != CPU::Bitwise_NOT)
    {
        if (args.size() != 2) throw std::logic_error("Ожидалось 2 аргумента");

        if (Flag_LongReg)//работа с 32 битными регистрами
        {
            Command_register temp;
            temp.Cmd.r1 = RCU32[args[0]];
            temp.Cmd.r2 = RCU32[args[1]];
            temp.Cmd.s = 1;
            temp.Cmd.code = code;
            temp.offset = 0;
            std::vector<word> value;
            for (int i = 0; i < 4; ++i)
            {
                value.push_back(temp.data.d32[i]);
            }
            WriteMem(value);
        }
        else//работа с 16 битными регистрами
        {
            Command_register temp;
            temp.Cmd.r1 = RCU16[args[0]];
            temp.Cmd.r2 = RCU16[args[1]];
            temp.Cmd.s = 0;
            temp.Cmd.code = code;
            std::vector<word> value;
            for (int i = 0; i < 2; ++i)
            {
                value.push_back(temp.data.d32[i]);
            }
            WriteMem(value);
        }
    }
    else
    {
        if (args.size() < 1) throw std::logic_error("Ожидалось не менее 1 аргумента");
        if (Flag_LongReg)//работа с 32 битными регистрами
            {
            Command_register temp;
            temp.Cmd.r1 = RCU32[args[0]];
            temp.Cmd.r2 = 0;
            temp.Cmd.s = 1;
            temp.Cmd.code = code;
            temp.offset = 0;
            std::vector<word> value;
            for (int i = 0; i < 4; ++i)
            {
                value.push_back(temp.data.d32[i]);
            }
            WriteMem(value);

            }
        else
        {
            Command_register temp;
            temp.Cmd.r1 = RCU16[args[0]];
            temp.Cmd.r2 = 0;
            temp.Cmd.s = 0;
            temp.Cmd.code = code;
            std::vector<word> value;
            for (int i = 0; i < 2; ++i)
            {
                value.push_back(temp.data.d32[i]);
            }
            WriteMem(value);
        }
    }

}

void Assembly::IO(uint16_t code, std::string str)
{
    std::vector<std::string> args = ParseArguments(str);
    if(args.size() != 2) throw std::logic_error("Ожидалось 2 аргумента");
    bool Flag_LongReg = false;
    if (Type(args[0]) != Operands::reg )
    {
        if(Type(args[0]) != Operands::lreg)
        {
            throw std::logic_error(args[0] + " ожидался регистр");
        }
        else
        {
            Flag_LongReg = true;
        }
    }

    if (Type(args[1]) != Operands::special) throw std::logic_error(args[1] + " Ожидалась конструкция #");
    int digitParam {0};
    if (isNumber(args[1].substr(1)))
    {
        int digitParam = std::atoi(args[1].substr(1).c_str());
        if (digitParam < 0 && digitParam > 2)
            throw std::logic_error(args[1] + " Ожидалась конструкция #<0..2>");
    }
    else
        throw std::logic_error(args[1] + " Ожидалась конструкция #<0..2>");
    Command_register temp{};
    temp.Cmd.r1 = RCU16[args[0]];
    temp.Cmd.r2 = digitParam;
    temp.Cmd.s = !Flag_LongReg ? 0 : 1;
    temp.Cmd.code = code;
    std::vector<word> value;
    int size = 2;
    if (Flag_LongReg) size = 4;
    for (int i = 0; i < size; ++i)
    {
        value.push_back(temp.data.d32[i]);
    }
    WriteMem(value);

}

void Assembly::Cmp(uint16_t code, std::string arguments) {

}

void Assembly::Call(uint16_t code, std::string arguments) {

}

void Assembly::Translate(std::string filename) {
    std::ifstream file;
    file.open(filename);
    if (!file.is_open())
    {
        std::cout << "File [ isn't open ]\n";
        return;
    }
    std::stringstream istr;
    istr << file.rdbuf();
    FirstTranslate(istr);
    file.seekg(0);
    istr.clear();
    istr << file.rdbuf();
    globalAddress = CntrData + CntrCode;
    Parse(istr);

}

bool Assembly::Check_Operands(std::vector<std::string> args) {
    Operands Temp = Type(args[0]);
    for (size_t i = 1; i < args.size(); i++)
    {
        if (Type(args[i]) != Temp)
        {
            throw std::logic_error("Команда должна выполняться на регистрах одного типа");
        }
    }
    return true;
}

void Assembly::MoveRR(uint16_t code, std::string arguments) {

}

void Assembly::MoveRM(uint16_t code, std::string arguments)
{
    std::vector<std::string> args = ParseArguments(arguments);
    if(args.size() != 2)
        throw std::logic_error("Ожидалось 2 аргумента");
    bool Flag_LongReg = Check_RegSizeSingle(args); //если в команде встречены не короткие регистры
    Command_register temp{};
    if (NameTable.find(args[1]) == NameTable.end())
        throw std::logic_error(args[1] + " не определена");
    if (Flag_LongReg)
    {
      temp.Cmd.code = code;
      temp.Cmd.s = 1;
      temp.Cmd.r1 = RCU32[args[0]];
      temp.Cmd.r2 = 0;
      temp.offset = NameTable[args[1]].Address;
    }
    else
    {
        temp.Cmd.code = code;
        temp.Cmd.s = 0;
        temp.Cmd.r1 = RCU16[args[0]];
        temp.Cmd.r2 = 0;
        temp.offset = NameTable[args[1]].Address;
    }
    std::vector<word> value;
    for (int i = 0; i < 4; ++i)//Команда всегда 32 битная
    {
        value.push_back(temp.data.d32[i]);
    }
    WriteMem(value);

}

void Assembly::MoveMR(uint16_t code, std::string arguments) {

}

void Assembly::MoveАR(uint16_t code, std::string arguments) {

}

bool Assembly::Check_RegSizeSingle(std::vector<std::string> args) {
    bool Flag_LongReg = false;

        if (Type(args[0]) != Operands::reg) Flag_LongReg = true;
        if(Flag_LongReg)
        {
            if (Type(args[0]) != Operands::lreg)
            {
                throw std::logic_error("Для данной команды первый операнд должен быть регистром");
            }
        }

    return Flag_LongReg;
}
