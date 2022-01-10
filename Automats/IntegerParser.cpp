//
// Created by timur on 08.01.2022.
//

#include "IntegerParser.h"

bool IntegerParser::IsDecimalNum(std::string Num) {
    State state = start;
    for (size_t i = 0; i < Num.size() && state != error; i++)
    {
        char ch = Num[i];
        switch (state)
        {
            case State::start:
            {
                if (ch == '+' || ch == '-')
                    state = s1;
                else if (digit(ch))
                    state = s2;
                else
                    state = error;
                break;
            }
            case State::s1:
            {
                if (digit(ch))
                    state = s2;
                else state = error;
                break;
            }
            case State::s2:
            {
                if (digit(ch))
                    state = s2;
                else state = error;
                break;
            }
        }
    }
    if (state == s2) return true;
    return false;

}

int IntegerParser::ToNum(std::string Num) {
    int res = 0;
    bool isNegative = false;
    for (size_t i = 0; i < Num.size(); i++)
    {
        char ch = Num[i];
        if (ch == '-')
            isNegative = true;
        else if (digit(ch))
        {
            res *= 10;
            res += ch - '0';
        }
    }
    if (isNegative)
        res *= -1;
    return res;

}

bool IntegerParser::InitializeNum(std::string Num, int &res) {
    if (IsDecimalNum(Num))
    {
        res = ToNum(Num);
        return true;
    }
    return false;
}
