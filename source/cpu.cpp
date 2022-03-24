#include "cpu.h"
#include "memory_map.h"

#include <cpp-common/helper_functions.h>
#include <iostream>

CPU::CPU(MemoryMap& memoryMap) :
	m_memoryMap(memoryMap)
{
	reset();
};

void CPU::reset()
{
	m_registerNamed.F.byte = 0x0;
	m_registerNamed.PC = 0x0;

	m_readByteFunc = &CPU::readByteInternal;

	// helper variables
	m_currentInstructionCyclesLeft = 4; // Let's say that reset takes 4 clock cycles
	m_isCBInstruction = false;
}

void CPU::doCycles(size_t cycles)
{
	while (cycles--) {
		if (m_currentInstructionCyclesLeft == 0) {

			uint8_t opcode = (this->*m_readByteFunc)(m_registerNamed.PC++);
			executeInstruction(opcode);
		}

		--m_currentInstructionCyclesLeft;
	}
}

void CPU::executeInstruction(uint8_t opcode)
{
	if (m_isCBInstruction)
		executeInstructionCBPrefix(opcode);
	else
		executeInstructionStandard(opcode);
}

void CPU::executeInstructionStandard(uint8_t opcode)
{
	// TODO: LD reg, reg looks like easly decodable...

	uint16_t tempWord;
	int8_t signedByte;
	uint8_t tempBit;

	switch (opcode) {
	case 0x00: m_currentInstructionCyclesLeft = 4; // NOP
		break;
	case 0x01: m_currentInstructionCyclesLeft = 12; // LD BC, u16
		m_registerNamed.BC = getImm16();
		break;
	case 0x02:  __debugbreak(); break;
	case 0x03: m_currentInstructionCyclesLeft = 8; // INC BC
		++m_registerNamed.BC;
		break;
	case 0x04: m_currentInstructionCyclesLeft = 4; // INC B
		tempBit = m_registerNamed.B & 0x10;
		++m_registerNamed.B;
		m_registerNamed.F.halfCarryFlag = ((m_registerNamed.B & 0x10) ^ tempBit) >> 4;
		m_registerNamed.F.subtractFlag = 0;
		m_registerNamed.F.zeroFlag = m_registerNamed.B == 0;
		break;
	case 0x05: m_currentInstructionCyclesLeft = 4; // DEC B
		tempBit = m_registerNamed.B & 0x10;
		--m_registerNamed.B;
		m_registerNamed.F.halfCarryFlag = ((m_registerNamed.B & 0x10) ^ tempBit) >> 4;
		m_registerNamed.F.subtractFlag = 1;
		m_registerNamed.F.zeroFlag = m_registerNamed.B == 0;
		break;
	case 0x06: m_currentInstructionCyclesLeft = 8; // LD B, u8
		m_registerNamed.B = (this->*m_readByteFunc)(m_registerNamed.PC++);
		break;
	case 0x07:  __debugbreak(); break;
	case 0x08:  __debugbreak(); break;
	case 0x09:  __debugbreak(); break;
	case 0x0A:  __debugbreak(); break;
	case 0x0B:  __debugbreak(); break;
	case 0x0C: m_currentInstructionCyclesLeft = 4; // INC C
		tempBit = m_registerNamed.C & 0x10;
		++m_registerNamed.C;
		m_registerNamed.F.halfCarryFlag = ((m_registerNamed.C & 0x10) ^ tempBit) >> 4;
		m_registerNamed.F.subtractFlag = 0;
		m_registerNamed.F.zeroFlag = m_registerNamed.C == 0;
		break;
	case 0x0D: m_currentInstructionCyclesLeft = 4; // DEC C
		tempBit = m_registerNamed.C & 0x10;
		--m_registerNamed.C;
		m_registerNamed.F.halfCarryFlag = ((m_registerNamed.C & 0x10) ^ tempBit) >> 4;
		m_registerNamed.F.subtractFlag = 1;
		m_registerNamed.F.zeroFlag = m_registerNamed.C == 0;
		break;
	case 0x0E: m_currentInstructionCyclesLeft = 8; // LD C, u8
		m_registerNamed.C = (this->*m_readByteFunc)(m_registerNamed.PC++);
		break;
	case 0x0F:  __debugbreak(); break;
	case 0x10:  __debugbreak(); break;
	case 0x11: m_currentInstructionCyclesLeft = 12; // LD DE, u16
		m_registerNamed.DE = getImm16();
		break;
	case 0x12:  __debugbreak(); break;
	case 0x13: m_currentInstructionCyclesLeft = 8; // INC DE
		++m_registerNamed.DE;
		break;
	case 0x14:  __debugbreak(); break;
	case 0x15: m_currentInstructionCyclesLeft = 4; // DEC D
		tempBit = m_registerNamed.D & 0x10;
		--m_registerNamed.D;
		m_registerNamed.F.halfCarryFlag = ((m_registerNamed.D & 0x10) ^ tempBit) >> 4;
		m_registerNamed.F.subtractFlag = 1;
		m_registerNamed.F.zeroFlag = m_registerNamed.D == 0;
		break;
	case 0x16: m_currentInstructionCyclesLeft = 8; // LD D, u8
		m_registerNamed.D = (this->*m_readByteFunc)(m_registerNamed.PC++);
		break;
	case 0x17: m_currentInstructionCyclesLeft = 4; // RLA
		tempBit = m_registerNamed.F.carryFlag;
		m_registerNamed.F.carryFlag = (m_registerNamed.A >> 7) & 1;
		m_registerNamed.A <<= 1;
		m_registerNamed.A |= tempBit;
		break;
	case 0x18: m_currentInstructionCyclesLeft = 12; // JR i8
		signedByte = (this->*m_readByteFunc)(m_registerNamed.PC++);
		m_registerNamed.PC += signedByte;
		break;
	case 0x19:  __debugbreak(); break;
	case 0x1A: m_currentInstructionCyclesLeft = 8; // LD A, (DE)
		m_registerNamed.A = (this->*m_readByteFunc)(m_registerNamed.DE);
		break;
	case 0x1B:  __debugbreak(); break;
	case 0x1C: m_currentInstructionCyclesLeft = 4; // INC E
		tempBit = m_registerNamed.E & 0x10;
		++m_registerNamed.E;
		m_registerNamed.F.halfCarryFlag = ((m_registerNamed.E & 0x10) ^ tempBit) >> 4;
		m_registerNamed.F.subtractFlag = 0;
		m_registerNamed.F.zeroFlag = m_registerNamed.E == 0;
		break;
	case 0x1D: m_currentInstructionCyclesLeft = 4; // DEC E
		tempBit = m_registerNamed.E & 0x10;
		--m_registerNamed.E;
		m_registerNamed.F.halfCarryFlag = ((m_registerNamed.E & 0x10) ^ tempBit) >> 4;
		m_registerNamed.F.subtractFlag = 1;
		m_registerNamed.F.zeroFlag = m_registerNamed.E == 0;
		break;
	case 0x1E: m_currentInstructionCyclesLeft = 8; // LD E, u8
		m_registerNamed.E = (this->*m_readByteFunc)(m_registerNamed.PC++);
		break;
	case 0x1F:  __debugbreak(); break;
	case 0x20: m_currentInstructionCyclesLeft = 8; // JR NZ, i8
		signedByte = (this->*m_readByteFunc)(m_registerNamed.PC++);
		if (m_registerNamed.F.zeroFlag == 0) {
			m_currentInstructionCyclesLeft += 4;
			m_registerNamed.PC += signedByte;
		}
		break;
	case 0x21: m_currentInstructionCyclesLeft = 12; // LD HL, u16
		m_registerNamed.HL = getImm16();
		break;
	case 0x22: m_currentInstructionCyclesLeft = 8; // LD (HL+), A
		storeByte(m_registerNamed.HL++, m_registerNamed.A);
		break;
	case 0x23: m_currentInstructionCyclesLeft = 8; // INC HL
		++m_registerNamed.HL;
		break;
	case 0x24: m_currentInstructionCyclesLeft = 4; // INC H
		tempBit = m_registerNamed.H & 0x10;
		++m_registerNamed.H;
		m_registerNamed.F.halfCarryFlag = ((m_registerNamed.H & 0x10) ^ tempBit) >> 4;
		m_registerNamed.F.subtractFlag = 0;
		m_registerNamed.F.zeroFlag = m_registerNamed.H == 0;
		break;
	case 0x25: m_currentInstructionCyclesLeft = 4; // DEC H
		tempBit = m_registerNamed.H & 0x10;
		--m_registerNamed.H;
		m_registerNamed.F.halfCarryFlag = ((m_registerNamed.H & 0x10) ^ tempBit) >> 4;
		m_registerNamed.F.subtractFlag = 1;
		m_registerNamed.F.zeroFlag = m_registerNamed.H == 0;
		break;
	case 0x26: m_currentInstructionCyclesLeft = 8; // LD H, u8
		m_registerNamed.H = (this->*m_readByteFunc)(m_registerNamed.PC++);
		break;
	case 0x27:  __debugbreak(); break;
	case 0x28: m_currentInstructionCyclesLeft = 8; // JR Z, i8
		signedByte = (this->*m_readByteFunc)(m_registerNamed.PC++);
		if (m_registerNamed.F.zeroFlag == 1) {
			m_currentInstructionCyclesLeft += 4;
			m_registerNamed.PC += signedByte;
		}
		break;
	case 0x29:  __debugbreak(); break;
	case 0x2A: m_currentInstructionCyclesLeft = 8; // LD A, (HL+)
		m_registerNamed.A = (this->*m_readByteFunc)(m_registerNamed.HL++);
		break;
	case 0x2B:  __debugbreak(); break;
	case 0x2C: m_currentInstructionCyclesLeft = 4; // INC L
		tempBit = m_registerNamed.L & 0x10;
		++m_registerNamed.L;
		m_registerNamed.F.halfCarryFlag = ((m_registerNamed.L & 0x10) ^ tempBit) >> 4;
		m_registerNamed.F.subtractFlag = 0;
		m_registerNamed.F.zeroFlag = m_registerNamed.L == 0;
		break;
	case 0x2D: m_currentInstructionCyclesLeft = 4; // DEC L
		tempBit = m_registerNamed.L & 0x10;
		--m_registerNamed.L;
		m_registerNamed.F.halfCarryFlag = ((m_registerNamed.L & 0x10) ^ tempBit) >> 4;
		m_registerNamed.F.subtractFlag = 1;
		m_registerNamed.F.zeroFlag = m_registerNamed.L == 0;
		break;
	case 0x2E: m_currentInstructionCyclesLeft = 8; // LD L, u8
		m_registerNamed.L = (this->*m_readByteFunc)(m_registerNamed.PC++);
		break;
	case 0x2F:  __debugbreak(); break;
	case 0x30:  __debugbreak(); break;
	case 0x31: m_currentInstructionCyclesLeft = 12; // LD SP, u16
		m_registerNamed.SP = getImm16();
		break;
	case 0x32: m_currentInstructionCyclesLeft = 8; // LD (HL-), A
		storeByte(m_registerNamed.HL--, m_registerNamed.A);
		break;
	case 0x33: m_currentInstructionCyclesLeft = 8; // INC SP
		++m_registerNamed.SP;
		break;
	case 0x34:  __debugbreak(); break;
	case 0x35:  __debugbreak(); break;
	case 0x36:  __debugbreak(); break;
	case 0x37:  __debugbreak(); break;
	case 0x38:  __debugbreak(); break;
	case 0x39:  __debugbreak(); break;
	case 0x3A:  __debugbreak(); break;
	case 0x3B:  __debugbreak(); break;
	case 0x3C: m_currentInstructionCyclesLeft = 4; // INC A
		tempBit = m_registerNamed.A & 0x10;
		++m_registerNamed.A;
		m_registerNamed.F.halfCarryFlag = ((m_registerNamed.A & 0x10) ^ tempBit) >> 4;
		m_registerNamed.F.subtractFlag = 0;
		m_registerNamed.F.zeroFlag = m_registerNamed.A == 0;
		break;
	case 0x3D: m_currentInstructionCyclesLeft = 4; // DEC A
		tempBit = m_registerNamed.A & 0x10;
		--m_registerNamed.A;
		m_registerNamed.F.halfCarryFlag = ((m_registerNamed.A & 0x10) ^ tempBit) >> 4;
		m_registerNamed.F.subtractFlag = 1;
		m_registerNamed.F.zeroFlag = m_registerNamed.A == 0;
		break;
	case 0x3E: m_currentInstructionCyclesLeft = 8; // LD C, u8
		m_registerNamed.A = (this->*m_readByteFunc)(m_registerNamed.PC++);
		break;
	case 0x3F:  __debugbreak(); break;
	case 0x40:  m_currentInstructionCyclesLeft = 4; // LD B, B
		m_registerNamed.B = m_registerNamed.B;
		break;
	case 0x41:  m_currentInstructionCyclesLeft = 4; // LD B, C
		m_registerNamed.B = m_registerNamed.C;
		break;
	case 0x42:  m_currentInstructionCyclesLeft = 4; // LD B, D
		m_registerNamed.B = m_registerNamed.D;
		break;
	case 0x43:  m_currentInstructionCyclesLeft = 4; // LD B, E
		m_registerNamed.B = m_registerNamed.E;
		break;
	case 0x44:  m_currentInstructionCyclesLeft = 4; // LD B, H
		m_registerNamed.B = m_registerNamed.H;
		break;
	case 0x45:  m_currentInstructionCyclesLeft = 4; // LD B, L
		m_registerNamed.B = m_registerNamed.L;
		break;
	case 0x46:  __debugbreak(); break;
	case 0x47:  m_currentInstructionCyclesLeft = 4; // LD B, A
		m_registerNamed.B = m_registerNamed.A;
		break;
	case 0x48:  m_currentInstructionCyclesLeft = 4; // LD C, B
		m_registerNamed.C = m_registerNamed.B;
		break;
	case 0x49:  m_currentInstructionCyclesLeft = 4; // LD C, C
		m_registerNamed.C = m_registerNamed.C;
		break;
	case 0x4A:  m_currentInstructionCyclesLeft = 4; // LD C, D
		m_registerNamed.C = m_registerNamed.D;
		break;
	case 0x4B:  m_currentInstructionCyclesLeft = 4; // LD C, E
		m_registerNamed.C = m_registerNamed.E;
		break;
	case 0x4C:  m_currentInstructionCyclesLeft = 4; // LD C, H
		m_registerNamed.C = m_registerNamed.H;
		break;
	case 0x4D:  m_currentInstructionCyclesLeft = 4; // LD C, L
		m_registerNamed.C = m_registerNamed.L;
		break;
	case 0x4E:  __debugbreak(); break;
	case 0x4F: m_currentInstructionCyclesLeft = 4; // LD C, A
		m_registerNamed.C = m_registerNamed.A;
		break;
	case 0x50:  __debugbreak(); break;
	case 0x51:  __debugbreak(); break;
	case 0x52:  __debugbreak(); break;
	case 0x53:  __debugbreak(); break;
	case 0x54:  __debugbreak(); break;
	case 0x55:  __debugbreak(); break;
	case 0x56:  __debugbreak(); break;
	case 0x57: m_currentInstructionCyclesLeft = 4; // LD D, A
		m_registerNamed.D = m_registerNamed.A;
		break;
	case 0x58:  __debugbreak(); break;
	case 0x59:  __debugbreak(); break;
	case 0x5A:  __debugbreak(); break;
	case 0x5B:  __debugbreak(); break;
	case 0x5C:  __debugbreak(); break;
	case 0x5D:  __debugbreak(); break;
	case 0x5E:  __debugbreak(); break;
	case 0x5F: m_currentInstructionCyclesLeft = 4; // LD E, A
		m_registerNamed.E = m_registerNamed.A;
		break;
	case 0x60:  __debugbreak(); break;
	case 0x61:  __debugbreak(); break;
	case 0x62:  __debugbreak(); break;
	case 0x63:  __debugbreak(); break;
	case 0x64:  __debugbreak(); break;
	case 0x65:  __debugbreak(); break;
	case 0x66:  __debugbreak(); break;
	case 0x67: m_currentInstructionCyclesLeft = 4; // LD H, A
		m_registerNamed.H = m_registerNamed.A;
		break;
	case 0x68:  __debugbreak(); break;
	case 0x69:  __debugbreak(); break;
	case 0x6A:  __debugbreak(); break;
	case 0x6B:  __debugbreak(); break;
	case 0x6C:  __debugbreak(); break;
	case 0x6D:  __debugbreak(); break;
	case 0x6E:  __debugbreak(); break;
	case 0x6F: m_currentInstructionCyclesLeft = 4; // LD L, A
		m_registerNamed.L = m_registerNamed.A;
		break;
	case 0x70:  __debugbreak(); break;
	case 0x71:  __debugbreak(); break;
	case 0x72:  __debugbreak(); break;
	case 0x73:  __debugbreak(); break;
	case 0x74:  __debugbreak(); break;
	case 0x75:  __debugbreak(); break;
	case 0x76:  __debugbreak(); break;
	case 0x77: m_currentInstructionCyclesLeft = 8; // LD (HL), A
		storeByte(m_registerNamed.HL, m_registerNamed.A);
		break;
	case 0x78: m_currentInstructionCyclesLeft = 4; // LD A, B
		m_registerNamed.A = m_registerNamed.B;
		break;
	case 0x79:  __debugbreak(); break;
	case 0x7A:  __debugbreak(); break;
	case 0x7B: m_currentInstructionCyclesLeft = 4; // LD A, E
		m_registerNamed.A = m_registerNamed.E;
		break;
	case 0x7C:  m_currentInstructionCyclesLeft = 4; // LD A, H
		m_registerNamed.A = m_registerNamed.H;
		break;
	case 0x7D: m_currentInstructionCyclesLeft = 4; // LD A, L
		m_registerNamed.A = m_registerNamed.L;
		break;
	case 0x7E:  __debugbreak(); break;
	case 0x7F: m_currentInstructionCyclesLeft = 4; // LD A, A
		m_registerNamed.A = m_registerNamed.A;
		break;
	case 0x80:  __debugbreak(); break;
	case 0x81:  __debugbreak(); break;
	case 0x82:  __debugbreak(); break;
	case 0x83:  __debugbreak(); break;
	case 0x84:  __debugbreak(); break;
	case 0x85:  __debugbreak(); break;
	case 0x86: m_currentInstructionCyclesLeft = 8; // ADD A, (HL)
		m_registerNamed.A += (this->*m_readByteFunc)(m_registerNamed.HL);
		m_registerNamed.F.zeroFlag = (m_registerNamed.A == 0);
		m_registerNamed.F.subtractFlag = 0;
		m_registerNamed.F.halfCarryFlag = 0; // TODO: set if not borrow from bit4
		m_registerNamed.F.carryFlag = 0; // TODO: ...
		break;
	case 0x87:  __debugbreak(); break;
	case 0x88:  __debugbreak(); break;
	case 0x89:  __debugbreak(); break;
	case 0x8A:  __debugbreak(); break;
	case 0x8B:  __debugbreak(); break;
	case 0x8C:  __debugbreak(); break;
	case 0x8D:  __debugbreak(); break;
	case 0x8E:  __debugbreak(); break;
	case 0x8F:  __debugbreak(); break;
	case 0x90: m_currentInstructionCyclesLeft = 4; // SUB B
		signedByte = m_registerNamed.A -= m_registerNamed.B;
		m_registerNamed.A -= m_registerNamed.B;
		m_registerNamed.F.zeroFlag = (m_registerNamed.A == 0);
		m_registerNamed.F.subtractFlag = 1;
		m_registerNamed.F.halfCarryFlag = 0; // TODO: set if not borrow from bit4
		m_registerNamed.F.carryFlag = (signedByte >= 0); // TODO: check this, propably wrong
		break;
	case 0x91:  __debugbreak(); break;
	case 0x92:  __debugbreak(); break;
	case 0x93:  __debugbreak(); break;
	case 0x94:  __debugbreak(); break;
	case 0x95:  __debugbreak(); break;
	case 0x96:  __debugbreak(); break;
	case 0x97:  __debugbreak(); break;
	case 0x98:  __debugbreak(); break;
	case 0x99:  __debugbreak(); break;
	case 0x9A:  __debugbreak(); break;
	case 0x9B:  __debugbreak(); break;
	case 0x9C:  __debugbreak(); break;
	case 0x9D:  __debugbreak(); break;
	case 0x9E:  __debugbreak(); break;
	case 0x9F:  __debugbreak(); break;
	case 0xA0:  __debugbreak(); break;
	case 0xA1:  __debugbreak(); break;
	case 0xA2:  __debugbreak(); break;
	case 0xA3:  __debugbreak(); break;
	case 0xA4:  __debugbreak(); break;
	case 0xA5:  __debugbreak(); break;
	case 0xA6:  __debugbreak(); break;
	case 0xA7:  __debugbreak(); break;
	case 0xA8:  __debugbreak(); break;
	case 0xA9:  __debugbreak(); break;
	case 0xAA:  __debugbreak(); break;
	case 0xAB:  __debugbreak(); break;
	case 0xAC:  __debugbreak(); break;
	case 0xAD:  __debugbreak(); break;
	case 0xAE:  __debugbreak(); break;
	case 0xAF: m_currentInstructionCyclesLeft = 4; // XOR A
		m_registerNamed.A ^= m_registerNamed.A;
		m_registerNamed.F.byte = 0;
		m_registerNamed.F.zeroFlag = m_registerNamed.A == 0;
		break;
	case 0xB0:  __debugbreak(); break;
	case 0xB1: m_currentInstructionCyclesLeft = 4; // OR C
		m_registerNamed.A |= m_registerNamed.C;
		m_registerNamed.F.byte = 0;
		m_registerNamed.F.zeroFlag = m_registerNamed.A == 0;
		break;
	case 0xB2:  __debugbreak(); break;
	case 0xB3:  __debugbreak(); break;
	case 0xB4:  __debugbreak(); break;
	case 0xB5:  __debugbreak(); break;
	case 0xB6:  __debugbreak(); break;
	case 0xB7:  __debugbreak(); break;
	case 0xB8:  __debugbreak(); break;
	case 0xB9:  __debugbreak(); break;
	case 0xBA:  __debugbreak(); break;
	case 0xBB:  __debugbreak(); break;
	case 0xBC:  __debugbreak(); break;
	case 0xBD:  __debugbreak(); break;
	case 0xBE: m_currentInstructionCyclesLeft = 8; // CP (HL)
		signedByte = m_registerNamed.A - (this->*m_readByteFunc)(m_registerNamed.HL);
		m_registerNamed.F.zeroFlag = (m_registerNamed.A == 0);
		m_registerNamed.F.subtractFlag = 1;
		m_registerNamed.F.halfCarryFlag = 0; // TODO: calculate halfCarry
		m_registerNamed.F.carryFlag = (signedByte < 0);
		break;
	case 0xBF:  __debugbreak(); break;
	case 0xC0:  __debugbreak(); break;
	case 0xC1: m_currentInstructionCyclesLeft = 12; // POP BC
		m_registerNamed.BC = popReg16();
		break;
	case 0xC2:  __debugbreak(); break;
	case 0xC3: m_currentInstructionCyclesLeft = 16; // JP u16
		m_registerNamed.PC = getImm16();
		break;
	case 0xC4: m_currentInstructionCyclesLeft = 12; // CALL NZ, u16
		tempWord = getImm16();
		if (m_registerNamed.F.zeroFlag == 0) {
			m_currentInstructionCyclesLeft += 12;
			pushReg16(m_registerNamed.PC + 2);
			m_registerNamed.PC = tempWord;
		}
		break;
	case 0xC5: m_currentInstructionCyclesLeft = 16; // PUSH BC
		pushReg16(m_registerNamed.BC);
		break;
	case 0xC6:  __debugbreak(); break;
	case 0xC7:  __debugbreak(); break;
	case 0xC8:  __debugbreak(); break;
	case 0xC9: m_currentInstructionCyclesLeft = 16; // RET
		m_registerNamed.PC = popReg16();
		break;
	case 0xCA:  __debugbreak(); break;
	case 0xCB: m_currentInstructionCyclesLeft = 4; // CB prefix
		m_isCBInstruction = true;
		break;
	case 0xCC:  __debugbreak(); break;
	case 0xCD: m_currentInstructionCyclesLeft = 24; // CALL u16
		pushReg16(m_registerNamed.PC + 2);
		m_registerNamed.PC = getImm16();
		break;
	case 0xCE:  __debugbreak(); break;
	case 0xCF:  __debugbreak(); break;
	case 0xD0:  __debugbreak(); break;
	case 0xD1: m_currentInstructionCyclesLeft = 12; // POP DE
		m_registerNamed.DE = popReg16();
		break;
	case 0xD2:  __debugbreak(); break;
	case 0xD3:  __debugbreak(); break;
	case 0xD4:  __debugbreak(); break;
	case 0xD5: m_currentInstructionCyclesLeft = 16; // PUSH DE
		pushReg16(m_registerNamed.DE);
		break;
	case 0xD6:  __debugbreak(); break;
	case 0xD7:  __debugbreak(); break;
	case 0xD8:  __debugbreak(); break;
	case 0xD9:  __debugbreak(); break;
	case 0xDA:  __debugbreak(); break;
	case 0xDB:  __debugbreak(); break;
	case 0xDC:  __debugbreak(); break;
	case 0xDD:  __debugbreak(); break;
	case 0xDE:  __debugbreak(); break;
	case 0xDF:  __debugbreak(); break;
	case 0xE0: m_currentInstructionCyclesLeft = 12; // LDH (u8), A
		storeByte(0xFF00 | (this->*m_readByteFunc)(m_registerNamed.PC++), m_registerNamed.A);
		break;
	case 0xE1: m_currentInstructionCyclesLeft = 12; // POP HL
		m_registerNamed.HL = popReg16();
		break;
	case 0xE2: m_currentInstructionCyclesLeft = 8; // LD (C), A
		storeByte(0xFF00 | m_registerNamed.C, m_registerNamed.A);
		break;
	case 0xE3:  __debugbreak(); break;
	case 0xE4:  __debugbreak(); break;
	case 0xE5: m_currentInstructionCyclesLeft = 16; // PUSH HL
		pushReg16(m_registerNamed.HL);
		break;
	case 0xE6: m_currentInstructionCyclesLeft = 8; // AND u8
		m_registerNamed.A &= (this->*m_readByteFunc)(m_registerNamed.PC++);
		m_registerNamed.F.byte = 0;
		m_registerNamed.F.zeroFlag = m_registerNamed.A == 0;
		m_registerNamed.F.halfCarryFlag = 1;
		break;
	case 0xE7:  __debugbreak(); break;
	case 0xE8:  __debugbreak(); break;
	case 0xE9:  __debugbreak(); break;
	case 0xEA: m_currentInstructionCyclesLeft = 16; // LD (u16), A
		storeByte(getImm16(), m_registerNamed.A);
		break;
	case 0xEB:  __debugbreak(); break;
	case 0xEC:  __debugbreak(); break;
	case 0xED:  __debugbreak(); break;
	case 0xEE:  __debugbreak(); break;
	case 0xEF:  __debugbreak(); break;
	case 0xF0: m_currentInstructionCyclesLeft = 12; // LDH A, (u8)
		m_registerNamed.A = (this->*m_readByteFunc)(0xFF00 | (this->*m_readByteFunc)(m_registerNamed.PC++));
		break;
	case 0xF1: m_currentInstructionCyclesLeft = 12; // POP AF
		m_registerNamed.AF = popReg16();
		break;
	case 0xF2:  __debugbreak(); break;
	case 0xF3: m_currentInstructionCyclesLeft = 4; // DI
		// TODO: impelement interrupts!s
		break;
	case 0xF4:  __debugbreak(); break;
	case 0xF5: m_currentInstructionCyclesLeft = 16; // PUSH AF
		pushReg16(m_registerNamed.AF);
		break;
	case 0xF6:  __debugbreak(); break;
	case 0xF7:  __debugbreak(); break;
	case 0xF8:  __debugbreak(); break;
	case 0xF9:  __debugbreak(); break;
	case 0xFA: m_currentInstructionCyclesLeft = 16; // LD A, (u16)
		m_registerNamed.A = (this->*m_readByteFunc)(getImm16());
		break;
	case 0xFB:  __debugbreak(); break;
	case 0xFC:  __debugbreak(); break;
	case 0xFD:  __debugbreak(); break;
	case 0xFE: m_currentInstructionCyclesLeft = 8; // CP u8
		signedByte = m_registerNamed.A - (this->*m_readByteFunc)(m_registerNamed.PC++);
		m_registerNamed.F.zeroFlag = (signedByte == 0);
		m_registerNamed.F.subtractFlag = 1;
		m_registerNamed.F.halfCarryFlag = 0; // TODO: calculate halfCarry
		m_registerNamed.F.carryFlag = (signedByte < 0);
		break;
	case 0xFF:  __debugbreak(); break;
	}
}

