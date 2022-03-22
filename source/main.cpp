#include "cpu.h"
#include "memory_map.h"

class GameBoy
{
public:
	GameBoy() :
		m_cpu(m_memoryMap)
	{
		m_memoryMap.insertCartridge("assets/test_roms/cpu_instrs.gb");
	}

	void run()
	{
		for (;;) {
			m_cpu.doCycles(100);
		}
	}
private:
	MemoryMap m_memoryMap;
	CPU m_cpu;
};

int main()
{
	GameBoy gameboy;
	gameboy.run();

	return 0;
}
