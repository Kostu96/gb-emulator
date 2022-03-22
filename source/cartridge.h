#pragma once
#include <cpp-common/non_copyable.h>
#include <cstdint>

class Cartridge :
    public NonCopyable
{
public:
    Cartridge(const char* filename);
    ~Cartridge();
private:
    uint8_t* m_data;
};
