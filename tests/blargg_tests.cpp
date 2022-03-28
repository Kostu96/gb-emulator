#include "cpu.h"
#include "memory_map.h"

#include <gtest/gtest.h>

struct GameBoyFixture :
	public testing::Test
{
	void SetUp() override {
		pCPU = new CPU{ *pMemoryMap };
		pMemoryMap = new MemoryMap{ *pCPU };
	}

	CPU* pCPU = nullptr;
	MemoryMap* pMemoryMap = nullptr;
};

TEST_F(GameBoyFixture, BlarggTest) {

}
