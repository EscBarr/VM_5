//
// Created by timur on 09.01.2022.
//

#pragma once
#include "../HeadersExtensions.h"
#include "MathParser.h"
class IndirectParser {
    std::pair<int, int> ToParseInderect(std::string str)
    {
        std::pair<int,int> result;

        std::string deleteChars = "[] ";

        for (size_t i = 0; i < deleteChars.size(); ++i)
        {
            str.erase(std::remove(str.begin(), str.end(), deleteChars[i]), str.end());
            // str.erase(remove_if(str.begin(), str.end(), deleteChars[i]), str.end());
        }
        int indexReg = str.find("lr");
        if (indexReg != std::string::npos)
        {
            int numReg = str[indexReg + 1] - '0';
            if (std::isdigit(str[indexReg + 1]) and numReg > 0 and numReg < 7)
            {
                result.first = numReg;
                str.erase(indexReg, 2); // r1,r2...r16
            }
            else
            {
                throw std::logic_error("Ошибка косвенной адресации");
            }
        }
        else
        {
            int posRegister = str.find('r');
            if (posRegister != std::string::npos)
            {
                int numReg = str[posRegister + 1] - '0';
                if (std::isdigit(str[posRegister + 1]) and numReg > 0 and numReg < 15)
                {
                    result.first = numReg;
                    str.erase(posRegister, 2); // r1,r2...r16
                }
                else
                {
                    throw std::logic_error("Ошибка косвенной адресации");
                }
                if (str[0]== '+' || str[0] == '-') // Проверка на запись формата -- "r +(-) выражение".
                    {
                    MathParser parser;
                    result.second = parser.evalExpression(str.c_str());
                    }
                else
                {
                    throw std::logic_error("Ошибка косвенной адресации");
                }
            }
            else
                throw std::logic_error("Ошибка косвенной адресации");
        }

        return result;
    }

};


