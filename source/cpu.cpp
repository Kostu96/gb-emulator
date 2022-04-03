#include "cpu.h"
#include "memory_map.h"

#include <cpp-common/helper_functions.h>
#include <iostream>

CPU::CPU()
{
	reset();
};

void CPU::connect(MemoryMap& memoryMap)
{
	m_memoryMap = &memoryMap;
}

void CPU::reset()
{
	AF = 0x018F;
	BC = 0x0013;
	DE = 0x00D8;
	HL = 0x014D;
	PC = 0x100;
	SP = 0xFFFE;

	m_interruptEnables.byte = 0x0;
	m_IRQs.byte = 0xE1;
	m_interruptsMasterEnable = false;
	m_EICalled = false;

	m_currentInstructionCyclesLeft = 0;
	m_isCBInstruction = false;
	m_isHalted = false;
}

void CPU::doCycles(size_t cycles)
{
	if (!m_isHalted) {
		while (cycles--) {
			m_memoryMap->getTimer().tick();
			m_memoryMap->getPPU().tick();
	
			// TODO: interrupt should be connected to m_currentInstructionCyclesLeft
			//if (m_interruptsMasterEnable) {
			//	// TODO: move this code swh
			//	if (m_interruptEnables.fields.VBlank & m_IRQs.fields.VBlank) {
			//		m_currentInstructionCyclesLeft += 8;
			//		m_interruptsMasterEnable = false;
			//		m_IRQs.fields.VBlank = 0;
			//		RST(0x40);
			//	}
			//	else if (m_interruptEnables.fields.LCDStat & m_IRQs.fields.LCDStat) {
			//		m_currentInstructionCyclesLeft += 8;
			//		m_interruptsMasterEnable = false;
			//		m_IRQs.fields.LCDStat = 0;
			//		RST(0x48);
			//	}
			//	else if (m_interruptEnables.fields.timer & m_IRQs.fields.timer) {
			//		m_currentInstructionCyclesLeft += 8;
			//		m_interruptsMasterEnable = false;
			//		m_IRQs.fields.timer = 0;
			//		RST(0x50);
			//	}
			//	else if (m_interruptEnables.fields.serial & m_IRQs.fields.serial) {
			//		m_currentInstructionCyclesLeft += 8;
			//		m_interruptsMasterEnable = false;
			//		m_IRQs.fields.serial = 0;
			//		RST(0x58);
			//	}
			//	else if (m_interruptEnables.fields.joypad & m_IRQs.fields.joypad) {
			//		m_currentInstructionCyclesLeft += 8;
			//		m_interruptsMasterEnable = false;
			//		m_IRQs.fields.joypad = 0;
			//		RST(0x60);
			//	}
			//}

			if (m_EICalled) {
				m_interruptsMasterEnable = true;
				m_EICalled = false;
			}

			if (m_currentInstructionCyclesLeft == 0) {

				uint8_t opcode = readByte(PC++);
				executeInstruction(opcode);
			}

			--m_currentInstructionCyclesLeft;
		}
	}
	else {
		if (m_IRQs.byte & m_interruptEnables.byte)
			m_isHalted = false;
	}
}

