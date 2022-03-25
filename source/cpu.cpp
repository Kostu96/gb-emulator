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

			uint8_t opcode = getImm8();
			executeInstruction(opcode);
		}

		--m_currentInstructionCyclesLeft;
		m_memoryMap.getPPU().tick();
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
	uint16_t tempWord;
	int8_t signedByte;
	uint8_t tempBit;
	m_currentInstructionCyclesLeft = 4;

	switch (opcode) {
	case 0x00: break;
	case 0x01: LDRR(m_registerNamed.BC, getImm16()); break;
	case 0x02: LDM(m_registerNamed.BC, m_registerNamed.A); break;
	case 0x03: INCRR(m_registerNamed.BC); break;
	case 0x04: INCR(m_registerNamed.B); break;
	case 0x05: DECR(m_registerNamed.B); break;
	case 0x06: LDR(m_registerNamed.B, getImm8()); break;
	case 0x07: RLCA(); break;
	case 0x08: __debugbreak(); break;
	case 0x09: ADDHL(m_registerNamed.BC); break;
	case 0x0A: __debugbreak(); break;
	case 0x0B: DECRR(m_registerNamed.BC); break;
	case 0x0C: INCR(m_registerNamed.C); break;
	case 0x0D: DECR(m_registerNamed.C); break;
	case 0x0E: LDR(m_registerNamed.C, getImm8()); break;
	case 0x0F: __debugbreak(); break;
	case 0x10: __debugbreak(); break;
	case 0x11: LDRR(m_registerNamed.DE, getImm16()); break;
	case 0x12: LDM(m_registerNamed.DE, m_registerNamed.A); break;
	case 0x13: INCRR(m_registerNamed.DE); break;
	case 0x14: INCR(m_registerNamed.D); break;
	case 0x15: DECR(m_registerNamed.D); break;
	case 0x16: LDR(m_registerNamed.D, getImm8()); break;
	case 0x17: RLA(); break;
	case 0x18: JR(true); break;
	case 0x19: ADDHL(m_registerNamed.DE); break;
	case 0x1A: LDR(m_registerNamed.A, (this->*m_readByteFunc)(m_registerNamed.DE)); break;
	case 0x1B: __debugbreak(); break;
	case 0x1C: INCR(m_registerNamed.E); break;
	case 0x1D: DECR(m_registerNamed.E); break;
	case 0x1E: LDR(m_registerNamed.E, getImm8()); break;
	case 0x1F: RRA(); break;
	case 0x20: JR(!m_registerNamed.F.zeroFlag); break;
	case 0x21: LDRR(m_registerNamed.HL, getImm16()); break;
	case 0x22: LDM(m_registerNamed.HL++, m_registerNamed.A); break;
	case 0x23: INCRR(m_registerNamed.HL); break;
	case 0x24: INCR(m_registerNamed.H); break;
	case 0x25: DECR(m_registerNamed.H); break;
	case 0x26: LDR(m_registerNamed.H, getImm8()); break;
	case 0x27: __debugbreak(); break;
	case 0x28: JR(m_registerNamed.F.zeroFlag); break;
	case 0x29: ADDHL(m_registerNamed.HL); break;
	case 0x2A: LDR(m_registerNamed.A, (this->*m_readByteFunc)(m_registerNamed.HL++)); break;
	case 0x2B: __debugbreak(); break;
	case 0x2C: INCR(m_registerNamed.L); break;
	case 0x2D: DECR(m_registerNamed.L); break;
	case 0x2E: LDR(m_registerNamed.L, getImm8()); break;
	case 0x2F: __debugbreak(); break;
	case 0x30: JR(!m_registerNamed.F.carryFlag); break;
	case 0x31: LDRR(m_registerNamed.SP, getImm16()); break;
	case 0x32: LDM(m_registerNamed.HL--, m_registerNamed.A); break;
	case 0x33: INCRR(m_registerNamed.SP); break;
	case 0x34: __debugbreak(); break;
	case 0x35: DECM(m_registerNamed.HL); break;
	case 0x36: LDM(m_registerNamed.HL, getImm8()); break;
	case 0x37: __debugbreak(); break;
	case 0x38: __debugbreak(); break;
	case 0x39: ADDHL(m_registerNamed.SP); break;
	case 0x3A: __debugbreak(); break;
	case 0x3B: __debugbreak(); break;
	case 0x3C: INCR(m_registerNamed.A); break;
	case 0x3D: DECR(m_registerNamed.A); break;
	case 0x3E: LDR(m_registerNamed.A, getImm8()); break;
	case 0x3F: __debugbreak(); break;
	case 0x40: LDR(m_registerNamed.B, m_registerNamed.B); break;
	case 0x41: LDR(m_registerNamed.B, m_registerNamed.C); break;
	case 0x42: LDR(m_registerNamed.B, m_registerNamed.D); break;
	case 0x43: LDR(m_registerNamed.B, m_registerNamed.E); break;
	case 0x44: LDR(m_registerNamed.B, m_registerNamed.H); break;
	case 0x45: LDR(m_registerNamed.B, m_registerNamed.L); break;
	case 0x46: LDR(m_registerNamed.B, (this->*m_readByteFunc)(m_registerNamed.HL)); break;
	case 0x47: LDR(m_registerNamed.B, m_registerNamed.A); break;
	case 0x48: LDR(m_registerNamed.C, m_registerNamed.B); break;
	case 0x49: LDR(m_registerNamed.C, m_registerNamed.C); break;
	case 0x4A: LDR(m_registerNamed.C, m_registerNamed.D); break;
	case 0x4B: LDR(m_registerNamed.C, m_registerNamed.E); break;
	case 0x4C: LDR(m_registerNamed.C, m_registerNamed.H); break;
	case 0x4D: LDR(m_registerNamed.C, m_registerNamed.L); break;
	case 0x4E: LDR(m_registerNamed.C, (this->*m_readByteFunc)(m_registerNamed.HL)); break;
	case 0x4F: LDR(m_registerNamed.C, m_registerNamed.A); break;
	case 0x50: LDR(m_registerNamed.D, m_registerNamed.B); break;
	case 0x51: LDR(m_registerNamed.D, m_registerNamed.C); break;
	case 0x52: LDR(m_registerNamed.D, m_registerNamed.D); break;
	case 0x53: LDR(m_registerNamed.D, m_registerNamed.E); break;
	case 0x54: LDR(m_registerNamed.D, m_registerNamed.H); break;
	case 0x55: LDR(m_registerNamed.D, m_registerNamed.L); break;
	case 0x56: LDR(m_registerNamed.D, (this->*m_readByteFunc)(m_registerNamed.HL)); break;
	case 0x57: LDR(m_registerNamed.A, m_registerNamed.L); break;
	case 0x58: LDR(m_registerNamed.E, m_registerNamed.B); break;
	case 0x59: LDR(m_registerNamed.E, m_registerNamed.C); break;
	case 0x5A: LDR(m_registerNamed.E, m_registerNamed.D); break;
	case 0x5B: LDR(m_registerNamed.E, m_registerNamed.E); break;
	case 0x5C: LDR(m_registerNamed.E, m_registerNamed.H); break;
	case 0x5D: LDR(m_registerNamed.E, m_registerNamed.L); break;
	case 0x5E: LDR(m_registerNamed.E, (this->*m_readByteFunc)(m_registerNamed.HL)); break;
	case 0x5F: LDR(m_registerNamed.E, m_registerNamed.H); break;
	case 0x60: LDR(m_registerNamed.H, m_registerNamed.B); break;
	case 0x61: LDR(m_registerNamed.H, m_registerNamed.C); break;
	case 0x62: LDR(m_registerNamed.H, m_registerNamed.D); break;
	case 0x63: LDR(m_registerNamed.H, m_registerNamed.E); break;
	case 0x64: LDR(m_registerNamed.H, m_registerNamed.H); break;
	case 0x65: LDR(m_registerNamed.H, m_registerNamed.L); break;
	case 0x66: LDR(m_registerNamed.H, (this->*m_readByteFunc)(m_registerNamed.HL)); break;
	case 0x67: LDR(m_registerNamed.H, m_registerNamed.A); break;
	case 0x68: LDR(m_registerNamed.L, m_registerNamed.B); break;
	case 0x69: LDR(m_registerNamed.L, m_registerNamed.C); break;
	case 0x6A: LDR(m_registerNamed.L, m_registerNamed.D); break;
	case 0x6B: LDR(m_registerNamed.L, m_registerNamed.E); break;
	case 0x6C: LDR(m_registerNamed.L, m_registerNamed.H); break;
	case 0x6D: LDR(m_registerNamed.L, m_registerNamed.L); break;
	case 0x6E: LDR(m_registerNamed.L, (this->*m_readByteFunc)(m_registerNamed.HL)); break;
	case 0x6F: LDR(m_registerNamed.L, m_registerNamed.A); break;
	case 0x70: LDM(m_registerNamed.HL, m_registerNamed.B); break;
	case 0x71: LDM(m_registerNamed.HL, m_registerNamed.C); break;
	case 0x72: LDM(m_registerNamed.HL, m_registerNamed.D); break;
	case 0x73: LDM(m_registerNamed.HL, m_registerNamed.E); break;
	case 0x74: LDM(m_registerNamed.HL, m_registerNamed.H); break;
	case 0x75: LDM(m_registerNamed.HL, m_registerNamed.L); break;
	case 0x76: __debugbreak(); break;
	case 0x77: LDM(m_registerNamed.HL, m_registerNamed.A); break;
	case 0x78: LDR(m_registerNamed.A, m_registerNamed.B); break;
	case 0x79: LDR(m_registerNamed.A, m_registerNamed.C); break;
	case 0x7A: LDR(m_registerNamed.A, m_registerNamed.D); break;
	case 0x7B: LDR(m_registerNamed.A, m_registerNamed.E); break;
	case 0x7C: LDR(m_registerNamed.A, m_registerNamed.H); break;
	case 0x7D: LDR(m_registerNamed.A, m_registerNamed.L); break;
	case 0x7E: LDR(m_registerNamed.A, (this->*m_readByteFunc)(m_registerNamed.HL)); break;
	case 0x7F: LDR(m_registerNamed.A, m_registerNamed.A); break;
	case 0x80: __debugbreak(); break;
	case 0x81: __debugbreak(); break;
	case 0x82: ADD(m_registerNamed.D); break;
	case 0x83: __debugbreak(); break;
	case 0x84: __debugbreak(); break;
	case 0x85: __debugbreak(); break;
	case 0x86: ADD((this->*m_readByteFunc)(m_registerNamed.HL)); break;
	case 0x87: __debugbreak(); break;
	case 0x88: __debugbreak(); break;
	case 0x89: __debugbreak(); break;
	case 0x8A: __debugbreak(); break;
	case 0x8B: __debugbreak(); break;
	case 0x8C: __debugbreak(); break;
	case 0x8D: ADC(m_registerNamed.L); break;
	case 0x8E: __debugbreak(); break;
	case 0x8F: __debugbreak(); break;
	case 0x90: SUB(m_registerNamed.B); break;
	case 0x91: __debugbreak(); break;
	case 0x92: __debugbreak(); break;
	case 0x93: __debugbreak(); break;
	case 0x94: __debugbreak(); break;
	case 0x95: __debugbreak(); break;
	case 0x96: __debugbreak(); break;
	case 0x97: __debugbreak(); break;
	case 0x98: __debugbreak(); break;
	case 0x99: __debugbreak(); break;
	case 0x9A: __debugbreak(); break;
	case 0x9B: __debugbreak(); break;
	case 0x9C: __debugbreak(); break;
	case 0x9D: __debugbreak(); break;
	case 0x9E: __debugbreak(); break;
	case 0x9F: __debugbreak(); break;
	case 0xA0: __debugbreak(); break;
	case 0xA1: __debugbreak(); break;
	case 0xA2: __debugbreak(); break;
	case 0xA3: __debugbreak(); break;
	case 0xA4: __debugbreak(); break;
	case 0xA5: __debugbreak(); break;
	case 0xA6: __debugbreak(); break;
	case 0xA7: __debugbreak(); break;
	case 0xA8: XOR(m_registerNamed.B); break;
	case 0xA9: XOR(m_registerNamed.C); break;
	case 0xAA: XOR(m_registerNamed.D); break;
	case 0xAB: XOR(m_registerNamed.E); break;
	case 0xAC: XOR(m_registerNamed.H); break;
	case 0xAD: XOR(m_registerNamed.L); break;
	case 0xAE: XOR((this->*m_readByteFunc)(m_registerNamed.HL)); break;
	case 0xAF: XOR(m_registerNamed.A); break;
	case 0xB0: OR(m_registerNamed.B); break;
	case 0xB1: OR(m_registerNamed.C); break;
	case 0xB2: OR(m_registerNamed.D); break;
	case 0xB3: OR(m_registerNamed.E); break;
	case 0xB4: OR(m_registerNamed.H); break;
	case 0xB5: OR(m_registerNamed.L); break;
	case 0xB6: OR((this->*m_readByteFunc)(m_registerNamed.HL)); break;
	case 0xB7: OR(m_registerNamed.A); break;
	case 0xB8: __debugbreak(); break;
	case 0xB9: __debugbreak(); break;
	case 0xBA: __debugbreak(); break;
	case 0xBB: __debugbreak(); break;
	case 0xBC: __debugbreak(); break;
	case 0xBD: __debugbreak(); break;
	case 0xBE: CP((this->*m_readByteFunc)(m_registerNamed.HL)); break;
	case 0xBF: __debugbreak(); break;
	case 0xC0: __debugbreak(); break;
	case 0xC1: m_registerNamed.BC = popReg16(); break;
	case 0xC2: JP(!m_registerNamed.F.zeroFlag); break;
	case 0xC3: JP(true); break;
	case 0xC4: CALL(!m_registerNamed.F.zeroFlag); break;
	case 0xC5: m_currentInstructionCyclesLeft += 4; pushReg16(m_registerNamed.BC); break;
	case 0xC6: ADD(getImm8()); break;
	case 0xC7: __debugbreak(); break;
	case 0xC8: RET(m_registerNamed.F.zeroFlag); break;
	case 0xC9: RET(true); break;
	case 0xCA:  __debugbreak(); break;
	case 0xCB: m_isCBInstruction = true; break;
	case 0xCC: __debugbreak(); break;
	case 0xCD: CALL(true); break;
	case 0xCE: ADC(getImm8()); break;
	case 0xCF: __debugbreak(); break;
	case 0xD0: RET(!m_registerNamed.F.carryFlag); break;
	case 0xD1: m_registerNamed.DE = popReg16(); break;
	case 0xD2: __debugbreak(); break;
	case 0xD3: __debugbreak(); break;
	case 0xD4: __debugbreak(); break;
	case 0xD5: m_currentInstructionCyclesLeft += 4; pushReg16(m_registerNamed.DE); break;
	case 0xD6: SUB(getImm8()); break;
	case 0xD7: __debugbreak(); break;
	case 0xD8: __debugbreak(); break;
	case 0xD9: __debugbreak(); break;
	case 0xDA: __debugbreak(); break;
	case 0xDB: __debugbreak(); break;
	case 0xDC: __debugbreak(); break;
	case 0xDD: __debugbreak(); break;
	case 0xDE: __debugbreak(); break;
	case 0xDF: __debugbreak(); break;
	case 0xE0: LDM(0xFF00 | getImm8(), m_registerNamed.A); break;
	case 0xE1: m_registerNamed.HL = popReg16(); break;
	case 0xE2: LDM(0xFF00 | m_registerNamed.C, m_registerNamed.A); break;
	case 0xE3: __debugbreak(); break;
	case 0xE4: __debugbreak(); break;
	case 0xE5: m_currentInstructionCyclesLeft += 4; pushReg16(m_registerNamed.HL); break;
	case 0xE6: AND(getImm8()); break;
	case 0xE7: __debugbreak(); break;
	case 0xE8: __debugbreak(); break;
	case 0xE9: JPHL(); break;
	case 0xEA: LDM(getImm16(), m_registerNamed.A); break;
	case 0xEB: __debugbreak(); break;
	case 0xEC: __debugbreak(); break;
	case 0xED: __debugbreak(); break;
	case 0xEE: XOR(getImm8()); break;
	case 0xEF: __debugbreak(); break;
	case 0xF0: LDR(m_registerNamed.A, (this->*m_readByteFunc)(0xFF00 | getImm8())); break;
	case 0xF1: m_registerNamed.AF = popReg16(); break;
	case 0xF2: __debugbreak(); break;
	case 0xF3: __debugbreak(); break;
	case 0xF4: __debugbreak(); break;
	case 0xF5: m_currentInstructionCyclesLeft += 4; pushReg16(m_registerNamed.AF); break;
	case 0xF6: OR(getImm8()); break;
	case 0xF7: __debugbreak(); break;
	case 0xF8: __debugbreak(); break;
	case 0xF9: LDRR(m_registerNamed.SP, m_registerNamed.HL); break;
	case 0xFA: LDR(m_registerNamed.A, (this->*m_readByteFunc)(getImm16())); break;
	case 0xFB: __debugbreak(); break;
	case 0xFC: __debugbreak(); break;
	case 0xFD: __debugbreak(); break;
	case 0xFE: CP(getImm8()); break;
	case 0xFF: __debugbreak(); break;
	default: __debugbreak();
	}
}

