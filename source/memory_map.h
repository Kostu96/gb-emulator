#pragma once
#include "cartridge.h"

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
	MemoryMap() : m_cartridge("assets/test_roms/dmg-acid2.gb") {} // TODO: temp until insertCartridge interface

	uint8_t load8(uint16_t address) const;
	void store8(uint16_t address, uint8_t byte);
private:
	// TODO: insertCartridge interface
	Cartridge m_cartridge;
};
