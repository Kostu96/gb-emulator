#include "helper_functions.h"

#include <fstream>

bool readFile(const char* filename, char* data, size_t& size, bool binary)
{
	std::ifstream fin(filename, binary ? std::ios::binary : 1);
    if (!fin.is_open())
        return false;

    if (data) {
        return (bool)fin.read(data, size);
    }
    else {
        fin.ignore(std::numeric_limits<std::streamsize>::max());
        size = fin.gcount();
        fin.close();
        return true;
    }
}
