#include "ppu.h"

PPU::PPU()
{
	m_VRAM = new uint8_t[VRAM_SIZE]{};

	// helper variables
	m_currentFrame = 0;
	m_lineTicks = 0;
	m_videoBuffer = new uint32_t[XRES * YRES];
}

PPU::~PPU()
{
	delete[] m_videoBuffer;
	delete[] m_VRAM;
}

void PPU::tick()
{
	++m_lineTicks;

	// TODO: add enumeration for modes
	// TODO: check timings here
	// TODO: interrupts from lcd
	switch (m_LCDStatus.mode)
	{
	case 0: // HBlank
		if (m_lineTicks >= TICKS_PER_LINE) {
			m_LCDStatus.LYCEQLY = (++m_LY == m_LYC);
			if (m_LCDStatus.LYCEQLY && m_LCDStatus.LYCEQLYINT) {
				// TODO: LY==LYC interrupt
			}

			if (m_LY >= YRES)
				m_LCDStatus.mode = 1;
			else
				m_LCDStatus.mode = 2;

			m_lineTicks = 0;
		}
		break;
	case 1: // VBlank
		if (m_lineTicks >= TICKS_PER_LINE) {
			m_LCDStatus.LYCEQLY = (++m_LY == m_LYC);
			if (m_LCDStatus.LYCEQLY && m_LCDStatus.LYCEQLYINT) {
				// TODO: LY==LYC interrupt
			}

			if (m_LY >= LINES_PER_FRAME) {
				m_LCDStatus.mode = 2;
				m_LY = 0;
			}

			m_lineTicks = 0;
		}
		break;
	case 2: // OAMSearch
		if (m_lineTicks >= 80)
			m_LCDStatus.mode = 3;
		break;
	case 3: // DataTransfer
		if (m_lineTicks >= 80 + 172)
			m_LCDStatus.mode = 0;
		break;
	}
}

uint8_t PPU::load8VRAM(uint16_t address) const
{
	return m_VRAM[address];
}

#include <thread>

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

uint8_t PPU::load8(uint16_t address) const
{
	return m_rawLCD[address];
}

void PPU::store8(uint16_t address, uint8_t byte)
{
	m_rawLCD[address] = byte;
}
