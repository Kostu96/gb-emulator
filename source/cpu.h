#pragma once
#include <cstdint>

class MemoryMap;

class CPU
{
public:
	CPU(MemoryMap& memoryMap) : m_memoryMap(memoryMap) {};
	CPU(CPU&) = delete;

	void runInstruction();
private:
	void NOP();
	void JR(uint8_t mask);
	void LD();
	void XOR();
	void BIT(uint8_t n);

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
		uint8_t registers[12];
		uint16_t registerPairs[6]{};
	};

	MemoryMap& m_memoryMap;
	uint8_t* m_activeReg8 = nullptr;
	uint16_t* m_activeReg16 = nullptr;
	uint8_t m_imm8 = 0;
	int8_t m_imm8_s = 0;
	uint16_t m_imm16 = 0;
	bool m_is16bitOp = false;
	bool m_isIndirectAddressing = false;
};
