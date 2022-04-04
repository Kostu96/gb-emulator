#include "cpu_tests_fixture.h"

using CPUTestsJumps = CPUTests;

TEST_F(CPUTestsJumps, JR_ImmediateTest)
{
	dummyROM[0x100] = 0x18;
	dummyROM[0x101] = 0x42;
	dummyROM[0x102] = 0x18;
	dummyROM[0x103] = 0xFC;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(12);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC = 0x144;

	compareCPUStates(preExecutionState, postExecutionState);

	cpu.PC = 0x102;

	preExecutionState = dumpCPUState();
	cpu.doCycles(12);
	postExecutionState = dumpCPUState();

	preExecutionState.PC = 0x100;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTestsJumps, JR_NZ_ImmediateTest)
{
	dummyROM[0x100] = 0x20;
	dummyROM[0x101] = 0x42;
	dummyROM[0x102] = 0x20;
	dummyROM[0x103] = 0xFC;
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

	cpu.PC = 0x102;

	preExecutionState = dumpCPUState();
	cpu.doCycles(12);
	postExecutionState = dumpCPUState();

	preExecutionState.PC = 0x100;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTestsJumps, JR_Z_ImmediateTest)
{
	dummyROM[0x100] = 0x28;
	dummyROM[0x101] = 0x42;
	dummyROM[0x102] = 0x28;
	dummyROM[0x103] = 0xFC;
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

	cpu.PC = 0x102;

	preExecutionState = dumpCPUState();
	cpu.doCycles(12);
	postExecutionState = dumpCPUState();

	preExecutionState.PC = 0x100;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTestsJumps, JR_NC_ImmediateTest)
{
	dummyROM[0x100] = 0x30;
	dummyROM[0x101] = 0x42;
	dummyROM[0x102] = 0x30;
	dummyROM[0x103] = 0xFC;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	// Condition false
	cpu.F.carryFlag = 1;

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(8);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC = 0x102;

	compareCPUStates(preExecutionState, postExecutionState);

	// Condition true
	cpu.F.carryFlag = 0;
	cpu.PC = 0x100;

	preExecutionState = dumpCPUState();
	cpu.doCycles(12);
	postExecutionState = dumpCPUState();

	preExecutionState.PC = 0x144;

	compareCPUStates(preExecutionState, postExecutionState);

	cpu.PC = 0x102;

	preExecutionState = dumpCPUState();
	cpu.doCycles(12);
	postExecutionState = dumpCPUState();

	preExecutionState.PC = 0x100;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTestsJumps, JR_C_ImmediateTest)
{
	dummyROM[0x100] = 0x38;
	dummyROM[0x101] = 0x42;
	dummyROM[0x102] = 0x38;
	dummyROM[0x103] = 0xFC;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	// Condition false
	cpu.F.carryFlag = 0;

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(8);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC = 0x102;

	compareCPUStates(preExecutionState, postExecutionState);

	// Condition true
	cpu.F.carryFlag = 1;
	cpu.PC = 0x100;

	preExecutionState = dumpCPUState();
	cpu.doCycles(12);
	postExecutionState = dumpCPUState();

	preExecutionState.PC = 0x144;

	compareCPUStates(preExecutionState, postExecutionState);

	cpu.PC = 0x102;

	preExecutionState = dumpCPUState();
	cpu.doCycles(12);
	postExecutionState = dumpCPUState();

	preExecutionState.PC = 0x100;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTestsJumps, RET_NZTest)
{
	dummyROM[0x100] = 0xC0;
	cpu.SP = 0xFFFC;
	memoryMap.getHRAM()[0x7C] = 0xAD;
	memoryMap.getHRAM()[0x7D] = 0xDE;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	// Condition false
	cpu.F.zeroFlag = 1;

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(4);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC = 0x101;

	compareCPUStates(preExecutionState, postExecutionState);

	// Condition true
	cpu.F.zeroFlag = 0;
	cpu.PC = 0x100;

	preExecutionState = dumpCPUState();
	cpu.doCycles(12);
	postExecutionState = dumpCPUState();

	preExecutionState.PC = 0xDEAD;
	preExecutionState.SP = 0xFFFE;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTestsJumps, JP_NZTest)
{
	dummyROM[0x100] = 0xC2;
	dummyROM[0x101] = 0xAD;
	dummyROM[0x102] = 0xDE;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	// Condition false
	cpu.F.zeroFlag = 1;

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(12);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC = 0x103;

	compareCPUStates(preExecutionState, postExecutionState);

	// Condition true
	cpu.F.zeroFlag = 0;
	cpu.PC = 0x100;

	preExecutionState = dumpCPUState();
	cpu.doCycles(12);
	postExecutionState = dumpCPUState();

	preExecutionState.PC = 0xDEAD;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTestsJumps, JPTest)
{
	dummyROM[0x100] = 0xC3;
	dummyROM[0x101] = 0xAD;
	dummyROM[0x102] = 0xDE;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(12);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC = 0xDEAD;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTestsJumps, CALL_NZTest)
{
	dummyROM[0x100] = 0xC4;
	dummyROM[0x101] = 0xAD;
	dummyROM[0x102] = 0xDE;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	// Condition false
	cpu.F.zeroFlag = 1;

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(12);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC = 0x103;

	compareCPUStates(preExecutionState, postExecutionState);

	// Condition true
	cpu.F.zeroFlag = 0;
	cpu.PC = 0x100;

	preExecutionState = dumpCPUState();
	cpu.doCycles(20);
	postExecutionState = dumpCPUState();

	preExecutionState.PC = 0xDEAD;
	preExecutionState.SP = 0xFFFC;

	compareCPUStates(preExecutionState, postExecutionState);
	EXPECT_EQ(memoryMap.getHRAM()[0x7C], 0x03);
	EXPECT_EQ(memoryMap.getHRAM()[0x7D], 0x01);
}

