#ifndef ERROR_MESSAGE_H
#define ERROR_MESSAGE_H

#include <string>

class ErrorMessage
{
    public:
        ErrorMessage() = delete;
        ErrorMessage(const std::string& message);
        ErrorMessage(const std::string& message, size_t line);
        ErrorMessage(const ErrorMessage& src) = delete;
        ErrorMessage& operator=(const ErrorMessage& rhs) = delete;
        ~ErrorMessage() = default;

    public:
        void                printErrorMessage() const;
        const std::string&  getErrorMessage() const;
        size_t              getErrorLine() const;

    private:
        std::string     errorMessage_;
        size_t          errorLine_;
};

#endif  // ERROR_MESSAHE_H
