#include "gameboy_fixture.h"

struct MooneyeTests :
	public GameBoyFixture,
	public testing::WithParamInterface<const char*>
{
	void SetUp() override {
		GameBoyFixture::SetUp();
	}
};

TEST_P(MooneyeTests, MooneyeTest) {
	GTEST_SKIP(); // temp
	memoryMap.getCartridge().loadFromFile(GetParam());

	size_t cycles = 1000000;
	while (cycles--) {
		cpu.doCycles(4);
	}

	std::string_view str{ memoryMap.getIO().getBuffer() };
	std::cout << str;
	bool failed = str.find("Failed") != str.npos;
	EXPECT_FALSE(failed);
}

INSTANTIATE_TEST_SUITE_P(Parametrized, MooneyeTests,
	testing::Values(
		"third_party/tests/mooneye-test-suite/acceptance/bits/reg_f.gb"
	)
);
