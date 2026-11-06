#include "parser.hpp"

#include <utility>

#include "parse_error.hpp"
#include "parsing_table.hpp"
#include "recognizer.hpp"
#include "token.hpp"

namespace parser {
namespace {

/// 分析栈中的一项：待匹配的文法符号 + 它在语法树中对应的结点。
struct StackEntry {
    std::string symbol;
    TreeNode* node;  // EOF 哨兵为 nullptr
};

}  // namespace

ParseResult parse_expression(const std::string& source) {
    std::vector<lexer::Token> tokens = lexer::tokenize(source);
    // 追加输入结束标记，使“缺少右括号”这类错误能在 EOF 处被报告
    tokens.push_back(lexer::Token{lexer::TokenType::END_OF_FILE, std::string{}});

    auto root = std::make_unique<TreeNode>("E");

    // 自底向上压栈：栈顶是下一个待处理的符号
    std::vector<StackEntry> stack;
    stack.push_back({"EOF", nullptr});
    stack.push_back({"E", root.get()});

    std::vector<std::string> productions;
    std::size_t index = 0;  // 当前输入 token 的下标

    while (!stack.empty()) {
        const StackEntry entry = stack.back();
        stack.pop_back();

        const lexer::Token& token = tokens[index];
        const std::string token_type = lexer::to_string(token.type);

        if (entry.symbol == "EOF") {
            // 栈底的结束标记：必须恰好读完所有 token
            if (token.type != lexer::TokenType::END_OF_FILE || index != tokens.size() - 1) {
                throw ParseError(index, {"EOF"}, token_type);
            }
            return ParseResult{std::move(root), std::move(productions)};
        }

        if (is_non_terminal(entry.symbol)) {
            // 查表：行 = 栈顶非终结符，列 = 当前输入符号
            const TableRow& row = PARSING_TABLE.at(entry.symbol);
            const auto production = row.find(token_type);
            if (production == row.end()) {
                std::vector<std::string> expected;
                expected.reserve(row.size());
                for (const auto& item : row) {
                    expected.push_back(item.first);  // 该行所有可用终结符，供错误提示
                }
                throw ParseError(index, expected, token_type, entry.symbol);
            }

            const Production& right = production->second;

            // 记录一条推导：A → X1 X2 ... Xn
            std::string text = entry.symbol + " → ";
            for (std::size_t i = 0; i < right.size(); ++i) {
                if (i != 0) {
                    text += ' ';
                }
                text += right[i];
            }
            productions.push_back(std::move(text));

            // 为右部每个符号建立子结点
            entry.node->children.clear();
            entry.node->children.reserve(right.size());
            for (const std::string& symbol : right) {
                entry.node->children.push_back(std::make_unique<TreeNode>(symbol));
            }

            // ε 产生式不压栈；否则逆序压栈，保证最左符号最先被处理
            if (!(right.size() == 1 && right[0] == "epsilon")) {
                for (std::size_t i = right.size(); i-- > 0;) {
                    stack.push_back(StackEntry{right[i], entry.node->children[i].get()});
                }
            }
        } else {
            // 终结符：与当前输入符号比对，成功则推进输入指针
            if (entry.symbol != token_type) {
                throw ParseError(index, {entry.symbol}, token_type);
            }
            entry.node->lexeme = token.lexeme;
            ++index;
        }
    }

    // 理论上不可达：EOF 哨兵保证循环总会先返回或抛异常
    throw ParseError(index, {"EOF"}, std::string("分析栈提前耗尽"));
}

}  // namespace parser
