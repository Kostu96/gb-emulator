#include "cpu_tests_fixture.h"

using CPUTestsMisc = CPUTests;

TEST_F(CPUTestsMisc, NOPTest)
{
	dummyROM[0x100] = 0x00;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(4);
	CPUState postExecutionState = dumpCPUState();

	++preExecutionState.PC;

	compareCPUStates(preExecutionState, postExecutionState);
}
