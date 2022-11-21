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
        bool        isKeyword(const std::string& value) const;
        bool        isType(const std::string& value) const;
        
        
    private:
        std::string                     input_;
        size_t                          variableCount_;
        size_t                          functionCount_;
        size_t                          classCount_;
       // multimap<std::string, size_t>   functions_;
        
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

extern  std::vector<std::string>    KEYWORDS;/* = {
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
*/
#endif  // ANALYZER_H
