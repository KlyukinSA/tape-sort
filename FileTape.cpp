#include "FileTape.hpp"

#include <iostream>

FileTape::FileTape(std::string fileName, std::ios_base::openmode mode):
    file(std::fstream(fileName, std::ios::in | std::ios::out | mode)) {
    if (!file.is_open()) {
        std::cerr << "failed to open " << fileName << '\n';
    }
}

bool FileTape::read(int& value) {
    std::streampos pos = file.tellg();
    // std::cout << "read" << pos << ' ' << value << !file << ",\n";
    file >> value;
    if (!file) {
        value = 0;
    }
    // std::cout << "read" << pos << ' ' << value << !file << ",\n";
    file.seekg(pos);
    return true;
}

bool FileTape::write(int value) {
    file.clear();
    std::streampos pos = file.tellg();
    file << value << ',';
    // std::cout << "write" << value << ",\n";
    file.flush();
    file.seekg(pos);
    return true;
}

bool FileTape::rewind() {
    file.clear();
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
        file.clear();
        std::streampos start = file.tellg();
        for (int i = 0; i < n; i++) {
            std::streampos cur = start;
            while (cur > 0) {
                cur -= 1;
                file.seekg(cur);
                file >> v >> c;
                if (file.tellg() == start) {
                    start = cur;
                    if (i == n - 1) {
                        return true;
                    }
                    break;
                }
            }
        }
        return false;
    }
}

bool FileTape::isFinished() {
    return !file;
}
