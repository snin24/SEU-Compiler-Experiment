#ifndef PARSER_PARSER_HPP
#define PARSER_PARSER_HPP

// LL(1) 表驱动语法分析器，对应 Python 版 syntax_tree.LL_parse_tree。
//
// 输入：字符流（内部先调用词法分析器得到 token 序列）
// 输出：语法分析树 + 最左推导的产生式序列
// 失败：抛出 LexicalError（词法错误）或 ParseError（语法错误）

#include <memory>
#include <string>
#include <vector>

#include "ast.hpp"

namespace parser {

struct ParseResult {
    std::unique_ptr<TreeNode> root;
    std::vector<std::string> productions;
};

ParseResult parse_expression(const std::string& source);

}  // namespace parser

#endif  // PARSER_PARSER_HPP
