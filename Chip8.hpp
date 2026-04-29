#include <cstdint>
#include <fstream>
#include <chrono>
#include <random>

class Chip8
{

public:
    static constexpr unsigned int START_ADDRESS = 0x200;
    static constexpr unsigned int FONTSET_SIZE = 80;
    static constexpr unsigned int FONTSET_START_ADDRESS = 0x50;
    uint8_t registers[16]{};
    uint8_t memory[4096]{};

    uint16_t index{};
    uint16_t pc{};
    uint16_t opcode{};
    uint16_t stack[16]{};
    uint8_t sp{};
    uint8_t keypad[16]{};
    uint8_t delayTimer[16]{};
    uint8_t soundTimer[16]{};
    uint32_t video[64 * 32]{};
    uint8_t fontset[FONTSET_SIZE] =
        {
            0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
            0x20, 0x60, 0x20, 0x20, 0x70, // 1
            0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
            0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
            0x90, 0x90, 0xF0, 0x10, 0x10, // 4
            0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
            0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
            0xF0, 0x10, 0x20, 0x40, 0x40, // 7
            0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
            0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
            0xF0, 0x90, 0xF0, 0x90, 0x90, // A
            0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
            0xF0, 0x80, 0x80, 0x80, 0xF0, // C
            0xE0, 0x90, 0x90, 0x90, 0xE0, // D
            0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
            0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };
    std::default_random_engine randGen;
    std::uniform_int_distribution<uint8_t> randByte;

    Chip8() : randGen(std::chrono::system_clock::now().time_since_epoch().count())
    {
        pc = Chip8::START_ADDRESS;
        randByte = std::uniform_int_distribution<uint8_t>(0, 255U);
    }

    void LoadROM(char const *filename);
    void OP_00E0();
    void OP_00EE();
    void OP_2nnn();
};

void Chip8::LoadROM(char const *filename)
{
    std::ifstream file(filename, std::ios::binary || std::ios::ate);

    if (file.is_open())
    {
        std::streampos size = file.tellg();
        char *buffer = new char[size];

        file.seekg(0, std::ios::beg);
        file.read(buffer, size);
        file.close();

        for (size_t i = 0; i < size; ++i)
        {
            memory[START_ADDRESS + i] = buffer[i];
        }
        delete[] buffer;
    }
}

void Chip8::OP_00E0()
{
    std::fill(std::begin(video), std::end(video), 0);
    return;
}
void Chip8::OP_00EE()
{
    --sp;
    pc = stack[sp];
    return;
}

void Chip8::OP_2nnn()
{
    uint16_t addr = opcode & 0x0FFu;
    pc = addr;
    return;
}