void CPU::requestInterrupt(uint8_t interrupt)
{
	m_IRQs.byte |= interrupt;
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
	switch (opcode) {
	case 0x00: break;
	case 0x01: LDRR(BC, getImm16()); break;
	case 0x02: LDM(BC, A); break;
	case 0x03: INCRR(BC); break;
	case 0x04: INCR(B); break;
	case 0x05: DECR(B); break;
	case 0x06: LDR(B, readByte(PC++)); break;
	case 0x07: RLCA(); break;
	case 0x08: LDMM(getImm16(), SP); break;
	case 0x09: ADDHL(BC); break;
	case 0x0A: LDR(A, readByte(BC)); break;
	case 0x0B: DECRR(BC); break;
	case 0x0C: INCR(C); break;
	case 0x0D: DECR(C); break;
	case 0x0E: LDR(C, readByte(PC++)); break;
	case 0x0F: RRCA(); break;
	case 0x10: __debugbreak(); break; //TODO: stop should have 0x10 0x00 otherwise it's undefined opode
	case 0x11: LDRR(DE, getImm16()); break;
	case 0x12: LDM(DE, A); break;
	case 0x13: INCRR(DE); break;
	case 0x14: INCR(D); break;
	case 0x15: DECR(D); break;
	case 0x16: LDR(D, readByte(PC++)); break;
	case 0x17: RLA(); break;
	case 0x18: JR(true); break;
	case 0x19: ADDHL(DE); break;
	case 0x1A: LDR(A, readByte(DE)); break;
	case 0x1B: DECRR(DE); break;
	case 0x1C: INCR(E); break;
	case 0x1D: DECR(E); break;
	case 0x1E: LDR(E, readByte(PC++)); break;
	case 0x1F: RRA(); break;
	case 0x20: JR(!F.zeroFlag); break;
	case 0x21: LDRR(HL, getImm16()); break;
	case 0x22: LDM(HL++, A); break;
	case 0x23: INCRR(HL); break;
	case 0x24: INCR(H); break;
	case 0x25: DECR(H); break;
	case 0x26: LDR(H, readByte(PC++)); break;
	case 0x27: DAA(); break;
	case 0x28: JR(F.zeroFlag); break;
	case 0x29: ADDHL(HL); break;
	case 0x2A: LDR(A, readByte(HL++)); break;
	case 0x2B: DECRR(HL); break;
	case 0x2C: INCR(L); break;
	case 0x2D: DECR(L); break;
	case 0x2E: LDR(L, readByte(PC++)); break;
	case 0x2F: CPL(); break;
	case 0x30: JR(!F.carryFlag); break;
	case 0x31: LDRR(SP, getImm16()); break;
	case 0x32: LDM(HL--, A); break;
	case 0x33: INCRR(SP); break;
	case 0x34: INCR(*getMemoryLocation(HL)); break;
	case 0x35: DECR(*getMemoryLocation(HL)); break;
	case 0x36: LDM(HL, readByte(PC++)); break;
	case 0x37: SCF(); break;
	case 0x38: JR(F.carryFlag); break;
	case 0x39: ADDHL(SP); break;
	case 0x3A: LDR(A, readByte(HL--)); break;
	case 0x3B: DECRR(SP); break;
	case 0x3C: INCR(A); break;
	case 0x3D: DECR(A); break;
	case 0x3E: LDR(A, readByte(PC++)); break;
	case 0x3F: CCF(); break;
	case 0x40: LDR(B, B); break;
	case 0x41: LDR(B, C); break;
	case 0x42: LDR(B, D); break;
	case 0x43: LDR(B, E); break;
	case 0x44: LDR(B, H); break;
	case 0x45: LDR(B, L); break;
	case 0x46: LDR(B, readByte(HL)); break;
	case 0x47: LDR(B, A); break;
	case 0x48: LDR(C, B); break;
	case 0x49: LDR(C, C); break;
	case 0x4A: LDR(C, D); break;
	case 0x4B: LDR(C, E); break;
	case 0x4C: LDR(C, H); break;
	case 0x4D: LDR(C, L); break;
	case 0x4E: LDR(C, readByte(HL)); break;
	case 0x4F: LDR(C, A); break;
	case 0x50: LDR(D, B); break;
	case 0x51: LDR(D, C); break;
	case 0x52: LDR(D, D); break;
	case 0x53: LDR(D, E); break;
	case 0x54: LDR(D, H); break;
	case 0x55: LDR(D, L); break;
	case 0x56: LDR(D, readByte(HL)); break;
	case 0x57: LDR(D, A); break;
	case 0x58: LDR(E, B); break;
	case 0x59: LDR(E, C); break;
	case 0x5A: LDR(E, D); break;
	case 0x5B: LDR(E, E); break;
	case 0x5C: LDR(E, H); break;
	case 0x5D: LDR(E, L); break;
	case 0x5E: LDR(E, readByte(HL)); break;
	case 0x5F: LDR(E, A); break;
	case 0x60: LDR(H, B); break;
	case 0x61: LDR(H, C); break;
	case 0x62: LDR(H, D); break;
	case 0x63: LDR(H, E); break;
	case 0x64: LDR(H, H); break;
	case 0x65: LDR(H, L); break;
	case 0x66: LDR(H, readByte(HL)); break;
	case 0x67: LDR(H, A); break;
	case 0x68: LDR(L, B); break;
	case 0x69: LDR(L, C); break;
	case 0x6A: LDR(L, D); break;
	case 0x6B: LDR(L, E); break;
	case 0x6C: LDR(L, H); break;
	case 0x6D: LDR(L, L); break;
	case 0x6E: LDR(L, readByte(HL)); break;
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
	case 0x7E: LDR(A, readByte(HL)); break;
	case 0x7F: LDR(A, A); break;
	case 0x80: ADD(B); break;
	case 0x81: ADD(C); break;
	case 0x82: ADD(D); break;
	case 0x83: ADD(E); break;
	case 0x84: ADD(H); break;
	case 0x85: ADD(L); break;
	case 0x86: ADD(readByte(HL)); break;
	case 0x87: ADD(A); break;
	case 0x88: ADC(B); break;
	case 0x89: ADC(C); break;
	case 0x8A: ADC(D); break;
	case 0x8B: ADC(E); break;
	case 0x8C: ADC(H); break;
	case 0x8D: ADC(L); break;
	case 0x8E: ADC(readByte(HL)); break;
	case 0x8F: ADC(A); break;
	case 0x90: SUB(B); break;
	case 0x91: SUB(C); break;
	case 0x92: SUB(D); break;
	case 0x93: SUB(E); break;
	case 0x94: SUB(H); break;
	case 0x95: SUB(L); break;
	case 0x96: SUB(readByte(HL)); break;
	case 0x97: SUB(A); break;
	case 0x98: SBC(B); break;
	case 0x99: SBC(C); break;
	case 0x9A: SBC(D); break;
	case 0x9B: SBC(E); break;
	case 0x9C: SBC(H); break;
	case 0x9D: SBC(L); break;
	case 0x9E: SBC(readByte(HL)); break;
	case 0x9F: SBC(A); break;
	case 0xA0: AND(B); break;
	case 0xA1: AND(C); break;
	case 0xA2: AND(D); break;
	case 0xA3: AND(E); break;
	case 0xA4: AND(H); break;
	case 0xA5: AND(L); break;
	case 0xA6: AND(readByte(HL)); break;
	case 0xA7: AND(A); break;
	case 0xA8: XOR(B); break;
	case 0xA9: XOR(C); break;
	case 0xAA: XOR(D); break;
	case 0xAB: XOR(E); break;
	case 0xAC: XOR(H); break;
	case 0xAD: XOR(L); break;
	case 0xAE: XOR(readByte(HL)); break;
	case 0xAF: XOR(A); break;
	case 0xB0: OR(B); break;
	case 0xB1: OR(C); break;
	case 0xB2: OR(D); break;
	case 0xB3: OR(E); break;
	case 0xB4: OR(H); break;
	case 0xB5: OR(L); break;
	case 0xB6: OR(readByte(HL)); break;
	case 0xB7: OR(A); break;
	case 0xB8: CP(B); break;
	case 0xB9: CP(C); break;
	case 0xBA: CP(D); break;
	case 0xBB: CP(E); break;
	case 0xBC: CP(H); break;
	case 0xBD: CP(L); break;
	case 0xBE: CP(readByte(HL)); break;
	case 0xBF: CP(A); break;
	case 0xC0: RET(!F.zeroFlag); break;
	case 0xC1: BC = popReg16(); break;
	case 0xC2: JP(!F.zeroFlag); break;
	case 0xC3: JP(true); break;
	case 0xC4: CALL(!F.zeroFlag); break;
	case 0xC5: m_currentInstructionCyclesLeft += 4; pushReg16(BC); break;
	case 0xC6: ADD(readByte(PC++)); break;
	case 0xC7: RST(0x00); break;
	case 0xC8: RET(F.zeroFlag); break;
	case 0xC9: RET(true); break;
	case 0xCA: JP(F.zeroFlag); break;
	case 0xCB: m_isCBInstruction = true; break;
	case 0xCC: CALL(F.zeroFlag); break;
	case 0xCD: CALL(true); break;
	case 0xCE: ADC(readByte(PC++)); break;
	case 0xCF: RST(0x08); break;
	case 0xD0: RET(!F.carryFlag); break;
	case 0xD1: DE = popReg16(); break;
	case 0xD2: JP(!F.carryFlag); break;
	case 0xD4: CALL(!F.carryFlag); break;
	case 0xD5: m_currentInstructionCyclesLeft += 4; pushReg16(DE); break;
	case 0xD6: SUB(readByte(PC++)); break;
	case 0xD7: RST(0x10); break;
	case 0xD8: RET(F.carryFlag); break;
	case 0xD9: m_interruptsMasterEnable = true; RET(true); break;
	case 0xDA: JP(F.carryFlag); break;
	case 0xDC: CALL(F.carryFlag); break;
	case 0xDE: SBC(readByte(PC++)); break;
	case 0xDF: RST(0x18); break;
	case 0xE0: LDM(0xFF00 | readByte(PC++), A); break;
	case 0xE1: HL = popReg16(); break;
	case 0xE2: LDM(0xFF00 | C, A); break;
	case 0xE5: m_currentInstructionCyclesLeft += 4; pushReg16(HL); break;
	case 0xE6: AND(readByte(PC++)); break;
	case 0xE7: RST(0x20); break;
	case 0xE8: ADDSP(); break;
	case 0xE9: JPHL(); break;
	case 0xEA: LDM(getImm16(), A); break;
	case 0xEE: XOR(readByte(PC++)); break;
	case 0xEF: RST(0x28); break;
	case 0xF0: LDR(A, readByte(0xFF00 | readByte(PC++))); break;
	case 0xF1: AF = popReg16(); F.notUsed = 0xF; break;
	case 0xF2: LDR(A, readByte(0xFF00 | C)); break;
	case 0xF3: m_interruptsMasterEnable = false; break;
	case 0xF5: m_currentInstructionCyclesLeft += 4; pushReg16(AF); break;
	case 0xF6: OR(readByte(PC++)); break;
	case 0xF7: RST(0x30); break;
	case 0xF8: LDHLSPImm(); break;
	case 0xF9: m_currentInstructionCyclesLeft += 4; LDRR(SP, HL); break;
	case 0xFA: LDR(A, readByte(getImm16())); break;
	case 0xFB: m_EICalled = true; break;
	case 0xFE: CP(readByte(PC++)); break;
	case 0xFF: RST(0x38); break;
	default: std::cerr << "Illegal opcode: " << std::hex << opcode << '\n'; abort();
	}
}

