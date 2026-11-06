"""完整语法分析树演示入口：可直接 Run Code。"""

from pathlib import Path
import sys

if __package__ in (None, ""):
    # 目录结构为 Lab1/python、Lab2/python，故上溯到包含 Lab1、Lab2 的实验根目录
    sys.path.insert(0, str(Path(__file__).resolve().parent.parent.parent))

from Lab1.python.utils.error import LexicalError
from Lab2.python.utils.error import ParseError
from Lab2.python.utils.syntax_tree import LL_parse_tree, format_tree


def main():
    # 当前文法支持表达式，不包含赋值、语句、关系运算和一元负号。
    # 每个例子独立分析；遇到错误后继续下一组。
    examples = [
        ("基本加法", "a + 3"),
        ("乘法优先级", "a + b * c"),
        ("多行与括号", """(_temp + 12)
    * (count1 - 3) / 2"""),
        ("语法错误：缺少右括号", "(a + 3"),
        ("词法错误：非法字符", "a @ 3"),
    ]

    for case_index, (title, source) in enumerate(examples, start=1):
        print(f"\n{'=' * 60}")
        print(f"例 {case_index}：{title}")
        print(f"输入：{source!r}")
        try:
            root, productions = LL_parse_tree(source)
        except (LexicalError, ParseError) as error:
            print(error)
            continue

        print("\n产生式序列：")
        for index, production in enumerate(productions, start=1):
            print(f"{index}. {production}")
        print("\n完整语法分析树：")
        print(format_tree(root))


if __name__ == "__main__":
    main()
