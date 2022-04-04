#include "cpu_tests_fixture.h"

using CPUTests8bitArithmetic = CPUTests;

TEST_F(CPUTests8bitArithmetic, INC_BTest)
{
	dummyROM[0x100] = 0x04;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(4);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC = 0x101;

	compareCPUStates(preExecutionState, postExecutionState);
	EXPECT_TRUE(false);
}
