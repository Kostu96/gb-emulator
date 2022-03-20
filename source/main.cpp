#include "helper_functions.h"

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

	for (;;) {

	}

	delete[] cartridgeROM;
	return 0;
}