TEST_F(CPUTestsJumps, RST_0x00Test)
{
	dummyROM[0x100] = 0xC7;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(12);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC = 0x0;
	preExecutionState.SP = 0xFFFC;

	compareCPUStates(preExecutionState, postExecutionState);
	EXPECT_EQ(memoryMap.getHRAM()[0x7C], 0x01);
	EXPECT_EQ(memoryMap.getHRAM()[0x7D], 0x01);
}


TEST_F(CPUTestsJumps, RET_ZTest)
{
	dummyROM[0x100] = 0xC8;
	cpu.SP = 0xFFFC;
	memoryMap.getHRAM()[0x7C] = 0xAD;
	memoryMap.getHRAM()[0x7D] = 0xDE;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	// Condition false
	cpu.F.zeroFlag = 0;

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(4);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC = 0x101;

	compareCPUStates(preExecutionState, postExecutionState);

	// Condition true
	cpu.F.zeroFlag = 1;
	cpu.PC = 0x100;

	preExecutionState = dumpCPUState();
	cpu.doCycles(12);
	postExecutionState = dumpCPUState();

	preExecutionState.PC = 0xDEAD;
	preExecutionState.SP = 0xFFFE;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTestsJumps, RETTest)
{
	dummyROM[0x100] = 0xC9;
	cpu.SP = 0xFFFC;
	memoryMap.getHRAM()[0x7C] = 0xAD;
	memoryMap.getHRAM()[0x7D] = 0xDE;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(12);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC = 0xDEAD;
	preExecutionState.SP = 0xFFFE;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTestsJumps, JP_ZTest)
{
	dummyROM[0x100] = 0xCA;
	dummyROM[0x101] = 0xAD;
	dummyROM[0x102] = 0xDE;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	// Condition false
	cpu.F.zeroFlag = 0;

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(12);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC = 0x103;

	compareCPUStates(preExecutionState, postExecutionState);

	// Condition true
	cpu.F.zeroFlag = 1;
	cpu.PC = 0x100;

	preExecutionState = dumpCPUState();
	cpu.doCycles(12);
	postExecutionState = dumpCPUState();

	preExecutionState.PC = 0xDEAD;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTestsJumps, CALL_ZTest)
{
	dummyROM[0x100] = 0xCC;
	dummyROM[0x101] = 0xAD;
	dummyROM[0x102] = 0xDE;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	// Condition false
	cpu.F.zeroFlag = 0;

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(12);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC = 0x103;

	compareCPUStates(preExecutionState, postExecutionState);

	// Condition true
	cpu.F.zeroFlag = 1;
	cpu.PC = 0x100;

	preExecutionState = dumpCPUState();
	cpu.doCycles(20);
	postExecutionState = dumpCPUState();

	preExecutionState.PC = 0xDEAD;
	preExecutionState.SP = 0xFFFC;

	compareCPUStates(preExecutionState, postExecutionState);
	EXPECT_EQ(memoryMap.getHRAM()[0x7C], 0x03);
	EXPECT_EQ(memoryMap.getHRAM()[0x7D], 0x01);
}

