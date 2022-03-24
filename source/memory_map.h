#pragma once
#include "cartridge.h"
#include "ppu.h"

#include <cpp-common/non_copyable.h>
#include <cstdint>

struct AddressRange {
	uint16_t start;
	uint16_t size;

	bool contains(uint16_t address, uint16_t& offset) const
	{
		offset = address - start;
		return (address >= start) && (address < start + size);
	}
};

class MemoryMap :
	public NonCopyable
{
public:
	MemoryMap() = default;

	uint8_t load8(uint16_t address) const;
	void store8(uint16_t address, uint8_t byte);

	void insertCartridge(const char* filename) { m_cartridge.insert(filename); }
private:
	Cartridge m_cartridge;
	PPU m_ppu;
	uint8_t m_WRAM[0x2000];
	uint8_t m_HRAM[0x80];
};