void CPU::executeInstructionCBPrefix(uint8_t opcode)
{
	m_isCBInstruction = false;
	m_currentInstructionCyclesLeft += 4;

	uint8_t instr = opcode >> 3;
	uint8_t reg = opcode & 7;
	uint8_t* param[8]{ &B, &C, &D, &E, &H, &L, reg == 6 ? getMemoryLocation(HL) : nullptr, &A };
	switch (instr) {
	case 0x00: RLC(*param[reg]); break;
	case 0x01: RRC(*param[reg]); break;
	case 0x02: RL(*param[reg]); break;
	case 0x03: RR(*param[reg]); break;
	case 0x04: SLA(*param[reg]); break;
	case 0x05: SRA(*param[reg]); break;
	case 0x06: SWAP(*param[reg]); break;
	case 0x07: SRL(*param[reg]); break;
	default:
		uint8_t subinstr = instr >> 3;
		uint8_t bit = instr & 7;
		switch (subinstr) {
		case 1: BIT(bit, *param[reg]); break;
		case 2: RES(bit, *param[reg]); break;
		case 3: SET(bit, *param[reg]); break;
		}
	}
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
		return m_memoryMap->load8(address);
	}
}

uint16_t CPU::getImm16()
{
	uint16_t value = readByte(PC + 1);
	value <<= 8;
	value |= readByte(PC);
	PC += 2;

	return value;
}

