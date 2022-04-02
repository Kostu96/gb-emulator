#include "cpu_tests_fixture.h"

using CPUTests8bitMoves = CPUTests;

TEST_F(CPUTests8bitMoves, LDM_BC_ATest)
{
	dummyROM[0x100] = 0x02;
	cpu.A = 0x42;
	cpu.BC = 0xC000;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(8);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 1;

	compareCPUStates(preExecutionState, postExecutionState);
	EXPECT_EQ(memoryMap.getWRAM()[0], 0x42);
}

TEST_F(CPUTests8bitMoves, LDR_B_ImmediateTest)
{
	dummyROM[0x100] = 0x06;
	dummyROM[0x101] = 0x42;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(8);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 2;
	preExecutionState.BC &= 0x00FF;
	preExecutionState.BC |= 0x42 << 8;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTests8bitMoves, LDR_A_BCTest)
{
	dummyROM[0x100] = 0x0A;
	memoryMap.getWRAM()[0] = 0x42;
	cpu.BC = 0xC000;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(8);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 1;
	preExecutionState.AF &= 0x00FF;
	preExecutionState.AF |= 0x42 << 8;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTests8bitMoves, LDR_C_ImmediateTest)
{
	dummyROM[0x100] = 0x0E;
	dummyROM[0x101] = 0x42;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(8);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 2;
	preExecutionState.BC &= 0xFF00;
	preExecutionState.BC |= 0x42;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTests8bitMoves, LDM_DE_ATest)
{
	dummyROM[0x100] = 0x12;
	cpu.A = 0x42;
	cpu.DE = 0xC000;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(8);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 1;

	compareCPUStates(preExecutionState, postExecutionState);
	EXPECT_EQ(memoryMap.getWRAM()[0], 0x42);
}

TEST_F(CPUTests8bitMoves, LDR_D_ImmediateTest)
{
	dummyROM[0x100] = 0x16;
	dummyROM[0x101] = 0x42;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(8);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 2;
	preExecutionState.DE &= 0x00FF;
	preExecutionState.DE |= 0x42 << 8;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTests8bitMoves, LDR_A_DETest)
{
	dummyROM[0x100] = 0x1A;
	memoryMap.getWRAM()[0] = 0x42;
	cpu.DE = 0xC000;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(8);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 1;
	preExecutionState.AF &= 0x00FF;
	preExecutionState.AF |= 0x42 << 8;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTests8bitMoves, LDR_E_ImmediateTest)
{
	dummyROM[0x100] = 0x1E;
	dummyROM[0x101] = 0x42;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(8);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 2;
	preExecutionState.DE &= 0xFF00;
	preExecutionState.DE |= 0x42;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTests8bitMoves, LDM_HLI_ATest)
{
	dummyROM[0x100] = 0x22;
	cpu.A = 0x42;
	cpu.HL = 0xC000;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(8);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 1;
	preExecutionState.HL += 1;

	compareCPUStates(preExecutionState, postExecutionState);
	EXPECT_EQ(memoryMap.getWRAM()[0], 0x42);
}

TEST_F(CPUTests8bitMoves, LDR_H_ImmediateTest)
{
	dummyROM[0x100] = 0x26;
	dummyROM[0x101] = 0x42;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(8);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 2;
	preExecutionState.HL &= 0x00FF;
	preExecutionState.HL |= 0x42 << 8;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTests8bitMoves, LDR_A_HLITest)
{
	dummyROM[0x100] = 0x2A;
	memoryMap.getWRAM()[0] = 0x42;
	cpu.HL = 0xC000;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(8);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 1;
	preExecutionState.HL += 1;
	preExecutionState.AF &= 0x00FF;
	preExecutionState.AF |= 0x42 << 8;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTests8bitMoves, LDR_L_ImmediateTest)
{
	dummyROM[0x100] = 0x2E;
	dummyROM[0x101] = 0x42;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(8);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 2;
	preExecutionState.HL &= 0xFF00;
	preExecutionState.HL |= 0x42;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTests8bitMoves, LDM_HLD_ATest)
{
	dummyROM[0x100] = 0x32;
	cpu.A = 0x42;
	cpu.HL = 0xC000;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(8);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 1;
	preExecutionState.HL -= 1;

	compareCPUStates(preExecutionState, postExecutionState);
	EXPECT_EQ(memoryMap.getWRAM()[0], 0x42);
}

