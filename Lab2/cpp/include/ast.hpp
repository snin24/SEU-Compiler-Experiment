#ifndef PARSER_AST_HPP
#define PARSER_AST_HPP

// 语法分析树结点，对应 Python 版 syntax_tree.py 中的 @dataclass TreeNode。
//
// Python 用 `lexeme: str | None` 区分“非终结符结点”和“终结符结点”：
//   * 非终结符结点：lexeme 为 None，children 为产生式右部展开出的子结点；
//   * 终结符结点  ：lexeme 为实际词素，children 为空。
// C++ 用 std::optional<std::string> 表达同一语义，用 unique_ptr 管理子树所有权。

#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace parser {

struct TreeNode {
    std::string symbol;
    std::optional<std::string> lexeme;
    std::vector<std::unique_ptr<TreeNode>> children;

    explicit TreeNode(std::string sym) : symbol(std::move(sym)) {}

    TreeNode(const TreeNode&) = delete;
    TreeNode& operator=(const TreeNode&) = delete;
};

/// 把语法分析树格式化为带制表符号的文本（与 Python format_tree 逐字符一致）。
std::string format_tree(const TreeNode& root);

}  // namespace parser

#endif  // PARSER_AST_HPP
