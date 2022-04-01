#include "gameboy_fixture.h"

#include <sstream>

using BlarggTests = GameBoyFixture;

TEST_F(BlarggTests, InterruptesTest) {
	memoryMap.insertCartridge("third_party/tests/gb-test-roms/cpu_instrs/individual/02-interrupts.gb");

	std::stringstream ss;
	size_t cycles = 134217728;
	while (cycles--) {
		cpu.doCycles(4);

		if (memoryMap.load8(0xFF02) == 0x81) {
			uint8_t data = memoryMap.load8(0xFF01);
			if (data) ss << data;
		}
	}

	std::string result = ss.str();
}
