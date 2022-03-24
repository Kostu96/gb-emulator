#include "memory_map.h"

#include <cstdlib>

static AddressRange ROM_RANGE{  0x0000, 0x8000 };
static AddressRange VRAM_RANGE{ 0x8000, 0x2000 };
static AddressRange WRAM_RANGE{ 0xC000, 0x2000 };
static AddressRange OAM_RANGE{  0xFE00, 0x00A0 };
static AddressRange SPU_RANGE{  0xFF00, 0x0040 };
static AddressRange PPU_RANGE{  0xFF40, 0x0030 };
static AddressRange HRAM_RANGE{ 0xFF80, 0x0080 };

MemoryMap::MemoryMap()
{
	m_WRAM = new uint8_t[WRAM_SIZE];
}

MemoryMap::~MemoryMap()
{
	delete[] m_WRAM;
}

uint8_t MemoryMap::load8(uint16_t address) const
{
	uint16_t offset;

	if (ROM_RANGE.contains(address, offset))
		return m_cartridge.load8(offset);

	if (VRAM_RANGE.contains(address, offset))
		return m_PPU.load8VRAM(offset);

	if (WRAM_RANGE.contains(address, offset))
		return m_WRAM[offset];

	if (OAM_RANGE.contains(address, offset))
		return m_PPU.load8OAM(offset);

	if (PPU_RANGE.contains(address, offset))
		return m_PPU.load8(offset);

	if (HRAM_RANGE.contains(address, offset))
		return m_HRAM[offset];

	__debugbreak();
	return 0xCD;
}

void MemoryMap::store8(uint16_t address, uint8_t byte)
{
	uint16_t offset;
	
	if (ROM_RANGE.contains(address, offset)) {
		m_cartridge.store8(offset, byte);
		return;
	}

	if (VRAM_RANGE.contains(address, offset))
		return m_PPU.store8VRAM(offset, byte);

	if (WRAM_RANGE.contains(address, offset)) {
		m_WRAM[offset] = byte;
		return;
	}

	if (OAM_RANGE.contains(address, offset))
		return m_PPU.store8OAM(offset, byte);

	if (SPU_RANGE.contains(address, offset))
		return; // TODO: ignore for now

	if (PPU_RANGE.contains(address, offset)) {
		m_PPU.store8(offset, byte);
		return;
	}

	if (HRAM_RANGE.contains(address, offset)) {
		m_HRAM[offset] = byte;
		return;
	}

	__debugbreak();
}
