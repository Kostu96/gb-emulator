#include "cpu.h"
#include "memory_map.h"

#include <cpp-common/helper_functions.h>
#include <iostream>

using i = CPU; // To make table concise

CPU::CPU(MemoryMap& memoryMap) :
	m_memoryMap(memoryMap),
	m_instructionSet{
        { &i::NOP,  &i::IMP, 4     }, { &i::LD,  &i::IMM, 128|12 }, { &i::LD,  &i::INR,  64|8 }, { &i::INC, &i::IMP, 8  }, { &i::INC,  &i::IMP, 4  }, { &i::DEC,  &i::IMP, 4  }, { &i::LD,   &i::IMM, 8  }, { &i::RLCA, &i::IMP, 4    }, { &i::LD,  &i::ABS, 20 }, { &i::ADD,  &i::IMP, 8  }, { &i::LD,  &i::INR, 8     }, { &i::DEC, &i::IMP, 8 }, { &i::INC,  &i::IMP, 4  }, { &i::DEC,  &i::IMP, 4  }, { &i::LD,  &i::IMM, 8 }, { &i::RRCA, &i::IMP, 4  },
        { &i::STOP, &i::IMP, 4     }, { &i::LD,  &i::IMM, 128|12 }, { &i::LD,  &i::INR,  64|8 }, { &i::INC, &i::IMP, 8  }, { &i::INC,  &i::IMP, 4  }, { &i::DEC,  &i::IMP, 4  }, { &i::LD,   &i::IMM, 8  }, { &i::RLA,  &i::IMP, 4    }, { &i::JR,  &i::REL, 12 }, { &i::ADD,  &i::IMP, 8  }, { &i::LD,  &i::INR, 8     }, { &i::DEC, &i::IMP, 8 }, { &i::INC,  &i::IMP, 4  }, { &i::DEC,  &i::IMP, 4  }, { &i::LD,  &i::IMM, 8 }, { &i::RRA,  &i::IMP, 4  },
        { &i::JR,   &i::REL, 8     }, { &i::LD,  &i::IMM, 128|12 }, { &i::LD,  &i::INR,  64|8 }, { &i::INC, &i::IMP, 8  }, { &i::INC,  &i::IMP, 4  }, { &i::DEC,  &i::IMP, 4  }, { &i::LD,   &i::IMM, 8  }, { &i::DAA,  &i::IMP, 4    }, { &i::JR,  &i::REL, 8  }, { &i::ADD,  &i::IMP, 8  }, { &i::LD,  &i::INR, 8     }, { &i::DEC, &i::IMP, 8 }, { &i::INC,  &i::IMP, 4  }, { &i::DEC,  &i::IMP, 4  }, { &i::LD,  &i::IMM, 8 }, { &i::CPL,  &i::IMP, 4  },
        { &i::JR,   &i::REL, 8     }, { &i::LD,  &i::IMM, 128|12 }, { &i::LD,  &i::INR,  64|8 }, { &i::INC, &i::IMP, 8  }, { &i::INC,  &i::INR, 12 }, { &i::DEC,  &i::INR, 12 }, { &i::LD,   &i::INR, 12 }, { &i::SCF,  &i::IMP, 4    }, { &i::JR,  &i::REL, 8  }, { &i::ADD,  &i::IMP, 8  }, { &i::LD,  &i::INR, 8     }, { &i::DEC, &i::IMP, 8 }, { &i::INC,  &i::IMP, 4  }, { &i::DEC,  &i::IMP, 4  }, { &i::LD,  &i::IMM, 8 }, { &i::CCF,  &i::IMP, 4  },
        { &i::LD,   &i::IMP, 4     }, { &i::LD,  &i::IMP, 4      }, { &i::LD,  &i::IMP,  4    }, { &i::LD,  &i::IMP, 4  }, { &i::LD,   &i::IMP, 4  }, { &i::LD,   &i::IMP, 4  }, { &i::LD,   &i::INR, 8  }, { &i::LD,   &i::IMP, 4    }, { &i::LD,  &i::IMP, 4  }, { &i::LD,   &i::IMP, 4  }, { &i::LD,  &i::IMP, 4     }, { &i::LD,  &i::IMP, 4 }, { &i::LD,   &i::IMP, 4  }, { &i::LD,   &i::IMP, 4  }, { &i::LD,  &i::INR, 8 }, { &i::LD,   &i::IMP, 4  },
        { &i::LD,   &i::IMP, 4     }, { &i::LD,  &i::IMP, 4      }, { &i::LD,  &i::IMP,  4    }, { &i::LD,  &i::IMP, 4  }, { &i::LD,   &i::IMP, 4  }, { &i::LD,   &i::IMP, 4  }, { &i::LD,   &i::INR, 8  }, { &i::LD,   &i::IMP, 4    }, { &i::LD,  &i::IMP, 4  }, { &i::LD,   &i::IMP, 4  }, { &i::LD,  &i::IMP, 4     }, { &i::LD,  &i::IMP, 4 }, { &i::LD,   &i::IMP, 4  }, { &i::LD,   &i::IMP, 4  }, { &i::LD,  &i::INR, 8 }, { &i::LD,   &i::IMP, 4  },
        { &i::LD,   &i::IMP, 4     }, { &i::LD,  &i::IMP, 4      }, { &i::LD,  &i::IMP,  4    }, { &i::LD,  &i::IMP, 4  }, { &i::LD,   &i::IMP, 4  }, { &i::LD,   &i::IMP, 4  }, { &i::LD,   &i::INR, 8  }, { &i::LD,   &i::IMP, 4    }, { &i::LD,  &i::IMP, 4  }, { &i::LD,   &i::IMP, 4  }, { &i::LD,  &i::IMP, 4     }, { &i::LD,  &i::IMP, 4 }, { &i::LD,   &i::IMP, 4  }, { &i::LD,   &i::IMP, 4  }, { &i::LD,  &i::INR, 8 }, { &i::LD,   &i::IMP, 4  },
        { &i::LD,   &i::INR, 8     }, { &i::LD,  &i::INR, 8      }, { &i::LD,  &i::INR,  8    }, { &i::LD,  &i::INR, 8  }, { &i::LD,   &i::INR, 8  }, { &i::LD,   &i::INR, 8  }, { &i::HALT, &i::IMP, 4  }, { &i::LD,   &i::INR, 64|8 }, { &i::LD,  &i::IMP, 4  }, { &i::LD,   &i::IMP, 4  }, { &i::LD,  &i::IMP, 4     }, { &i::LD,  &i::IMP, 4 }, { &i::LD,   &i::IMP, 4  }, { &i::LD,   &i::IMP, 4  }, { &i::LD,  &i::INR, 8 }, { &i::LD,   &i::IMP, 4  },
        { &i::ADD,  &i::IMP, 4     }, { &i::ADD, &i::IMP, 4      }, { &i::ADD, &i::IMP,  4    }, { &i::ADD, &i::IMP, 4  }, { &i::ADD,  &i::IMP, 4  }, { &i::ADD,  &i::IMP, 4  }, { &i::ADD,  &i::INR, 8  }, { &i::ADD,  &i::IMP, 4    }, { &i::ADC, &i::IMP, 4  }, { &i::ADC,  &i::IMP, 4  }, { &i::ADC, &i::IMP, 4     }, { &i::ADC, &i::IMP, 4 }, { &i::ADC,  &i::IMP, 4  }, { &i::ADC,  &i::IMP, 4  }, { &i::ADC, &i::INR, 8 }, { &i::ADC,  &i::IMP, 4  },
        { &i::SUB,  &i::IMP, 4     }, { &i::SUB, &i::IMP, 4      }, { &i::SUB, &i::IMP,  4    }, { &i::SUB, &i::IMP, 4  }, { &i::SUB,  &i::IMP, 4  }, { &i::SUB,  &i::IMP, 4  }, { &i::SUB,  &i::INR, 8  }, { &i::SUB,  &i::IMP, 4    }, { &i::SBC, &i::IMP, 4  }, { &i::SBC,  &i::IMP, 4  }, { &i::SBC, &i::IMP, 4     }, { &i::SBC, &i::IMP, 4 }, { &i::SBC,  &i::IMP, 4  }, { &i::SBC,  &i::IMP, 4  }, { &i::SBC, &i::INR, 8 }, { &i::SBC,  &i::IMP, 4  },
        { &i::AND,  &i::IMP, 4     }, { &i::AND, &i::IMP, 4      }, { &i::AND, &i::IMP,  4    }, { &i::AND, &i::IMP, 4  }, { &i::AND,  &i::IMP, 4  }, { &i::AND,  &i::IMP, 4  }, { &i::AND,  &i::INR, 8  }, { &i::AND,  &i::IMP, 4    }, { &i::XOR, &i::IMP, 4  }, { &i::XOR,  &i::IMP, 4  }, { &i::XOR, &i::IMP, 4     }, { &i::XOR, &i::IMP, 4 }, { &i::XOR,  &i::IMP, 4  }, { &i::XOR,  &i::IMP, 4  }, { &i::XOR, &i::INR, 8 }, { &i::XOR,  &i::IMP, 4  },
        { &i::OR,   &i::IMP, 4     }, { &i::OR,  &i::IMP, 4      }, { &i::OR,  &i::IMP,  4    }, { &i::OR,  &i::IMP, 4  }, { &i::OR,   &i::IMP, 4  }, { &i::OR,   &i::IMP, 4  }, { &i::OR,   &i::INR, 8  }, { &i::OR,   &i::IMP, 4    }, { &i::CP,  &i::IMP, 4  }, { &i::CP,   &i::IMP, 4  }, { &i::CP,  &i::IMP, 4     }, { &i::CP,  &i::IMP, 4 }, { &i::CP,   &i::IMP, 4  }, { &i::CP,   &i::IMP, 4  }, { &i::CP,  &i::INR, 8 }, { &i::CP,   &i::IMP, 4  },
        { &i::RET,  &i::IMP, 8     }, { &i::POP, &i::IMP, 12     }, { &i::JP,  &i::ABS,  12   }, { &i::JP,  &i::ABS, 16 }, { &i::CALL, &i::ABS, 12 }, { &i::PUSH, &i::IMP, 4  }, { &i::ADD,  &i::IMM, 8  }, { &i::RST,  &i::MZP, 16   }, { &i::RET, &i::IMP, 8  }, { &i::RET,  &i::IMP, 16 }, { &i::JP,  &i::ABS, 12    }, { &i::CB,  &i::IMP, 4 }, { &i::CALL, &i::ABS, 12 }, { &i::CALL, &i::ABS, 24 }, { &i::ADC, &i::IMM, 8 }, { &i::RST,  &i::MZP, 16 },
		{ &i::RET,  &i::IMP, 8     }, { &i::POP, &i::IMP, 12     }, { &i::JP,  &i::ABS,  12   }, { &i::NOP, &i::IMP, 4  }, { &i::CALL, &i::ABS, 12 }, { &i::PUSH, &i::IMP, 16 }, { &i::SUB,  &i::IMM, 8  }, { &i::RST,  &i::MZP, 16   }, { &i::RET, &i::IMP, 8  }, { &i::RETI, &i::IMP, 16 }, { &i::JP,  &i::ABS, 12    }, { &i::NOP, &i::IMP, 4 }, { &i::CALL, &i::ABS, 12 }, { &i::NOP,  &i::IMP, 4  }, { &i::SBC, &i::IMM, 8 }, { &i::RST,  &i::MZP, 16 },
		{ &i::LDH,  &i::IMM, 64|12 }, { &i::POP, &i::IMP, 12     }, { &i::LD,  &i::MIDX, 64|8 }, { &i::NOP, &i::IMP, 4  }, { &i::NOP,  &i::IMP, 4  }, { &i::PUSH, &i::IMP, 16 }, { &i::AND,  &i::IMM, 8  }, { &i::RST,  &i::MZP, 16   }, { &i::ADD, &i::IMM, 16 }, { &i::JP,   &i::INR, 4  }, { &i::LD,  &i::ABS, 64|16 }, { &i::NOP, &i::IMP, 4 }, { &i::NOP,  &i::IMP, 4  }, { &i::NOP,  &i::IMP, 4  }, { &i::XOR, &i::IMM, 8 }, { &i::RST,  &i::MZP, 16 },
		{ &i::LDH,  &i::IMM, 12    }, { &i::POP, &i::IMP, 12     }, { &i::LD,  &i::MIDX, 8    }, { &i::DI,  &i::IMP, 4  }, { &i::NOP,  &i::IMP, 4  }, { &i::PUSH, &i::IMP, 16 }, { &i::OR,   &i::IMM, 8  }, { &i::RST,  &i::MZP, 16   }, { &i::LD,  &i::IMP, 12 }, { &i::LD,   &i::IMP, 8, }, { &i::LD,  &i::ABS, 16    }, { &i::EI,  &i::IMP, 4 }, { &i::NOP,  &i::IMP, 4  }, { &i::NOP,  &i::IMP, 4  }, { &i::CP,  &i::IMM, 8 }, { &i::RST,  &i::MZP, 16 }
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
	m_registerNamed.F.byte = 0x0;
	m_registerNamed.PC = 0x0;

	// helper variables
	m_currentOpcode = 0x0;
	m_isStore = false;
	m_is16bit = false;
	m_isCBInstruction = false;
	m_operandAddress = 0x0;
}

void CPU::doCycles(size_t cycles)
{
	while (cycles--) {
		if (m_currentInstructionCyclesLeft == 0) {

			m_currentOpcode = (this->*m_readByteFunc)(m_registerNamed.PC++);
			const Instruction& instruction = m_instructionSet[m_currentOpcode];
			m_currentInstructionCyclesLeft = instruction.cycles;
			m_isStore = instruction.isStore;
			m_is16bit = instruction.is16bit;
			if (m_isCBInstruction)
				CB();
			else {
				(this->*instruction.addressing)();
				(this->*instruction.operation)();
			}
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
