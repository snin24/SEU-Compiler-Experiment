#include "error.hpp"

#include <cstdio>

#include "utf8.hpp"

namespace lexer {
namespace {

/// 计算 prefix 的换行符个数与最后一行的长度，语义与 Python 的
/// `prefix.count("\n")` / `prefix.rsplit("\n", 1)[-1]` 相同，只是先统一了 \r\n 与 \r。
void locate(const std::string& source, std::size_t byte_end, std::size_t& line, std::size_t& column) {
    std::size_t current_line = 1;
    std::size_t current_column = 1;
    std::size_t pos = 0;

    while (pos < byte_end) {
        const auto [code_point, length] = utf8::decode(source, pos);
        pos += length;

        if (code_point == '\r') {
            // \r\n 只算一次换行
            if (pos < byte_end && source[pos] == '\n') {
                ++pos;
            }
            ++current_line;
            current_column = 1;
        } else if (code_point == '\n') {
            ++current_line;
            current_column = 1;
        } else {
            ++current_column;
        }
    }

    line = current_line;
    column = current_column;
}

/// 生成与 Python `repr(char)` 一致的文本，例如 '@'、'\t'、'\x07'、'中'。
std::string character_repr(char32_t code_point) {
    std::string body;

    if (code_point == '\\') {
        body = "\\\\";
    } else if (code_point == '\'') {
        // Python 遇到单引号会改用双引号，避免转义
        return "\"'\"";
    } else if (code_point == '\n') {
        body = "\\n";
    } else if (code_point == '\r') {
        body = "\\r";
    } else if (code_point == '\t') {
        body = "\\t";
    } else if (code_point < 0x20 || code_point == 0x7F) {
        char buffer[8];
        std::snprintf(buffer, sizeof(buffer), "\\x%02x", static_cast<unsigned>(code_point));
        body = buffer;
    } else {
        body = utf8::encode(code_point);
    }

    return "'" + body + "'";
}

}  // namespace

LexicalErrorInfo make_lexical_error_info(const std::string& source, std::size_t byte_pos) {
    LexicalErrorInfo info{};
    const std::size_t limit = byte_pos < source.size() ? byte_pos : source.size();

    info.position = utf8::count(std::string_view(source).substr(0, limit));
    locate(source, limit, info.line, info.column);

    if (limit < source.size()) {
        const auto [code_point, length] = utf8::decode(source, limit);
        (void)length;
        info.message = "词法错误：非法字符 " + character_repr(code_point) + "，位置 " +
                       std::to_string(info.position) + "（第 " + std::to_string(info.line) +
                       " 行，第 " + std::to_string(info.column) + " 列）";
    } else {
        info.message = "词法错误：输入意外结束，位置 " + std::to_string(info.position) + "（第 " +
                       std::to_string(info.line) + " 行，第 " + std::to_string(info.column) + " 列）";
    }
    return info;
}

LexicalError::LexicalError(const std::string& source, std::size_t byte_pos)
    : LexicalError(make_lexical_error_info(source, byte_pos)) {}

LexicalError::LexicalError(LexicalErrorInfo info)
    : std::runtime_error(info.message),
      position_(info.position),
      line_(info.line),
      column_(info.column) {}

}  // namespace lexer