TEST_F(CPUTests8bitMoves, LDM_HL_ImmediateTest)
{
	dummyROM[0x100] = 0x36;
	dummyROM[0x101] = 0x42;
	cpu.HL = 0xC000;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(12);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 2;

	compareCPUStates(preExecutionState, postExecutionState);
	EXPECT_EQ(memoryMap.getWRAM()[0], 0x42);
}

TEST_F(CPUTests8bitMoves, LDR_A_HLDTest)
{
	dummyROM[0x100] = 0x3A;
	memoryMap.getWRAM()[0] = 0x42;
	cpu.HL = 0xC000;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(8);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 1;
	preExecutionState.HL -= 1;
	preExecutionState.AF &= 0x00FF;
	preExecutionState.AF |= 0x42 << 8;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTests8bitMoves, LDR_A_ImmediateTest)
{
	dummyROM[0x100] = 0x3E;
	dummyROM[0x101] = 0x42;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(8);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 2;
	preExecutionState.AF &= 0x00FF;
	preExecutionState.AF |= 0x42 << 8;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTests8bitMoves, LDR_B_BTest)
{
	dummyROM[0x100] = 0x40;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(4);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 1;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTests8bitMoves, LDR_B_CTest)
{
	dummyROM[0x100] = 0x41;
	cpu.C = 0x42;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(4);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 1;
	preExecutionState.BC &= 0x00FF;
	preExecutionState.BC |= 0x42 << 8;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTests8bitMoves, LDR_B_DTest)
{
	dummyROM[0x100] = 0x42;
	cpu.D = 0x42;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(4);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 1;
	preExecutionState.BC &= 0x00FF;
	preExecutionState.BC |= 0x42 << 8;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTests8bitMoves, LDR_B_ETest)
{
	dummyROM[0x100] = 0x43;
	cpu.E = 0x42;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(4);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 1;
	preExecutionState.BC &= 0x00FF;
	preExecutionState.BC |= 0x42 << 8;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTests8bitMoves, LDR_B_HTest)
{
	dummyROM[0x100] = 0x44;
	cpu.H = 0x42;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(4);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 1;
	preExecutionState.BC &= 0x00FF;
	preExecutionState.BC |= 0x42 << 8;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTests8bitMoves, LDR_B_LTest)
{
	dummyROM[0x100] = 0x45;
	cpu.L = 0x42;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(4);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 1;
	preExecutionState.BC &= 0x00FF;
	preExecutionState.BC |= 0x42 << 8;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTests8bitMoves, LDR_B_HLTest)
{
	dummyROM[0x100] = 0x46;
	memoryMap.getWRAM()[0] = 0x42;
	cpu.HL = 0xC000;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(8);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 1;
	preExecutionState.BC &= 0x00FF;
	preExecutionState.BC |= 0x42 << 8;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTests8bitMoves, LDR_B_ATest)
{
	dummyROM[0x100] = 0x47;
	cpu.A = 0x42;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(4);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 1;
	preExecutionState.BC &= 0x00FF;
	preExecutionState.BC |= 0x42 << 8;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTests8bitMoves, LDR_C_BTest)
{
	dummyROM[0x100] = 0x48;
	cpu.B = 0x42;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(4);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 1;
	preExecutionState.BC &= 0xFF00;
	preExecutionState.BC |= 0x42;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTests8bitMoves, LDR_C_CTest)
{
	dummyROM[0x100] = 0x49;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(4);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 1;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTests8bitMoves, LDR_C_DTest)
{
	dummyROM[0x100] = 0x4A;
	cpu.D = 0x42;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(4);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 1;
	preExecutionState.BC &= 0xFF00;
	preExecutionState.BC |= 0x42;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTests8bitMoves, LDR_C_ETest)
{
	dummyROM[0x100] = 0x4B;
	cpu.E = 0x42;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(4);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 1;
	preExecutionState.BC &= 0xFF00;
	preExecutionState.BC |= 0x42;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTests8bitMoves, LDR_C_HTest)
{
	dummyROM[0x100] = 0x4C;
	cpu.H = 0x42;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(4);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 1;
	preExecutionState.BC &= 0xFF00;
	preExecutionState.BC |= 0x42;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTests8bitMoves, LDR_C_LTest)
{
	dummyROM[0x100] = 0x4D;
	cpu.L = 0x42;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(4);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 1;
	preExecutionState.BC &= 0xFF00;
	preExecutionState.BC |= 0x42;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTests8bitMoves, LDR_C_HLTest)
{
	dummyROM[0x100] = 0x4E;
	memoryMap.getWRAM()[0] = 0x42;
	cpu.HL = 0xC000;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(8);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 1;
	preExecutionState.BC &= 0xFF00;
	preExecutionState.BC |= 0x42;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTests8bitMoves, LDR_C_ATest)
{
	dummyROM[0x100] = 0x4F;
	cpu.A = 0x42;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(4);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 1;
	preExecutionState.BC &= 0xFF00;
	preExecutionState.BC |= 0x42;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTests8bitMoves, LDR_D_BTest)
{
	dummyROM[0x100] = 0x50;
	cpu.B = 0x42;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(4);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 1;
	preExecutionState.DE &= 0x00FF;
	preExecutionState.DE |= 0x42 << 8;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTests8bitMoves, LDR_D_CTest)
{
	dummyROM[0x100] = 0x51;
	cpu.C = 0x42;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(4);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 1;
	preExecutionState.DE &= 0x00FF;
	preExecutionState.DE |= 0x42 << 8;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTests8bitMoves, LDR_D_DTest)
{
	dummyROM[0x100] = 0x52;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(4);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 1;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTests8bitMoves, LDR_D_ETest)
{
	dummyROM[0x100] = 0x53;
	cpu.E = 0x42;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(4);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 1;
	preExecutionState.DE &= 0x00FF;
	preExecutionState.DE |= 0x42 << 8;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTests8bitMoves, LDR_D_HTest)
{
	dummyROM[0x100] = 0x54;
	cpu.H = 0x42;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(4);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 1;
	preExecutionState.DE &= 0x00FF;
	preExecutionState.DE |= 0x42 << 8;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTests8bitMoves, LDR_D_LTest)
{
	dummyROM[0x100] = 0x55;
	cpu.L = 0x42;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(4);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 1;
	preExecutionState.DE &= 0x00FF;
	preExecutionState.DE |= 0x42 << 8;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTests8bitMoves, LDR_D_HLTest)
{
	dummyROM[0x100] = 0x56;
	memoryMap.getWRAM()[0] = 0x42;
	cpu.HL = 0xC000;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(8);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 1;
	preExecutionState.DE &= 0x00FF;
	preExecutionState.DE |= 0x42 << 8;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTests8bitMoves, LDR_D_ATest)
{
	dummyROM[0x100] = 0x57;
	cpu.A = 0x42;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(4);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 1;
	preExecutionState.DE &= 0x00FF;
	preExecutionState.DE |= 0x42 << 8;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTests8bitMoves, LDR_E_BTest)
{
	dummyROM[0x100] = 0x58;
	cpu.B = 0x42;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(4);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 1;
	preExecutionState.DE &= 0xFF00;
	preExecutionState.DE |= 0x42;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTests8bitMoves, LDR_E_CTest)
{
	dummyROM[0x100] = 0x59;
	cpu.C = 0x42;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(4);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 1;
	preExecutionState.DE &= 0xFF00;
	preExecutionState.DE |= 0x42;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTests8bitMoves, LDR_E_DTest)
{
	dummyROM[0x100] = 0x5A;
	cpu.D = 0x42;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(4);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 1;
	preExecutionState.DE &= 0xFF00;
	preExecutionState.DE |= 0x42;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTests8bitMoves, LDR_E_ETest)
{
	dummyROM[0x100] = 0x5B;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(4);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 1;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTests8bitMoves, LDR_E_HTest)
{
	dummyROM[0x100] = 0x5C;
	cpu.H = 0x42;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(4);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 1;
	preExecutionState.DE &= 0xFF00;
	preExecutionState.DE |= 0x42;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTests8bitMoves, LDR_E_LTest)
{
	dummyROM[0x100] = 0x5D;
	cpu.L = 0x42;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(4);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 1;
	preExecutionState.DE &= 0xFF00;
	preExecutionState.DE |= 0x42;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTests8bitMoves, LDR_E_HLTest)
{
	dummyROM[0x100] = 0x5E;
	memoryMap.getWRAM()[0] = 0x42;
	cpu.HL = 0xC000;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(8);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 1;
	preExecutionState.DE &= 0xFF00;
	preExecutionState.DE |= 0x42;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTests8bitMoves, LDR_E_ATest)
{
	dummyROM[0x100] = 0x5F;
	cpu.A = 0x42;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(4);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 1;
	preExecutionState.DE &= 0xFF00;
	preExecutionState.DE |= 0x42;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTests8bitMoves, LDR_H_BTest)
{
	dummyROM[0x100] = 0x60;
	cpu.B = 0x42;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(4);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 1;
	preExecutionState.HL &= 0x00FF;
	preExecutionState.HL |= 0x42 << 8;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTests8bitMoves, LDR_H_CTest)
{
	dummyROM[0x100] = 0x61;
	cpu.C = 0x42;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(4);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 1;
	preExecutionState.HL &= 0x00FF;
	preExecutionState.HL |= 0x42 << 8;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTests8bitMoves, LDR_H_DTest)
{
	dummyROM[0x100] = 0x62;
	cpu.D = 0x42;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(4);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 1;
	preExecutionState.HL &= 0x00FF;
	preExecutionState.HL |= 0x42 << 8;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTests8bitMoves, LDR_H_ETest)
{
	dummyROM[0x100] = 0x63;
	cpu.E = 0x42;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(4);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 1;
	preExecutionState.HL &= 0x00FF;
	preExecutionState.HL |= 0x42 << 8;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTests8bitMoves, LDR_H_HTest)
{
	dummyROM[0x100] = 0x64;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(4);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 1;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTests8bitMoves, LDR_H_LTest)
{
	dummyROM[0x100] = 0x65;
	cpu.L = 0x42;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(4);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 1;
	preExecutionState.HL &= 0x00FF;
	preExecutionState.HL |= 0x42 << 8;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTests8bitMoves, LDR_H_HLTest)
{
	dummyROM[0x100] = 0x66;
	memoryMap.getWRAM()[0] = 0x42;
	cpu.HL = 0xC000;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(8);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 1;
	preExecutionState.HL &= 0x00FF;
	preExecutionState.HL |= 0x42 << 8;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTests8bitMoves, LDR_H_ATest)
{
	dummyROM[0x100] = 0x67;
	cpu.A = 0x42;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(4);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 1;
	preExecutionState.HL &= 0x00FF;
	preExecutionState.HL |= 0x42 << 8;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTests8bitMoves, LDR_L_BTest)
{
	dummyROM[0x100] = 0x68;
	cpu.B = 0x42;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(4);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 1;
	preExecutionState.HL &= 0xFF00;
	preExecutionState.HL |= 0x42;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTests8bitMoves, LDR_L_CTest)
{
	dummyROM[0x100] = 0x69;
	cpu.C = 0x42;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(4);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 1;
	preExecutionState.HL &= 0xFF00;
	preExecutionState.HL |= 0x42;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTests8bitMoves, LDR_L_DTest)
{
	dummyROM[0x100] = 0x6A;
	cpu.D = 0x42;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(4);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 1;
	preExecutionState.HL &= 0xFF00;
	preExecutionState.HL |= 0x42;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTests8bitMoves, LDR_L_ETest)
{
	dummyROM[0x100] = 0x6B;
	cpu.E = 0x42;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(4);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 1;
	preExecutionState.HL &= 0xFF00;
	preExecutionState.HL |= 0x42;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTests8bitMoves, LDR_L_HTest)
{
	dummyROM[0x100] = 0x6C;
	cpu.H = 0x42;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(4);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 1;
	preExecutionState.HL &= 0xFF00;
	preExecutionState.HL |= 0x42;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTests8bitMoves, LDR_L_LTest)
{
	dummyROM[0x100] = 0x6D;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(4);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 1;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTests8bitMoves, LDR_L_HLTest)
{
	dummyROM[0x100] = 0x6E;
	memoryMap.getWRAM()[0] = 0x42;
	cpu.HL = 0xC000;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(8);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 1;
	preExecutionState.HL &= 0xFF00;
	preExecutionState.HL |= 0x42;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTests8bitMoves, LDR_L_ATest)
{
	dummyROM[0x100] = 0x6F;
	cpu.A = 0x42;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(4);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 1;
	preExecutionState.HL &= 0xFF00;
	preExecutionState.HL |= 0x42;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTests8bitMoves, LDM_HL_BTest)
{
	dummyROM[0x100] = 0x70;
	cpu.B = 0x42;
	cpu.HL = 0xC000;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(8);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 1;

	compareCPUStates(preExecutionState, postExecutionState);
	EXPECT_EQ(memoryMap.getWRAM()[0], 0x42);
}

