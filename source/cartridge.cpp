#include "cartridge.h"
#include "helper_functions.h"

#include <iostream>

Cartridge::Cartridge(const char* filename)
{
	size_t size;
	if (!readFile(filename, nullptr, size, true)) {
		std::cerr << "Failed to read size of cartridge ROM file!\n";
		abort();
	}
	
	m_data = new uint8_t[size];
	if (!readFile(filename, (char*)m_data, size, true)) {
		std::cerr << "Failed to read cartridge ROM file!\n";
		abort();
	}
}

Cartridge::~Cartridge()
{
	delete[] m_data;
}
