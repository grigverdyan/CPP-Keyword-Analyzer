#include "helpers.hpp"

#include <vector>

bool    isValidExtension(const std::string& file)
{
    if (file.length() < 5)
        return false;
    std::vector<std::string> cppExtensions{
        ".cpp", ".c++", ".cxx"
    };
    for (auto it = cppExtensions.begin(); it != cppExtensions.end(); ++it)
    {
        if (file.compare(file.length() - it->length(), it->length(), *it) == 0) {
            return true;
        }
    }
    return false;
}
