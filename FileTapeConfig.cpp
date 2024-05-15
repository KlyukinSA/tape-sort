#include "FileTapeConfig.hpp"

#include <string>
#include <sstream>

static void store_line(FileTapeConfig& res, std::string key, std::string value) {
    // std::istringstream is_val(value);
    int val = std::stoi(value);
    // is_val >> val;
    if (key == "writeDelay") {
        res.writeDelay = val;
    } else if (key == "readDelay") {
        res.readDelay = val;
    } else if (key == "shiftDelay") {
        res.shiftDelay = val;
    } else if (key == "rewindDelay") {
        res.rewindDelay = val;
    }
}

FileTapeConfig readFileTapeConfigFromFile(std::ifstream& file) {
    FileTapeConfig res{0, 0, 0, 0};
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream is_line(line);
        std::string key;
        if (std::getline(is_line, key, '=')) {
            std::string value;
            if (std::getline(is_line, value)) {
                store_line(res, key, value);
            }
        }
    }
    return res;
}
