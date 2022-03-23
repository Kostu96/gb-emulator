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
	uint8_t readByte(uint16_t address) const;
	uint8_t readByteInternal(uint16_t address) const;
	void storeByte(uint16_t address, uint8_t byte);
	uint16_t getImm16();
	uint16_t popReg16();
	void pushReg16(uint16_t reg);

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

	struct RegisterNamed {
		union {
			struct {
				FlagsRegister F;
				uint8_t A;
			};
			uint16_t AF;
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
		uint16_t SP;
		uint16_t PC;
	};

	union {
		RegisterNamed m_registerNamed;
		uint8_t m_registerArray[12];
	};

	uint8_t m_interruptControl;

	uint8_t(CPU::* m_readByteFunc)(uint16_t) const = &CPU::readByteInternal;
	MemoryMap& m_memoryMap;

	// helper variables
	uint8_t m_currentInstructionCyclesLeft;
	bool m_isCBInstruction;
};
