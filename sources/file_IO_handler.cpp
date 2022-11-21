#include <fstream>

#include "file_IO_handler.hpp"
#include "error_message.hpp"

IOFileHandler::IOFileHandler(const std::string& file)
    : file_{std::move(file)}
{}

void    IOFileHandler::readFile()
{
    std::fstream    inData;
    std::string     oneLineData;
    inData.open(file_, std::ios::in);

    if (!inData.is_open()) {
        throw   ErrorMessage("Could not open file " + file_ + "\n");
    }
    std::string line;
    while (std::getline(inData, oneLineData))
    {
        line = skipSingleLineComments(oneLineData);
        if (line.empty()) {
            continue;
        }
        input_ += line;
        input_ += "\n";
    }
    input_ += "\n";
    inData.close();
}

std::string IOFileHandler::skipSingleLineComments(std::string line)
{
    auto  commentIndex = line.find("//");
    if (commentIndex == std::string::npos) {
        return line;
    }
    return line.substr(0, commentIndex);
}

void    IOFileHandler::skipMultiLineComments()
{
    while (true)
    {
        auto    commIdxStart = input_.find("/*");
        if (commIdxStart == std::string::npos) {
            break;
        }
        auto    commIdxEnd = input_.find("*/");
        if (commIdxEnd == std::string::npos) {
               throw ErrorMessage("Start of Multi Line Comment appeared withoud its appropriate end\n");
        }
        std::string temp = input_.substr(0, commIdxStart);
        temp += input_.substr(commIdxEnd + 2, input_.size() - commIdxEnd);
        input_ = temp;
    }
}

void    IOFileHandler::dataHandler()
{
    readFile();
    skipMultiLineComments();
}

std::string&    IOFileHandler::getHandledInput()
{
    dataHandler();
    return input_;
}
