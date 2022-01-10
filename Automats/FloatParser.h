//
// Created by timur on 08.01.2022.
//

#pragma once
#include "../HeadersExtensions.h"
class FloatParser {
    enum State
            {
        s0,
        s1,
        s2,
        s3,
        s4,
        s5,
        s6,
        s7,
        final,
        err
            };
    enum Symbol
            {
        Sign,
        Digit,
        Point,
        Exp,
        Other,
        ClassCount
            };
    State StateMatrix[ClassCount][final] =
            { {s1,  err, err, err, s6,  err, err, err },
              {s2,  s2,  s2,  s5,  s7,  s5,  s7,  s7},
              {err, err, s3,  err, err, err, err, err},
              {err, err, s4,  err, err, s4,  err, err},
              {err, err, err, err, err, final, err, final}
            };
    static Symbol Checktoken(char ch);
public:

    bool IsDoubleNum(const std::string& Num);
};

