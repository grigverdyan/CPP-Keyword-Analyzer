#include <iostream>
#include <sstream>
#include <fstream>

#include "analyzer.hpp"
#include "error_message.hpp"

std::vector<std::string>  KEYWORDS = {
    "alignas", "alignof", "and", "and_eq", "asm",
    "atomic_cancel", "atomic_commit", "atomic_noexcept",
    "auto", "bitand", "bitor", "bool", "break", "case",
    "catch", "char", "char8_t", "char16_t", "char32_t",
    "class", "compl", "concept", "const", "consteval",
    "constexpr", "constinit", "const_cast", "continue",
    "co_await", "co_return", "co_yield", "decltype",
    "default", "delete", "do", "double", "dynamic_cast",
    "else", "enum", "explicit", "export", "extern", "false",
    "float", "for", "friend", "goto", "if", "inline", "int",
    "long" "mutable", "namespace", "new", "noexcept", "not",
    "not_eq", "nullptr", "operator", "or", "or_eq", "private",
    "protected", "public", "reflexpr", "register", "reinterpret_cast", 
    "requires", "return", "short", "signed", "sizeof", "static",
    "static_cast", "static_assert", "struct", "switch", "synchronized",
    "template", "this", "thread_local", "throw", "true", "try",
    "typedef", "typeid", "typename", "union", "unsigned", "using",
    "virtual", "void", "volatile", "wchar_t", "while", "xor", "xor_eq"
};

/*
 * Constructor with parameters.
 * Gets the data wich sould be analized
 * in the string format and a file name in which
 * should be showed analysis results.
 */
Analyzer::Analyzer(std::string& input, const std::string& outputFile)
    : input_{input}, outputFile_{outputFile}
{}


/*
 * Member function which analises the user 
 * defined types(class, struct, enum class).
 */
void    Analyzer::findUserDefinedTypes(const std::string& udType)
{
    while (true)
    {
        auto classStartIndex = input_.find(udType);
        if (classStartIndex == std::string::npos) {
            break;
        }
        auto classEndIndex = input_.find("};");
        if (classEndIndex == std::string::npos) {
            throw ErrorMessage("Can not find the appropriate ending of user defined type!\n");
        }

        std::string classBody = input_.substr(classStartIndex, classEndIndex - classStartIndex + 2);
        std::string temp = input_.substr(0, classStartIndex);
        temp += input_.substr(classEndIndex + 2 , input_.size() - (classEndIndex + 2));
        input_ = temp;
        addUserDefinedType(classBody, udType);
    }
}

/*
 * Member function which adds found user defined type
 * into the vector of types and does the user defined 
 * type analysis.
 */
void    Analyzer::addUserDefinedType(std::string& body, const std::string& udType)
{
    std::stringstream   sBody(body);
    std::string         word;
    sBody >> word;
    sBody >> word;
    if (isKeyword(word)) {
        throw ErrorMessage("User defined type can not have name of keyword!\n");
    }
    identifierTypes_.push_back(word);
    std::string typeName = word;
    outputStream_ << "In " << udType << " " << typeName << ":\n";
    sBody.clear();
    
    auto typeStart = body.find("{");
    auto typeEnd = body.find("};");
    if (typeStart == std::string::npos || typeEnd == std::string::npos) {
        throw ErrorMessage("Wrong input file!\n");
    }
    std::string tBody = body.substr(typeStart + 1, typeEnd - typeStart - 1);
    body = tBody;
    size_t  propertyCount = 0;
    while (true)
    {
        auto functionStart = body.find("(");
        if (functionStart != std::string::npos) {
            functionAnalysis(body);
            if (body.empty()) {
                break;
            }

        } else {
            break;
        } 
        /*else {
            std::stringstream   temp(body);
            while (temp >> word)
            {
                if (isType(word)) {
                    temp >> word;
                    isKeyword(word) ? throw ErrorMessage("Variable name can not be a keyword!\n") 
                                    : ++propertyCount;
                }
            }
            temp.clear();
        }*/
    }
    outputStream_ << "Member variables: " << propertyCount << std::endl;
}

