#ifndef ANALYZER_H
#define ANALYZER_H

#include <string>
#include <vector>
#include <map>

class Analyzer
{
    public:
        Analyzer() = delete;
        Analyzer(std::string& input);
        Analyzer(const Analyzer& other) = delete;
        Analyzer& operator=(const Analyzer& rhs) = delete;
        ~Analyzer() = default;

    public:
        void        startAnalysis();

    private:
       // void        findClass();
        void        findUserDefinedTypes(const std::string& udType);
        void        addUserDefinedType(std::string& body, const std::string& udType);
        size_t      functionAnalysis(size_t i, std::vector<std::string> v);
        size_t      functionVariableCount(std::string& str);
        bool        isKeyword(const std::string& value) const;
        bool        isType(const std::string& value) const;
        
        
    private:
        std::string                         input_;
        size_t                              variableCount_;
        size_t                              functionCount_;
        size_t                              classCount_;
        std::multimap<std::string, size_t>  functions_;
        
        std::vector<std::string>    identifierTypes_ = {
            "auto",
            "bool",
            "char",
            "char8_t",
            "char16_t",
            "char32_t",
            "double",
            "float",
            "int",
            "long",
            "short",
            "signed",
            "unsigned",
            "void",
            "wchar_t",
        };


};

extern  std::vector<std::string>    KEYWORDS;

#endif  // ANALYZER_H
