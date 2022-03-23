#pragma once
#include <cpp-common/non_copyable.h>
#include <cstdint>

class PPU :
	public NonCopyable
{
public:
	PPU();
	~PPU();

	uint8_t load8VRAM(uint16_t address) const;
	void store8VRAM(uint16_t address, uint8_t byte);
	uint8_t load8OAM(uint16_t address) const;
	void store8OAM(uint16_t address, uint8_t byte);
private:
	struct OAMEntry {
		uint8_t y;
		uint8_t x;
		uint8_t tile;

		uint8_t cgbPaletteNumber : 3; // 0-2
		uint8_t cgbVRAMBank      : 1; // 3
		uint8_t paletteNumber    : 1; // 4
		uint8_t xFlip            : 1; // 5
		uint8_t yFlip            : 1; // 6
		uint8_t bgAndWindowOver  : 1; // 7
	};

	uint8_t m_VRAM[0x2000];
	union {
		OAMEntry m_OAM[40];
		uint8_t m_rawOAM[40 * sizeof(OAMEntry)];
	};
};