TEST_F(CPUTests8bitMoves, LDM_HL_CTest)
{
	dummyROM[0x100] = 0x71;
	cpu.C = 0x42;
	cpu.HL = 0xC000;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(8);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 1;

	compareCPUStates(preExecutionState, postExecutionState);
	EXPECT_EQ(memoryMap.getWRAM()[0], 0x42);
}

TEST_F(CPUTests8bitMoves, LDM_HL_DTest)
{
	dummyROM[0x100] = 0x72;
	cpu.D = 0x42;
	cpu.HL = 0xC000;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(8);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 1;

	compareCPUStates(preExecutionState, postExecutionState);
	EXPECT_EQ(memoryMap.getWRAM()[0], 0x42);
}

TEST_F(CPUTests8bitMoves, LDM_HL_ETest)
{
	dummyROM[0x100] = 0x73;
	cpu.E = 0x42;
	cpu.HL = 0xC000;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(8);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 1;

	compareCPUStates(preExecutionState, postExecutionState);
	EXPECT_EQ(memoryMap.getWRAM()[0], 0x42);
}

TEST_F(CPUTests8bitMoves, LDM_HL_HTest)
{
	dummyROM[0x100] = 0x74;
	cpu.HL = 0xC000;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(8);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 1;

	compareCPUStates(preExecutionState, postExecutionState);
	EXPECT_EQ(memoryMap.getWRAM()[0], 0xC0);
}

