
#include <cpp-common/helper_functions.h>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>

namespace fs = std::filesystem;

int main(int argc, char* argv[])
{
    constexpr size_t BOOTLOADER_SIZE = 256;

    if (argc < 4) {
        std::cerr << "Failed!\n";
        return -1;
    }

    uint8_t bootloaderData[BOOTLOADER_SIZE];
    size_t bootloaderSize = BOOTLOADER_SIZE;
    if (!readFile(argv[1], (char*)bootloaderData, bootloaderSize, true)) {
        std::cerr << "Failed!\n";
        return -1;
    }
    if (bootloaderSize != BOOTLOADER_SIZE) {
        std::cerr << "Failed!\n";
        return -1;
    }
    std::cout << "Loaded bootloader file: " << argv[1] << '\n';

    fs::path patchesDir = argv[3];
    std::unordered_map<uint16_t, std::vector<uint8_t>> patches;
    for (auto& dirEntry : fs::directory_iterator{ patchesDir })
        if (dirEntry.is_regular_file())
        {
            std::cout << "Found patch file: " << dirEntry.path() << "\nLoading... ";

            std::ifstream patchFile{ dirEntry.path() };

            if (!patchFile.is_open()) {
                std::cerr << "Failed!\n";
                return -1;
            }

            std::string line;
            uint16_t address;
            while (std::getline(patchFile, line)) {
                std::stringstream ss(line);
                std::vector<uint8_t> data;
                ss << std::hex;
                ss >> address;
                uint16_t byte;
                while (ss >> byte)
                    data.push_back(byte & 0xFF);

                patches.insert({ address, std::move(data) });
            }

            std::cout << "Success.\n";
        }

    std::cout << "Applying patches... ";
    for (const auto& patch : patches) {
        uint16_t address = patch.first;
        for (auto byte : patch.second) {
            if (address >= BOOTLOADER_SIZE) {
                std::cerr << "Failed!\n";
                return -1;
            }

            bootloaderData[address++] = byte;
        }
    }
    std::cout << "Done.\n";

    std::cout << "Generatgin output file... ";

    std::ofstream outputFile{ argv[2] };
    if (!outputFile.is_open()) {
        std::cerr << "Failed!\n";
        return -1;
    }

    outputFile << "static const uint8_t internalROM[256] = {";
    outputFile << std::hex << std::setfill('0');

    for (uint16_t i = 0; i < BOOTLOADER_SIZE; ++i) {
        if (i % 16 == 0) outputFile << "\n\t";
        outputFile << "0x" << std::setw(2) << (uint16_t)bootloaderData[i];
        if (i != 255) outputFile << ", ";
    }

    outputFile << "\n};\n";

    std::cout << "Done.\n";

    return 0;
}
