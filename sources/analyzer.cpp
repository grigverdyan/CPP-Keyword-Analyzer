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

        std::string classBody = input_.substr(classStartIndex + udType.size(), classEndIndex - (classStartIndex + udType.size()));
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
    if (isKeyword(word)) {
        throw ErrorMessage("User defined type can not have name of keyword!\n");
    }
    identifierTypes_.push_back(word);
    std::string typeName = word;
    outputStream_ << "In " << udType << " " << typeName << ":\n";
    sBody.clear();
    
    auto typeIndex = body.find(typeName);
    std::string tBody = body.substr(typeIndex + typeName.size(), body.size() - (typeIndex + typeName.size()));
    body = tBody;

    size_t  propertyCount = 0;
    std::vector<std::string>    vBody;
    while (true)
    {
        auto index = body.find("\n");
        if (index != std::string::npos) {
            std::string temp = body.substr(index + 1, body.size() - (index + 1));
            vBody.push_back(body.substr(0, index));
            body = temp;
        } else {
            break;
        }
    }

    size_t  i = 0;
    while (!vBody[i].empty())
    {
        auto functionStart = vBody[i].find("(");
        if (functionStart != std::string::npos) {
            i = functionAnalysis(i, vBody);
            if (!vBody[i].empty()) {
                break;
            }
        } else {
            isKeyword(word) ? throw ErrorMessage("Variable name can not be a keyword!\n")
                            : ++propertyCount;
        }
        ++i;
    }

    outputStream_ << "Member variables: " << propertyCount << std::endl;
}

/*
 * Member function which analyses the function.
 */
size_t  Analyzer::functionAnalysis(size_t i, std::vector<std::string> body)
{
    std::stringstream   streamFunctionName(body[i]);
    std::string         word;

    std::string     functionName;
    while (streamFunctionName >> word)
    {
        if (isType(word)) {
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
        }
    }
    streamFunctionName.clear();

    auto argStart = body[i].find("(");
    auto argEnd = body[i].find(")");
    if (argStart == std::string::npos || argEnd == std::string::npos) {
        throw ErrorMessage("Function's parametrs must be pn the same line as its name\n");
    }
    size_t  argCount;
    std::string argumentScope = body[i].substr(argStart + 1, argEnd - (argStart + 1));

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
    ++i;

    size_t  variableCount;
    size_t  endIndex;
    while (!body[i].empty())
    {
        auto isEnd = body[i].find("}");
        if (isEnd != std::string::npos) {
            endIndex = i;
            break;
        }
        ++i;
    }
    std::string tempBody{};
    while (i < endIndex + 1)
    {
        tempBody += body[i++];
    }

    variableCount = functionVariableCount(tempBody);

    outputStream_ << functionName << " has " << argCount << " arguments and " << variableCount << " variables in the body\n";

    return endIndex;
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
            isKeyword(word) ? throw ErrorMessage("Variable name can not be a keyword!\n")
                            : ++variableCount;
        }
    }

    return variableCount;
}

/*
 * Member function which is so called 
 * the main function of class.
 */
void    Analyzer::startAnalysis()
{
    // Find and analize first user defined types
    findUserDefinedTypes("class");
    findUserDefinedTypes("struct");
    findUserDefinedTypes("enum class");

    // Global functions analysis
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
