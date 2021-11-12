#include <iostream>
#include "array"

using word = uint8_t;
//Короткая команда
struct base_short_command {
    uint8_t code: 7;       // Код операции
    uint8_t s: 1;       // Размер целого операнда
    uint8_t r1: 4;         // Регистр №1
    uint8_t r2: 4;            // Регистр №2
};

// Длинная команда
struct base_command_t {
    base_short_command Cmd;
    uint16_t offset;         // Третий адресный регистр
};
union word_t// Все возможные данные
{
    union datatype16//2
    {
        int16_t i16;
        uint16_t ui16;
        word d16[2];
    };

    union datatype32//4
    {
        int32_t i32;
        uint32_t u32;
        float f;
        word d32[4];
    };
    datatype16 d16_t[2];
    datatype32 d32_t;
};
union datatype16//2
{
    int16_t i16;
    uint16_t ui16;
    word d16[2];
};
union datatype32//4
{
    int32_t i32;
    uint32_t u32;
    float f;
    word d32[4];
};
union mem_word_t {
    word_t word;
    base_command_t cmd;
};
union registers {
    std::array<datatype16, 8> RCU_16;
    std::array<datatype32, 4> RCU_32;
};

int main() {
    base_command_t temp;
    temp.Cmd.code = 2;
    temp.Cmd.s = 1;
    temp.Cmd.r1 = 0;
    temp.Cmd.r2 = 16;
    std::cout << sizeof(registers);


    return 0;
}
