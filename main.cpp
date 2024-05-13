#include "ExternalSorter.hpp"
#include "FileTape.hpp"

#include <iostream>

int main(int argc, char* argv[]) {
    if (argc < 3) {
        // Вывести сообщение об использовании
        return 1;
    }

    std::fstream inputFile;
    inputFile.open(argv[1], std::ios::in);
    std::fstream outputFile;
    outputFile.open(argv[2]);
    if (!outputFile.is_open())
    {
        outputFile.clear();
        outputFile.open(argv[2], std::ios::out); // create file
        outputFile.close();
        outputFile.open(argv[2]);
    }
    // int i;
    // inputFile >> i;
    // outputFile << i << '\n';
    // char c;
    // inputFile >> c;
    // outputFile << c << '\n';
    // inputFile >> i;
    // outputFile << i << '\n';

    FileTape inputTape(inputFile);
    // inputTape.read(i);
    // std::cout << i << ' ';
    // inputTape.shift(1);
    // inputTape.read(i);
    // std::cout << i << '\n';
    // inputTape.rewind();
    // inputTape.read(i);
    // std::cout << i << '\n';

    FileTape outputTape(outputFile);
    // outputTape.write(3);
    // outputTape.shift(1);
    // outputTape.write(4);

    ExternalSorter sorter;
    sorter.sort(inputTape, outputTape, 1000, 1000000); // Пример значений chunkSize и memoryLimit

    inputFile.close();
    outputFile.close();

    return 0;
}
