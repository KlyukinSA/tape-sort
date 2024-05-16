#include "ExternalSorter.hpp"
#include "FileTape.hpp"
#include "FileTapeFactory.hpp"

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

    {
        std::fstream outputFile(argv[2], std::ios::out); // create file
    }
    FileTape outputTape{std::fstream{std::string(argv[2])}, config};

    int availableChunkSize = 7;
    ExternalSorter<FileTape> sorter{availableChunkSize};

    int groupSize = 3;
    FileTapeFactory factory{config, "tmp"};
    sorter.sort(inputTape, outputTape, groupSize, factory);

    return 0;
}