TEST_F(CPUTests8bitMoves, LDM_HL_LTest)
{
	dummyROM[0x100] = 0x75;
	cpu.HL = 0xC042;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(8);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 1;

	compareCPUStates(preExecutionState, postExecutionState);
	EXPECT_EQ(memoryMap.getWRAM()[0x42], 0x42);
}

TEST_F(CPUTests8bitMoves, LDM_HL_ATest)
{
	dummyROM[0x100] = 0x77;
	cpu.A = 0x42;
	cpu.HL = 0xC000;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(8);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 1;

	compareCPUStates(preExecutionState, postExecutionState);
	EXPECT_EQ(memoryMap.getWRAM()[0], 0x42);
}

TEST_F(CPUTests8bitMoves, LDR_A_BTest)
{
	dummyROM[0x100] = 0x78;
	cpu.B = 0x42;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(4);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 1;
	preExecutionState.AF &= 0x00FF;
	preExecutionState.AF |= 0x42 << 8;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTests8bitMoves, LDR_A_CTest)
{
	dummyROM[0x100] = 0x79;
	cpu.C = 0x42;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(4);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 1;
	preExecutionState.AF &= 0x00FF;
	preExecutionState.AF |= 0x42 << 8;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTests8bitMoves, LDR_A_DTest)
{
	dummyROM[0x100] = 0x7A;
	cpu.D = 0x42;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(4);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 1;
	preExecutionState.AF &= 0x00FF;
	preExecutionState.AF |= 0x42 << 8;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTests8bitMoves, LDR_A_ETest)
{
	dummyROM[0x100] = 0x7B;
	cpu.E = 0x42;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(4);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 1;
	preExecutionState.AF &= 0x00FF;
	preExecutionState.AF |= 0x42 << 8;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTests8bitMoves, LDR_A_HTest)
{
	dummyROM[0x100] = 0x7C;
	cpu.H = 0x42;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(4);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 1;
	preExecutionState.AF &= 0x00FF;
	preExecutionState.AF |= 0x42 << 8;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTests8bitMoves, LDR_A_LTest)
{
	dummyROM[0x100] = 0x7D;
	cpu.L = 0x42;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(4);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 1;
	preExecutionState.AF &= 0x00FF;
	preExecutionState.AF |= 0x42 << 8;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTests8bitMoves, LDR_A_HLTest)
{
	dummyROM[0x100] = 0x7E;
	memoryMap.getWRAM()[0] = 0x42;
	cpu.HL = 0xC000;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(8);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 1;
	preExecutionState.AF &= 0x00FF;
	preExecutionState.AF |= 0x42 << 8;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTests8bitMoves, LDR_A_ATest)
{
	dummyROM[0x100] = 0x7F;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(4);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 1;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTests8bitMoves, LDM_Immediate8_ATest)
{
	dummyROM[0x100] = 0xE0;
	dummyROM[0x101] = 0x80;
	cpu.A = 0x42;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(12);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 2;

	compareCPUStates(preExecutionState, postExecutionState);
	EXPECT_EQ(memoryMap.getHRAM()[0], 0x42);
}

