#pragma once
#include "base.h"

#include <cpp-common/non_copyable.h>

class MemoryMap;

class CPU :
	public NonCopyable
{
public:
	CPU();

	void connect(MemoryMap& memoryMap);
	void reset();
	void doCycles(size_t cycles);
	void requestInterrupt(uint8_t interrupt);
PRIVATE:
	void executeInstruction(uint8_t opcode);
	void executeInstructionStandard(uint8_t opcode);
	void executeInstructionCBPrefix(uint8_t opcode);

	uint8_t readByte(uint16_t address);
	void storeByte(uint16_t address, uint8_t byte);
	uint8_t* getMemoryLocation(uint16_t address);
	uint16_t getImm16();
	uint16_t popReg16();
	void pushReg16(uint16_t reg);

	void ADC(uint8_t value);
	void ADD(uint8_t value);
	void ADDHL(uint16_t value);
	void ADDSP();
	void AND(uint8_t value);
	void BIT(uint8_t bit, uint8_t value);
	void CALL(bool flag);
	void CCF();
	void CP(uint8_t value);
	void CPL();
	void DAA();
	void DECR(uint8_t& reg);
	void DECRR(uint16_t& reg);
	void INCR(uint8_t& reg);
	void INCRR(uint16_t& reg);
	void JP(bool flag);
	void JPHL();
	void JR(bool flag);
	void LDHLSPImm();
	void LDM(uint16_t address, uint8_t value);
	void LDMM(uint16_t address, uint16_t value);
	void LDR(uint8_t& reg, uint8_t value);
	void LDRR(uint16_t& reg, uint16_t value);
	void OR(uint8_t value);
	void RES(uint8_t bit, uint8_t& reg);
	void RET(bool flag);
	void RL(uint8_t& reg);
	void RLA();
	void RLC(uint8_t& reg);
	void RLCA();
	void RR(uint8_t& reg);
	void RRA();
	void RRC(uint8_t& reg);
	void RRCA();
	void RST(uint16_t address);
	void SBC(uint8_t value);
	void SCF();
	void SET(uint8_t bit, uint8_t& reg);
	void SLA(uint8_t& reg);
	void SRA(uint8_t& reg);
	void SRL(uint8_t& reg);
	void SUB(uint8_t value);
	void SWAP(uint8_t& reg);
	void XOR(uint8_t value);

	union FlagsRegister {
		struct {
			uint8_t notUsed       : 4; // 0-3
			uint8_t carryFlag     : 1; // 4
			uint8_t halfCarryFlag : 1; // 5
			uint8_t subtractFlag  : 1; // 6
			uint8_t zeroFlag      : 1; // 7
		};
		uint8_t byte;
	};

	union {
		struct {
			uint8_t C;
			uint8_t B;
		};
		uint16_t BC;
	};
	union {
		struct {
			uint8_t E;
			uint8_t D;
		};
		uint16_t DE;
	};
	union {
		struct {
			uint8_t L;
			uint8_t H;
		};
		uint16_t HL;
	};
	union {
		struct {
			FlagsRegister F;
			uint8_t A;
		};
		uint16_t AF;
	};
	uint16_t SP;
	uint16_t PC;

	struct InterruptFlags {
		uint8_t VBlank  : 1; // 0
		uint8_t LCDStat : 1; // 1
		uint8_t timer   : 1; // 2
		uint8_t serial  : 1; // 3
		uint8_t joypad  : 1; // 4
		uint8_t unused  : 3; // 5-7
	};

	union {
		InterruptFlags fields;
		uint8_t byte;
	} m_interruptEnables;
	union {
		InterruptFlags fields;
		uint8_t byte;
	} m_IRQs;
	bool m_interruptsMasterEnable;
	bool m_EICalled;

	uint8_t m_currentInstructionCyclesLeft;
	bool m_isCBInstruction;
	bool m_isHalted;

	MemoryMap* m_memoryMap;
};
