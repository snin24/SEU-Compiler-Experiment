#ifndef LEXER_TOKEN_HPP
#define LEXER_TOKEN_HPP

// 词法单元（Token）类型定义。
// 与 Python 版 utils/keywords_and_id.py、single_operator.py 中的字符串类型名一一对应，
// 用强类型枚举替代字符串，既避免拼写错误，也便于编译器做穷尽性检查。

#include <string>

namespace lexer {

enum class TokenType {
    // 关键字
    IF, THEN, ELSE, END, REPEAT, UNTIL, READ, WRITE,
    // 自定义符
    ID, NUM,
    // 运算符
    ASSIGN, RELOP, PLUS, MINUS, TIMES, OVER, LPAREN, RPAREN, SEMI,
    // 输入结束标记，由语法分析器追加，词法分析器本身不产生
    END_OF_FILE
};

/// 返回与 Python 版完全一致的类别名，例如 "IF"、"ID"、"RELOP"、"EOF"。
const char* to_string(TokenType type);

/// 一个词法单元：类别 + 原始词素。
struct Token {
    TokenType type;
    std::string lexeme;
};

}  // namespace lexer

#endif  // LEXER_TOKEN_HPP
