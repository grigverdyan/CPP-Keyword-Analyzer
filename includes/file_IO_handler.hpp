#ifndef IO_FILE_HANDLER_H
#define IO_FILE_HANDLER_H

#include <string>

class IOFileHandler
{
    public:
        IOFileHandler() = delete;
        IOFileHandler(const std::string& inFile);
        IOFileHandler(const IOFileHandler& src) = delete;
        IOFileHandler& operator=(const IOFileHandler& rhs) = delete;
        ~IOFileHandler() = default;

    public:
        std::string&    getHandledInput();

    private:
        std::string     skipSingleLineComments(std::string line);
        void            skipMultiLineComments();
        void            readFile();
        void            dataHandler(); 
    private:
        const std::string           file_;
        std::string                 input_;
};

#endif  // IO_FILE_HANDLER_H
