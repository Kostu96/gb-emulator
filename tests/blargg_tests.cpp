#include "gameboy_fixture.h"

struct BlarggTests :
	public GameBoyFixture,
	public testing::WithParamInterface<const char*>
{
	void SetUp() override {
		GameBoyFixture::SetUp();
	}
};

TEST_P(BlarggTests, BlarggTest) {
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

INSTANTIATE_TEST_SUITE_P(Parametrized, BlarggTests,
	testing::Values(
		"third_party/tests/gb-test-roms/cpu_instrs/individual/01-special.gb",
		"third_party/tests/gb-test-roms/cpu_instrs/individual/02-interrupts.gb"/*,
		"third_party/tests/gb-test-roms/cpu_instrs/individual/03-op sp,hl.gb",
		"third_party/tests/gb-test-roms/cpu_instrs/individual/04-op r,imm.gb",
		"third_party/tests/gb-test-roms/cpu_instrs/individual/05-op rp.gb",
		"third_party/tests/gb-test-roms/cpu_instrs/individual/06-ld r,r.gb",
		"third_party/tests/gb-test-roms/cpu_instrs/individual/07-jr,jp,call,ret,rst.gb",
		"third_party/tests/gb-test-roms/cpu_instrs/individual/08-misc instrs.gb",
		"third_party/tests/gb-test-roms/cpu_instrs/individual/09-op r,r.gb",
		"third_party/tests/gb-test-roms/cpu_instrs/individual/10-bit ops.gb",
		"third_party/tests/gb-test-roms/cpu_instrs/individual/11-op a,(hl).gb"*/
	)
);
