#include <iostream>
#include <memory>

#include "helpers.hpp"
#include "error_message.hpp"
#include "file_IO_handler.hpp"

int main(int argc, char* argv[])
{
    try {
        if (argc < 2) {
            throw ErrorMessage("No input file included!\n");
        }
        if (isValidExtension(argv[1]) == false) {
            throw ErrorMessage("Included file must have a valid CPP file extension!\n");
        }
    } catch(const ErrorMessage& error) {
        error.printErrorMessage();
        return EXIT_FAILURE;
    }
    std::string fileName = argv[1];

    auto    file = std::make_shared<IOFileHandler>("data/" + fileName);
    
    return EXIT_SUCCESS;
}
