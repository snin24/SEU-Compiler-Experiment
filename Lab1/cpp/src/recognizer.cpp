#include "recognizer.hpp"

#include <array>
#include <unordered_map>

#include "error.hpp"

namespace lexer {
namespace {

const std::unordered_map<std::string, TokenType> KEYWORDS = {
    {"if", TokenType::IF},         {"then", TokenType::THEN},
    {"else", TokenType::ELSE},     {"end", TokenType::END},
    {"repeat", TokenType::REPEAT}, {"until", TokenType::UNTIL},
    {"read", TokenType::READ},     {"write", TokenType::WRITE},
};

const std::unordered_map<char, TokenType> SINGLE_OPERATORS = {
    {'=', TokenType::RELOP}, {'-', TokenType::MINUS},  {'+', TokenType::PLUS},
    {'*', TokenType::TIMES}, {'/', TokenType::OVER},   {'(', TokenType::LPAREN},
    {')', TokenType::RPAREN}, {';', TokenType::SEMI},
};

// 识别器的尝试顺序即“最长匹配优先”的优先级：
// 标识符/关键字 -> 双字符运算符 -> 数字 -> 单字符运算符
// （关键字必须先于单字符运算符，否则 "if" 会被拆成字母；双字符运算符必须先于单字符。）
constexpr std::array<Recognizer, 4> RECOGNIZERS = {
    recognize_keyword_or_id,
    recognize_multi_operator,
    recognize_number,
    recognize_single_operator,
};

bool is_letter(char c) {
    return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}

bool is_digit(char c) {
    return '0' <= c && c <= '9';
}

bool is_blank(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

}  // namespace

std::optional<RecognizeResult> recognize_keyword_or_id(const std::string& source, std::size_t pos) {
    if (pos >= source.size()) {
        return std::nullopt;
    }

    const std::size_t start = pos;
    int state = 0;  // 0：初始态；1：已读入标识符首字符

    while (pos < source.size()) {
        const char now = source[pos];

        if (state == 0) {
            // 首字符必须是字母或下划线，否则本识别器放弃
            if (!(is_letter(now) || now == '_')) {
                return std::nullopt;
            }
            state = 1;
        } else if (!(is_letter(now) || is_digit(now) || now == '_')) {
            // 遇到不属于标识符的字符，当前标识符结束（最长匹配）
            break;
        }

        ++pos;
    }

    const std::string lexeme = source.substr(start, pos - start);
    const auto keyword = KEYWORDS.find(lexeme);
    // 识别出完整标识符后查表：命中则为关键字，否则为普通标识符 ID
    const TokenType type = keyword != KEYWORDS.end() ? keyword->second : TokenType::ID;

    return RecognizeResult{type, lexeme, pos};
}

std::optional<RecognizeResult> recognize_multi_operator(const std::string& source, std::size_t pos) {
    if (pos >= source.size()) {
        return std::nullopt;
    }

    const char now = source[pos];
    const char following = pos + 1 < source.size() ? source[pos + 1] : '\0';

    if (now == ':') {
        if (following == '=') {
            return RecognizeResult{TokenType::ASSIGN, ":=", pos + 2};
        }
        return std::nullopt;  // 单独的 ':' 不是合法词素，交给后面的识别器（同样失败）
    }

    if (now == '<') {
        if (following == '>') {
            return RecognizeResult{TokenType::RELOP, "<>", pos + 2};
        }
        if (following == '=') {
            return RecognizeResult{TokenType::RELOP, "<=", pos + 2};
        }
        return RecognizeResult{TokenType::RELOP, "<", pos + 1};
    }

    if (now == '>') {
        if (following == '=') {
            return RecognizeResult{TokenType::RELOP, ">=", pos + 2};
        }
        return RecognizeResult{TokenType::RELOP, ">", pos + 1};
    }

    return std::nullopt;
}

std::optional<RecognizeResult> recognize_number(const std::string& source, std::size_t pos) {
    if (pos >= source.size()) {
        return std::nullopt;
    }

    const std::size_t start = pos;
    int state = 0;  // 0：初始态；1：已读入至少一位数字

    while (pos < source.size()) {
        const bool digit = is_digit(source[pos]);

        if (state == 0) {
            if (!digit) {
                return std::nullopt;
            }
            state = 1;
        } else if (!digit) {
            break;
        }
        ++pos;
    }

    return RecognizeResult{TokenType::NUM, source.substr(start, pos - start), pos};
}

std::optional<RecognizeResult> recognize_single_operator(const std::string& source, std::size_t pos) {
    if (pos >= source.size()) {
        return std::nullopt;
    }

    const auto found = SINGLE_OPERATORS.find(source[pos]);
    if (found == SINGLE_OPERATORS.end()) {
        return std::nullopt;
    }

    return RecognizeResult{found->second, std::string(1, source[pos]), pos + 1};
}

std::vector<Token> tokenize(const std::string& source) {
    std::vector<Token> tokens;
    const std::size_t size = source.size();
    std::size_t pos = 0;

    while (pos < size) {
        // 空白字符只起分隔作用，直接跳过，不产生 Token
        if (is_blank(source[pos])) {
            ++pos;
            continue;
        }

        bool matched = false;
        for (const Recognizer recognizer : RECOGNIZERS) {
            const auto result = recognizer(source, pos);
            if (!result.has_value()) {
                continue;
            }

            tokens.push_back(Token{result->type, result->lexeme});
            pos = result->next;
            matched = true;
            break;
        }

        if (!matched) {
            // 所有识别器都失败：报告词法错误并终止当前输入的分析
            throw LexicalError(source, pos);
        }
    }

    return tokens;
}

}  // namespace lexer
