#include "cpu.h"
#include "memory_map.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/System/Sleep.hpp>
#include <thread>

class GameBoy
{
public:
	GameBoy() :
		m_window{ sf::VideoMode{ 2 * 160, 2 * 144 }, "GameBoy emulator", sf::Style::Close },
		m_cpu { m_memoryMap }
	{
		m_window.setFramerateLimit(60);

		m_memoryMap.insertCartridge("assets/test_roms/cpu_instrs.gb");
	}

	void run()
	{
		std::thread cpuThread{
			[&]() {
				std::this_thread::sleep_for(std::chrono::milliseconds{ 500 }); // TODO: temp to wait until window initializes
				for (;;)
					m_cpu.doCycles(5000);
			} 
		};

		while (m_window.isOpen()) {
			sf::Event event;
			while (m_window.pollEvent(event)) {
				switch (event.type) {
				case sf::Event::Closed:
					m_window.close();
				}
			}

			m_window.clear(sf::Color::Magenta);

			m_window.display();
		}

		cpuThread.join();
	}
private:
	sf::RenderWindow m_window;

	MemoryMap m_memoryMap;
	CPU m_cpu;
};

int main()
{
	GameBoy gameboy;
	gameboy.run();

	return 0;
}
