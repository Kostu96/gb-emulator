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
	F.byte = 0x0;
	PC = 0x0;

	m_interruptEnables.byte = 0x0;
	m_IRQs.byte = 0x0;
	m_interruptsMasterEnable = true;
	m_EICalled = false;

	m_currentInstructionCyclesLeft = 4; // Let's say that reset takes 4 clock cycles
	m_isCBInstruction = false;
	m_isHalted = false;

	m_readByteFunc = &CPU::readByteInternal;
}

void CPU::doCycles(size_t cycles)
{
	if (!m_isHalted) {
		while (cycles--) {
			if (m_currentInstructionCyclesLeft == 0) {

				uint8_t opcode = getImm8();
				executeInstruction(opcode);
			}

			--m_currentInstructionCyclesLeft;
			m_memoryMap.getPPU().tick();
		}
	}
	else {
		if (m_IRQs.byte & m_interruptEnables.byte)
			m_isHalted = false;
	}

	// TODO: interrupt should be connected to m_currentInstructionCyclesLeft
	// TODO: interrupts are checked before fething next instruction
	if (m_interruptsMasterEnable) {
		// TODO: move this code swh
		if (m_interruptEnables.fields.VBlank & m_IRQs.fields.VBlank) {
			m_currentInstructionCyclesLeft += 8;
			m_interruptsMasterEnable = false;
			m_IRQs.fields.VBlank = 0;
			RST(0x40);
		}
		else if (m_interruptEnables.fields.LCDStat & m_IRQs.fields.LCDStat) {
			m_currentInstructionCyclesLeft += 8;
			m_interruptsMasterEnable = false;
			m_IRQs.fields.LCDStat = 0;
			RST(0x48);
		}
		else if (m_interruptEnables.fields.timer & m_IRQs.fields.timer) {
			m_currentInstructionCyclesLeft += 8;
			m_interruptsMasterEnable = false;
			m_IRQs.fields.timer = 0;
			RST(0x50);
		}
		else if (m_interruptEnables.fields.serial & m_IRQs.fields.serial) {
			m_currentInstructionCyclesLeft += 8;
			m_interruptsMasterEnable = false;
			m_IRQs.fields.serial = 0;
			RST(0x58);
		}
		else if (m_interruptEnables.fields.joypad & m_IRQs.fields.joypad) {
			m_currentInstructionCyclesLeft += 8;
			m_interruptsMasterEnable = false;
			m_IRQs.fields.joypad = 0;
			RST(0x60);
		}
	}

	if (m_EICalled) {
		m_interruptsMasterEnable = true;
		m_EICalled = false;
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
	m_currentInstructionCyclesLeft = 4;

	switch (opcode) {
	case 0x00: break;
	case 0x01: LDRR(BC, getImm16()); break;
	case 0x02: LDM(BC, A); break;
	case 0x03: INCRR(BC); break;
	case 0x04: INCR(B); break;
	case 0x05: DECR(B); break;
	case 0x06: LDR(B, getImm8()); break;
	case 0x07: RLCA(); break;
	case 0x08: m_currentInstructionCyclesLeft += 8; LDM(getImm16(), SP); break;
	case 0x09: ADDHL(BC); break;
	case 0x0A: __debugbreak(); break;
	case 0x0B: DECRR(BC); break;
	case 0x0C: INCR(C); break;
	case 0x0D: DECR(C); break;
	case 0x0E: LDR(C, getImm8()); break;
	case 0x0F: RRCA(); break;
	case 0x10: __debugbreak(); break; //TODO: stop should have 0x10 0x00 otherwise it's undefined opode
	case 0x11: LDRR(DE, getImm16()); break;
	case 0x12: LDM(DE, A); break;
	case 0x13: INCRR(DE); break;
	case 0x14: INCR(D); break;
	case 0x15: DECR(D); break;
	case 0x16: LDR(D, getImm8()); break;
	case 0x17: RLA(); break;
	case 0x18: JR(true); break;
	case 0x19: ADDHL(DE); break;
	case 0x1A: LDR(A, (this->*m_readByteFunc)(DE)); break;
	case 0x1B: DECRR(DE); break;
	case 0x1C: INCR(E); break;
	case 0x1D: DECR(E); break;
	case 0x1E: LDR(E, getImm8()); break;
	case 0x1F: RRA(); break;
	case 0x20: JR(!F.zeroFlag); break;
	case 0x21: LDRR(HL, getImm16()); break;
	case 0x22: LDM(HL++, A); break;
	case 0x23: INCRR(HL); break;
	case 0x24: INCR(H); break;
	case 0x25: DECR(H); break;
	case 0x26: LDR(H, getImm8()); break;
	case 0x27: DAA(); break;
	case 0x28: JR(F.zeroFlag); break;
	case 0x29: ADDHL(HL); break;
	case 0x2A: LDR(A, (this->*m_readByteFunc)(HL++)); break;
	case 0x2B: DECRR(HL); break;
	case 0x2C: INCR(L); break;
	case 0x2D: DECR(L); break;
	case 0x2E: LDR(L, getImm8()); break;
	case 0x2F: CPL(); break;
	case 0x30: JR(!F.carryFlag); break;
	case 0x31: LDRR(SP, getImm16()); break;
	case 0x32: LDM(HL--, A); break;
	case 0x33: INCRR(SP); break;
	case 0x34: __debugbreak(); break;
	case 0x35: DECM(HL); break;
	case 0x36: LDM(HL, getImm8()); break;
	case 0x37: SCF(); break;
	case 0x38: JR(F.carryFlag); break;
	case 0x39: ADDHL(SP); break;
	case 0x3A: __debugbreak(); break;
	case 0x3B: __debugbreak(); break;
	case 0x3C: INCR(A); break;
	case 0x3D: DECR(A); break;
	case 0x3E: LDR(A, getImm8()); break;
	case 0x3F: CCF(); break;
	case 0x40: LDR(B, B); break;
	case 0x41: LDR(B, C); break;
	case 0x42: LDR(B, D); break;
	case 0x43: LDR(B, E); break;
	case 0x44: LDR(B, H); break;
	case 0x45: LDR(B, L); break;
	case 0x46: LDR(B, (this->*m_readByteFunc)(HL)); break;
	case 0x47: LDR(B, A); break;
	case 0x48: LDR(C, B); break;
	case 0x49: LDR(C, C); break;
	case 0x4A: LDR(C, D); break;
	case 0x4B: LDR(C, E); break;
	case 0x4C: LDR(C, H); break;
	case 0x4D: LDR(C, L); break;
	case 0x4E: LDR(C, (this->*m_readByteFunc)(HL)); break;
	case 0x4F: LDR(C, A); break;
	case 0x50: LDR(D, B); break;
	case 0x51: LDR(D, C); break;
	case 0x52: LDR(D, D); break;
	case 0x53: LDR(D, E); break;
	case 0x54: LDR(D, H); break;
	case 0x55: LDR(D, L); break;
	case 0x56: LDR(D, (this->*m_readByteFunc)(HL)); break;
	case 0x57: LDR(A, L); break;
	case 0x58: LDR(E, B); break;
	case 0x59: LDR(E, C); break;
	case 0x5A: LDR(E, D); break;
	case 0x5B: LDR(E, E); break;
	case 0x5C: LDR(E, H); break;
	case 0x5D: LDR(E, L); break;
	case 0x5E: LDR(E, (this->*m_readByteFunc)(HL)); break;
	case 0x5F: LDR(E, H); break;
	case 0x60: LDR(H, B); break;
	case 0x61: LDR(H, C); break;
	case 0x62: LDR(H, D); break;
	case 0x63: LDR(H, E); break;
	case 0x64: LDR(H, H); break;
	case 0x65: LDR(H, L); break;
	case 0x66: LDR(H, (this->*m_readByteFunc)(HL)); break;
	case 0x67: LDR(H, A); break;
	case 0x68: LDR(L, B); break;
	case 0x69: LDR(L, C); break;
	case 0x6A: LDR(L, D); break;
	case 0x6B: LDR(L, E); break;
	case 0x6C: LDR(L, H); break;
	case 0x6D: LDR(L, L); break;
	case 0x6E: LDR(L, (this->*m_readByteFunc)(HL)); break;
	case 0x6F: LDR(L, A); break;
	case 0x70: LDM(HL, B); break;
	case 0x71: LDM(HL, C); break;
	case 0x72: LDM(HL, D); break;
	case 0x73: LDM(HL, E); break;
	case 0x74: LDM(HL, H); break;
	case 0x75: LDM(HL, L); break;
	case 0x76: m_isHalted = true; break;
	case 0x77: LDM(HL, A); break;
	case 0x78: LDR(A, B); break;
	case 0x79: LDR(A, C); break;
	case 0x7A: LDR(A, D); break;
	case 0x7B: LDR(A, E); break;
	case 0x7C: LDR(A, H); break;
	case 0x7D: LDR(A, L); break;
	case 0x7E: LDR(A, (this->*m_readByteFunc)(HL)); break;
	case 0x7F: LDR(A, A); break;
	case 0x80: ADD(B); break;
	case 0x81: ADD(C); break;
	case 0x82: ADD(D); break;
	case 0x83: ADD(E); break;
	case 0x84: ADD(H); break;
	case 0x85: ADD(L); break;
	case 0x86: ADD((this->*m_readByteFunc)(HL)); break;
	case 0x87: ADD(A); break;
	case 0x88: ADC(B); break;
	case 0x89: ADC(C); break;
	case 0x8A: ADC(D); break;
	case 0x8B: ADC(E); break;
	case 0x8C: ADC(H); break;
	case 0x8D: ADC(L); break;
	case 0x8E: ADC((this->*m_readByteFunc)(HL)); break;
	case 0x8F: ADC(A); break;
	case 0x90: SUB(B); break;
	case 0x91: SUB(C); break;
	case 0x92: SUB(D); break;
	case 0x93: SUB(E); break;
	case 0x94: SUB(H); break;
	case 0x95: SUB(L); break;
	case 0x96: SUB((this->*m_readByteFunc)(HL)); break;
	case 0x97: SUB(A); break;
	case 0x98: SBC(B); break;
	case 0x99: SBC(C); break;
	case 0x9A: SBC(D); break;
	case 0x9B: SBC(E); break;
	case 0x9C: SBC(H); break;
	case 0x9D: SBC(L); break;
	case 0x9E: SBC((this->*m_readByteFunc)(HL)); break;
	case 0x9F: SBC(A); break;
	case 0xA0: AND(B); break;
	case 0xA1: AND(C); break;
	case 0xA2: AND(D); break;
	case 0xA3: AND(E); break;
	case 0xA4: AND(H); break;
	case 0xA5: AND(L); break;
	case 0xA6: AND((this->*m_readByteFunc)(HL)); break;
	case 0xA7: AND(A); break;
	case 0xA8: XOR(B); break;
	case 0xA9: XOR(C); break;
	case 0xAA: XOR(D); break;
	case 0xAB: XOR(E); break;
	case 0xAC: XOR(H); break;
	case 0xAD: XOR(L); break;
	case 0xAE: XOR((this->*m_readByteFunc)(HL)); break;
	case 0xAF: XOR(A); break;
	case 0xB0: OR(B); break;
	case 0xB1: OR(C); break;
	case 0xB2: OR(D); break;
	case 0xB3: OR(E); break;
	case 0xB4: OR(H); break;
	case 0xB5: OR(L); break;
	case 0xB6: OR((this->*m_readByteFunc)(HL)); break;
	case 0xB7: OR(A); break;
	case 0xB8: CP(B); break;
	case 0xB9: CP(C); break;
	case 0xBA: CP(D); break;
	case 0xBB: CP(E); break;
	case 0xBC: CP(H); break;
	case 0xBD: CP(L); break;
	case 0xBE: CP((this->*m_readByteFunc)(HL)); break;
	case 0xBF: CP(A); break;
	case 0xC0: RET(!F.zeroFlag); break;
	case 0xC1: BC = popReg16(); break;
	case 0xC2: JP(!F.zeroFlag); break;
	case 0xC3: JP(true); break;
	case 0xC4: CALL(!F.zeroFlag); break;
	case 0xC5: m_currentInstructionCyclesLeft += 4; pushReg16(BC); break;
	case 0xC6: ADD(getImm8()); break;
	case 0xC7: RST(0x00); break;
	case 0xC8: RET(F.zeroFlag); break;
	case 0xC9: RET(true); break;
	case 0xCA:  __debugbreak(); break;
	case 0xCB: m_isCBInstruction = true; break;
	case 0xCC: __debugbreak(); break;
	case 0xCD: CALL(true); break;
	case 0xCE: ADC(getImm8()); break;
	case 0xCF: RST(0x08); break;
	case 0xD0: RET(!F.carryFlag); break;
	case 0xD1: DE = popReg16(); break;
	case 0xD2: __debugbreak(); break;
	case 0xD3: __debugbreak(); break;
	case 0xD4: __debugbreak(); break;
	case 0xD5: m_currentInstructionCyclesLeft += 4; pushReg16(DE); break;
	case 0xD6: SUB(getImm8()); break;
	case 0xD7: RST(0x10); break;
	case 0xD8: RET(F.carryFlag); break;
	case 0xD9: m_EICalled = true; RET(true); break;
	case 0xDA: __debugbreak(); break;
	case 0xDB: __debugbreak(); break;
	case 0xDC: __debugbreak(); break;
	case 0xDD: __debugbreak(); break;
	case 0xDE: SBC(getImm8()); break;
	case 0xDF: RST(0x18); break;
	case 0xE0: LDM(0xFF00 | getImm8(), A); break;
	case 0xE1: HL = popReg16(); break;
	case 0xE2: LDM(0xFF00 | C, A); break;
	case 0xE3: __debugbreak(); break;
	case 0xE4: __debugbreak(); break;
	case 0xE5: m_currentInstructionCyclesLeft += 4; pushReg16(HL); break;
	case 0xE6: AND(getImm8()); break;
	case 0xE7: RST(0x20); break;
	case 0xE8: __debugbreak(); break;
	case 0xE9: JPHL(); break;
	case 0xEA: LDM(getImm16(), A); break;
	case 0xEB: __debugbreak(); break;
	case 0xEC: __debugbreak(); break;
	case 0xED: __debugbreak(); break;
	case 0xEE: XOR(getImm8()); break;
	case 0xEF: RST(0x28); break;
	case 0xF0: LDR(A, (this->*m_readByteFunc)(0xFF00 | getImm8())); break;
	case 0xF1: AF = popReg16(); F.alwaysZero = 0; break;
	case 0xF2: __debugbreak(); break;
	case 0xF3: m_interruptsMasterEnable = false; break;
	case 0xF4: __debugbreak(); break;
	case 0xF5: m_currentInstructionCyclesLeft += 4; pushReg16(AF); break;
	case 0xF6: OR(getImm8()); break;
	case 0xF7: RST(0x30); break;
	case 0xF8: m_currentInstructionCyclesLeft += 4; LDRR(HL, SP + getImm8()); break;
	case 0xF9: m_currentInstructionCyclesLeft += 4; LDRR(SP, HL); break;
	case 0xFA: LDR(A, (this->*m_readByteFunc)(getImm16())); break;
	case 0xFB: m_EICalled = true; break;
	case 0xFC: __debugbreak(); break;
	case 0xFD: __debugbreak(); break;
	case 0xFE: CP(getImm8()); break;
	case 0xFF: RST(0x38); break;
	default: __debugbreak();
	}
}

void CPU::executeInstructionCBPrefix(uint8_t opcode)
{
	uint8_t tempBit;
	m_isCBInstruction = false;
	m_currentInstructionCyclesLeft = 8;

	switch (opcode) {
	case 0x00: RLC(B); break;
	case 0x01: RLC(C); break;
	case 0x02: RLC(D); break;
	case 0x03: RLC(E); break;
	case 0x04: RLC(H); break;
	case 0x05: RLC(L); break;
	case 0x06: // m_currentInstructionCyclesLeft += 4; RLC((this->*m_readByteFunc)(HL));
		__debugbreak(); break;
	case 0x07: RRC(A); break;
	case 0x08: RRC(B); break;
	case 0x09: RRC(C); break;
	case 0x0A: RRC(D); break;
	case 0x0B: RRC(E); break;
	case 0x0C: RRC(H); break;
	case 0x0D: RRC(L); break;
	case 0x0E: __debugbreak(); break;
	case 0x0F: RRC(A); break;
	case 0x10: RL(B); break;
	case 0x11: RL(C); break;
	case 0x12: RL(D); break;
	case 0x13: RL(E); break;
	case 0x14: RL(H); break;
	case 0x15: RL(L); break;
	case 0x16: __debugbreak(); break;
	case 0x17: RL(A); break;
	case 0x18: RR(B); break;
	case 0x19: RR(C); break;
	case 0x1A: RR(D); break;
	case 0x1B: RR(E); break;
	case 0x1C: RR(H); break;
	case 0x1D: RR(L); break;
	case 0x1E: __debugbreak(); break;
	case 0x1F: SLA(A); break;
	case 0x20: SLA(B); break;
	case 0x21: SLA(C); break;
	case 0x22: SLA(D); break;
	case 0x23: SLA(E); break;
	case 0x24: SLA(H); break;
	case 0x25: SLA(L); break;
	case 0x26: __debugbreak(); break;
	case 0x27: SLA(A); break;
	case 0x28: SRA(B); break;
	case 0x29: SRA(C); break;
	case 0x2A: SRA(D); break;
	case 0x2B: SRA(E); break;
	case 0x2C: SRA(H); break;
	case 0x2D: SRA(L); break;
	case 0x2E: __debugbreak(); break;
	case 0x2F: SRA(A); break;
	case 0x30: SWAP(B); break;
	case 0x31: SWAP(C); break;
	case 0x32: SWAP(D); break;
	case 0x33: SWAP(E); break;
	case 0x34: SWAP(H); break;
	case 0x35: SWAP(L); break;
	case 0x36: __debugbreak(); break;
	case 0x37: SWAP(A); break;
	case 0x38: SRL(B); break;
	case 0x39: SRL(C); break;
	case 0x3A: SRL(D); break;
	case 0x3B: SRL(E); break;
	case 0x3C: SRL(H); break;
	case 0x3D: SRL(L); break;
	case 0x3E: __debugbreak(); break;
	case 0x3F: SRL(A); break;
	case 0x40: BIT(0, B); break;
	case 0x41: BIT(0, C); break;
	case 0x42: BIT(0, D); break;
	case 0x43: BIT(0, E); break;
	case 0x44: BIT(0, H); break;
	case 0x45: BIT(0, L); break;
	case 0x46: __debugbreak(); break;
	case 0x47: BIT(0, A); break;
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
	case 0x78: BIT(7, B); break;
	case 0x79: BIT(7, C); break;
	case 0x7A: BIT(7, D); break;
	case 0x7B: BIT(7, E); break;
	case 0x7C: BIT(7, H); break;
	case 0x7D: BIT(7, L); break;
	case 0x7E: m_currentInstructionCyclesLeft += 4; BIT(7, (this->*m_readByteFunc)(HL)); break;
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
	default: __debugbreak();
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

	switch (address) {
	case 0xFF0F:
		return m_IRQs.byte;
	case 0xFFFF:
		return m_interruptEnables.byte;
	default:
		return m_memoryMap.load8(address);
	}
}

void CPU::storeByte(uint16_t address, uint8_t byte)
{
	m_currentInstructionCyclesLeft += 4;

	switch (address) {
	case 0xFF50:
		m_readByteFunc = &CPU::readByte;
		break;
	case 0xFF0F:
		m_IRQs.byte = byte;
		break;
	case 0xFFFF:
		m_interruptEnables.byte = byte;
		break;
	default:
		m_memoryMap.store8(address, byte);
	}	
}

uint8_t CPU::getImm8()
{
	return (this->*m_readByteFunc)(PC++);
}

uint16_t CPU::getImm16()
{
	uint16_t value = (this->*m_readByteFunc)(PC + 1);
	value <<= 8;
	value |= (this->*m_readByteFunc)(PC);
	PC += 2;
	
	return value;
}

uint16_t CPU::popReg16()
{
	uint16_t value = (this->*m_readByteFunc)(SP + 1);
	value <<= 8;
	value |= (this->*m_readByteFunc)(SP);
	SP += 2;
	
	return value;
}

void CPU::pushReg16(uint16_t reg)
{
	SP -= 2;
	storeByte(SP, reg & 0xFF);
	storeByte(SP + 1, (reg >> 8) & 0xFF);
}

void CPU::ADC(uint8_t value)
{
	uint16_t result = A + value + F.carryFlag;
	uint8_t result4bit = (A & 0xF) + (value & 0xF) + F.carryFlag;
	A = result;
	F.zeroFlag = (A == 0);
	F.subtractFlag = 0;
	F.halfCarryFlag = result4bit >> 4;
	F.carryFlag = result >> 8;
}

void CPU::ADD(uint8_t value)
{
	uint16_t result = A + value;
	uint8_t result4bit = (A & 0xF) + (value & 0xF);
	A = result;
	F.zeroFlag = (A == 0);
	F.subtractFlag = 0;
	F.halfCarryFlag = result4bit >> 4;
	F.carryFlag = result >> 8;
}

void CPU::ADDHL(uint16_t value)
{
	m_currentInstructionCyclesLeft += 4;
	uint32_t result = HL + value;
	uint16_t result12bit = (A & 0xFFF) + (value & 0xFFF);
	HL = result;
	F.zeroFlag = (HL == 0); // TODO: different specs
	F.subtractFlag = 0;
	F.halfCarryFlag = result12bit >> 12;
	F.carryFlag = result >> 16;
}

void CPU::AND(uint8_t value)
{
	A &= value;
	F.byte = 0;
	F.zeroFlag = A == 0;
	F.halfCarryFlag = 1;
}

void CPU::BIT(uint8_t bit, uint8_t value)
{
	F.zeroFlag = (value >> bit) & 1;
	F.subtractFlag = 0;
	F.halfCarryFlag = 1;
}

void CPU::CALL(bool flag)
{
	uint16_t address = getImm16();
	if (flag) {
		m_currentInstructionCyclesLeft += 4;
		pushReg16(PC);
		PC = address;
	}
}

void CPU::CCF()
{
	F.subtractFlag = 0;
	F.halfCarryFlag = 0;
	F.carryFlag ^= 1;
}

void CPU::CP(uint8_t value)
{
	uint16_t result = (int16_t)A - (int16_t)value;
	uint8_t result4bit = (int8_t)A - (int8_t)value;
	F.zeroFlag = (result == 0);
	F.subtractFlag = 1;
	F.halfCarryFlag = (result4bit >> 4) & 1;
	F.carryFlag = (result >> 8) & 1;
}

void CPU::CPL()
{
	A ^= 0xFF;
	F.subtractFlag = 1;
	F.halfCarryFlag = 1;
}

void CPU::DAA()
{
	uint8_t adjust = 0;
	uint8_t NHCFlags = (F.byte >> 4) & 7;
	switch (NHCFlags) {
	case 0b000:
		if ((A & 0xF) > 9) adjust = 6;
		break;
	case 0b001:
		if ((A & 0xF) <= 3) adjust = 6;
		break;
	case 0b010:
		break;
	case 0b011:
		break;
	case 0b100:
		break;
	case 0b101:
		break;
	case 0b110:
		break;
	case 0b111:
		break;
	}

	A += adjust;
	F.zeroFlag = (A == 0);
	F.halfCarryFlag = 0;
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
	F.halfCarryFlag = ((reg & 0x10) ^ tempBit) >> 4;
	F.subtractFlag = 1;
	F.zeroFlag = reg == 0;
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
	F.halfCarryFlag = ((reg & 0x10) ^ tempBit) >> 4;
	F.subtractFlag = 0;
	F.zeroFlag = reg == 0;
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
		PC = address;
	}
}

void CPU::JPHL()
{
	PC = HL;
}

void CPU::JR(bool flag)
{
	int8_t signedByte = getImm8();
	if (flag) {
		m_currentInstructionCyclesLeft += 4;
		PC += signedByte;
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
	A |= value;
	F.byte = 0;
	F.zeroFlag = A == 0;
}

void CPU::RET(bool flag)
{
	if (flag) {
		m_currentInstructionCyclesLeft += 4;
		PC = popReg16();
	}
}

void CPU::RL(uint8_t& reg)
{
	uint8_t oldCarry = F.carryFlag;
	F.byte = 0;
	F.carryFlag = reg >> 7;
	reg <<= 1;
	reg |= oldCarry;
	F.zeroFlag = (reg == 0);
}

void CPU::RLA()
{
	uint8_t oldCarry = F.carryFlag;
	F.byte = 0; // TODO: zero flag affected!
	F.carryFlag = A >> 7;
	A <<= 1;
	A |= oldCarry;
	// F.zeroFlag = 0; // TODO: not sure if not affected (Z80 Manual) or reset (pastraiser.com GB opcodes)
}

void CPU::RLC(uint8_t& reg)
{
	F.byte = 0;
	F.carryFlag = reg >> 7;
	reg <<= 1;
	reg |= F.carryFlag;
	F.zeroFlag = (reg == 0);
}

void CPU::RLCA()
{
	F.byte = 0;
	F.carryFlag = A >> 7;
	A <<= 1;
	A |= F.carryFlag;
}

void CPU::RR(uint8_t& reg)
{
	uint8_t oldCarry = F.carryFlag << 7;
	F.byte = 0;
	F.carryFlag = reg & 1;
	reg >>= 1;
	reg |= oldCarry;
	F.zeroFlag = (reg == 0);
}

void CPU::RRA()
{
	uint8_t oldCarry = F.carryFlag << 7;
	F.byte = 0; // TODO: zero flag affected!
	F.carryFlag = A & 1;
	A >>= 1;
	A |= oldCarry;
	// F.zeroFlag = 0; // TODO: not sure if not affected (Z80 Manual) or reset (pastraiser.com GB opcodes)
}

void CPU::RRC(uint8_t& reg)
{
	F.byte = 0;
	F.carryFlag = reg & 1;
	reg >>= 1;
	reg |= F.carryFlag;
	F.zeroFlag = (reg == 0);
}

void CPU::RRCA()
{
	F.byte = 0;
	F.carryFlag = A & 1;
	A >>= 1;
	A |= F.carryFlag;
}

void CPU::RST(uint16_t address)
{
	m_currentInstructionCyclesLeft += 4;
	pushReg16(PC);
	PC = address;
}

void CPU::SBC(uint8_t value)
{
	uint16_t result = (int16_t)A - (int16_t)value - F.carryFlag;
	uint8_t result4bit = (int8_t)A - (int8_t)value - F.carryFlag;
	A = result & 0xFF;
	F.zeroFlag = (result == 0);
	F.subtractFlag = 1;
	F.halfCarryFlag = (result4bit >> 4) & 1;
	F.carryFlag = (result >> 8) & 1;
}

void CPU::SCF()
{
	F.subtractFlag = 0;
	F.halfCarryFlag = 0;
	F.carryFlag = 1;
}

void CPU::SETM(uint8_t bit)
{
	uint8_t value = (this->*m_readByteFunc)(HL);
	value |= 1 << bit;
	storeByte(HL, value);
}

void CPU::SLA(uint8_t& reg)
{
	F.byte = 0;
	F.carryFlag = reg >> 7;
	reg <<= 1;
	F.zeroFlag = (reg == 0);
}

void CPU::SRA(uint8_t& reg)
{
	F.byte = 0;
	F.carryFlag = reg & 1;
	reg >>= 1;
	F.zeroFlag = (reg == 0);
}

void CPU::SRL(uint8_t& reg)
{
	F.byte = 0;
	F.carryFlag = reg & 1;
	reg >>= 1;
	F.zeroFlag = (reg == 0);
}

void CPU::SUB(uint8_t value)
{
	uint16_t result = (int16_t)A - (int16_t)value;
	uint8_t result4bit = (int8_t)A - (int8_t)value;
	A = result & 0xFF;
	F.zeroFlag = (result == 0);
	F.subtractFlag = 1;
	F.halfCarryFlag = (result4bit >> 4) & 1;
	F.carryFlag = (result >> 8) & 1;
}

void CPU::SWAP(uint8_t& reg)
{
	uint8_t lo = reg & 0xF;
	reg >>= 4;
	reg |= lo;
	F.byte = 0;
	F.zeroFlag = (reg == 0);
}

void CPU::XOR(uint8_t value)
{
	A ^= value;
	F.byte = 0;
	F.zeroFlag = A == 0;
}
