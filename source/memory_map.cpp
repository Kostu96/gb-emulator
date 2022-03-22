#include "memory_map.h"

#include <cstdlib>

static AddressRange ROM_RANGE{  0x0000, 0x8000 };
static AddressRange VRAM_RANGE{ 0x8000, 0x2000 };
static AddressRange SPU_RANGE{  0xFF00, 0x0040 };
static AddressRange PPU_RANGE{  0xFF40, 0x0030 };
static AddressRange HRAM_RANGE{ 0xFF80, 0x0080 };

uint8_t MemoryMap::load8(uint16_t address) const
{
	uint16_t offset;

	if (ROM_RANGE.contains(address, offset))
		return m_cartridge.load8(offset);

	if (HRAM_RANGE.contains(address, offset))
		return m_HRAM[offset];

	abort();
	return 0xCD;
}

void MemoryMap::store8(uint16_t address, uint8_t byte)
{
	uint16_t offset;
	
	if (VRAM_RANGE.contains(address, offset))
		return; // TODO: ignore for now

	if (SPU_RANGE.contains(address, offset))
		return; // TODO: ignore for now

	if (PPU_RANGE.contains(address, offset))
		return; // TODO: ignore for now

	if (HRAM_RANGE.contains(address, offset)) {
		m_HRAM[offset] = byte;
		return;
	}

	abort();
}
