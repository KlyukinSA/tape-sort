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

    int N, M;
    if (argc > 4) {
        N = std::atoi(argv[3]);
        M = std::atoi(argv[4]);
    } else {
        N = 3 * 7 * 2;
        M = 7 * sizeof(int);
    }
    int availableChunkSize = M / sizeof(int);
    ExternalSorter<FileTape> sorter{availableChunkSize};

    int chunksAmount = N / availableChunkSize;
    if (chunksAmount < 4) {
        chunksAmount = 4;
    }
    FileTapeFactory factory{config, "tmp"};
    sorter.sort(inputTape, outputTape, chunksAmount / 2, factory);

    return 0;
}
