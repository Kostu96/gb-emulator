#pragma once
#include <cstdint>

class MemoryMap;

class CPU
{
public:
	CPU(MemoryMap& memoryMap);
	CPU(CPU&) = delete;

	void reset();
	void doCycles(size_t cycles);
private:
	uint8_t readByte(uint16_t address);
	uint8_t readByteInternal(uint16_t address);

	// addressing modes:
	void IMP();   // implied
	void IMM();   // immediate
	void IMMEX(); // extended immediate
	void REG();   // register
	void INR();   // indirect register
	void ABS();   // absolute
	void MZP();   // modified zero page
	void REL();   // relative
	void MIDX();  // modified index
	void BITA();  // bit

	// 1 byte instructions:
	void NOP();  void LD();  void INC();  void DEC();
	void RLCA(); void ADD(); void RRCA(); void STOP();
	void RLA();  void JR();  void RRA();  void DAA();
	void CPL();  void SCF(); void CCF();  void HALT();
	void ADC();  void SUB(); void SBC();  void AND();
	void XOR();  void OR();  void CP();   void RET();
	void POP();  void JP();  void CALL(); void PUSH();
	void RST();  void CB();  void RETI(); void LDH();
	void DI();   void EI();
	// prefix CB instructions:
	void RLC();  void RRC(); void RL();   void RR();
	void SLA();  void SRA(); void SWAP(); void SRL();
	void BIT();  void RES(); void SET();

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

	uint8_t(CPU::* m_readByteFunc)(uint16_t) = &CPU::readByteInternal;
	uint8_t m_internalROM[256];
	MemoryMap& m_memoryMap;

	struct Instruction {
		void(CPU::* operation)();
		void(CPU::* addressing)();
		union {
			uint8_t byte;
			struct {
				uint8_t cycles : 5;  // 0-4
				uint8_t unused : 1;  // 5
				uint8_t isStore : 1; // 6
				uint8_t is16bit : 1; // 7
			};
		};
	};
	Instruction m_instructionSet[256];
	//Instruction m_prefixCBinstructionSet[256];
	uint8_t m_currentInstructionCyclesLeft;

	// helper variables
	uint8_t m_currentOpcode;
	bool m_isStore;
	bool m_is16bit;
	bool m_isREG;
	uint16_t m_operandAddress;
};
