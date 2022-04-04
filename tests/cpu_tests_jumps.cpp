#include "cpu_tests_fixture.h"

using CPUTestsJumps = CPUTests;

TEST_F(CPUTestsJumps, JR_ImmediateTest)
{
	dummyROM[0x100] = 0x18;
	dummyROM[0x101] = 0x42;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(12);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC = 0x144;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTestsJumps, JR_NZ_ImmediateTest)
{
	dummyROM[0x100] = 0x20;
	dummyROM[0x101] = 0x42;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	// Condition false
	cpu.F.zeroFlag = 1;

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(8);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC = 0x102;

	compareCPUStates(preExecutionState, postExecutionState);

	// Condition true
	cpu.F.zeroFlag = 0;
	cpu.PC = 0x100;

	preExecutionState = dumpCPUState();
	cpu.doCycles(12);
	postExecutionState = dumpCPUState();

	preExecutionState.PC = 0x144;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTestsJumps, JR_Z_ImmediateTest)
{
	dummyROM[0x100] = 0x28;
	dummyROM[0x101] = 0x42;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	// Condition false
	cpu.F.zeroFlag = 0;

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(8);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC = 0x102;

	compareCPUStates(preExecutionState, postExecutionState);

	// Condition true
	cpu.F.zeroFlag = 1;
	cpu.PC = 0x100;

	preExecutionState = dumpCPUState();
	cpu.doCycles(12);
	postExecutionState = dumpCPUState();

	preExecutionState.PC = 0x144;

	compareCPUStates(preExecutionState, postExecutionState);
}
