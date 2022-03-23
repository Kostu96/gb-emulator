#include "cpu.h"
#include "memory_map.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/System/Sleep.hpp>
#include <thread>

constexpr uint8_t SCALE = 8;
static uint32_t palette[4] = { 0xFFFFFFFF, 0xAAAAAAFF, 0x555555FF, 0x000000FF }; // TODO: temp

class GameBoy
{
public:
	GameBoy() :
		m_window{ sf::VideoMode{ SCALE * 160, SCALE * 144 }, "GameBoy emulator", sf::Style::Close },
		m_cpu { m_memoryMap }
	{
		m_window.setFramerateLimit(60);

		m_memoryMap.insertCartridge("third_party/tests/gb-test-roms/cpu_instrs/cpu_instrs.gb");
	}

	void run()
	{
		std::thread cpuThread{
			[&]() {
				std::this_thread::sleep_for(std::chrono::milliseconds{ 250 }); // TODO: temp to wait until window initializes
				for (;;)
					m_cpu.doCycles(4);
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

			uint8_t xDraw = 0;
			uint8_t yDraw = 0;
			uint16_t address = 0x8000;
			for (int y = 0; y < 24; y++) {
				for (int x = 0; x < 16; x++) {
					drawTile(address, xDraw + (x * SCALE), yDraw + (y * SCALE));
					xDraw += (8 * SCALE);
					address += 16;
				}

				yDraw += (8 * SCALE);
				xDraw = 0;
			}

			m_window.display();
		}

		cpuThread.join();
	}
private:
	void drawTile(uint16_t address, uint8_t x, uint8_t y) {
		sf::RectangleShape rect;
		for (uint8_t tileY = 0; tileY < 16; tileY += 2) {
			uint8_t b1 = m_memoryMap.load8(address + tileY);
			uint8_t b2 = m_memoryMap.load8(address + tileY + 1);
			for (int8_t bit = 7; bit >= 0; --bit) {
				uint8_t color = !!(b1 & (1 << bit)) << 1;
				color |= !!(b2 & (1 << bit));

				rect.setPosition( x + (7 - bit) * SCALE, y + (tileY / 2) * SCALE);
				rect.setSize({ SCALE, SCALE });
				rect.setFillColor(sf::Color{ palette[color] });

				m_window.draw(rect);
			}
		}
	}

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
