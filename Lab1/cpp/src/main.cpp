// Lab1：词法分析器演示入口（C++ 版，与 main.py 输出逐行对齐）。
//
// 用法：
//   lab1                 运行内置的 7 组示例（与 Python 版完全相同）
//   lab1 <源文件>        对文件内容做词法分析，输出词法单元序列
//   lab1 -h | --help     显示帮助

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "error.hpp"
#include "recognizer.hpp"
#include "text_util.hpp"
#include "token.hpp"

namespace {

void print_tokens(const std::string& source) {
    for (const auto& token : lexer::tokenize(source)) {
        std::cout << '(' << lexer::to_string(token.type) << ", \"" << token.lexeme << "\")\n";
    }
}

void run_examples() {
    const std::vector<std::pair<std::string, std::string>> examples = {
        {"基本表达式", "example_id := 123 + 45;"},
        {"关键字与标识符", "if ifx then example_id"},
        {"关系运算符", "a >= 10; b <> 20; c <= 30; d = 40;"},
        {"所有关键字", "if then else end repeat until read write"},
        {"括号与算术运算符", "(a + b) * c / 2 - 1;"},
        {"空白字符", "read\tx;\r\n  write x\n"},
        {"非法字符", "a := 10 @ 20"},
    };

    for (const auto& [title, source] : examples) {
        std::cout << '\n' << title << "：" << text::python_repr(source) << '\n';
        try {
            print_tokens(source);
        } catch (const lexer::LexicalError& error) {
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

    // 去掉可能存在的 UTF-8 BOM，避免它被当成非法字符
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
        print_tokens(content);
    } catch (const lexer::LexicalError& error) {
        std::cout << error.what() << '\n';
        return 1;
    }
    return 0;
}

void usage(const char* program) {
    std::cout << "用法：" << program << " [源文件]\n"
              << "  不带参数时运行内置示例；给出源文件时输出该文件的词法单元序列。\n";
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
