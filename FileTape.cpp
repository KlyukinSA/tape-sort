#include "FileTape.hpp"

#include <chrono>
#include <thread>
#include <utility>

FileTape::FileTape(std::fstream&& file, const FileTapeConfig& config)
    : file(std::move(file))
    , config(config)
{}

bool FileTape::step(int& v) {
    char c;
    while (true) {
        file >> v;
        if (!file) {
            file.clear();
            file >> c;
            if (!file) {
                v = 0;
                return false;
            }
        } else {
            break;
        }
    }
    std::streampos pos = file.tellg();
    file >> c;
    if (std::isdigit(c)) {
        file.seekg(pos);
    }
    file.clear();
    return true;
}

bool FileTape::read(int& value) {
    std::this_thread::sleep_for(std::chrono::microseconds(config.readDelay));
    std::streampos pos = file.tellg();
    step(value);
    file.seekg(pos);
    return true;
}

bool FileTape::write(int value) {
    std::this_thread::sleep_for(std::chrono::microseconds(config.writeDelay));
    file.clear();
    std::streampos pos = file.tellg();
    file << value << ',';
    file.flush();
    file.seekg(pos);
    return true;
}

bool FileTape::rewind() {
    std::this_thread::sleep_for(std::chrono::microseconds(config.rewindDelay));
    file.clear();
    file.seekg(0);
    return true;
}

bool FileTape::shift(int n) {
    int v;
    std::this_thread::sleep_for(std::chrono::microseconds(config.shiftDelay * n));
    if (n == 0) {
        return true;
    } else if (n > 0) {
        for (int i = 0; i < n; i++) {
            step(v);
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
                step(v);
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
