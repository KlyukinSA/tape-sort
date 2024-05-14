#include "ExternalSorter.hpp"
#include "FileTape.hpp"

#include <iostream>

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "usage: ./prog input.txt output.txt\n";
        return 1;
    }

    // std::fstream inputFile;
    // inputFile.open(argv[1]);
    {
        std::fstream outputFile(argv[2]);
        if (!outputFile.is_open()) {
            outputFile.clear();
            outputFile.open(argv[2], std::ios::out); // create file
        }
    }
    // int i;
    // inputFile >> i;
    // outputFile << i << '\n';
    // char c;
    // inputFile >> c;
    // outputFile << c << '\n';
    // inputFile >> i;
    // outputFile << i << '\n';

    FileTape inputTape{std::string(argv[1])};
    // inputTape.read(i);
    // std::cout << i << ' ';
    // inputTape.shift(1);
    // inputTape.read(i);
    // std::cout << i << '\n';
    // inputTape.rewind();
    // inputTape.read(i);
    // std::cout << i << '\n';

    FileTape outputTape{std::string(argv[2])};
    // outputTape.write(3);
    // outputTape.shift(1);
    // outputTape.write(4);

    int availableChunkSize = 7;
    ExternalSorter sorter(availableChunkSize);
    int groupSize = availableChunkSize / 2;
    sorter.sort(inputTape, outputTape, groupSize);

    return 0;
}
