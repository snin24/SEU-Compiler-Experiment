#ifndef LEXER_UTF8_HPP
#define LEXER_UTF8_HPP

// 极简 UTF-8 工具。
// C++ 的 std::string 按字节存储，而 Python 的 str 以“字符（码点）”为单位，
// 为了让错误信息里的“位置/列号”与 Python 版严格一致，这里统一按码点计数。
// 本实验的输入是 ASCII 源码，只有在报告非法字符（例如中文、全角符号）时才会用到多字节分支。

#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>
#include <utility>

namespace utf8 {

/// 返回字符串包含的 UTF-8 码点个数。
std::size_t count(std::string_view text);

/// 解码 pos 处的一个码点，返回 {码点, 该码点占用字节数}；非法字节按单字节处理。
std::pair<char32_t, std::size_t> decode(std::string_view text, std::size_t pos);

/// 把码点编码成 UTF-8 字节串。
std::string encode(char32_t code_point);

}  // namespace utf8

#endif  // LEXER_UTF8_HPP
