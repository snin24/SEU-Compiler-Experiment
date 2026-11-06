#ifndef LEXER_RECOGNIZER_HPP
#define LEXER_RECOGNIZER_HPP

// 词法识别器：与 Python 版 utils/ 下的四个模块逐一对应。
//   recognize_keyword_or_id  <-> keywords_and_id.recognize_keys_and_ids
//   recognize_multi_operator <-> multi_operator.recognize_multi_operator
//   recognize_number         <-> number.recognize_number
//   recognize_single_operator<-> single_operator.recognize_single_operator
//
// 每个识别器都是“最长匹配 + 失败返回空”的形式：
//   输入 (源串, 起始字节下标)
//   输出 {类别, 词素, 匹配结束后的下标}，无法识别时返回 std::nullopt。
// 这等价于为每条词法规则构造一个 DFA，并用同一个函数模拟运行该 DFA。

#include <cstddef>
#include <optional>
#include <string>
#include <vector>

#include "token.hpp"

namespace lexer {

struct RecognizeResult {
    TokenType type;
    std::string lexeme;
    std::size_t next;  // 下一个待扫描的字节下标
};

using Recognizer = std::optional<RecognizeResult> (*)(const std::string&, std::size_t);

/// 标识符 / 关键字：以字母或下划线开头，后接字母、数字或下划线；识别完整词素后查关键字表。
std::optional<RecognizeResult> recognize_keyword_or_id(const std::string& source, std::size_t pos);

/// 双字符运算符 := <> >= <=，顺带识别单字符关系运算符 < >。
std::optional<RecognizeResult> recognize_multi_operator(const std::string& source, std::size_t pos);

/// 非负整数：[0-9]+
std::optional<RecognizeResult> recognize_number(const std::string& source, std::size_t pos);

/// 单字符运算符与分隔符 = - + * / ( ) ;
std::optional<RecognizeResult> recognize_single_operator(const std::string& source, std::size_t pos);

/// 扫描整个字符流，返回词法单元序列；遇到非法字符抛出 LexicalError。
std::vector<Token> tokenize(const std::string& source);

}  // namespace lexer

#endif  // LEXER_RECOGNIZER_HPP
