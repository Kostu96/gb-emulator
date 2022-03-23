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
    if (argc < 4)
        return -1;

    std::ifstream bootloaderFile{ argv[1], std::ios::binary };
    if (!bootloaderFile.is_open())
        return -1;

    bootloaderFile.ignore(std::numeric_limits<std::streamsize>::max());
    size_t bootloadeSize = bootloaderFile.gcount();
    if (bootloadeSize != 256)
        return -1;
    bootloaderFile.clear();
    bootloaderFile.seekg(std::ios::beg);

    fs::path patchesDir = argv[3];
    std::unordered_map<uint16_t, std::vector<uint8_t>> patches;
    for (auto& dirEntry : fs::directory_iterator{ patchesDir })
        if (dirEntry.is_regular_file())
        {
            std::cout << "Found patch file: " << dirEntry.path() << '\n';

            std::ifstream patchFile{ dirEntry.path() };

            if (!patchFile.is_open())
                return -1;

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
        }

    std::ofstream outputFile{ argv[2] };
    if (!outputFile.is_open())
        return -1;

    outputFile << "static const uint8_t internalROM[256] = {";

    outputFile << std::hex << std::setfill('0');
    uint8_t byte;
    for (uint16_t i = 0; i < 256; ++i)
    {
        if (patches.contains(i)) {
            auto& patch = patches[i];
            for (size_t x = 0; x < patch.size(); ++x) {
                if (i % 16 == 0) outputFile << "\n\t";
                outputFile << "0x" << std::setw(2) << (uint16_t)patch[x];
                if (i != 255) outputFile << ", ";
                if (x < patch.size() -1) ++i;
                bootloaderFile.ignore();
            }
        }
        else {
            bootloaderFile.read((char*)&byte, 1);
            if (i % 16 == 0) outputFile << "\n\t";
            outputFile << "0x" << std::setw(2) << (uint16_t)byte;
            if (i != 255) outputFile << ", ";
        }
    }

    outputFile << "\n};\n";

    return 0;
}
