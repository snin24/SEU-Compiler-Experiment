#include "ast.hpp"

#include <functional>
#include <string>
#include <vector>

namespace parser {

std::string format_tree(const TreeNode& root) {
    // 终结符结点显示 "ID (a)"，非终结符结点只显示符号名；ε 作为普通结点显示 "epsilon"
    const auto label = [](const TreeNode& node) -> std::string {
        if (!node.lexeme.has_value()) {
            return node.symbol;
        }
        return node.symbol + " (" + *node.lexeme + ")";
    };

    std::vector<std::string> lines;
    lines.push_back(label(root));

    // 先序遍历；prefix 累积父结点用的竖线与缩进，画法与 Python 版完全相同
    std::function<void(const TreeNode&, const std::string&)> visit =
        [&](const TreeNode& node, const std::string& prefix) {
            for (std::size_t i = 0; i < node.children.size(); ++i) {
                const bool last = (i + 1 == node.children.size());
                const TreeNode& child = *node.children[i];

                lines.push_back(prefix + (last ? "└── " : "├── ") + label(child));
                visit(child, prefix + (last ? "    " : "│   "));
            }
        };

    visit(root, "");

    std::string out;
    for (std::size_t i = 0; i < lines.size(); ++i) {
        if (i != 0) {
            out += '\n';
        }
        out += lines[i];
    }
    return out;
}

}  // namespace parser