void CPU::executeInstructionCBPrefix(uint8_t opcode)
{
	uint8_t tempBit;
	m_isCBInstruction = false;
	m_currentInstructionCyclesLeft = 8;

	switch (opcode) {
	case 0x00: __debugbreak(); break;
	case 0x01: __debugbreak(); break;
	case 0x02: __debugbreak(); break;
	case 0x03: __debugbreak(); break;
	case 0x04: __debugbreak(); break;
	case 0x05: __debugbreak(); break;
	case 0x06: __debugbreak(); break;
	case 0x07: __debugbreak(); break;
	case 0x08: __debugbreak(); break;
	case 0x09: __debugbreak(); break;
	case 0x0A: __debugbreak(); break;
	case 0x0B: __debugbreak(); break;
	case 0x0C: __debugbreak(); break;
	case 0x0D: __debugbreak(); break;
	case 0x0E: __debugbreak(); break;
	case 0x0F: __debugbreak(); break;
	case 0x10: __debugbreak(); break;
	case 0x11: RL(m_registerNamed.C); break;
	case 0x12: __debugbreak(); break;
	case 0x13: __debugbreak(); break;
	case 0x14: __debugbreak(); break;
	case 0x15: __debugbreak(); break;
	case 0x16: __debugbreak(); break;
	case 0x17: __debugbreak(); break;
	case 0x18: RR(m_registerNamed.B); break;
	case 0x19: RR(m_registerNamed.C); break;
	case 0x1A: RR(m_registerNamed.D); break;
	case 0x1B: RR(m_registerNamed.E); break;
	case 0x1C: RR(m_registerNamed.H); break;
	case 0x1D: RR(m_registerNamed.L); break;
	case 0x1E: __debugbreak(); break;
	case 0x1F: RR(m_registerNamed.A); break;
	case 0x20: __debugbreak(); break;
	case 0x21: __debugbreak(); break;
	case 0x22: __debugbreak(); break;
	case 0x23: __debugbreak(); break;
	case 0x24: __debugbreak(); break;
	case 0x25: __debugbreak(); break;
	case 0x26: __debugbreak(); break;
	case 0x27: __debugbreak(); break;
	case 0x28: __debugbreak(); break;
	case 0x29: __debugbreak(); break;
	case 0x2A: __debugbreak(); break;
	case 0x2B: __debugbreak(); break;
	case 0x2C: __debugbreak(); break;
	case 0x2D: __debugbreak(); break;
	case 0x2E: __debugbreak(); break;
	case 0x2F: __debugbreak(); break;
	case 0x30: SWAP(m_registerNamed.B); break;
	case 0x31: SWAP(m_registerNamed.C); break;
	case 0x32: SWAP(m_registerNamed.D); break;
	case 0x33: SWAP(m_registerNamed.E); break;
	case 0x34: SWAP(m_registerNamed.H); break;
	case 0x35: SWAP(m_registerNamed.L); break;
	case 0x36: __debugbreak(); break;
	case 0x37: SWAP(m_registerNamed.A); break;
	case 0x38: SRL(m_registerNamed.B); break;
	case 0x39: SRL(m_registerNamed.C); break;
	case 0x3A: SRL(m_registerNamed.D); break;
	case 0x3B: SRL(m_registerNamed.E); break;
	case 0x3C: SRL(m_registerNamed.H); break;
	case 0x3D: SRL(m_registerNamed.L); break;
	case 0x3E: __debugbreak(); break;
	case 0x3F: SRL(m_registerNamed.A); break;
	case 0x40: __debugbreak(); break;
	case 0x41: __debugbreak(); break;
	case 0x42: __debugbreak(); break;
	case 0x43: __debugbreak(); break;
	case 0x44: __debugbreak(); break;
	case 0x45: __debugbreak(); break;
	case 0x46: __debugbreak(); break;
	case 0x47: __debugbreak(); break;
	case 0x48: __debugbreak(); break;
	case 0x49: __debugbreak(); break;
	case 0x4A: __debugbreak(); break;
	case 0x4B: __debugbreak(); break;
	case 0x4C: __debugbreak(); break;
	case 0x4D: __debugbreak(); break;
	case 0x4E: __debugbreak(); break;
	case 0x4F: __debugbreak(); break;
	case 0x50: __debugbreak(); break;
	case 0x51: __debugbreak(); break;
	case 0x52: __debugbreak(); break;
	case 0x53: __debugbreak(); break;
	case 0x54: __debugbreak(); break;
	case 0x55: __debugbreak(); break;
	case 0x56: __debugbreak(); break;
	case 0x57: __debugbreak(); break;
	case 0x58: __debugbreak(); break;
	case 0x59: __debugbreak(); break;
	case 0x5A: __debugbreak(); break;
	case 0x5B: __debugbreak(); break;
	case 0x5C: __debugbreak(); break;
	case 0x5D: __debugbreak(); break;
	case 0x5E: __debugbreak(); break;
	case 0x5F: __debugbreak(); break;
	case 0x60: __debugbreak(); break;
	case 0x61: __debugbreak(); break;
	case 0x62: __debugbreak(); break;
	case 0x63: __debugbreak(); break;
	case 0x64: __debugbreak(); break;
	case 0x65: __debugbreak(); break;
	case 0x66: __debugbreak(); break;
	case 0x67: __debugbreak(); break;
	case 0x68: __debugbreak(); break;
	case 0x69: __debugbreak(); break;
	case 0x6A: __debugbreak(); break;
	case 0x6B: __debugbreak(); break;
	case 0x6C: __debugbreak(); break;
	case 0x6D: __debugbreak(); break;
	case 0x6E: __debugbreak(); break;
	case 0x6F: __debugbreak(); break;
	case 0x70: __debugbreak(); break;
	case 0x71: __debugbreak(); break;
	case 0x72: __debugbreak(); break;
	case 0x73: __debugbreak(); break;
	case 0x74: __debugbreak(); break;
	case 0x75: __debugbreak(); break;
	case 0x76: __debugbreak(); break;
	case 0x77: __debugbreak(); break;
	case 0x78: BIT(7, m_registerNamed.B); break;
	case 0x79: BIT(7, m_registerNamed.C); break;
	case 0x7A: BIT(7, m_registerNamed.D); break;
	case 0x7B: BIT(7, m_registerNamed.E); break;
	case 0x7C: BIT(7, m_registerNamed.H); break;
	case 0x7D: BIT(7, m_registerNamed.L); break;
	case 0x7E: m_currentInstructionCyclesLeft += 4; BIT(7, (this->*m_readByteFunc)(m_registerNamed.HL)); break;
	case 0x7F: __debugbreak(); break;
	case 0x80: __debugbreak(); break;
	case 0x81: __debugbreak(); break;
	case 0x82: __debugbreak(); break;
	case 0x83: __debugbreak(); break;
	case 0x84: __debugbreak(); break;
	case 0x85: __debugbreak(); break;
	case 0x86: __debugbreak(); break;
	case 0x87: __debugbreak(); break;
	case 0x88: __debugbreak(); break;
	case 0x89: __debugbreak(); break;
	case 0x8A: __debugbreak(); break;
	case 0x8B: __debugbreak(); break;
	case 0x8C: __debugbreak(); break;
	case 0x8D: __debugbreak(); break;
	case 0x8E: __debugbreak(); break;
	case 0x8F: __debugbreak(); break;
	case 0x90: __debugbreak(); break;
	case 0x91: __debugbreak(); break;
	case 0x92: __debugbreak(); break;
	case 0x93: __debugbreak(); break;
	case 0x94: __debugbreak(); break;
	case 0x95: __debugbreak(); break;
	case 0x96: __debugbreak(); break;
	case 0x97: __debugbreak(); break;
	case 0x98: __debugbreak(); break;
	case 0x99: __debugbreak(); break;
	case 0x9A: __debugbreak(); break;
	case 0x9B: __debugbreak(); break;
	case 0x9C: __debugbreak(); break;
	case 0x9D: __debugbreak(); break;
	case 0x9E: __debugbreak(); break;
	case 0x9F: __debugbreak(); break;
	case 0xA0: __debugbreak(); break;
	case 0xA1: __debugbreak(); break;
	case 0xA2: __debugbreak(); break;
	case 0xA3: __debugbreak(); break;
	case 0xA4: __debugbreak(); break;
	case 0xA5: __debugbreak(); break;
	case 0xA6: __debugbreak(); break;
	case 0xA7: __debugbreak(); break;
	case 0xA8: __debugbreak(); break;
	case 0xA9: __debugbreak(); break;
	case 0xAA: __debugbreak(); break;
	case 0xAB: __debugbreak(); break;
	case 0xAC: __debugbreak(); break;
	case 0xAD: __debugbreak(); break;
	case 0xAE: __debugbreak(); break;
	case 0xAF: __debugbreak(); break;
	case 0xB0: __debugbreak(); break;
	case 0xB1: __debugbreak(); break;
	case 0xB2: __debugbreak(); break;
	case 0xB3: __debugbreak(); break;
	case 0xB4: __debugbreak(); break;
	case 0xB5: __debugbreak(); break;
	case 0xB6: __debugbreak(); break;
	case 0xB7: __debugbreak(); break;
	case 0xB8: __debugbreak(); break;
	case 0xB9: __debugbreak(); break;
	case 0xBA: __debugbreak(); break;
	case 0xBB: __debugbreak(); break;
	case 0xBC: __debugbreak(); break;
	case 0xBD: __debugbreak(); break;
	case 0xBE: __debugbreak(); break;
	case 0xBF: __debugbreak(); break;
	case 0xC0: __debugbreak(); break;
	case 0xC1: __debugbreak(); break;
	case 0xC2: __debugbreak(); break;
	case 0xC3: __debugbreak(); break;
	case 0xC4: __debugbreak(); break;
	case 0xC5: __debugbreak(); break;
	case 0xC6: SETM(0); break;
	case 0xC7: __debugbreak(); break;
	case 0xC8: __debugbreak(); break;
	case 0xC9: __debugbreak(); break;
	case 0xCA: __debugbreak(); break;
	case 0xCB: __debugbreak(); break;
	case 0xCC: __debugbreak(); break;
	case 0xCD: __debugbreak(); break;
	case 0xCE: SETM(1); break;
	case 0xCF: __debugbreak(); break;
	case 0xD0: __debugbreak(); break;
	case 0xD1: __debugbreak(); break;
	case 0xD2: __debugbreak(); break;
	case 0xD3: __debugbreak(); break;
	case 0xD4: __debugbreak(); break;
	case 0xD5: __debugbreak(); break;
	case 0xD6: SETM(2); break;
	case 0xD7: __debugbreak(); break;
	case 0xD8: __debugbreak(); break;
	case 0xD9: __debugbreak(); break;
	case 0xDA: __debugbreak(); break;
	case 0xDB: __debugbreak(); break;
	case 0xDC: __debugbreak(); break;
	case 0xDD: __debugbreak(); break;
	case 0xDE: SETM(3); break;
	case 0xDF: __debugbreak(); break;
	case 0xE0: __debugbreak(); break;
	case 0xE1: __debugbreak(); break;
	case 0xE2: __debugbreak(); break;
	case 0xE3: __debugbreak(); break;
	case 0xE4: __debugbreak(); break;
	case 0xE5: __debugbreak(); break;
	case 0xE6: SETM(4); break;
	case 0xE7: __debugbreak(); break;
	case 0xE8: __debugbreak(); break;
	case 0xE9: __debugbreak(); break;
	case 0xEA: __debugbreak(); break;
	case 0xEB: __debugbreak(); break;
	case 0xEC: __debugbreak(); break;
	case 0xED: __debugbreak(); break;
	case 0xEE: SETM(5); break;
	case 0xEF: __debugbreak(); break;
	case 0xF0: __debugbreak(); break;
	case 0xF1: __debugbreak(); break;
	case 0xF2: __debugbreak(); break;
	case 0xF3: __debugbreak(); break;
	case 0xF4: __debugbreak(); break;
	case 0xF5: __debugbreak(); break;
	case 0xF6: SETM(6); break;
	case 0xF7: __debugbreak(); break;
	case 0xF8: __debugbreak(); break;
	case 0xF9: __debugbreak(); break;
	case 0xFA: __debugbreak(); break;
	case 0xFB: __debugbreak(); break;
	case 0xFC: __debugbreak(); break;
	case 0xFD: __debugbreak(); break;
	case 0xFE: SETM(7); break;
	case 0xFF: __debugbreak(); break;
	}
}

