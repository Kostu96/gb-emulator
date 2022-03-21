#include "memory_map.h"

#include <cstdlib>

static AddressRange VRAM_RANGE{ 0x8000, 0x2000 };

uint8_t MemoryMap::load8(uint16_t address) const
{
		

	abort();
	return 0xCD;
}

void MemoryMap::store8(uint16_t address, uint8_t byte)
{
	uint16_t offset;
	
	if (VRAM_RANGE.contains(address, offset))
		return; // TODO: ignore for now

	abort();
}