void CPU::executeInstructionCBPrefix(uint8_t opcode)
{
	uint8_t tempBit;
	m_isCBInstruction = false;
	m_currentInstructionCyclesLeft = 8;

	switch (opcode) {
	case 0x00:  __debugbreak(); break;
	case 0x01:  __debugbreak(); break;
	case 0x02:  __debugbreak(); break;
	case 0x03:  __debugbreak(); break;
	case 0x04:  __debugbreak(); break;
	case 0x05:  __debugbreak(); break;
	case 0x06:  __debugbreak(); break;
	case 0x07:  __debugbreak(); break;
	case 0x08:  __debugbreak(); break;
	case 0x09:  __debugbreak(); break;
	case 0x0A:  __debugbreak(); break;
	case 0x0B:  __debugbreak(); break;
	case 0x0C:  __debugbreak(); break;
	case 0x0D:  __debugbreak(); break;
	case 0x0E:  __debugbreak(); break;
	case 0x0F:  __debugbreak(); break;
	case 0x10:  __debugbreak(); break;
	case 0x11: // RL C
		tempBit = m_registerNamed.F.carryFlag;
		m_registerNamed.F.carryFlag = (m_registerNamed.C >> 7) & 1;
		m_registerNamed.C <<= 1;
		m_registerNamed.C |= tempBit;
		break;
	case 0x12:  __debugbreak(); break;
	case 0x13:  __debugbreak(); break;
	case 0x14:  __debugbreak(); break;
	case 0x15:  __debugbreak(); break;
	case 0x16:  __debugbreak(); break;
	case 0x17:  __debugbreak(); break;
	case 0x18:  __debugbreak(); break;
	case 0x19:  __debugbreak(); break;
	case 0x1A:  __debugbreak(); break;
	case 0x1B:  __debugbreak(); break;
	case 0x1C:  __debugbreak(); break;
	case 0x1D:  __debugbreak(); break;
	case 0x1E:  __debugbreak(); break;
	case 0x1F:  __debugbreak(); break;
	case 0x20:  __debugbreak(); break;
	case 0x21: __debugbreak(); break;
	case 0x22:  __debugbreak(); break;
	case 0x23:  __debugbreak(); break;
	case 0x24:  __debugbreak(); break;
	case 0x25:  __debugbreak(); break;
	case 0x26:  __debugbreak(); break;
	case 0x27:  __debugbreak(); break;
	case 0x28:  __debugbreak(); break;
	case 0x29:  __debugbreak(); break;
	case 0x2A:  __debugbreak(); break;
	case 0x2B:  __debugbreak(); break;
	case 0x2C:  __debugbreak(); break;
	case 0x2D:  __debugbreak(); break;
	case 0x2E:  __debugbreak(); break;
	case 0x2F:  __debugbreak(); break;
	case 0x30:  __debugbreak(); break;
	case 0x31: __debugbreak(); break;
	case 0x33:  __debugbreak(); break;
	case 0x34:  __debugbreak(); break;
	case 0x35:  __debugbreak(); break;
	case 0x36:  __debugbreak(); break;
	case 0x37:  __debugbreak(); break;
	case 0x38:  __debugbreak(); break;
	case 0x39:  __debugbreak(); break;
	case 0x3A:  __debugbreak(); break;
	case 0x3B:  __debugbreak(); break;
	case 0x3C:  __debugbreak(); break;
	case 0x3D:  __debugbreak(); break;
	case 0x3E:  __debugbreak(); break;
	case 0x3F:  __debugbreak(); break;
	case 0x40:  __debugbreak(); break;
	case 0x41:  __debugbreak(); break;
	case 0x42:  __debugbreak(); break;
	case 0x43:  __debugbreak(); break;
	case 0x44:  __debugbreak(); break;
	case 0x45:  __debugbreak(); break;
	case 0x46:  __debugbreak(); break;
	case 0x47:  __debugbreak(); break;
	case 0x48:  __debugbreak(); break;
	case 0x49:  __debugbreak(); break;
	case 0x4A:  __debugbreak(); break;
	case 0x4B:  __debugbreak(); break;
	case 0x4C:  __debugbreak(); break;
	case 0x4D:  __debugbreak(); break;
	case 0x4E:  __debugbreak(); break;
	case 0x4F:  __debugbreak(); break;
	case 0x50:  __debugbreak(); break;
	case 0x51:  __debugbreak(); break;
	case 0x52:  __debugbreak(); break;
	case 0x53:  __debugbreak(); break;
	case 0x54:  __debugbreak(); break;
	case 0x55:  __debugbreak(); break;
	case 0x56:  __debugbreak(); break;
	case 0x57:  __debugbreak(); break;
	case 0x58:  __debugbreak(); break;
	case 0x59:  __debugbreak(); break;
	case 0x5A:  __debugbreak(); break;
	case 0x5B:  __debugbreak(); break;
	case 0x5C:  __debugbreak(); break;
	case 0x5D:  __debugbreak(); break;
	case 0x5E:  __debugbreak(); break;
	case 0x5F:  __debugbreak(); break;
	case 0x60:  __debugbreak(); break;
	case 0x61:  __debugbreak(); break;
	case 0x62:  __debugbreak(); break;
	case 0x63:  __debugbreak(); break;
	case 0x64:  __debugbreak(); break;
	case 0x65:  __debugbreak(); break;
	case 0x66:  __debugbreak(); break;
	case 0x67:  __debugbreak(); break;
	case 0x68:  __debugbreak(); break;
	case 0x69:  __debugbreak(); break;
	case 0x6A:  __debugbreak(); break;
	case 0x6B:  __debugbreak(); break;
	case 0x6C:  __debugbreak(); break;
	case 0x6D:  __debugbreak(); break;
	case 0x6E:  __debugbreak(); break;
	case 0x6F:  __debugbreak(); break;
	case 0x70:  __debugbreak(); break;
	case 0x71:  __debugbreak(); break;
	case 0x72:  __debugbreak(); break;
	case 0x73:  __debugbreak(); break;
	case 0x74:  __debugbreak(); break;
	case 0x75:  __debugbreak(); break;
	case 0x76:  __debugbreak(); break;
	case 0x77:  __debugbreak(); break;
	case 0x78:  __debugbreak(); break;
	case 0x79:  __debugbreak(); break;
	case 0x7A:  __debugbreak(); break;
	case 0x7B:  __debugbreak(); break;
	case 0x7C: // BIT 7, H
		m_registerNamed.F.zeroFlag = !((m_registerNamed.H >> 7) & 1);
		m_registerNamed.F.subtractFlag = 0;
		m_registerNamed.F.halfCarryFlag = 1;
		break;
	case 0x7D:  __debugbreak(); break;
	case 0x7E:  __debugbreak(); break;
	case 0x7F:  __debugbreak(); break;
	case 0x80:  __debugbreak(); break;
	case 0x81:  __debugbreak(); break;
	case 0x82:  __debugbreak(); break;
	case 0x83:  __debugbreak(); break;
	case 0x84:  __debugbreak(); break;
	case 0x85:  __debugbreak(); break;
	case 0x86:  __debugbreak(); break;
	case 0x87:  __debugbreak(); break;
	case 0x88:  __debugbreak(); break;
	case 0x89:  __debugbreak(); break;
	case 0x8A:  __debugbreak(); break;
	case 0x8B:  __debugbreak(); break;
	case 0x8C:  __debugbreak(); break;
	case 0x8D:  __debugbreak(); break;
	case 0x8E:  __debugbreak(); break;
	case 0x8F:  __debugbreak(); break;
	case 0x90:  __debugbreak(); break;
	case 0x91:  __debugbreak(); break;
	case 0x92:  __debugbreak(); break;
	case 0x93:  __debugbreak(); break;
	case 0x94:  __debugbreak(); break;
	case 0x95:  __debugbreak(); break;
	case 0x96:  __debugbreak(); break;
	case 0x97:  __debugbreak(); break;
	case 0x98:  __debugbreak(); break;
	case 0x99:  __debugbreak(); break;
	case 0x9A:  __debugbreak(); break;
	case 0x9B:  __debugbreak(); break;
	case 0x9C:  __debugbreak(); break;
	case 0x9D:  __debugbreak(); break;
	case 0x9E:  __debugbreak(); break;
	case 0x9F:  __debugbreak(); break;
	case 0xA0:  __debugbreak(); break;
	case 0xA1:  __debugbreak(); break;
	case 0xA2:  __debugbreak(); break;
	case 0xA3:  __debugbreak(); break;
	case 0xA4:  __debugbreak(); break;
	case 0xA5:  __debugbreak(); break;
	case 0xA6:  __debugbreak(); break;
	case 0xA7:  __debugbreak(); break;
	case 0xA8:  __debugbreak(); break;
	case 0xA9:  __debugbreak(); break;
	case 0xAA:  __debugbreak(); break;
	case 0xAB:  __debugbreak(); break;
	case 0xAC:  __debugbreak(); break;
	case 0xAD:  __debugbreak(); break;
	case 0xAE:  __debugbreak(); break;
	case 0xAF: __debugbreak(); break;
	case 0xB0:  __debugbreak(); break;
	case 0xB1:  __debugbreak(); break;
	case 0xB2:  __debugbreak(); break;
	case 0xB3:  __debugbreak(); break;
	case 0xB4:  __debugbreak(); break;
	case 0xB5:  __debugbreak(); break;
	case 0xB6:  __debugbreak(); break;
	case 0xB7:  __debugbreak(); break;
	case 0xB8:  __debugbreak(); break;
	case 0xB9:  __debugbreak(); break;
	case 0xBA:  __debugbreak(); break;
	case 0xBB:  __debugbreak(); break;
	case 0xBC:  __debugbreak(); break;
	case 0xBD:  __debugbreak(); break;
	case 0xBE:  __debugbreak(); break;
	case 0xBF:  __debugbreak(); break;
	case 0xC0:  __debugbreak(); break;
	case 0xC1:  __debugbreak(); break;
	case 0xC2:  __debugbreak(); break;
	case 0xC3:  __debugbreak(); break;
	case 0xC4:  __debugbreak(); break;
	case 0xC5:  __debugbreak(); break;
	case 0xC6:  __debugbreak(); break;
	case 0xC7:  __debugbreak(); break;
	case 0xC8:  __debugbreak(); break;
	case 0xC9:  __debugbreak(); break;
	case 0xCA:  __debugbreak(); break;
	case 0xCB: __debugbreak(); break;
	case 0xCC:  __debugbreak(); break;
	case 0xCD:  __debugbreak(); break;
	case 0xCE:  __debugbreak(); break;
	case 0xCF:  __debugbreak(); break;
	case 0xD0:  __debugbreak(); break;
	case 0xD1:  __debugbreak(); break;
	case 0xD2:  __debugbreak(); break;
	case 0xD3:  __debugbreak(); break;
	case 0xD4:  __debugbreak(); break;
	case 0xD5:  __debugbreak(); break;
	case 0xD6:  __debugbreak(); break;
	case 0xD7:  __debugbreak(); break;
	case 0xD8:  __debugbreak(); break;
	case 0xD9:  __debugbreak(); break;
	case 0xDA:  __debugbreak(); break;
	case 0xDB:  __debugbreak(); break;
	case 0xDC:  __debugbreak(); break;
	case 0xDD:  __debugbreak(); break;
	case 0xDE:  __debugbreak(); break;
	case 0xDF:  __debugbreak(); break;
	case 0xE0:  __debugbreak(); break;
	case 0xE1:  __debugbreak(); break;
	case 0xE2:  __debugbreak(); break;
	case 0xE3:  __debugbreak(); break;
	case 0xE4:  __debugbreak(); break;
	case 0xE5:  __debugbreak(); break;
	case 0xE6:  __debugbreak(); break;
	case 0xE7:  __debugbreak(); break;
	case 0xE8:  __debugbreak(); break;
	case 0xE9:  __debugbreak(); break;
	case 0xEA:  __debugbreak(); break;
	case 0xEB:  __debugbreak(); break;
	case 0xEC:  __debugbreak(); break;
	case 0xED:  __debugbreak(); break;
	case 0xEE:  __debugbreak(); break;
	case 0xEF:  __debugbreak(); break;
	case 0xF0:  __debugbreak(); break;
	case 0xF1:  __debugbreak(); break;
	case 0xF2:  __debugbreak(); break;
	case 0xF3:  __debugbreak(); break;
	case 0xF4:  __debugbreak(); break;
	case 0xF5:  __debugbreak(); break;
	case 0xF6:  __debugbreak(); break;
	case 0xF7:  __debugbreak(); break;
	case 0xF8:  __debugbreak(); break;
	case 0xF9:  __debugbreak(); break;
	case 0xFA:  __debugbreak(); break;
	case 0xFB:  __debugbreak(); break;
	case 0xFC:  __debugbreak(); break;
	case 0xFD:  __debugbreak(); break;
	case 0xFE:  __debugbreak(); break;
	case 0xFF:  __debugbreak(); break;
	}
}

