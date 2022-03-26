#pragma once
#include <cpp-common/non_copyable.h>
#include <cstdint>

class MemoryMap;

class CPU :
	public NonCopyable
{
public:
	CPU(MemoryMap& memoryMap);

	void reset();
	void doCycles(size_t cycles);
private:
	void executeInstruction(uint8_t opcode);
	void executeInstructionStandard(uint8_t opcode);
	void executeInstructionCBPrefix(uint8_t opcode);

	uint8_t readByteInternal(uint16_t address);
	uint8_t readByte(uint16_t address);
	void storeByte(uint16_t address, uint8_t byte);
	uint8_t getImm8();
	uint16_t getImm16();
	uint16_t popReg16();
	void pushReg16(uint16_t reg);

	void ADC(uint8_t value);
	void ADD(uint8_t value);
	void ADDHL(uint16_t value);
	void AND(uint8_t value);
	void BIT(uint8_t bit, uint8_t value);
	void CALL(bool flag);
	void CP(uint8_t value);
	void DECM(uint16_t address);
	void DECR(uint8_t& reg);
	void DECRR(uint16_t& reg);
	void INCR(uint8_t& reg);
	void INCRR(uint16_t& reg);
	void JP(bool flag);
	void JPHL();
	void JR(bool flag);
	void LDM(uint16_t address, uint8_t value);
	void LDR(uint8_t& reg, uint8_t value);
	void LDRR(uint16_t& reg, uint16_t value);
	void OR(uint8_t value);
	void RET(bool flag);
	void RL(uint8_t& reg);
	void RLA();
	void RLCA();
	void RR(uint8_t& reg);
	void RRA();
	void RST(uint16_t address);
	void SETM(uint8_t bit);
	void SRL(uint8_t& reg);
	void SUB(uint8_t value);
	void SWAP(uint8_t& reg);
	void XOR(uint8_t value);

	union FlagsRegister {
		struct {
			uint8_t alwaysZero    : 4; // 0-3
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

	uint8_t(CPU::* m_readByteFunc)(uint16_t);
	MemoryMap& m_memoryMap;
};
