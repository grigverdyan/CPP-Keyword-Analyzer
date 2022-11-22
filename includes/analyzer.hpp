#ifndef ANALYZER_H
#define ANALYZER_H

#include <string>
#include <vector>
#include <map>

class Analyzer
{
    public:
        Analyzer() = delete;
        Analyzer(std::string& input, const std::string& outputFile);
        Analyzer(const Analyzer& other) = delete;
        Analyzer& operator=(const Analyzer& rhs) = delete;
        ~Analyzer() = default;

    public:
        void        startAnalysis();

    private:
        void        findUserDefinedTypes(const std::string& udType);
        void        addUserDefinedType(std::string& body, const std::string& udType);
        void        functionAnalysis();
        void        functionAnalysis(std::string& body);
        size_t      functionAnalysis(size_t i, std::vector<std::string> v);
        size_t      functionVariableCount(std::string& str);
        bool        isKeyword(const std::string& value) const;
        bool        isType(const std::string& value) const;
        void        writeResults();
    
    private:
        std::string             input_;
        const std::string       outputFile_;
        std::stringstream       outputStream_;
        
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
