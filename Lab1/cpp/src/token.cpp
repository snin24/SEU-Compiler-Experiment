#include "token.hpp"

namespace lexer {

const char* to_string(TokenType type) {
    switch (type) {
        case TokenType::IF:          return "IF";
        case TokenType::THEN:        return "THEN";
        case TokenType::ELSE:        return "ELSE";
        case TokenType::END:         return "END";
        case TokenType::REPEAT:      return "REPEAT";
        case TokenType::UNTIL:       return "UNTIL";
        case TokenType::READ:        return "READ";
        case TokenType::WRITE:       return "WRITE";
        case TokenType::ID:          return "ID";
        case TokenType::NUM:         return "NUM";
        case TokenType::ASSIGN:      return "ASSIGN";
        case TokenType::RELOP:       return "RELOP";
        case TokenType::PLUS:        return "PLUS";
        case TokenType::MINUS:       return "MINUS";
        case TokenType::TIMES:       return "TIMES";
        case TokenType::OVER:        return "OVER";
        case TokenType::LPAREN:      return "LPAREN";
        case TokenType::RPAREN:      return "RPAREN";
        case TokenType::SEMI:        return "SEMI";
        case TokenType::END_OF_FILE: return "EOF";
    }
    return "UNKNOWN";
}

}  // namespace lexer
