#pragma once
#include <cpp-common/non_copyable.h>
#include <cstdint>

class Timers :
    public NonCopyable
{
public:
    Timers() = default;
    
    uint8_t load8(uint16_t address) const;
    void store8(uint16_t address, uint8_t byte);
private:
    
};
