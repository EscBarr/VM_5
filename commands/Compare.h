#pragma once

#include "iMath.h"
#include "FMath.h"

class Compare : public Command {
    void operator()(CPU &Cpu) noexcept override;
};

class CompareF : public Command {
    void operator()(CPU &Cpu) noexcept override;
};