#include "bootloader.inl"

uint8_t CPU::readByteInternal(uint16_t address)
{
	if (address < 0x100)
		return internalROM[address];

	return readByte(address);
}

uint8_t CPU::readByte(uint16_t address)
{
	m_currentInstructionCyclesLeft += 4;

	if (address == 0xFFFF)
		return m_interruptControl;

	return m_memoryMap.load8(address);
}

void CPU::storeByte(uint16_t address, uint8_t byte)
{
	m_currentInstructionCyclesLeft += 4;

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

uint8_t CPU::getImm8()
{
	return (this->*m_readByteFunc)(m_registerNamed.PC++);
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

void CPU::ADC(uint8_t value)
{
	uint16_t result = m_registerNamed.A + value + m_registerNamed.F.carryFlag;
	uint8_t result4bit = (m_registerNamed.A & 0xF) + (value & 0xF) + m_registerNamed.F.carryFlag;
	m_registerNamed.A = result;
	m_registerNamed.F.zeroFlag = (m_registerNamed.A == 0);
	m_registerNamed.F.subtractFlag = 0;
	m_registerNamed.F.halfCarryFlag = result4bit >> 4;
	m_registerNamed.F.carryFlag = result >> 8;
}

void CPU::ADD(uint8_t value)
{
	uint16_t result = m_registerNamed.A + value;
	uint8_t result4bit = (m_registerNamed.A & 0xF) + (value & 0xF);
	m_registerNamed.A = result;
	m_registerNamed.F.zeroFlag = (m_registerNamed.A == 0);
	m_registerNamed.F.subtractFlag = 0;
	m_registerNamed.F.halfCarryFlag = result4bit >> 4;
	m_registerNamed.F.carryFlag = result >> 8;
}

void CPU::ADDHL(uint16_t value)
{
	m_currentInstructionCyclesLeft += 4;
	uint32_t result = m_registerNamed.HL + value;
	uint16_t result12bit = (m_registerNamed.A & 0xFFF) + (value & 0xFFF);
	m_registerNamed.HL = result;
	m_registerNamed.F.zeroFlag = (m_registerNamed.HL == 0); // TODO: different specs
	m_registerNamed.F.subtractFlag = 0;
	m_registerNamed.F.halfCarryFlag = result12bit >> 12;
	m_registerNamed.F.carryFlag = result >> 16;
}

void CPU::AND(uint8_t value)
{
	m_registerNamed.A &= value;
	m_registerNamed.F.byte = 0;
	m_registerNamed.F.zeroFlag = m_registerNamed.A == 0;
	m_registerNamed.F.halfCarryFlag = 1;
}

void CPU::BIT(uint8_t bit, uint8_t value)
{
	m_registerNamed.F.zeroFlag = (value >> bit) & 1;
	m_registerNamed.F.subtractFlag = 0;
	m_registerNamed.F.halfCarryFlag = 1;
}

void CPU::CALL(bool flag)
{
	uint16_t address = getImm16();
	if (flag) {
		m_currentInstructionCyclesLeft += 4;
		pushReg16(m_registerNamed.PC);
		m_registerNamed.PC = address;
	}
}

void CPU::CP(uint8_t value)
{
	uint16_t result = (int16_t)m_registerNamed.A - (int16_t)value;
	m_registerNamed.F.zeroFlag = (result == 0);
	m_registerNamed.F.subtractFlag = 1;
	m_registerNamed.F.halfCarryFlag = 0; // TODO: calculate halfCarry
	m_registerNamed.F.carryFlag = (result >> 8) & 1;
}

void CPU::DECM(uint16_t address)
{
	uint8_t value = (this->*m_readByteFunc)(address);
	DECR(value);
	storeByte(address, value);
}

void CPU::DECR(uint8_t& reg)
{
	uint8_t tempBit = reg & 0x10;
	--reg;
	m_registerNamed.F.halfCarryFlag = ((reg & 0x10) ^ tempBit) >> 4;
	m_registerNamed.F.subtractFlag = 1;
	m_registerNamed.F.zeroFlag = reg == 0;
}

void CPU::DECRR(uint16_t& reg)
{
	m_currentInstructionCyclesLeft += 4;
	--reg;
}

void CPU::INCR(uint8_t& reg)
{
	uint8_t tempBit = reg & 0x10;
	++reg;
	m_registerNamed.F.halfCarryFlag = ((reg & 0x10) ^ tempBit) >> 4;
	m_registerNamed.F.subtractFlag = 0;
	m_registerNamed.F.zeroFlag = reg == 0;
}

void CPU::INCRR(uint16_t& reg)
{
	m_currentInstructionCyclesLeft += 4;
	++reg;
}

void CPU::JP(bool flag)
{
	uint16_t address = getImm16();
	if (flag) {
		m_currentInstructionCyclesLeft += 4;
		m_registerNamed.PC = address;
	}
}

void CPU::JPHL()
{
	m_registerNamed.PC = m_registerNamed.HL;
}

void CPU::JR(bool flag)
{
	int8_t signedByte = getImm8();
	if (flag) {
		m_currentInstructionCyclesLeft += 4;
		m_registerNamed.PC += signedByte;
	}
}

void CPU::LDM(uint16_t address, uint8_t value)
{
	storeByte(address, value);
}

void CPU::LDR(uint8_t& reg, uint8_t value)
{
	reg = value;
}

void CPU::LDRR(uint16_t& reg, uint16_t value)
{
	reg = value;
}

void CPU::OR(uint8_t value)
{
	m_registerNamed.A |= value;
	m_registerNamed.F.byte = 0;
	m_registerNamed.F.zeroFlag = m_registerNamed.A == 0;
}

void CPU::RET(bool flag)
{
	if (flag) {
		m_currentInstructionCyclesLeft += 4;
		m_registerNamed.PC = popReg16();
	}
}

void CPU::RL(uint8_t& reg)
{
	uint8_t oldCarry = m_registerNamed.F.carryFlag;
	m_registerNamed.F.byte = 0;
	m_registerNamed.F.carryFlag = reg >> 7;
	reg <<= 1;
	reg |= oldCarry;
	m_registerNamed.F.zeroFlag = (reg == 0);
}

void CPU::RLA()
{
	uint8_t oldCarry = m_registerNamed.F.carryFlag;
	m_registerNamed.F.byte = 0; // TODO: zero flag affected!
	m_registerNamed.F.carryFlag = m_registerNamed.A >> 7;
	m_registerNamed.A <<= 1;
	m_registerNamed.A |= oldCarry;
	// m_registerNamed.F.zeroFlag = 0; // TODO: not sure if not affected (Z80 Manual) or reset (pastraiser.com GB opcodes)
}

void CPU::RLCA()
{
	m_registerNamed.F.byte = 0;
	m_registerNamed.F.carryFlag = m_registerNamed.A >> 7;
	m_registerNamed.A <<= 1;
	m_registerNamed.A |= m_registerNamed.F.carryFlag;
}

void CPU::RR(uint8_t& reg)
{
	uint8_t oldCarry = m_registerNamed.F.carryFlag << 7;
	m_registerNamed.F.byte = 0;
	m_registerNamed.F.carryFlag = reg & 1;
	reg >>= 1;
	reg |= oldCarry;
	m_registerNamed.F.zeroFlag = (reg == 0);
}

void CPU::RRA()
{
	uint8_t oldCarry = m_registerNamed.F.carryFlag << 7;
	m_registerNamed.F.byte = 0; // TODO: zero flag affected!
	m_registerNamed.F.carryFlag = m_registerNamed.A & 1;
	m_registerNamed.A >>= 1;
	m_registerNamed.A |= oldCarry;
	// m_registerNamed.F.zeroFlag = 0; // TODO: not sure if not affected (Z80 Manual) or reset (pastraiser.com GB opcodes)
}

void CPU::SETM(uint8_t bit)
{
	uint8_t value = (this->*m_readByteFunc)(m_registerNamed.HL);
	value |= 1 << bit;
	storeByte(m_registerNamed.HL, value);
}

void CPU::SRL(uint8_t& reg)
{
	m_registerNamed.F.byte = 0;
	m_registerNamed.F.carryFlag = reg & 1;
	reg >>= 1;
	m_registerNamed.F.zeroFlag = (reg == 0);
}

void CPU::SUB(uint8_t value)
{
	int16_t result = m_registerNamed.A -= value;
	m_registerNamed.A = result & 0xFF;
	m_registerNamed.F.zeroFlag = (m_registerNamed.A == 0);
	m_registerNamed.F.subtractFlag = 1;
	m_registerNamed.F.halfCarryFlag = 0; // TODO: set if not borrow from bit4
	m_registerNamed.F.carryFlag = !(result & 0xFF00); // TODO: test this
}

void CPU::SWAP(uint8_t& reg)
{
	uint8_t lo = reg & 0xF;
	reg >>= 4;
	reg |= lo;
	m_registerNamed.F.byte = 0;
	m_registerNamed.F.zeroFlag = (reg == 0);
}

void CPU::XOR(uint8_t value)
{
	m_registerNamed.A ^= value;
	m_registerNamed.F.byte = 0;
	m_registerNamed.F.zeroFlag = m_registerNamed.A == 0;
}
