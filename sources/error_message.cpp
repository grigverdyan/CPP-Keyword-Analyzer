#include <iostream>

#include "error_message.hpp"



ErrorMessage::ErrorMessage(const std::string& src)
    : errorMessage_{src}, errorLine_{0}
{}

ErrorMessage::ErrorMessage(const std::string& src, size_t line)
    : errorMessage_{src}, errorLine_{line}
{}

void    ErrorMessage::printErrorMessage() const
{
    if (errorLine_ != 0) {
        std::cerr << "ERROR at " << std::to_string(errorLine_)
                  << "line: " << errorMessage_;
    } else {
        std::cerr << "ERROR: " << errorMessage_;
    }
}

const std::string&    ErrorMessage::getErrorMessage() const
{
    return errorMessage_;
}

size_t  ErrorMessage::getErrorLine() const
{
    return errorLine_;
}
