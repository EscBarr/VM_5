//
// Created by timur on 08.01.2022.
//

#include "FloatParser.h"

FloatParser::Symbol FloatParser::Checktoken(char ch)
{
    if (digit(ch)) return Digit;
    if (ch == '.') return Point;
    if ((ch == 'e') || (ch == 'E')) return Exp;
    if ((ch == '-') || (ch == '+')) return Sign;
    return Other;
}

bool FloatParser::IsDoubleNum(const std::string &Num) {
    size_t index = 0;
    State currentState = s0;                // начальное состояние
    while (currentState != final)
    {
        Symbol charType = Checktoken(Num[index++]);          // тип символа
        currentState = StateMatrix[charType][currentState]; // следующее состояние
        if (currentState == err) return false;
    }
    return true; //Успешно обработано число
}
