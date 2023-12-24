#include "input_file.h"
#include <fstream>
input_file::input_file(const std::string& filename): filename(filename){}
std::vector<char> input_file::readAll() {
    std::ifstream file(filename);
    if(!file.is_open()){
        throw std::runtime_error("Unable to open file");
    }
    return std::vector<char>((std::istreambuf_iterator<char>(file)),
                             std::istreambuf_iterator<char>());
}
