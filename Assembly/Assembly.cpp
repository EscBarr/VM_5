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
        if (tempDataArray.size() != 3)
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
    if (RCU16.find(str) != RCU16.end() || RCU32.find(str) != RCU32.end())
    {
        return Operands::reg;
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
    // Константа
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
            if (str == "main") // Точка входа в программу
                {
                if(isstartLabel)
                    throw std::logic_error("стартовая метка уже объявлена");
                istr >> nameStartLabel;
                isstartLabel = true;
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
                        if (typeOp == Operands::indirect || typeOp == Operands::var || typeOp == Operands::cnst)// Типы операндов, которые занимают дополнительную ячейку памяти ВМ
                            {
                            globalAddress++;
                            break; // В команде не может быть более 1 адреса
                            }
                    }
                    globalAddress++;
                }
            }
        }
        istr >> str;
    }
    if(!isstartLabel) throw std::logic_error("Стартовая метка не найдена");
    if (NameTable.find(nameStartLabel) != NameTable.end())
    {
        if (NameTable[nameStartLabel].Op_Type != 0) throw std::logic_error(nameStartLabel + "не является меткой");
        startLabel = NameTable[nameStartLabel].Address;
    }
    else throw std::logic_error("Метка " + nameStartLabel + " не определена");

}

void Assembly::ParseCommand(std::tuple<int, int> cmd, std::string arguments) {
    switch (std::get<1>(cmd))
    {
        case CommandList::move_reg_reg:
        {
            Move(std::get<0>(cmd), arguments); break;
        }
        case CommandList::lea:
        {
            Lea(std::get<0>(cmd), arguments); break;
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
            base_short_command temp;
            temp.r1 = 15;
            temp.s = 0;
            temp.code = CPU::Return;

            WriteMem(temp);
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
        else if (str == "start")
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
    WriteBufferFile(PathToDirectory + "testbin.bin"); // Запись в бинарный файл

}

void Assembly::WriteBufferFile(std::string filename) {
    // Адрес таблицы tableMovingName = Данные(по 2/4 байта) + код(по 2 байта)
    uint32_t countData{0};

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

    for (std::pair<std::string, NameTableCell> elem : NameTable) // Запись данных
        {
        if (elem.second.Op_Type != 0)
        {
            if (elem.second.Op_Type == DataType::Int16)
            {
                file.write((char*)&elem.second.Val16, sizeof(elem.second.Val16.ui16));
            } else
            {
                file.write((char*)&elem.second.Val32, sizeof(elem.second.Val32.u32));
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

void Assembly::Lea(uint16_t code, std::string arguments)
{

}

void Assembly::Move(uint16_t code, std::string arguments) {

}

void Assembly::Jump(uint16_t code, std::string arguments) {

}

void Assembly::Math(uint16_t code, std::string arguments) {

}

void Assembly::IO(uint16_t code, std::string str) {

}

void Assembly::Cmp(uint16_t code, std::string arguments) {

}

void Assembly::Call(uint16_t code, std::string arguments) {

}

void Assembly::Translate(std::string filename) {

}
