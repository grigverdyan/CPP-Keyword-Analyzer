#include <iostream>
#include <sstream>

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

Analyzer::Analyzer(std::string& input)
    : input_{input}
{}

void    Analyzer::findUserDefinedTypes(const std::string& udType)
{
    while (true)
    {
        auto classStartIndex = input_.find(udType);
        if (classStartIndex == std::string::npos) {
            break;;
        }
        auto classEndIndex = input_.find("};");
        if (classEndIndex == std::string::npos) {
            throw ErrorMessage("Can not find the appropriate ending of user defined type!\n");
        }

        std::string classBody = input_.substr(classStartIndex + udType.size(), input_.size() - classStartIndex);
        std::string temp = input_.substr(0, classStartIndex);
        temp += input_.substr(classEndIndex + udType.size(), input_.size() - (classEndIndex + udType.size()));
        input_ = temp;
        addUserDefinedType(classBody, udType);
    }
}

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
    std::cout << "In " << typeName << " " << udType << ":\n";
    sBody.clear();
    
    auto typeIndex = body.find(typeName);
    std::string tBody = body.substr(typeIndex + typeName.size(), body.size());
    body = tBody;

    size_t  propertyCount = 0;
    std::vector<std::string>    vBody;
    while (true)
    {
        auto index = body.find("\n");
        if (index != std::string::npos) {
            std::string temp = body.substr(index + 1, body.size());
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

    /*
    while (sBody >> word)
    {
        if (isType(word)) {
            sBody >> word;
            if (!word.empty()) {
                auto functionStart = word.find('(');
                if (functionStart != std::string::npos) {
                    std::string methodName = word.substr(0, functionStart);
                    while (sBody >> word)
                    {
                        if (word.find(')') != std::string::npos) {
                            
                            break;
                        }
                    }
                } else {
                    isKeyword(word) ? throw ErrorMessage("Variable can not have name of keyword!\n")
                                : ++propertyCount;
                }
            }
            else {
                break;
            }
        }
    }
    */
    std::cout << "Member variables: " << propertyCount << std::endl;
    //std::cout << "Member functions: " << functionCount << std::endl;

}

size_t  Analyzer::functionAnalysis(size_t i, std::vector<std::string> body)
{
    std::stringstream   stream(body[i]);
    std::string         word;

    std::string     functionName;
    while (stream >> word)
    {
        if (isType(word)) {
            stream >> word;
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
    stream.clear();

    auto argStart = body[i].find("(");
    auto argEnd = body[i].find(")");
    if (argStart == std::string::npos || argEnd == std::string::npos) {
        throw ErrorMessage("Function's parametrs must be pn the same line as its name\n");
    }
    size_t  argCount;
    std::string argumentScope = body[i].substr(argStart + 1, argEnd);

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

    std::cout << functionName << " has " << argCount << " arguments and " << variableCount << " variables in the body\n";

    return endIndex;
}

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

void    Analyzer::startAnalysis()
{
    findUserDefinedTypes("class");
}


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
