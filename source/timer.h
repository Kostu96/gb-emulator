#pragma once
#include <cpp-common/non_copyable.h>
#include <cstdint>

class CPU;

class Timer :
    public NonCopyable
{
public:
    Timer();
    
    void connect(CPU& cpu);
    void tick();

    uint8_t load8(uint16_t address) const;
    void store8(uint16_t address, uint8_t byte);
private:
    union {
        struct {
            union {
                struct {
                    uint8_t m_DIVLower;
                    uint8_t m_DIVUpper;
                };
                uint16_t m_DIV;
            };
            uint8_t m_TIMA;
            uint8_t m_TMA;
            uint8_t m_TAC;
        };
        uint8_t m_rawData[5];
    };

    CPU* m_CPU;
};
