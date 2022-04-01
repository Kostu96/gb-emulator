#include "timer.h"
#include "cpu.h"

#include <iostream>

Timer::Timer()
{
	// TODO: reset timers when gameboy resets
	m_DIV = 0x0;
	m_TIMA = 0;
	m_TMA = 0;
	m_TAC = 0xF8;
}

void Timer::connect(CPU& cpu)
{
	m_CPU = &cpu;
}

void Timer::tick()
{
	uint16_t bit = 1;
	switch (m_TAC & 3) {
	case 0: bit <<= 9; break;
	case 1: bit <<= 3; break;
	case 2: bit <<= 5; break;
	case 3: bit <<= 7; break;
	}

	uint16_t prevDIV = m_DIV;
	++m_DIV;

	uint16_t edgeDetectorInput = m_TAC & 0x100 ? 0xFFFF : 0;
	edgeDetectorInput &= (m_DIV & bit);
	if (!edgeDetectorInput && (prevDIV & bit)) ++m_TIMA;
	if (m_TIMA == 0xFF) {
		m_TIMA = m_TMA;
		m_CPU->requestInterrupt(4); // Timer IRQ
	}
}

uint8_t Timer::load8(uint16_t address) const
{
	return m_rawData[address + 1];
}

void Timer::store8(uint16_t address, uint8_t byte)
{
	if (address == 0) m_DIV = 0;
	else m_rawData[address + 1] = byte;
}

void Timer::handoffReset()
{
	m_DIV = 0xABCC;
	m_TIMA = 0;
	m_TMA = 0;
}
