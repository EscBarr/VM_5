//
// Created by timur on 07.01.2022.
//
#pragma once
#include <sstream>
#include <string>
#include <fstream>
#include <vector>
#include <filesystem>
#include <iostream>
#include <unordered_map>
#include "CPU.h"
#include "Types.h"

inline bool digit(char ch)
{
    std::string digits = "0123456789";
    int d;
    for(d = 0; d < 10; ++d)
        if(ch == digits[d]) return true;
        return false;              // символ - не цифра
}

inline std::vector<std::string> split(const std::string& s, char symb)

{
    std::vector<std::string> elems;
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, symb))
    {
        elems.push_back(item);
    }
    return elems;
}
inline bool isNumber(std::string str)
{
    for (size_t i = 0; i < str.size(); i++)
    {
        if (str[i] > '9' || str[i] < '0')
            return false;
    }
    return true;
}
