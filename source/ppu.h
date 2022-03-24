#pragma once
#include <cpp-common/non_copyable.h>
#include <cstdint>

class PPU :
	public NonCopyable
{
public:
	constexpr static size_t VRAM_SIZE = 0x2000;

	PPU();
	~PPU();

	void tick();

	uint8_t load8VRAM(uint16_t address) const;
	void store8VRAM(uint16_t address, uint8_t byte);
	uint8_t load8OAM(uint16_t address) const;
	void store8OAM(uint16_t address, uint8_t byte);
	uint8_t load8(uint16_t address) const;
	void store8(uint16_t address, uint8_t byte);
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

	uint8_t* m_VRAM;
	union {
		OAMEntry m_OAM[40];
		uint8_t m_rawOAM[40 * sizeof(OAMEntry)];
	};

	struct LCDControl {
		uint8_t BGWEnable         : 1; // 0
		uint8_t OBJEnable         : 1; // 1
		uint8_t OBJSize           : 1; // 2
		uint8_t BGTileMapArea     : 1; // 3
		uint8_t BGWTileDataArea   : 1; // 4
		uint8_t windowEnable      : 1; // 5
		uint8_t windowTileMapArea : 1; // 6
		uint8_t enable            : 1; // 7
	};

	struct LCDStatus {
		uint8_t mode       : 2; // 0-1
		uint8_t LYCEQLY    : 1; // 2
		uint8_t hBlankINT  : 1; // 3
		uint8_t vBlankINT  : 1; // 4
		uint8_t OAMINT     : 1; // 5
		uint8_t LYCEQLYINT : 1; // 6
		uint8_t unknown    : 1; // 7
	};

	union {
		struct {
			LCDControl m_LCDControl;
			LCDStatus m_LCDStatus;
			uint8_t m_SCY;
			uint8_t m_SCX;
			uint8_t m_LY;
			uint8_t m_LYC;
			uint8_t m_DMA;
			uint8_t m_BGPData;
			uint8_t m_cgbOBP0;
			uint8_t m_cgbOBP1;
			uint8_t m_windowY;
			uint8_t m_windowX;
		};
		uint8_t m_rawLCD[12];
	};

	// helper variables
	static constexpr uint32_t LINES_PER_FRAME = 154;
	static constexpr uint32_t TICKS_PER_LINE = 456;
	static constexpr uint32_t YRES = 144;
	static constexpr uint32_t XRES = 160;

	uint32_t m_currentFrame;
	uint32_t m_lineTicks;
	uint32_t* m_videoBuffer;
};
