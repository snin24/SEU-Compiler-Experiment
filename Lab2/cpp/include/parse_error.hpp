#ifndef PARSER_PARSE_ERROR_HPP
#define PARSER_PARSE_ERROR_HPP

// 语法错误：与 Python 版 Lab2/utils/error.py 的 ParseError 对应。

#include <cstddef>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

namespace parser {

class ParseError : public std::runtime_error {
public:
    /// index    : 出错 token 的下标（0 起）
    /// expected : 期望的符号；可以是一个符号，也可以是分析表中该行的全部可选符号
    /// actual   : 实际读到的 token 类型
    /// symbol   : 因分析表查不到而产生时，记录栈顶的非终结符
    explicit ParseError(std::optional<std::size_t> index = std::nullopt,
                        std::vector<std::string> expected = {},
                        std::optional<std::string> actual = std::nullopt,
                        std::optional<std::string> symbol = std::nullopt);
};

}  // namespace parser

#endif  // PARSER_PARSE_ERROR_HPP
