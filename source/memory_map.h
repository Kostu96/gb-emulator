#pragma once
#include <cstdint>
#include <initializer_list>
#include <utility>
#include <list>

struct AddressRange {
	uint16_t start;
	uint16_t size;

	bool contains(uint16_t address, uint16_t& offset) const
	{
		offset = address - start;
		return (address >= start) && (address < start + size);
	}
};

class MemoryMap
{
public:
	MemoryMap(std::initializer_list<std::pair<uint8_t*, AddressRange>> map);
	MemoryMap(MemoryMap&) = delete;

	uint8_t load8(uint16_t address) const;
	uint16_t load16(uint16_t address) const;
	void store8(uint16_t address, uint8_t byte);
private:
	std::list<std::pair<uint8_t*, AddressRange>> m_map;
};