TEST_F(CPUTests8bitMoves, LDM_C_ATest)
{
	dummyROM[0x100] = 0xE2;
	cpu.C = 0x80;
	cpu.A = 0x42;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(8);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 1;

	compareCPUStates(preExecutionState, postExecutionState);
	EXPECT_EQ(memoryMap.getHRAM()[0], 0x42);
}

TEST_F(CPUTests8bitMoves, LDM_Immediate16_ATest)
{
	dummyROM[0x100] = 0xEA;
	dummyROM[0x101] = 0x69;
	dummyROM[0x102] = 0xC0;
	cpu.A = 0x42;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(16);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 3;

	compareCPUStates(preExecutionState, postExecutionState);
	EXPECT_EQ(memoryMap.getWRAM()[0x69], 0x42);
}

TEST_F(CPUTests8bitMoves, LDR_A_Immedaite8IndirectTest)
{
	dummyROM[0x100] = 0xF0;
	dummyROM[0x101] = 0x80;
	memoryMap.getHRAM()[0] = 0x42;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(12);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 2;
	preExecutionState.AF &= 0x00FF;
	preExecutionState.AF |= 0x42 << 8;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTests8bitMoves, LDR_A_CIndirectTest)
{
	dummyROM[0x100] = 0xF2;
	cpu.C = 0x80;
	memoryMap.getHRAM()[0] = 0x42;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(8);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 1;
	preExecutionState.AF &= 0x00FF;
	preExecutionState.AF |= 0x42 << 8;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTests8bitMoves, LDR_A_Immedaite16IndirectTest)
{
	dummyROM[0x100] = 0xFA;
	dummyROM[0x101] = 0x69;
	dummyROM[0x102] = 0xC0;
	memoryMap.getWRAM()[0x69] = 0x42;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(16);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 3;
	preExecutionState.AF &= 0x00FF;
	preExecutionState.AF |= 0x42 << 8;

	compareCPUStates(preExecutionState, postExecutionState);
}
