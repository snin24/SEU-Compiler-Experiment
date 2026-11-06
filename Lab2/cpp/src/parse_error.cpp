#include "parse_error.hpp"

#include <sstream>

namespace parser {
namespace {

std::string join(const std::vector<std::string>& symbols) {
    std::string out;
    for (std::size_t i = 0; i < symbols.size(); ++i) {
        if (i != 0) {
            out += " / ";
        }
        out += symbols[i];
    }
    return out;
}

std::string build_message(std::optional<std::size_t> index,
                          const std::vector<std::string>& expected,
                          std::optional<std::string> actual,
                          std::optional<std::string> symbol) {
    const std::string where = index.has_value() ? "第 " + std::to_string(*index + 1) + " 个 token " : "";

    std::string detail;
    if (symbol.has_value() && !expected.empty() && actual.has_value()) {
        detail = "非终结符 " + *symbol + " 无法用 " + *actual + " 展开，可选 " + join(expected);
    } else if (!expected.empty() && actual.has_value()) {
        detail = "期望 " + join(expected) + "，实际读到 " + *actual;
    } else if (actual.has_value()) {
        detail = "读到 " + *actual + "，无法继续分析";
    } else if (!expected.empty()) {
        detail = "期望 " + join(expected);
    } else {
        detail = "无法继续分析";
    }

    return "语法错误：" + where + detail;
}

}  // namespace

ParseError::ParseError(std::optional<std::size_t> index, std::vector<std::string> expected,
                       std::optional<std::string> actual, std::optional<std::string> symbol)
    : std::runtime_error(build_message(index, expected, actual, symbol)) {}

}  // namespace parser
