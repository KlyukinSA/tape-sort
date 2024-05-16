#include "FileTapeFactory.hpp"

FileTapeFactory::FileTapeFactory(const FileTapeConfig& config, const std::string& directory)
    : i(0)
    , config(config)
    , directory(directory)
{}

FileTape FileTapeFactory::create() {
    i++;
    return FileTape{std::fstream{getTempTapeFileName(i), std::ios::in | std::ios::out | std::ios::trunc}, config};
}

std::string FileTapeFactory::getTempTapeFileName(int id) {
    return directory + '/' + std::to_string(id) + ".txt";
}
