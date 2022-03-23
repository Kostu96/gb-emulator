#include "ppu.h"

PPU::PPU()
{

}

PPU::~PPU()
{

}

uint8_t PPU::load8VRAM(uint16_t address) const
{
	return m_VRAM[address];
}

void PPU::store8VRAM(uint16_t address, uint8_t byte)
{
	m_VRAM[address] = byte;
}

uint8_t PPU::load8OAM(uint16_t address) const
{
	return m_rawOAM[address];
}

void PPU::store8OAM(uint16_t address, uint8_t byte)
{
	m_rawOAM[address] = byte;
}