TEST_F(CPUTestsJumps, CALLTest)
{
	dummyROM[0x100] = 0xCD;
	dummyROM[0x101] = 0xAD;
	dummyROM[0x102] = 0xDE;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(20);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC = 0xDEAD;
	preExecutionState.SP = 0xFFFC;

	compareCPUStates(preExecutionState, postExecutionState);
	EXPECT_EQ(memoryMap.getHRAM()[0x7C], 0x03);
	EXPECT_EQ(memoryMap.getHRAM()[0x7D], 0x01);
}

TEST_F(CPUTestsJumps, RST_0x08Test)
{
	dummyROM[0x100] = 0xCF;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(12);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC = 0x08;
	preExecutionState.SP = 0xFFFC;

	compareCPUStates(preExecutionState, postExecutionState);
	EXPECT_EQ(memoryMap.getHRAM()[0x7C], 0x01);
	EXPECT_EQ(memoryMap.getHRAM()[0x7D], 0x01);
}

TEST_F(CPUTestsJumps, RET_NCTest)
{
	dummyROM[0x100] = 0xD0;
	cpu.SP = 0xFFFC;
	memoryMap.getHRAM()[0x7C] = 0xAD;
	memoryMap.getHRAM()[0x7D] = 0xDE;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	// Condition false
	cpu.F.carryFlag = 1;

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(4);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC = 0x101;

	compareCPUStates(preExecutionState, postExecutionState);

	// Condition true
	cpu.F.carryFlag = 0;
	cpu.PC = 0x100;

	preExecutionState = dumpCPUState();
	cpu.doCycles(12);
	postExecutionState = dumpCPUState();

	preExecutionState.PC = 0xDEAD;
	preExecutionState.SP = 0xFFFE;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTestsJumps, JP_NCTest)
{
	dummyROM[0x100] = 0xD2;
	dummyROM[0x101] = 0xAD;
	dummyROM[0x102] = 0xDE;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	// Condition false
	cpu.F.carryFlag = 1;

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(12);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC = 0x103;

	compareCPUStates(preExecutionState, postExecutionState);

	// Condition true
	cpu.F.carryFlag = 0;
	cpu.PC = 0x100;

	preExecutionState = dumpCPUState();
	cpu.doCycles(12);
	postExecutionState = dumpCPUState();

	preExecutionState.PC = 0xDEAD;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTestsJumps, CALL_NCTest)
{
	dummyROM[0x100] = 0xD4;
	dummyROM[0x101] = 0xAD;
	dummyROM[0x102] = 0xDE;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	// Condition false
	cpu.F.carryFlag = 1;

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(12);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC = 0x103;

	compareCPUStates(preExecutionState, postExecutionState);

	// Condition true
	cpu.F.carryFlag = 0;
	cpu.PC = 0x100;

	preExecutionState = dumpCPUState();
	cpu.doCycles(20);
	postExecutionState = dumpCPUState();

	preExecutionState.PC = 0xDEAD;
	preExecutionState.SP = 0xFFFC;

	compareCPUStates(preExecutionState, postExecutionState);
	EXPECT_EQ(memoryMap.getHRAM()[0x7C], 0x03);
	EXPECT_EQ(memoryMap.getHRAM()[0x7D], 0x01);
}

TEST_F(CPUTestsJumps, RST_0x10Test)
{
	dummyROM[0x100] = 0xD7;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(12);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC = 0x10;
	preExecutionState.SP = 0xFFFC;

	compareCPUStates(preExecutionState, postExecutionState);
	EXPECT_EQ(memoryMap.getHRAM()[0x7C], 0x01);
	EXPECT_EQ(memoryMap.getHRAM()[0x7D], 0x01);
}

