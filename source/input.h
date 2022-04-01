#pragma once
#include <cpp-common/non_copyable.h>
#include <cstdint>

class IO :
    public NonCopyable
{
public:
    IO() = default;
    
    uint8_t load8(uint16_t address) const;
    void store8(uint16_t address, uint8_t byte);

    const char* getBuffer();
private:
    union {
        struct {
            uint8_t m_joypad;
            uint8_t m_serialData;
            uint8_t m_serialControl;
        };
        uint8_t m_rawData[3];
    };
    char m_buffer[512];
    size_t m_bufferIndex = 0;
};
