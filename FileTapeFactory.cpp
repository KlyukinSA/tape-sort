#include "FileTapeFactory.hpp"

#include <stdexcept>
#include <utility>

FileTapeFactory::FileTapeFactory(const FileTapeConfig& config, const std::string& directory)
    : i(0)
    , config(config)
    , directory(directory)
{}

FileTape FileTapeFactory::create() {
    i++;
    std::fstream file{getTempTapeFileName(i), std::ios::in | std::ios::out | std::ios::trunc};
    if (!file.is_open()) {
        throw std::runtime_error{std::string{"cant open "} + getTempTapeFileName(i) + ". check directory existance"};
    }
    return FileTape{std::move(file), config};
}

std::string FileTapeFactory::getTempTapeFileName(int id) {
    return directory + '/' + std::to_string(id) + ".txt";
}
