#include "cpu.h"
#include "memory_map.h"
#include "helper_functions.h"

#include <iostream>

using i = CPU; // To make table concise

CPU::CPU(MemoryMap& memoryMap) :
	m_memoryMap(memoryMap),
	m_instructionSet{
		{ &i::NOP,  &i::IMP, 4  }, { &i::LD,  &i::IMMEX, 12     }, { &i::LD,  &i::INR, 8 }, { &i::INC, &i::IMP, 8 }, { &i::INC, &i::IMP, 4  }, { &i::DEC, &i::IMP, 4  }, { &i::LD,   &i::IMM, 8  }, { &i::RLCA, &i::IMP, 4 },
		{ &i::LD,   &i::ABS, 20 }, { &i::ADD, &i::REG,   8      }, { &i::LD,  &i::INR, 8 }, { &i::DEC, &i::IMP, 8 }, { &i::INC, &i::IMP, 4  }, { &i::DEC, &i::IMP, 4  }, { &i::LD,   &i::IMM, 8  }, { &i::RRCA, &i::IMP, 4 },
		{ &i::STOP, &i::IMP, 4  }, { &i::LD,  &i::IMMEX, 12     }, { &i::LD,  &i::INR, 8 }, { &i::INC, &i::IMP, 8 }, { &i::INC, &i::IMP, 4  }, { &i::DEC, &i::IMP, 4  }, { &i::LD,   &i::IMM, 8  }, { &i::RLA,  &i::IMP, 4 },
		{ &i::JR,   &i::REL, 12 }, { &i::ADD, &i::REG,   8      }, { &i::LD,  &i::INR, 8 }, { &i::DEC, &i::IMP, 8 }, { &i::INC, &i::IMP, 4  }, { &i::DEC, &i::IMP, 4  }, { &i::LD,   &i::IMM, 8  }, { &i::RRA,  &i::IMP, 4 },
		{ &i::JR,   &i::REL, 8  }, { &i::LD,  &i::IMMEX, 12     }, { &i::LD,  &i::INR, 8 }, { &i::INC, &i::IMP, 8 }, { &i::INC, &i::IMP, 4  }, { &i::DEC, &i::IMP, 4  }, { &i::LD,   &i::IMM, 8  }, { &i::DAA,  &i::IMP, 4 },
		{ &i::JR,   &i::REL, 8  }, { &i::ADD, &i::REG,   8      }, { &i::LD,  &i::INR, 8 }, { &i::DEC, &i::IMP, 8 }, { &i::INC, &i::IMP, 4  }, { &i::DEC, &i::IMP, 4  }, { &i::LD,   &i::IMM, 8  }, { &i::CPL,  &i::IMP, 4 },
		{ &i::JR,   &i::REL, 8  }, { &i::LD,  &i::IMMEX, 128|12 }, { &i::LD,  &i::INR, 8 }, { &i::INC, &i::IMP, 8 }, { &i::INC, &i::INR, 12 }, { &i::DEC, &i::INR, 12 }, { &i::LD,   &i::INR, 12 }, { &i::SCF,  &i::IMP, 4 },
		{ &i::JR,   &i::REL, 8  }, { &i::ADD, &i::REG,   8      }, { &i::LD,  &i::INR, 8 }, { &i::DEC, &i::IMP, 8 }, { &i::INC, &i::IMP, 4  }, { &i::DEC, &i::IMP, 4  }, { &i::LD,   &i::IMM, 8  }, { &i::CCF,  &i::IMP, 4 },
		{ &i::LD,   &i::REG, 4  }, { &i::LD,  &i::REG,   4      }, { &i::LD,  &i::REG, 4 }, { &i::LD,  &i::REG, 4 }, { &i::LD,  &i::REG, 4  }, { &i::LD,  &i::REG, 4  }, { &i::LD,   &i::INR, 8  }, { &i::LD,   &i::REG, 4 },
		{ &i::LD,   &i::REG, 4  }, { &i::LD,  &i::REG,   4      }, { &i::LD,  &i::REG, 4 }, { &i::LD,  &i::REG, 4 }, { &i::LD,  &i::REG, 4  }, { &i::LD,  &i::REG, 4  }, { &i::LD,   &i::INR, 8  }, { &i::LD,   &i::REG, 4 },
		{ &i::LD,   &i::REG, 4  }, { &i::LD,  &i::REG,   4      }, { &i::LD,  &i::REG, 4 }, { &i::LD,  &i::REG, 4 }, { &i::LD,  &i::REG, 4  }, { &i::LD,  &i::REG, 4  }, { &i::LD,   &i::INR, 8  }, { &i::LD,   &i::REG, 4 },
		{ &i::LD,   &i::REG, 4  }, { &i::LD,  &i::REG,   4      }, { &i::LD,  &i::REG, 4 }, { &i::LD,  &i::REG, 4 }, { &i::LD,  &i::REG, 4  }, { &i::LD,  &i::REG, 4  }, { &i::LD,   &i::INR, 8  }, { &i::LD,   &i::REG, 4 },
		{ &i::LD,   &i::REG, 4  }, { &i::LD,  &i::REG,   4      }, { &i::LD,  &i::REG, 4 }, { &i::LD,  &i::REG, 4 }, { &i::LD,  &i::REG, 4  }, { &i::LD,  &i::REG, 4  }, { &i::LD,   &i::INR, 8  }, { &i::LD,   &i::REG, 4 },
		{ &i::LD,   &i::REG, 4  }, { &i::LD,  &i::REG,   4      }, { &i::LD,  &i::REG, 4 }, { &i::LD,  &i::REG, 4 }, { &i::LD,  &i::REG, 4  }, { &i::LD,  &i::REG, 4  }, { &i::LD,   &i::INR, 8  }, { &i::LD,   &i::REG, 4 },
		{ &i::LD,   &i::INR, 8  }, { &i::LD,  &i::INR,   8      }, { &i::LD,  &i::INR, 8 }, { &i::LD,  &i::INR, 8 }, { &i::LD,  &i::INR, 8  }, { &i::LD,  &i::INR, 8  }, { &i::HALT, &i::IMP, 4  }, { &i::LD,   &i::INR, 8 },
		{ &i::LD,   &i::REG, 4  }, { &i::LD,  &i::REG,   4      }, { &i::LD,  &i::REG, 4 }, { &i::LD,  &i::REG, 4 }, { &i::LD,  &i::REG, 4  }, { &i::LD,  &i::REG, 4  }, { &i::LD,   &i::INR, 8  }, { &i::LD,   &i::REG, 4 },
		{ &i::ADD,  &i::REG, 4  }, { &i::ADD, &i::REG,   4      }, { &i::ADD, &i::REG, 4 }, { &i::ADD, &i::REG, 4 }, { &i::ADD, &i::REG, 4  }, { &i::ADD, &i::REG, 4  }, { &i::ADD,  &i::INR, 8  }, { &i::ADD,  &i::REG, 4 },
		{ &i::ADC,  &i::REG, 4  }, { &i::ADC, &i::REG,   4      }, { &i::ADC, &i::REG, 4 }, { &i::ADC, &i::REG, 4 }, { &i::ADC, &i::REG, 4  }, { &i::ADC, &i::REG, 4  }, { &i::ADC,  &i::INR, 8  }, { &i::ADC,  &i::REG, 4 },
		{ &i::SUB,  &i::REG, 4  }, { &i::SUB, &i::REG,   4      }, { &i::SUB, &i::REG, 4 }, { &i::SUB, &i::REG, 4 }, { &i::SUB, &i::REG, 4  }, { &i::SUB, &i::REG, 4  }, { &i::SUB,  &i::INR, 8  }, { &i::SUB,  &i::REG, 4 },
		{ &i::SBC,  &i::REG, 4	}, { &i::SBC, &i::REG,   4      }, { &i::SBC, &i::REG, 4 }, { &i::SBC, &i::REG, 4 }, { &i::SBC, &i::REG, 4  }, { &i::SBC, &i::REG, 4  }, { &i::SBC,  &i::INR, 8  }, { &i::SBC,  &i::REG, 4 },
		{ &i::AND,  &i::REG, 4  }, { &i::AND, &i::REG,   4      }, { &i::AND, &i::REG, 4 }, { &i::AND, &i::REG, 4 }, { &i::AND, &i::REG, 4  }, { &i::AND, &i::REG, 4  }, { &i::AND,  &i::INR, 8  }, { &i::AND,  &i::REG, 4 },
		{ &i::XOR,  &i::REG, 4	}, { &i::XOR, &i::REG,   4      }, { &i::XOR, &i::REG, 4 }, { &i::XOR, &i::REG, 4 }, { &i::XOR, &i::REG, 4  }, { &i::XOR, &i::REG, 4  }, { &i::XOR,  &i::INR, 8  }, { &i::XOR,  &i::REG, 4 },
		{ &i::OR,   &i::REG, 4  }, { &i::OR,  &i::REG,   4      }, { &i::OR,  &i::REG, 4 }, { &i::OR,  &i::REG, 4 }, { &i::OR,  &i::REG, 4  }, { &i::OR,  &i::REG, 4  }, { &i::OR,   &i::INR, 8  }, { &i::OR,   &i::REG, 4 },
		{ &i::CP,   &i::REG, 4  }, { &i::CP,  &i::REG,   4      }, { &i::CP,  &i::REG, 4 }, { &i::CP,  &i::REG, 4 }, { &i::CP,  &i::REG, 4  }, { &i::CP,  &i::REG, 4  }, { &i::CP,   &i::INR, 8  }, { &i::CP,   &i::REG, 4 },
		/*{&i::RET NZ 20 / 8
, { &i::POP BC 12
, { &i::JP NZ,a16 16 / 12
, { &i::JP a16 16
, { &i::CALL NZ,a16 24 / 12
, { &i::PUSH BC 16
, { &i::ADD A,d8 8
, { &i::RST 00H 16
, { &i::RET Z 20 / 8
, { &i::RET 16
, { &i::JP Z,a16 16 / 12
, { &i::CB 4
, { &i::CALL Z,a16 24 / 12
, { &i::CALL a16 24
, { &i::ADC A,d8 8
, { &i::RST 08H 16
, { &i::RET NC 20 / 8
, { &i::POP DE 12
, { &i::JP NC,a16 16 / 12
, { &i::CALL NC,a16 24 / 12
, { &i::PUSH DE 16
, { &i::SUB d8 8
, { &i::RST 10H 16
, { &i::RET C 20 / 8
, { &i::RETI 16
, { &i::JP C,a16 16 / 12
, { &i::CALL C,a16 24 / 12
, { &i::SBC A,d8 8
, { &i::RST 18H 16
, { &i::LDH(a8),A 12
, { &i::POP HL 12
, { &i::LD(C),A 8
, { &i::PUSH HL 16
, { &i::AND d8 8
, { &i::RST 20H 16
, { &i::ADD SP,r8 16
, { &i::JP(HL) 4
, { &i::LD(a16),A 16
, { &i::XOR d8 8
, { &i::RST 28H 16
, { &i::LDH A,(a8) 12
, { &i::POP AF 12
, { &i::LD A,(C) 8
, { &i::DI 4
, { &i::PUSH AF 16
, { &i::OR d8 8
, { &i::RST 30H 16
, { &i::LD HL,SP + r8 12
, { &i::LD SP,HL 8
, { &i::LD A,(a16) 16
, { &i::EI 4
, { &i::CP d8 8
, { &i::RST 38H 16*/
}
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
	m_currentInstructionCyclesLeft = 4; // Let's say that reset takes 4 clock cycles
	PC = 0x0;

	// helper variables
	m_currentOpcode = 0x0;
	m_is16bit = false;
	m_operandAddress = 0x0;
}

void CPU::doCycles(size_t cycles)
{
	while (cycles--) {
		if (m_currentInstructionCyclesLeft == 0) {

			m_currentOpcode = (this->*m_readByteFunc)(PC++);
			const Instruction& instruction = m_instructionSet[m_currentOpcode];
			m_currentInstructionCyclesLeft = instruction.cycles;
			m_is16bit = instruction.is16bit;
			m_isREG = instruction.addressing == &CPU::REG;
			(this->*instruction.addressing)();
			(this->*instruction.operation)();
		}

		--m_currentInstructionCyclesLeft;
	}
}

uint8_t CPU::readByte(uint16_t address)
{
	return m_memoryMap.load8(address);
}

uint8_t CPU::readByteInternal(uint16_t address)
{
	if (address < 0x100)
		return m_internalROM[address];

	return m_memoryMap.load8(address);
}
