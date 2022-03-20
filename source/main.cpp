#include "helper_functions.h"
#include "cpu.h"
#include "memory_map.h"

#include <cstdint>
#include <iostream>

int main()
{
	uint8_t internalROM[256];
	size_t size = 256;
	if (!readFile("assets/DMG_ROM.bin", (char*)internalROM, size, true)) {
		std::cerr << "Failed to read internal ROM file!\n";
		abort();
	}

	if (!readFile("assets/tetris.gb", nullptr, size, true)) {
		std::cerr << "Failed to read size of cartridge ROM file!\n";
		abort();
	}
	uint8_t* cartridgeROM = new uint8_t[size];
	if (!readFile("assets/tetris.gb", (char*)cartridgeROM, size, true)) {
		std::cerr << "Failed to read cartridge ROM file!\n";
		abort();
	}

	uint8_t* vram = new uint8_t[0x200];
	uint8_t* io = new uint8_t[0x80];

	MemoryMap memoryMap{
		{ internalROM,  AddressRange{ 0x0,    0x100 } },
		{ cartridgeROM, AddressRange{ 0x0,    0x4000 } },
		{ vram,         AddressRange{ 0x8000, 0x2000 } },
		{ io,           AddressRange{ 0xFF00, 0x80 } }
	};

	CPU cpu(memoryMap);
	for (;;) {
		cpu.runInstruction();
	}

	delete[] vram;
	delete[] cartridgeROM;
	return 0;
}
