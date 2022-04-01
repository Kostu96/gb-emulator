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
	if (m_serialControl == 0x81) {
		if (m_serialData) {
			m_buffer[m_bufferIndex++] = m_serialData;
			m_bufferIndex %= 512;
		}
		
		m_serialControl = 0;
	}
}

const char* IO::getBuffer()
{
	m_buffer[m_bufferIndex] = 0;
	m_bufferIndex = 0;
	return m_buffer;
}
