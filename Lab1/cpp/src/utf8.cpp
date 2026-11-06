#include "utf8.hpp"

namespace utf8 {

std::size_t count(std::string_view text) {
    std::size_t total = 0;
    std::size_t pos = 0;
    while (pos < text.size()) {
        pos += decode(text, pos).second;
        ++total;
    }
    return total;
}

std::pair<char32_t, std::size_t> decode(std::string_view text, std::size_t pos) {
    const auto byte = static_cast<unsigned char>(text[pos]);

    // 单字节：0xxxxxxx
    if (byte < 0x80) {
        return {static_cast<char32_t>(byte), 1};
    }

    std::size_t length = 0;
    char32_t code_point = 0;
    if ((byte & 0xE0) == 0xC0) {          // 110xxxxx
        length = 2;
        code_point = byte & 0x1F;
    } else if ((byte & 0xF0) == 0xE0) {   // 1110xxxx
        length = 3;
        code_point = byte & 0x0F;
    } else if ((byte & 0xF8) == 0xF0) {   // 11110xxx
        length = 4;
        code_point = byte & 0x07;
    } else {
        return {char32_t{0xFFFD}, 1};     // 非法起始字节
    }

    if (pos + length > text.size()) {
        return {char32_t{0xFFFD}, 1};
    }

    for (std::size_t i = 1; i < length; ++i) {
        const auto next = static_cast<unsigned char>(text[pos + i]);
        if ((next & 0xC0) != 0x80) {
            return {char32_t{0xFFFD}, 1};
        }
        code_point = (code_point << 6) | (next & 0x3F);
    }
    return {code_point, length};
}

std::string encode(char32_t code_point) {
    std::string out;
    if (code_point < 0x80) {
        out.push_back(static_cast<char>(code_point));
    } else if (code_point < 0x800) {
        out.push_back(static_cast<char>(0xC0 | (code_point >> 6)));
        out.push_back(static_cast<char>(0x80 | (code_point & 0x3F)));
    } else if (code_point < 0x10000) {
        out.push_back(static_cast<char>(0xE0 | (code_point >> 12)));
        out.push_back(static_cast<char>(0x80 | ((code_point >> 6) & 0x3F)));
        out.push_back(static_cast<char>(0x80 | (code_point & 0x3F)));
    } else {
        out.push_back(static_cast<char>(0xF0 | (code_point >> 18)));
        out.push_back(static_cast<char>(0x80 | ((code_point >> 12) & 0x3F)));
        out.push_back(static_cast<char>(0x80 | ((code_point >> 6) & 0x3F)));
        out.push_back(static_cast<char>(0x80 | (code_point & 0x3F)));
    }
    return out;
}

}  // namespace utf8
