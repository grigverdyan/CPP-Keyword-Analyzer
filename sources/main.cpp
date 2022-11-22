#include <iostream>
#include <memory>
#include <sstream>

#include "helpers.hpp"
#include "error_message.hpp"
#include "file_IO_handler.hpp"
#include "analyzer.hpp"

int main(int argc, char* argv[]) try
{
    if (argc < 2) {
        throw ErrorMessage("No input file included!\n");
    }
    if (isValidExtension(argv[1]) == false) {
        throw ErrorMessage("Included file must have a valid CPP file extension!\n");
    }
    
    std::string fileName = argv[1];

    auto    file = std::make_shared<IOFileHandler>("data/" + fileName);
    auto    analysis = std::make_shared<Analyzer>(file->getHandledInput(), "data/analysis.txt");    
    analysis->startAnalysis();
    
    return EXIT_SUCCESS;

} catch (const ErrorMessage& error) {
    error.printErrorMessage();
    return EXIT_FAILURE;
}
