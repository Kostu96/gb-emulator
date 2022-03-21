#include "cpu.h"
#include "memory_map.h"

#include <cstdint>
#include <iostream>

class GameBoy
{
public:
	GameBoy() :
		m_cpu(m_memoryMap)
	{

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
