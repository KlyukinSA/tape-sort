#include "ExternalSorter.hpp"
#include "FileTape.hpp"

#include <iostream>

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "usage: ./prog input.txt output.txt\n";
        return 1;
    }

    const char* configFileName = "test/config.txt";
    std::ifstream configFile{configFileName};
    if (!configFile.is_open()) {
        std::cerr << "cant open " << configFileName << '\n';
    }
    FileTapeConfig config = readFileTapeConfigFromFile(configFile);

    {
        std::fstream outputFile(argv[2], std::ios::out); // create file
    }
    FileTape inputTape{std::string(argv[1]), config};
    // inputTape.read(i);
    // std::cout << i << ' ';
    // inputTape.shift(1);
    // inputTape.read(i);
    // std::cout << i << '\n';
    // inputTape.rewind();
    // inputTape.read(i);
    // std::cout << i << '\n';

    FileTape outputTape{std::string(argv[2]), config};
    // outputTape.write(3);
    // outputTape.shift(1);
    // outputTape.write(4);

    int availableChunkSize = 7;
    ExternalSorter sorter{availableChunkSize};
    int groupSize = 3;
    sorter.sort(inputTape, outputTape, groupSize, config);

    return 0;
}
