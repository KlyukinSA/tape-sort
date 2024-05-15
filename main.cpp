#include "ExternalSorter.hpp"
#include "FileTape.hpp"

#include <iostream>

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "usage: ./prog input.txt output.txt\n";
        return 1;
    }
    {
        std::fstream outputFile(argv[2], std::ios::out); // create file
    }
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

    int availableChunkSize = 5;
    ExternalSorter sorter(availableChunkSize);
    // int groupSize = availableChunkSize / 2;
    int groupSize = 15;
    sorter.sort(inputTape, outputTape, groupSize);

    return 0;
}
