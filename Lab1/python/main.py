from utils.error import LexicalError
from utils.tokenize import tokenize


def main():
    examples = [
        ("基本表达式", "example_id := 123 + 45;"),
        ("关键字与标识符", "if ifx then example_id"),
        ("关系运算符", "a >= 10; b <> 20; c <= 30; d = 40;"),
        ("所有关键字", "if then else end repeat until read write"),
        ("括号与算术运算符", "(a + b) * c / 2 - 1;"),
        ("空白字符", "read\tx;\r\n  write x\n"),
        ("非法字符", "a := 10 @ 20"),
    ]
    for title, source in examples:
        print(f"\n{title}：{source!r}")
        try:
            for token_type, lexeme in tokenize(source):
                print(f'({token_type}, "{lexeme}")')
        except LexicalError as error:
            print(error)


if __name__ == "__main__":
    main()