uint8_t CPU::readByte(uint16_t address) const
{
	if (address == 0xFFFF)
		return m_interruptControl;

	return m_memoryMap.load8(address);
}

#include "bootloader.inl"

uint8_t CPU::readByteInternal(uint16_t address) const
{
	if (address == 0xFFFF)
		return m_interruptControl;

	if (address < 0x100)
		return internalROM[address];

	return m_memoryMap.load8(address);
}

void CPU::storeByte(uint16_t address, uint8_t byte)
{
	switch (address) {
	case 0xFF50:
		m_readByteFunc = &CPU::readByte;
		break;
	case 0xFFFF:
		m_interruptControl = byte;
		break;
	default:
		m_memoryMap.store8(address, byte);
	}	
}

uint16_t CPU::getImm16()
{
	uint16_t value = (this->*m_readByteFunc)(m_registerNamed.PC + 1);
	value <<= 8;
	value |= (this->*m_readByteFunc)(m_registerNamed.PC);
	m_registerNamed.PC += 2;
	
	return value;
}

uint16_t CPU::popReg16()
{
	uint16_t value = (this->*m_readByteFunc)(m_registerNamed.SP + 1);
	value <<= 8;
	value |= (this->*m_readByteFunc)(m_registerNamed.SP);
	m_registerNamed.SP += 2;
	
	return value;
}

void CPU::pushReg16(uint16_t reg)
{
	m_registerNamed.SP -= 2;
	storeByte(m_registerNamed.SP, reg & 0xFF);
	storeByte(m_registerNamed.SP + 1, (reg >> 8) & 0xFF);
}
