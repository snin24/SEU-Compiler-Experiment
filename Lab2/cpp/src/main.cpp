// Lab2：LL(1) 语法分析器演示入口（C++ 版，与 main.py 输出逐行对齐）。
//
// 用法：
//   lab2                 运行内置的 5 组示例（与 Python 版完全相同）
//   lab2 <源文件>        分析文件中的表达式，输出产生式序列与语法分析树
//   lab2 -h | --help     显示帮助

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "error.hpp"
#include "parse_error.hpp"
#include "parser.hpp"
#include "text_util.hpp"

namespace {

void print_result(const parser::ParseResult& result) {
    std::cout << "\n产生式序列：\n";
    for (std::size_t i = 0; i < result.productions.size(); ++i) {
        std::cout << (i + 1) << ". " << result.productions[i] << '\n';
    }
    std::cout << "\n完整语法分析树：\n" << parser::format_tree(*result.root) << '\n';
}

void run_examples() {
    const std::vector<std::pair<std::string, std::string>> examples = {
        {"基本加法", "a + 3"},
        {"乘法优先级", "a + b * c"},
        {"多行与括号",
         "(_temp + 12)\n"
         "    * (count1 - 3) / 2"},
        {"语法错误：缺少右括号", "(a + 3"},
        {"词法错误：非法字符", "a @ 3"},
    };

    for (std::size_t i = 0; i < examples.size(); ++i) {
        const auto& [title, source] = examples[i];

        std::cout << '\n' << std::string(60, '=') << '\n';
        std::cout << "例 " << (i + 1) << "：" << title << '\n';
        std::cout << "输入：" << text::python_repr(source) << '\n';

        try {
            print_result(parser::parse_expression(source));
        } catch (const lexer::LexicalError& error) {
            std::cout << error.what() << '\n';
        } catch (const parser::ParseError& error) {
            std::cout << error.what() << '\n';
        }
    }
}

bool read_file(const std::string& path, std::string& content) {
    std::ifstream input(path, std::ios::binary);
    if (!input) {
        return false;
    }
    std::ostringstream buffer;
    buffer << input.rdbuf();
    content = buffer.str();

    if (content.size() >= 3 && static_cast<unsigned char>(content[0]) == 0xEF &&
        static_cast<unsigned char>(content[1]) == 0xBB &&
        static_cast<unsigned char>(content[2]) == 0xBF) {
        content.erase(0, 3);
    }
    return true;
}

int run_file(const std::string& path) {
    std::string content;
    if (!read_file(path, content)) {
        std::cerr << "无法打开输入文件：" << path << '\n';
        return 2;
    }

    try {
        const parser::ParseResult result = parser::parse_expression(content);
        print_result(result);
    } catch (const lexer::LexicalError& error) {
        std::cout << error.what() << '\n';
        return 1;
    } catch (const parser::ParseError& error) {
        std::cout << error.what() << '\n';
        return 1;
    }
    return 0;
}

void usage(const char* program) {
    std::cout << "用法：" << program << " [源文件]\n"
              << "  不带参数时运行内置示例；给出源文件时分析该文件中的表达式。\n";
}

}  // namespace

int main(int argc, char** argv) {
    text::enable_utf8_console();

    if (argc == 1) {
        run_examples();
        return 0;
    }

    const std::string argument = argv[1];
    if (argument == "-h" || argument == "--help") {
        usage(argv[0]);
        return 0;
    }
    if (argc > 2) {
        usage(argv[0]);
        return 2;
    }
    return run_file(argument);
}
