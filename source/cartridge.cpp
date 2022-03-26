#include "cartridge.h"

#include <cpp-common/helper_functions.h>
#include <iostream>

static const char* CartridgeTypeCodeToStr(uint8_t code) {
	switch (code) {
	case 0x00: return "ROM ONLY";
	case 0x01: return "MBC1";
	case 0x02: return "MBC1 + RAM";
	case 0x03: return "MBC1 + RAM + BATTERY";
	case 0x05: return "MBC2";
	case 0x06: return "MBC2 + BATTERY";
	case 0x08: return "ROM + RAM 1";
	case 0x09: return "ROM + RAM + BATTERY 1";
	case 0x0B: return "MMM01";
	case 0x0C: return "MMM01 + RAM";
	case 0x0D: return "MMM01 + RAM + BATTERY";
	case 0x0F: return "MBC3 + TIMER + BATTERY";
	case 0x10: return "MBC3 + TIMER + RAM + BATTERY 2";
	case 0x11: return "MBC3";
	case 0x12: return "MBC3 + RAM 2";
	case 0x13: return "MBC3 + RAM + BATTERY 2";
	case 0x19: return "MBC5";
	case 0x1A: return "MBC5 + RAM";
	case 0x1B: return "MBC5 + RAM + BATTERY";
	case 0x1C: return "MBC5 + RUMBLE";
	case 0x1D: return "MBC5 + RUMBLE + RAM";
	case 0x1E: return "MBC5 + RUMBLE + RAM + BATTERY";
	case 0x20: return "MBC6";
	case 0x22: return "MBC7 + SENSOR + RUMBLE + RAM + BATTERY";
	case 0xFC: return "POCKET CAMERA";
	case 0xFD: return "BANDAI TAMA5";
	case 0xFE: return "HuC3";
	case 0xFF: return "HuC1 + RAM + BATTERY";
	}

	abort();
	return "DEADBEEF";
}

static size_t ROMSizeCodeToKB(uint8_t code) {
	switch (code) {
	case 0x00: return 32;
	case 0x01: return 64;
	case 0x02: return 128;
	case 0x03: return 256;
	case 0x04: return 512;
	case 0x05: return 1024;
	case 0x06: return 1024 * 2;
	case 0x07: return 1024 * 4;
	case 0x08: return 1024 * 8;
	}

	abort();
	return 0xDEADBEEF;
}

static size_t RAMSizeCodeToKB(uint8_t code) {
	switch (code) {
	case 0x00: return 0;
	case 0x02: return 8;
	case 0x03: return 32;
	case 0x04: return 128;
	case 0x05: return 64;
	}

	abort();
	return 0xDEADBEEF;
}

Cartridge::~Cartridge()
{
	delete[] m_data;
}

uint8_t Cartridge::load8(uint16_t address) const
{
	return m_data[address];
}

void Cartridge::store8(uint16_t address, uint8_t byte)
{
	//__debugbreak();
}

void Cartridge::insert(const char* filename)
{
	if (!readFile(filename, nullptr, m_size, true)) {
		std::cerr << "Failed to read size of cartridge ROM file!\n";
		abort(); // TODO: exit more gracefuly
	}

	if (m_size < 0x8000) {
		std::cerr << "Size of cartridge ROM is less than 32KB!\n";
		abort();
	}

	delete[] m_data;
	m_data = new uint8_t[m_size];
	if (!readFile(filename, (char*)m_data, m_size, true)) {
		std::cerr << "Failed to read cartridge ROM file!\n";
		abort();
	}

	m_header = (Header*)(m_data + 0x100);

	uint16_t x = 0;
	for (uint16_t i = 0x0134; i <= 0x014C; ++i)
		x = x - m_data[i] - 1;

	if (m_header->CGBFlag == 0xC0)
		std::cerr << "WARNING: CGB only flag is set!";

	if (m_header->headerChecksum != (x & 0xFF))
		std::cerr << "WARNING: Header checksum test doen't pass!\n";

	if (ROMSizeCodeToKB(m_header->ROMSizeCode) != m_size / 1024)
		std::cerr << "WARNING: Header ROM size is different than file size read!\n";

	std::cout << "Loaded cartridge ROM file: " << filename << '\n'
		<< "Size loaded: " << m_size / 1024 << "KB\n"
		<< "Header:\n"
		<< "  Title:    " << m_header->title << '\n'
		<< "  Type:     " << CartridgeTypeCodeToStr(m_header->cartridgeTypeCode) << '\n'
		<< "  ROM size: " << ROMSizeCodeToKB(m_header->ROMSizeCode) << "KB\n"
		<< "  RAM size: " << RAMSizeCodeToKB(m_header->RAMSizeCode) << "KB\n";
}
