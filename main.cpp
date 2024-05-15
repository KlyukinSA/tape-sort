#include "ExternalSorter.hpp"
#include "FileTape.hpp"

#include <iostream>
#include <utility>

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "usage: ./prog input.txt output.txt\n";
        return 1;
    }

    FileTapeConfig config = readFileTapeConfigFromFile(std::ifstream{"test/config.txt"});

    std::fstream inputFile{argv[1]};
    if (!inputFile.is_open()) {
        std::cerr << "failed to open " << argv[1] << '\n';
        return 2;
    }
    FileTape inputTape{std::move(inputFile), config};
    // inputTape.read(i);
    // std::cout << i << ' ';
    // inputTape.shift(1);
    // inputTape.read(i);
    // std::cout << i << '\n';
    // inputTape.rewind();
    // inputTape.read(i);
    // std::cout << i << '\n';

    {
        std::fstream outputFile(argv[2], std::ios::out); // create file
    }
    FileTape outputTape{std::fstream{std::string(argv[2])}, config};
    // outputTape.write(3);
    // outputTape.shift(1);
    // outputTape.write(4);

    int availableChunkSize = 7;
    ExternalSorter sorter{availableChunkSize};
    int groupSize = 3;
    sorter.sort(inputTape, outputTape, groupSize, config);

    return 0;
}
