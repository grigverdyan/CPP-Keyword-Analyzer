#include <fstream>

#include "file_IO_handler.hpp"
#include "error_message.hpp"


/*
 * Constructor with parameters.
 * Gets name of file
 */
IOFileHandler::IOFileHandler(const std::string& file)
    : file_{std::move(file)}
{}

/*
 * Member function which read open file,
 * reads it, and fill the data in into the
 * input_ member variable.
 */
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

/*
 * Member function which skips single line comments of file data
 * and do not put it into the input_ member variable. 
 */
std::string IOFileHandler::skipSingleLineComments(std::string line)
{
    auto  commentIndex = line.find("//");
    if (commentIndex == std::string::npos) {
        return line;
    }
    return line.substr(0, commentIndex);
}

/*
 * Member function which skips mutiple line comments of file
 * data and do not put these into the input_ member variable.
 */
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

/*
 * Member function which can be characterized as main.
 * Does reading file, with skipping comments.
 */
void    IOFileHandler::dataHandler()
{
    readFile();
    skipMultiLineComments();
}

/*
 * Member function which calls the main Member function
 * and returns the input_ member variable.
 */
std::string&    IOFileHandler::getHandledInput()
{
    dataHandler();
    return input_;
}
