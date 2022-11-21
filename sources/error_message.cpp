#include <iostream>

#include "error_message.hpp"

/*
 * Constructor with parameters which gets the 
 * error message.
 */
ErrorMessage::ErrorMessage(const std::string& src)
    : errorMessage_{src}
{}

/*
 * Member function which prints the error message.
 */
void    ErrorMessage::printErrorMessage() const
{
    std::cerr << "ERROR: " << errorMessage_;
}

/*
 * Member Function -> getter which returns the error
 * message.
 */
const std::string&    ErrorMessage::getErrorMessage() const
{
    return errorMessage_;
}
