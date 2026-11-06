#ifndef LEXER_ERROR_HPP
#define LEXER_ERROR_HPP

// 词法错误：与 Python 版 utils/error.py 的 LexicalError 对应。
// 错误策略同样是“抛出异常、终止当前输入的分析”，只是 C++ 用异常类而非 ValueError。

#include <cstddef>
#include <stdexcept>
#include <string>

namespace lexer {

/// 构造错误信息所需的全部数据，先算好再交给基类，避免在初始化列表里重复计算。
struct LexicalErrorInfo {
    std::string message;
    std::size_t position;  // 以“字符（码点）”为单位的下标，与 Python 一致
    std::size_t line;      // 从 1 开始
    std::size_t column;    // 从 1 开始
};

LexicalErrorInfo make_lexical_error_info(const std::string& source, std::size_t byte_pos);

class LexicalError : public std::runtime_error {
public:
    LexicalError(const std::string& source, std::size_t byte_pos);

    std::size_t position() const noexcept { return position_; }
    std::size_t line() const noexcept { return line_; }
    std::size_t column() const noexcept { return column_; }

private:
    explicit LexicalError(LexicalErrorInfo info);

    std::size_t position_;
    std::size_t line_;
    std::size_t column_;
};

}  // namespace lexer

#endif  // LEXER_ERROR_HPP
