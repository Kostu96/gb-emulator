#pragma once
#include "cartridge.h"

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

class MemoryMap
{
public:
	MemoryMap() : m_cartridge("assets/dmg-acid2.gb") {} // TODO: temp until insertCartridge interface
	MemoryMap(MemoryMap&) = delete;

	uint8_t load8(uint16_t address) const;
	void store8(uint16_t address, uint8_t byte);
private:
	// TODO: insertCartridge interface
	Cartridge m_cartridge;
};
