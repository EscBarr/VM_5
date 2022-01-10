//
// Created by timur on 09.01.2022.
//

#pragma once
#include<cstdlib>
#include<stdexcept>
#include "../HeadersExtensions.h"
#include "../Automats/IntegerParser.h"

class MathParser {
public:
    inline double evalPrefixAndBracket(const char*&expression)
    {
        while(isspace(*expression))
            ++expression;
        if (digit(*expression))
        {
            return strtod(expression,(char**)&expression);
        }
        switch(*expression)
        {
                case'-':
                    return -evalPrefixAndBracket(++expression);
                    case'+':
                        return evalPrefixAndBracket(++expression);
                        case'(':
                        {
                            double res=evalPlusMinus(++expression);
                            if(*expression++ ==')')
                                return res;
                        }
                        default:
                            throw std::runtime_error("Ошибка в выражении");
        }
    }

    inline double evalMulDiv(const char *&expression)
    {
        double res=evalPrefixAndBracket(expression);
        while(true)
        {
            while(isspace(*expression))
                ++expression;
            switch(*expression)
            {
                case 0:
                    return res;
                    case'*':
                        res*=evalPrefixAndBracket(++expression);
                        break;
                        case'/':
                            res/=evalPrefixAndBracket(++expression);
                            break;
                            default:
                                return res;
            }
        }
    }

    inline double evalPlusMinus(const char *&expression)
    {
        double res=evalMulDiv(expression);
        while(true)
        {
            while(isspace(*expression))
                ++expression;
            switch(*expression)
            {
                case 0:
                    return res;
                    case'+':
                        res+=evalMulDiv(++expression);
                        break;
                        case'-':
                            res-=evalMulDiv(++expression);
                            break;
                            default:
                                return res;
            }
        }
    }

    inline double evalExpression(const char *expression)
    {
        const double res=evalPlusMinus(expression);
        while(isspace(*expression))
            ++expression;
        if(*expression)
            throw std::runtime_error("Ошибка в выражении");
        return res;
    }
};



