#include "cpu.h"
#include "memory_map.h"
#include "helper_functions.h"

#include <iostream>

CPU::CPU(MemoryMap& memoryMap) :
	m_memoryMap(memoryMap)
{
	size_t size = 256;
	if (!readFile("assets/DMG_ROM.bin", (char*)m_internalROM, size, true)) {
		std::cerr << "Failed to read internal ROM file!\n";
		abort();
	}

	reset();
};

void CPU::reset()
{
	PC = 0x0;
}

void CPU::doCycles(size_t cycles)
{
	m_is16bitOp = false;
	m_isIndirectAddressing = false;

	uint8_t instruction = (this->*m_readByteFunc)(PC++);
	switch (instruction)
	{
	case 0x00:
		NOP();
		break;
	case 0x0E:
		m_activeReg8 = &C;
		m_imm8 = m_memoryMap.load8(PC++);
		LD();
		break;
	case 0x20:
		m_imm8_s = m_memoryMap.load8(PC++);
		JR(0x80);
		break;
	case 0x21:
		m_is16bitOp = true;
		m_activeReg16 = &HL;
		//m_imm16 = m_memoryMap.load16(PC);
		PC += 2;
		LD();
		break;
	case 0x31:
		m_is16bitOp = true;
		m_activeReg16 = &SP;
		//m_imm16 = m_memoryMap.load16(PC);
		PC += 2;
		LD();
		break;
	case 0x32:
		m_isIndirectAddressing = true;
		m_activeReg16 = &HL;
		m_activeReg8 = &A;
		LD();
		--HL;
		break;
	case 0x3E:
		m_activeReg8 = &A;
		m_imm8 = m_memoryMap.load8(PC++);
		LD();
		break;
	case 0xAF:
		m_activeReg8 = &A;
		XOR();
		break;
	case 0xCB:
		instruction = m_memoryMap.load8(PC++);
		switch (instruction)
		{
		case 0x7C:
			m_activeReg8 = &H;
			BIT(7);
			break;
		default:
			abort();
		}
		break;
	case 0xE2:
		m_isIndirectAddressing = true;
		break;
	default:
		abort();
	}
}

uint8_t CPU::readByte(uint16_t address)
{
	return m_memoryMap.load8(address);
}

uint8_t CPU::readByteInternal(uint16_t address)
{
	if (address > 0xFF)
		abort();

	return m_internalROM[address];
}

void CPU::NOP()
{
}

void CPU::JR(uint8_t mask)
{
	bool con = (F.byte & mask);
	if (con)
		PC += m_imm8_s;
}

void CPU::LD()
{
	if (m_is16bitOp)
		*m_activeReg16 = m_imm16;
	else {
		if (m_isIndirectAddressing) {
			m_memoryMap.store8(*m_activeReg16, *m_activeReg8);
		}
		else {
			*m_activeReg8 = m_imm8;
		}
	}
}

void CPU::XOR()
{
	A ^= *m_activeReg8;
	F.zeroFlag = !A;
}

void CPU::BIT(uint8_t n)
{
	F.zeroFlag = (*m_activeReg8 >> n) & 1;
	F.subtractFlag = 0;
	F.halfCarryFlag = 1;
}
