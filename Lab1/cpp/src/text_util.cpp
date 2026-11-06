#include "text_util.hpp"

#include <cstdio>

#include "utf8.hpp"

#ifdef _WIN32
#include <windows.h>
#endif

namespace text {

std::string python_repr(std::string_view value) {
    // Python 的规则：默认用单引号；若串中含单引号且不含双引号，则改用双引号。
    const bool has_single_quote = value.find('\'') != std::string_view::npos;
    const bool has_double_quote = value.find('"') != std::string_view::npos;
    const char quote = (has_single_quote && !has_double_quote) ? '"' : '\'';

    std::string out;
    out.push_back(quote);

    std::size_t pos = 0;
    while (pos < value.size()) {
        const auto [code_point, length] = utf8::decode(value, pos);
        pos += length;

        if (code_point == '\\' || code_point == static_cast<char32_t>(quote)) {
            out.push_back('\\');
            out += utf8::encode(code_point);
        } else if (code_point == '\n') {
            out += "\\n";
        } else if (code_point == '\r') {
            out += "\\r";
        } else if (code_point == '\t') {
            out += "\\t";
        } else if (code_point < 0x20 || code_point == 0x7F) {
            char buffer[8];
            std::snprintf(buffer, sizeof(buffer), "\\x%02x", static_cast<unsigned>(code_point));
            out += buffer;
        } else {
            // 可打印字符（含中文等非 ASCII 字符）原样保留
            out += utf8::encode(code_point);
        }
    }

    out.push_back(quote);
    return out;
}

void enable_utf8_console() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
}

}  // namespace text
