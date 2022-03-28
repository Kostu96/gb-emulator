#include "input.h"

#include <iostream>

uint8_t IO::load8(uint16_t address) const
{
	return m_rawData[address];
}

void IO::store8(uint16_t address, uint8_t byte)
{
	m_rawData[address] = byte;

	// TODO: temporary handled serial comunication
	if (m_serialControl = 0x81) {
		if (m_serialData)
			std::cout << m_serialData;
		
		m_serialData = 0;
	}
}
