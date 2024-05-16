#include "../FileTapeConfig.hpp"

#include <iostream>
#include <cassert>
#include <utility>

int main(int argc, char* argv[]) {
    std::ifstream configFile{"config.txt"};
    FileTapeConfig config = readFileTapeConfigFromFile(std::move(configFile));
    assert(config.readDelay == 1);
    std::cout << config.writeDelay << '\n';
    assert(config.writeDelay == 2);
    return 0;
}
