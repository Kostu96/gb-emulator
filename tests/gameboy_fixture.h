#pragma once
#include "cpu.h"
#include "memory_map.h"

#include <gtest/gtest.h>

struct GameBoyFixture :
	public testing::Test
{
	GameBoyFixture() {
		cpu.connect(memoryMap);
		memoryMap.connect(cpu);
	}

	void SetUp() override {
		cpu.reset();
	}

	CPU cpu;
	MemoryMap memoryMap;
};