/*
 * Member function which analyses the function.
 */
void  Analyzer::functionAnalysis(std::string& input)
{
    std::stringstream   streamFunctionName(input);
    std::string         word;
    std::string         funcReturnType;

    std::string     functionName;
    while (streamFunctionName >> word)
    {
        if (isType(word)) {
            funcReturnType = word;
            streamFunctionName >> word;
            if (word.empty()) {
                break;
            }
            auto isCleanName = word.find("(");
            if (isCleanName != std::string::npos) {
                functionName = word.substr(0, isCleanName);
            } else {
                functionName = word;
            }
            break;
        }
    }
    streamFunctionName.clear();

    auto fStart = input_.find(funcReturnType);
    if (fStart == std::string::npos) {
        throw ErrorMessage("Error occured during parsing!\n");
    }
    auto fEnd = input_.find("}");
    if (fEnd == std::string::npos) { 
        throw ErrorMessage("Error occured during parsing!\n");
    }
    std::string funcBody = input.substr(fStart, fEnd  - fStart + 1);
    std::string temp = input.substr(0, fStart);
    std::cout << temp;
    temp += input.substr(fEnd + 1, input_.size() - fEnd - 1);
    input = temp;

    auto argStart = funcBody.find("(");
    auto argEnd = funcBody.find(")");
    if (argStart == std::string::npos || argEnd == std::string::npos) {
        throw ErrorMessage("Function's parameters must be on the same line as its name\n");
    }
    size_t  argCount = 0;
    std::string argumentScope = funcBody.substr(argStart + 1, argEnd - argStart - 1);
   
    std::stringstream    streamScope(argumentScope);
    while (streamScope >> word)
    {
        if (isType(word)) {
            streamScope >> word;
            if (word.empty()) {
                break;
            }
            if (isKeyword(word)) {
                throw ErrorMessage("Argument name can not be a keyword!\n");
            }
            ++argCount;
        }
    }
    streamScope.clear();
    
    funcBody = funcBody.substr(argEnd + 1, funcBody.size() - argEnd + 1);
    size_t  variableCount = functionVariableCount(funcBody);
    outputStream_ << functionName << " has " << argCount << " arguments and " << variableCount << " variables in the body\n";
}

/*
 * Member function which computes variable
 * count in a function.
 */
size_t  Analyzer::functionVariableCount(std::string& tempBody)
{
    std::stringstream   sBody(tempBody);
    std::string         word;

    size_t  variableCount = 0;
    while (sBody >> word)
    {
        if (isType(word)) {
            sBody >> word;
            if (word.empty()) {
                break;
            }
            if (isKeyword(word)) {
                throw ErrorMessage("Variable name can not be a keyword!\n");
            }
            ++variableCount;
        }
    }
    sBody.clear();

    return variableCount;
}

/*
 * Member function which is so called 
 * the main function of class.
 */
void    Analyzer::startAnalysis()
{
    // Find and analize first user defined types
    //findUserDefinedTypes("class");
    //findUserDefinedTypes("struct");
    //findUserDefinedTypes("enum class");

    // Global functions analysis
    functionAnalysis(input_);
    writeResults();
}

/*
 * Member function which checks if give value is
 * a C++ keyword or not.
 */
bool    Analyzer::isKeyword(const std::string& value) const
{   
    for (auto& keyword : KEYWORDS)
    {
        if (keyword == value) {
            return true;
        }
    }
    
    return false;
}

/*
 * Member function which checks if given value is
 * basic or user defined data type or not.
 */
bool    Analyzer::isType(const std::string& value) const
{
    for (auto& type : identifierTypes_)
    {
        if (type == value) {
            return true;
        }
    }

    return false;
}

/*
 * Member function which writes results in a file.
 */
void    Analyzer::writeResults()
{
    std::fstream    out;
    std::string     outString = outputStream_.str();

    out.open(outputFile_, std::ios::out);

    if (!out.is_open()) {
        throw ErrorMessage("Could not show analysis results with a file!\n");
    }
    
    out << outString;
    out.close();

    std::cout << "Analysis successfully written in " << outputFile_ << " file!\n";
}