TEST_F(CPUTestsJumps, RET_CTest)
{
	dummyROM[0x100] = 0xD8;
	cpu.SP = 0xFFFC;
	memoryMap.getHRAM()[0x7C] = 0xAD;
	memoryMap.getHRAM()[0x7D] = 0xDE;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	// Condition false
	cpu.F.carryFlag = 0;

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(4);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC = 0x101;

	compareCPUStates(preExecutionState, postExecutionState);

	// Condition true
	cpu.F.carryFlag = 1;
	cpu.PC = 0x100;

	preExecutionState = dumpCPUState();
	cpu.doCycles(12);
	postExecutionState = dumpCPUState();

	preExecutionState.PC = 0xDEAD;
	preExecutionState.SP = 0xFFFE;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTestsJumps, RETITest)
{
	dummyROM[0x100] = 0xD9;
	cpu.SP = 0xFFFC;
	memoryMap.getHRAM()[0x7C] = 0xAD;
	memoryMap.getHRAM()[0x7D] = 0xDE;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(12);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC = 0xDEAD;
	preExecutionState.SP = 0xFFFE;
	preExecutionState.interruptsMasterEnable = true;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTestsJumps, JP_CTest)
{
	dummyROM[0x100] = 0xDA;
	dummyROM[0x101] = 0xAD;
	dummyROM[0x102] = 0xDE;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	// Condition false
	cpu.F.carryFlag = 0;

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(12);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC = 0x103;

	compareCPUStates(preExecutionState, postExecutionState);

	// Condition true
	cpu.F.carryFlag = 1;
	cpu.PC = 0x100;

	preExecutionState = dumpCPUState();
	cpu.doCycles(12);
	postExecutionState = dumpCPUState();

	preExecutionState.PC = 0xDEAD;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTestsJumps, CALL_CTest)
{
	dummyROM[0x100] = 0xDC;
	dummyROM[0x101] = 0xAD;
	dummyROM[0x102] = 0xDE;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	// Condition false
	cpu.F.carryFlag = 0;

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(12);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC = 0x103;

	compareCPUStates(preExecutionState, postExecutionState);

	// Condition true
	cpu.F.carryFlag = 1;
	cpu.PC = 0x100;

	preExecutionState = dumpCPUState();
	cpu.doCycles(20);
	postExecutionState = dumpCPUState();

	preExecutionState.PC = 0xDEAD;
	preExecutionState.SP = 0xFFFC;

	compareCPUStates(preExecutionState, postExecutionState);
	EXPECT_EQ(memoryMap.getHRAM()[0x7C], 0x03);
	EXPECT_EQ(memoryMap.getHRAM()[0x7D], 0x01);
}

TEST_F(CPUTestsJumps, RST_0x18Test)
{
	dummyROM[0x100] = 0xDF;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(12);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC = 0x18;
	preExecutionState.SP = 0xFFFC;

	compareCPUStates(preExecutionState, postExecutionState);
	EXPECT_EQ(memoryMap.getHRAM()[0x7C], 0x01);
	EXPECT_EQ(memoryMap.getHRAM()[0x7D], 0x01);
}

TEST_F(CPUTestsJumps, RST_0x20Test)
{
	dummyROM[0x100] = 0xE7;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(12);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC = 0x20;
	preExecutionState.SP = 0xFFFC;

	compareCPUStates(preExecutionState, postExecutionState);
	EXPECT_EQ(memoryMap.getHRAM()[0x7C], 0x01);
	EXPECT_EQ(memoryMap.getHRAM()[0x7D], 0x01);
}

TEST_F(CPUTestsJumps, JP_HLTest)
{
	dummyROM[0x100] = 0xE9;
	cpu.HL = 0xDEAD;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(4);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC = 0xDEAD;

	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUTestsJumps, RST_0x28Test)
{
	dummyROM[0x100] = 0xEF;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(12);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC = 0x28;
	preExecutionState.SP = 0xFFFC;

	compareCPUStates(preExecutionState, postExecutionState);
	EXPECT_EQ(memoryMap.getHRAM()[0x7C], 0x01);
	EXPECT_EQ(memoryMap.getHRAM()[0x7D], 0x01);
}

TEST_F(CPUTestsJumps, RST_0x30Test)
{
	dummyROM[0x100] = 0xF7;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(12);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC = 0x30;
	preExecutionState.SP = 0xFFFC;

	compareCPUStates(preExecutionState, postExecutionState);
	EXPECT_EQ(memoryMap.getHRAM()[0x7C], 0x01);
	EXPECT_EQ(memoryMap.getHRAM()[0x7D], 0x01);
}

TEST_F(CPUTestsJumps, RST_0x38Test)
{
	dummyROM[0x100] = 0xFF;
	memoryMap.getCartridge().loadFromMemory(dummyROM, sizeof(dummyROM));

	CPUState preExecutionState = dumpCPUState();
	cpu.doCycles(12);
	CPUState postExecutionState = dumpCPUState();

	preExecutionState.PC = 0x38;
	preExecutionState.SP = 0xFFFC;

	compareCPUStates(preExecutionState, postExecutionState);
	EXPECT_EQ(memoryMap.getHRAM()[0x7C], 0x01);
	EXPECT_EQ(memoryMap.getHRAM()[0x7D], 0x01);
}
