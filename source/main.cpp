// TODO: debug build with checks and realease build without
// TODO: unit testing

#include "cpu.h"
#include "memory_map.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/System/Sleep.hpp>
#include <thread>

constexpr uint8_t SCALE = 4;
static uint32_t palette[4] = { 0xFFFFFFFF, 0xAAAAAAFF, 0x555555FF, 0x000000FF }; // TODO: temp

class GameBoy
{
public:
	GameBoy() :
		m_window{
			sf::VideoMode{ (SCALE * 160) + (2 * SCALE) + ((15 + 16 * 8) * SCALE),
			               std::max((SCALE * 144), ((23 + 24 * 8) * SCALE)) },
			"GameBoy emulator",
			sf::Style::Close
		},
		m_cpu { m_memoryMap }
	{
		m_window.setVerticalSyncEnabled(true);

		m_memoryMap.insertCartridge("third_party/tests/gb-test-roms/cpu_instrs/cpu_instrs.gb");
		//m_memoryMap.insertCartridge("third_party/tests/gb-test-roms/instr_timing/instr_timing.gb");
		//m_memoryMap.insertCartridge("assets/test_roms/dmg-acid2.gb");
		//m_memoryMap.insertCartridge("E:/Retro/GameBoy/tetris.gb");
	}

	void run()
	{
		std::thread cpuThread{
			[&]() {
				while (true) {
					std::this_thread::sleep_for(std::chrono::nanoseconds{ 128 }); // TODO: temp
					m_cpu.doCycles(16);
				}
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

			// draw debug screen area:
			sf::RectangleShape rect{ { 160 * SCALE, 144 * SCALE } };
			rect.setFillColor(sf::Color::Green);
			m_window.draw(rect);

			uint32_t xDraw = (SCALE * 160) + (2 * SCALE);
			uint32_t yDraw = 0;
			uint16_t address = 0x8000;
			for (int y = 0; y < 24; y++) {
				for (int x = 0; x < 16; x++) {
					drawTile(address, xDraw, yDraw);
					xDraw += (9 * SCALE);
					address += 16;
				}

				yDraw += (9 * SCALE);
				xDraw = (SCALE * 160) + (2 * SCALE);
			}

			m_window.display();
		}

		cpuThread.join();
	}
private:
	void drawTile(uint16_t address, uint32_t x, uint32_t y) {
		sf::RectangleShape rect;
		for (uint8_t tileY = 0; tileY < 16; tileY += 2) {
			uint8_t b1 = m_memoryMap.load8(address + tileY);
			uint8_t b2 = m_memoryMap.load8(address + tileY + 1);
			for (int8_t bit = 7; bit >= 0; --bit) {
				uint8_t color = !!(b1 & (1 << bit)) << 1;
				color |= !!(b2 & (1 << bit));

				rect.setPosition( x + ((7 - bit) * SCALE), y + (tileY / 2 * SCALE));
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
