#ifndef LEXER_TEXT_UTIL_HPP
#define LEXER_TEXT_UTIL_HPP

// 与 Python 输出格式对齐的文本工具：
//   * python_repr：复刻 f-string 中 {source!r} 的单引号表示，便于两份程序输出逐字节对比；
//   * enable_utf8_console：把 Windows 控制台切到 UTF-8，保证中文与制表符号正常显示。

#include <string>
#include <string_view>

namespace text {

/// 等价于 Python 的 repr(str)，例如 'a + 3'、'read\tx;\r\n  write x\n'。
std::string python_repr(std::string_view value);

/// 在 Windows 下把标准输出/输入代码页设为 65001（UTF-8）；其他平台为空操作。
void enable_utf8_console();

}  // namespace text

#endif  // LEXER_TEXT_UTIL_HPP
