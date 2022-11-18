#include <fstream>

#include "file_IO_handler.hpp"
#include "error_message.hpp"

IOFileHandler::IOFileHandler(const std::string& file)
    : file_{std::move(file)}
{}

void    IOFileHandler::readFile()
{
    std::fstream    fileFromRead;
    fileFromRead.open(file_, std::ios::in);

    if (!fileFromRead.is_open()) {
        throw   ErrorMessage("Could not open file " + file_ + "\n");
    }
}


