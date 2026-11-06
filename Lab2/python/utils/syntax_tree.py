"""复用 LL(1) 分析表构造完整语法分析树。"""

from dataclasses import dataclass, field

from Lab1.python.utils.tokenize import tokenize
from .error import ParseError
from .parser_table import NON_TERMINAL, parsing_table


@dataclass
class TreeNode:
    symbol: str
    lexeme: str | None = None
    children: list["TreeNode"] = field(default_factory=list)


def LL_parse_tree(source):
    """返回 (语法分析树根节点, 产生式序列)，失败时抛出词法或语法异常。"""
    tokens = list(tokenize(source)) + [("EOF", "")]

    root = TreeNode("E")

    stack = [("EOF", None), ("E", root)]

    productions = []
    ptr = 0

    while stack:

        symbol, node = stack.pop()
        token_type, lexeme = tokens[ptr]

        if symbol == "EOF":
            if token_type != "EOF" or ptr != len(tokens) - 1:
                raise ParseError(ptr, "EOF", token_type)
            
            return root, productions

        if symbol in NON_TERMINAL:
            row = parsing_table[symbol]
            right = row.get(token_type)
            if right is None:
                raise ParseError(ptr, sorted(row), token_type, symbol=symbol)

            productions.append(f"{symbol} → {' '.join(right)}")

            node.children = [TreeNode(item) for item in right]
            
            if right != ["epsilon"]:
                pairs = list(zip(right, node.children))

                reverse_pairs = reversed(pairs)

                stack.extend(reverse_pairs)
        else:
            if symbol != token_type:
                raise ParseError(ptr, symbol, token_type)
            
            node.lexeme = lexeme
            ptr += 1

    raise ParseError(ptr, "EOF", "分析栈提前耗尽")


def format_tree(root):
    """返回完整语法分析树的文本，保留文法符号、词素和 epsilon。"""
    def label(node):
        if node.lexeme is None:
            return node.symbol
        
        return f"{node.symbol} ({node.lexeme})"

    lines = [label(root)]

    def visit(node, prefix):
        for index, child in enumerate(node.children):
            last = index == len(node.children) - 1

            lines.append(prefix + ("└── " if last else "├── ") + label(child))

            visit(child, prefix + ("    " if last else "│   "))

    visit(root, "")
    return "\n".join(lines)
