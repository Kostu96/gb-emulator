#pragma once
#include "cartridge.h"
#include "ppu.h"
#include "input.h"
#include "timer.h"

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
	constexpr static size_t WRAM_SIZE = 0x2000;
	constexpr static size_t HRAM_SIZE = 0x80;

	MemoryMap();
	~MemoryMap();

	void connect(CPU& cpu);

	PPU& getPPU() { return m_PPU; }
	IO& getIO() { return m_IO; }
	Timer& getTimer() { return m_timer; }

	uint8_t load8(uint16_t address) const;
	uint8_t* getMemoryLocation(uint16_t address);
	void store8(uint16_t address, uint8_t byte);

	void insertCartridge(const char* filename) { m_cartridge.insert(filename); }
private:
	Cartridge m_cartridge;
	PPU m_PPU;
	IO m_IO;
	Timer m_timer;
	uint8_t* m_WRAM;
	uint8_t m_HRAM[HRAM_SIZE];
};
