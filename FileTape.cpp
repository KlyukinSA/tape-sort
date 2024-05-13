#include "FileTape.hpp"

#include <iostream>

FileTape::FileTape(std::fstream& fileStream):
    file(fileStream) {}

bool FileTape::read(int& value) {
    std::streampos pos = file.tellg();
    file >> value;
    // std::cout << value << ",\n";
    file.seekg(pos);
    return true;
}

bool FileTape::write(int value) {
    std::streampos pos = file.tellg();
    file << value << ',';
    // std::cout << value << ",\n";
    file.flush();
    file.seekg(pos);
    return true;
}

bool FileTape::rewind() {
    file.seekg(0);
    return true;
}

bool FileTape::shift(int n) {
    int v;
    char c;
    if (n == 0) {
        return true;
    } else if (n > 0) {
        for (int i = 0; i < n; i++) {
            file >> v >> c;
            // std::cout << '\n' << v << c << '\n';
        }
        return true;
    } else {
        std::streampos start = file.tellg();
        std::streampos cur = start;
        while (cur > 0) {
            cur -= 1;
            file >> v >> c;
            if (file.tellg() == start) {
                file.seekg(cur);
                return true;
            }
        }
        return false;
    }
}

bool FileTape::isFinished() {
    return !file;
}
