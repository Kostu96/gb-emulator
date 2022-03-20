#include "memory_map.h"

MemoryMap::MemoryMap(std::initializer_list<std::pair<uint8_t*, AddressRange>> map) :
	m_map(map)
{}

uint8_t MemoryMap::load8(uint16_t address) const
{
	uint16_t offset;
	for (auto& element : m_map) {
		if (element.second.contains(address, offset)) {
			return element.first[offset];
		}
	}

	abort();
	return 0xCD;
}

uint16_t MemoryMap::load16(uint16_t address) const
{
	uint16_t data = 0;
	uint16_t offset;
	for (auto& element : m_map) {
		if (element.second.contains(address, offset)) {
			data = element.first[offset];
			data |= element.first[offset + 1] << 8;
			return data;
		}
	}

	abort();
	return 0xCDCD;
}

void MemoryMap::store8(uint16_t address, uint8_t byte)
{
	uint16_t offset;
	for (auto& element : m_map) {
		if (element.second.contains(address, offset)) {
			element.first[offset] = byte;
			return;
		}
	}

	abort();
}
