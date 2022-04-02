#include "cpu_tests_fixture.h"

using CPUTests16bitMoves = CPUTests;

TEST_F(CPUTests16bitMoves, LDRR_BC_ImmediateTest)
{
	dummyROM[0x100] = 0x01;
	dummyROM[0x101] = 0x42;
	dummyROM[0x102] = 0x69;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(12);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 3;
	preExecutionState.BC = 0x6942;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTests16bitMoves, LDM_Immediate_SPTest)
{
	dummyROM[0x100] = 0x08;
	dummyROM[0x101] = 0x42;
	dummyROM[0x102] = 0xC0;
	cpu.SP = 0xDEAD;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(20);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 3;

	compareCPUStates(preExecutionState, postExecutionState);
	EXPECT_EQ(memoryMap.getWRAM()[0x42], 0xAD);
	EXPECT_EQ(memoryMap.getWRAM()[0x43], 0xDE);
}

TEST_F(CPUTests16bitMoves, LDRR_DE_ImmediateTest)
{
	dummyROM[0x100] = 0x11;
	dummyROM[0x101] = 0x42;
	dummyROM[0x102] = 0x69;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(12);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 3;
	preExecutionState.DE = 0x6942;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTests16bitMoves, LDRR_HL_ImmediateTest)
{
	dummyROM[0x100] = 0x21;
	dummyROM[0x101] = 0x42;
	dummyROM[0x102] = 0x69;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(12);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 3;
	preExecutionState.HL = 0x6942;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTests16bitMoves, LDRR_SP_ImmediateTest)
{
	dummyROM[0x100] = 0x31;
	dummyROM[0x101] = 0x42;
	dummyROM[0x102] = 0x69;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(12);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 3;
	preExecutionState.SP = 0x6942;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTests16bitMoves, POP_BCTest)
{
	dummyROM[0x100] = 0xC1;
	cpu.SP = 0xFFFC;
	memoryMap.getHRAM()[0x7C] = 0xAD;
	memoryMap.getHRAM()[0x7D] = 0xDE;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(12);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 1;
	preExecutionState.SP += 2;
	preExecutionState.BC = 0xDEAD;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTests16bitMoves, PUSH_BCTest)
{
	dummyROM[0x100] = 0xC5;
	cpu.BC = 0xDEAD;
	cpu.SP = 0xFFFE;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(16);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 1;
	preExecutionState.SP -= 2;

	compareCPUStates(preExecutionState, postExecutionState);
	EXPECT_EQ(memoryMap.getHRAM()[0x7C], 0xAD);
	EXPECT_EQ(memoryMap.getHRAM()[0x7D], 0xDE);
}

TEST_F(CPUTests16bitMoves, POP_DETest)
{
	dummyROM[0x100] = 0xD1;
	cpu.SP = 0xFFFC;
	memoryMap.getHRAM()[0x7C] = 0xAD;
	memoryMap.getHRAM()[0x7D] = 0xDE;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(12);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 1;
	preExecutionState.SP += 2;
	preExecutionState.DE = 0xDEAD;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTests16bitMoves, PUSH_DETest)
{
	dummyROM[0x100] = 0xD5;
	cpu.DE = 0xDEAD;
	cpu.SP = 0xFFFE;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(16);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 1;
	preExecutionState.SP -= 2;

	compareCPUStates(preExecutionState, postExecutionState);
	EXPECT_EQ(memoryMap.getHRAM()[0x7C], 0xAD);
	EXPECT_EQ(memoryMap.getHRAM()[0x7D], 0xDE);
}

TEST_F(CPUTests16bitMoves, POP_HLTest)
{
	dummyROM[0x100] = 0xE1;
	cpu.SP = 0xFFFC;
	memoryMap.getHRAM()[0x7C] = 0xAD;
	memoryMap.getHRAM()[0x7D] = 0xDE;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(12);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 1;
	preExecutionState.SP += 2;
	preExecutionState.HL = 0xDEAD;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTests16bitMoves, PUSH_HLTest)
{
	dummyROM[0x100] = 0xE5;
	cpu.HL = 0xDEAD;
	cpu.SP = 0xFFFE;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(16);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 1;
	preExecutionState.SP -= 2;

	compareCPUStates(preExecutionState, postExecutionState);
	EXPECT_EQ(memoryMap.getHRAM()[0x7C], 0xAD);
	EXPECT_EQ(memoryMap.getHRAM()[0x7D], 0xDE);
}

TEST_F(CPUTests16bitMoves, POP_AFTest)
{
	dummyROM[0x100] = 0xF1;
	cpu.SP = 0xFFFC;
	memoryMap.getHRAM()[0x7C] = 0xAD;
	memoryMap.getHRAM()[0x7D] = 0xDE;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(12);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 1;
	preExecutionState.SP += 2;
	preExecutionState.AF = 0xDEAD | 0x000F;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTests16bitMoves, PUSH_AFTest)
{
	dummyROM[0x100] = 0xF5;
	cpu.AF= 0xDEAD;
	cpu.SP = 0xFFFE;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(16);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 1;
	preExecutionState.SP -= 2;

	compareCPUStates(preExecutionState, postExecutionState);
	EXPECT_EQ(memoryMap.getHRAM()[0x7C], 0xAD);
	EXPECT_EQ(memoryMap.getHRAM()[0x7D], 0xDE);
}

TEST_F(CPUTests16bitMoves, LDRR_HL_SPPlusImmediateTest)
{
	// TODO: test negative numbers
	dummyROM[0x100] = 0xF8;
	dummyROM[0x101] = 0x42;
	cpu.SP = 0xDE00;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(12);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 2;
	preExecutionState.HL = 0xDE42;
	preExecutionState.AF &= 0xFF00;
	preExecutionState.AF |= 0x000F;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTests16bitMoves, LDRR_SP_HLTest)
{
	dummyROM[0x100] = 0xF9;
	cpu.HL = 0xDEAD;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(8);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 1;
	preExecutionState.SP = 0xDEAD;

	compareCPUStates(preExecutionState, postExecutionState);
}
