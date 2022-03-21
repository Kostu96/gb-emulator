#pragma once
#include <cstdint>

class Cartridge
{
public:
    Cartridge(const char* filename);
    ~Cartridge();

    Cartridge(Cartridge&) = delete;
    Cartridge& operator=(Cartridge&) = delete;
private:
    uint8_t* m_data;
};