void CPU::storeByte(uint16_t address, uint8_t byte)
{
	m_currentInstructionCyclesLeft += 4;

	switch (address) {
	case 0xFF0F:
		m_IRQs.byte = byte;
		break;
	case 0xFFFF:
		m_interruptEnables.byte = byte;
		break;
	default:
		m_memoryMap->store8(address, byte);
	}
}

uint8_t* CPU::getMemoryLocation(uint16_t address)
{
	m_currentInstructionCyclesLeft += 8;

	switch (address) {
	case 0xFF0F:
		return &m_IRQs.byte;
	case 0xFFFF:
		return &m_interruptEnables.byte;
	default:
		return m_memoryMap->getMemoryLocation(address);
	}
}

uint16_t CPU::popReg16()
{
	uint16_t value = readByte(SP + 1);
	value <<= 8;
	value |= readByte(SP);
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
	uint16_t result12bit = (HL & 0xFFF) + (value & 0xFFF);
	HL = result;
	F.zeroFlag = (HL == 0); // TODO: different specs
	F.subtractFlag = 0;
	F.halfCarryFlag = result12bit >> 12;
	F.carryFlag = result >> 16;
}

void CPU::ADDSP()
{
	m_currentInstructionCyclesLeft += 8;
	int8_t value = readByte(PC++);
	uint32_t result = SP + value;
	uint16_t result12bit = (SP & 0xFFF) + (value & 0xFFF);
	SP = result;
	F.zeroFlag = (SP == 0); // TODO: different specs
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
	uint8_t u = 0;
	if (F.halfCarryFlag || (!F.subtractFlag && (A & 0xF) > 9)) {
		u = 6;
	}
	if (F.carryFlag || (!F.subtractFlag && A > 0x99)) {
		u |= 0x60;
		F.carryFlag = 1;
	}
	A += F.subtractFlag ? -u : u;
	F.zeroFlag = (A == 0);
	F.halfCarryFlag = 0;
}

