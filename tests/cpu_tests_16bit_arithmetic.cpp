#include "cpu_tests_fixture.h"

using CPUTests16bitArithmetic = CPUTests;

TEST_F(CPUTests16bitArithmetic, INC_BCTest)
{
	dummyROM[0x100] = 0x03;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(8);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 1;
	preExecutionState.BC += 1;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTests16bitArithmetic, ADD_HL_BCTest)
{
	dummyROM[0x100] = 0x09;
	cpu.HL = 0xDE00;
	cpu.BC = 0x00AD;
	cpu.F.byte &= 0x0F;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(8);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 1;
	preExecutionState.HL = 0xDEAD;
	preExecutionState.AF |= 0x0;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTests16bitArithmetic, DEC_BCTest)
{
	dummyROM[0x100] = 0x0B;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(8);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 1;
	preExecutionState.BC -= 1;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTests16bitArithmetic, INC_DETest)
{
	dummyROM[0x100] = 0x13;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(8);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 1;
	preExecutionState.DE += 1;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTests16bitArithmetic, ADD_HL_DETest)
{
	dummyROM[0x100] = 0x19;
	cpu.HL = 0xDE00;
	cpu.DE = 0x00AD;
	cpu.F.byte &= 0x0F;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(8);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 1;
	preExecutionState.HL = 0xDEAD;
	preExecutionState.AF |= 0x0;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTests16bitArithmetic, DEC_DETest)
{
	dummyROM[0x100] = 0x1B;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(8);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 1;
	preExecutionState.DE -= 1;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTests16bitArithmetic, INC_HLTest)
{
	dummyROM[0x100] = 0x23;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(8);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 1;
	preExecutionState.HL += 1;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTests16bitArithmetic, ADD_HL_HLTest)
{
	dummyROM[0x100] = 0x29;
	cpu.HL = 0x4269;
	cpu.F.byte &= 0x0F;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(8);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 1;
	preExecutionState.HL = 0x84D2;
	preExecutionState.AF |= 0x0;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTests16bitArithmetic, DEC_HLTest)
{
	dummyROM[0x100] = 0x2B;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(8);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 1;
	preExecutionState.HL -= 1;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTests16bitArithmetic, INC_SPTest)
{
	dummyROM[0x100] = 0x33;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(8);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 1;
	preExecutionState.SP += 1;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTests16bitArithmetic, ADD_HL_SPTest)
{
	dummyROM[0x100] = 0x39;
	cpu.HL = 0xDE00;
	cpu.SP = 0x00AD;
	cpu.F.byte &= 0x0F;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(8);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 1;
	preExecutionState.HL = 0xDEAD;
	preExecutionState.AF |= 0x000F;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTests16bitArithmetic, DEC_SPTest)
{
	dummyROM[0x100] = 0x3B;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(8);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 1;
	preExecutionState.SP -= 1;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTests16bitArithmetic, ADD_SP_ImmediateTest)
{
	dummyROM[0x100] = 0xE8;
	dummyROM[0x101] = 0x42;
	cpu.SP = 0xDE00;
	cpu.F.byte &= 0x0F;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(16);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC += 2;
	preExecutionState.SP = 0xDE42;
	preExecutionState.AF |= 0x000F;

	compareCPUStates(preExecutionState, postExecutionState);
}
