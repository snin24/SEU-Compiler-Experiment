"""错误策略：抛出异常，停止当前输入的分析。"""


class LexicalError(ValueError):
    def __init__(self, source, pos):
        self.character = source[pos] if 0 <= pos < len(source) else ""
        self.position = pos
        prefix = source[:max(pos, 0)].replace("\r\n", "\n").replace("\r", "\n")
        self.line = prefix.count("\n") + 1
        self.column = len(prefix.rsplit("\n", 1)[-1]) + 1
        detail = f"非法字符 {self.character!r}" if self.character else "输入意外结束"
        super().__init__(
            f"词法错误：{detail}，位置 {pos}"
            f"（第 {self.line} 行，第 {self.column} 列）"
        )

