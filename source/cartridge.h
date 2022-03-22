#pragma once
#include <cpp-common/non_copyable.h>
#include <cstdint>

class Cartridge :
    public NonCopyable
{
public:
    Cartridge() = default;
    ~Cartridge();

    uint8_t load8(uint16_t address) const;
    void store8(uint16_t address, uint8_t byte);

    void insert(const char* filename);
private:
    struct Header {
        uint8_t entryPoint[4];      // 0x100 - 0x103
        uint8_t nintendoLogo[0x30]; // 0x104 - 0x133
        char title[15];             // 0x134 - 0x142
        uint8_t CGBFlag;            // 0x143
        uint16_t newLicenseeCode;   // 0x144
        uint8_t SGBFlag;            // 0x146
        uint8_t cartridgeTypeCode;  // 0x147
        uint8_t ROMSizeCode;        // 0x148
        uint8_t RAMSizeCode;        // 0x149
        uint8_t destinationCode;    // 0x14A
        uint8_t oldLicenseeCode;    // 0x14B
        uint8_t version;            // 0x14C
        uint8_t headerChecksum;     // 0x14D
        uint8_t globalChecksum;     // 0x14E - 0x14F
    };

    Header* m_header = nullptr;
    uint8_t* m_data = nullptr;
    size_t m_size = 0;
};
