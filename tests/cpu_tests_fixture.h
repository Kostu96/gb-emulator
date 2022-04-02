#pragma once
#include "gameboy_fixture.h"

struct CPUTests :
	public GameBoyFixture
{
	struct CPUState {
		uint16_t PC;
		uint16_t SP;
		uint16_t AF;
		uint16_t BC;
		uint16_t DE;
		uint16_t HL;
		uint8_t instructionCyclesLeft;
	};

	CPUTests() {
		memset(dummyROM, 0, sizeof(dummyROM));
	}

	void SetUp() override {
		GameBoyFixture::SetUp();
		memset(dummyROM, 0, sizeof(dummyROM));
	}

	CPUState dumpCPUState() {
		CPUState state{
			cpu.PC,
			cpu.SP,
			cpu.AF,
			cpu.BC,
			cpu.DE,
			cpu.HL,
			cpu.m_currentInstructionCyclesLeft
		};
		return state;
	}

	void compareCPUStates(const CPUState& state1, const CPUState& state2) {
		EXPECT_EQ(state1.PC, state2.PC);
		EXPECT_EQ(state1.SP, state2.SP);
		EXPECT_EQ(state1.AF, state2.AF);
		EXPECT_EQ(state1.BC, state2.BC);
		EXPECT_EQ(state1.DE, state2.DE);
		EXPECT_EQ(state1.HL, state2.HL);
		EXPECT_EQ(state1.instructionCyclesLeft, state2.instructionCyclesLeft);
	}

	uint8_t dummyROM[0x110]{};
};
