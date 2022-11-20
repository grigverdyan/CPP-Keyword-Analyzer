
#include "analyzer.hpp"

Analyzer::Analyzer(std::string& input)
    : input_{input}
{}
/*
void    Analyzer::findClasses()
{
    // this does not take into count strings with values of "class"
    auto classStartIndex = input_.find("class");
    if (classStartIndex == std::string::npos) {
        return;
    }
    
}
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
