"""语法错误：预测分析无法继续时抛出，终止当前输入的分析。"""


def _fmt(symbols):
    if isinstance(symbols, (list, tuple, set)):
        return " / ".join(str(s) for s in symbols)
    return str(symbols)


class ParseError(ValueError):
    """记录出错位置、期望符号和实际符号；参数全可省，`raise ParseError()` 也合法。

    index    : 出错 token 的下标（0 起），即 token_types[index]
    expected : 期望的符号；可以是单个符号，也可以是可选符号的集合
    actual   : 实际读到的 token 类型
    symbol   : 若因分析表查不到而产生，记录栈顶的非终结符；终结符对不上时留空
    """

    def __init__(self, index=None, expected=None, actual=None, symbol=None):
        self.index = index
        self.expected = expected
        self.actual = actual
        self.symbol = symbol

        where = "" if index is None else f"第 {index + 1} 个 token "

        if symbol is not None and expected is not None and actual is not None:
            detail = f"非终结符 {symbol} 无法用 {actual} 展开，可选 {_fmt(expected)}"
        elif expected is not None and actual is not None:
            detail = f"期望 {_fmt(expected)}，实际读到 {actual}"
        elif actual is not None:
            detail = f"读到 {actual}，无法继续分析"
        elif expected is not None:
            detail = f"期望 {_fmt(expected)}"
        else:
            detail = "无法继续分析"

        super().__init__(f"语法错误：{where}{detail}")
