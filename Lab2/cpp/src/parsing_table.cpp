#include "parsing_table.hpp"

#include <set>

namespace parser {

bool is_non_terminal(const std::string& symbol) {
    static const std::set<std::string> NON_TERMINALS = {"E", "E'", "T", "T'", "F"};
    return NON_TERMINALS.count(symbol) != 0;
}

// 注意：E' 与 T' 的行里同时包含 RPAREN 与 EOF，这是消除左递归后
// FOLLOW(E') = FOLLOW(T') = { ')', EOF } 的体现；epsilon 用字符串 "epsilon" 表示。
const std::map<std::string, TableRow> PARSING_TABLE = {
    {"E",
     {
         {"LPAREN", {"T", "E'"}},
         {"ID", {"T", "E'"}},
         {"NUM", {"T", "E'"}},
     }},
    {"E'",
     {
         {"PLUS", {"PLUS", "T", "E'"}},
         {"MINUS", {"MINUS", "T", "E'"}},
         {"RPAREN", {"epsilon"}},
         {"EOF", {"epsilon"}},
     }},
    {"T",
     {
         {"LPAREN", {"F", "T'"}},
         {"ID", {"F", "T'"}},
         {"NUM", {"F", "T'"}},
     }},
    {"T'",
     {
         {"PLUS", {"epsilon"}},
         {"MINUS", {"epsilon"}},
         {"TIMES", {"TIMES", "F", "T'"}},
         {"OVER", {"OVER", "F", "T'"}},
         {"RPAREN", {"epsilon"}},
         {"EOF", {"epsilon"}},
     }},
    {"F",
     {
         {"LPAREN", {"LPAREN", "E", "RPAREN"}},
         {"ID", {"ID"}},
         {"NUM", {"NUM"}},
     }},
};

}  // namespace parser
