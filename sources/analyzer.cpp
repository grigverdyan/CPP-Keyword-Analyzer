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
    size_t  propertyCount = 0;
    while (sBody >> word)
    {
        if (isType(word)) {
            sBody >> word;
            if (!word.empty()) {
                isKeyword(word) ? throw ErrorMessage("Variable can not have name of keyword!\n")
                                : ++propertyCount;  
            }
            else {
                break;
            }
        }
    }
    std::cout << "Member variables: " << propertyCount << std::endl;
    //std::cout << "Member functions: " << functionCount << std::endl;

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
