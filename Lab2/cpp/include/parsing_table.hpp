#ifndef PARSER_PARSING_TABLE_HPP
#define PARSER_PARSING_TABLE_HPP

// LL(1) 预测分析表（表达式文法），与 Python 版 parser_table.py 完全等价。
//
// 文法 G[E]：
//     E  -> T E'
//     E' -> + T E' | - T E' | ε
//     T  -> F T'
//     T' -> * F T' | / F T' | ε
//     F  -> ( E ) | id | num

#include <map>
#include <string>
#include <vector>

namespace parser {

using Production = std::vector<std::string>;      // 产生式右部，终结符/非终结符按序存放
using TableRow = std::map<std::string, Production>;  // 行：当前输入符号 -> 选用右部

/// 表的外层键为非终结符；内层键为该非终结符在 FIRST 集中遇到的终结符。
extern const std::map<std::string, TableRow> PARSING_TABLE;

/// 非终结符集合 {'E', "E'", 'T', "T'", 'F'}
bool is_non_terminal(const std::string& symbol);

}  // namespace parser

#endif  // PARSER_PARSING_TABLE_HPP