void CPU::DECR(uint8_t& reg)
{
	uint8_t tempBit = ~reg & 0x10;
	--reg;
	F.halfCarryFlag = ((reg & 0x10) & tempBit) >> 4;
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
	F.halfCarryFlag = (~(reg & 0x10) & tempBit) >> 4;
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
	int8_t signedByte = readByte(PC++);
	if (flag) {
		m_currentInstructionCyclesLeft += 4;
		PC += signedByte;
	}
}

void CPU::LDHLSPImm()
{
	m_currentInstructionCyclesLeft += 4;
	int8_t value = readByte(PC++);
	uint32_t result = SP + value;
	uint16_t result12bit = (SP & 0xFFF) + (value & 0xFFF);
	HL = result;
	F.zeroFlag = (SP == 0); // TODO: different specs
	F.subtractFlag = 0;
	F.halfCarryFlag = result12bit >> 12;
	F.carryFlag = result >> 16;
}

void CPU::LDM(uint16_t address, uint8_t value)
{
	storeByte(address, value);
}

void CPU::LDMM(uint16_t address, uint16_t value)
{
	storeByte(address, value);
	storeByte(address + 1, value >> 8);
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

void CPU::RES(uint8_t bit, uint8_t& reg)
{
	reg &= ~(1 << bit);
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

void CPU::SET(uint8_t bit, uint8_t& reg)
{
	reg |= 1 << bit;
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
	F.byte = 0x0F;
	F.zeroFlag = A == 0;
}
