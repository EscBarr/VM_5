#pragma once

#include "iMath.h"

class Bitwise_op : public IntArithmetic {
public:
    Bitwise_op(std::function<int64_t(int64_t, int64_t)> operation) : IntArithmetic(operation) {}
};
