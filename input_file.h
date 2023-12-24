#ifndef INPUT_FILE_H
#define INPUT_FILE_H

#include <string>
#include <vector>
class input_file
{
public:
    explicit input_file(const std::string& filename);
       std::vector<char> readAll();
private:
    std::string filename;
};

#endif // INPUT_FILE_H
