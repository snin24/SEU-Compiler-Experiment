# 实验一：词法分析程序（Python 实现）

> 导航：[仓库首页](../../README.md) · [实验一说明](../README.md) · [C++ 实现](../cpp/README.md) · 当前：Python 实现

## 实验内容

把源程序的字符流切分为词法单元（Token）序列，每个单元由“类别 + 词素”组成；
遇到非法字符时报错并给出位置。本实现是实验一的原始参考实现，算法代码未作修改，
支持的语言子集与 C++ 版完全一致：

| 类别 | Token 类型 | 词素 / 正则式 |
|---|---|---|
| 关键字 | `IF` `THEN` `ELSE` `END` `REPEAT` `UNTIL` `READ` `WRITE` | 8 个小写关键字 |
| 标识符 | `ID` | `[a-zA-Z_][a-zA-Z0-9_]*` |
| 整数 | `NUM` | `[0-9]+` |
| 赋值 | `ASSIGN` | `:=` |
| 关系 | `RELOP` | `=` `<>` `>` `<` `>=` `<=` |
| 算术 | `PLUS` `MINUS` `TIMES` `OVER` | `+` `-` `*` `/` |
| 括号 | `LPAREN` `RPAREN` | `(` `)` |
| 分隔 | `SEMI` | `;` |
| 空白 | — | 空格、`\t`、`\n`、`\r`，跳过 |

识别采用“最长匹配 + 关键字回填”，空白字符不产生 Token；非法字符由 `LexicalError`
报告字符、位置与行列号。

## 文件

| 文件 | 说明 |
|---|---|
| `__init__.py` | 包标记，使本目录代码可作为 `Lab1.python.*` 导入 |
| `main.py` | 演示入口，运行 7 组示例（基本表达式、关键字、关系运算符、全部关键字、括号、空白、非法字符） |
| `keywords.md` | 关键字与完整词法单元清单 |
| `utils/tokenize.py` | 扫描循环：跳过空白，按优先级依次调用四个识别器，全部失败则报错 |
| `utils/keywords_and_id.py` | 标识符 / 关键字识别（两状态 DFA + 关键字表回填） |
| `utils/multi_operator.py` | 双字符运算符 `:=` `<>` `>=` `<=` 及单字符 `<` `>`，向前看一个字符 |
| `utils/number.py` | 整数 `[0-9]+` 识别 |
| `utils/single_operator.py` | 单字符运算符与分隔符表 |
| `utils/error.py` | `LexicalError`：报告非法字符、位置、行号与列号 |

## 运行

在实验根目录（本目录的上两级）下：

```powershell
python Lab1\python\main.py
```

`main.py` 通过 `from utils.tokenize import tokenize` 导入同目录的模块，直接运行即可，
无需安装第三方库（Python 3.8 及以上）。

输出示例：

```text
基本表达式：'example_id := 123 + 45;'
(ID, "example_id")
(ASSIGN, ":=")
(NUM, "123")
(PLUS, "+")
(NUM, "45")
(SEMI, ";")
```

## 目录结构说明

实验目录采用 `Lab1/python`、`Lab2/python` 的分层结构，`python/` 与 `utils/` 下各有一个
`__init__.py`，把 Python 代码组织成标准包：`Lab1.python.utils.*`。实验二通过
`from Lab1.python.utils.tokenize import tokenize` 复用本实验的词法分析器。

## 与 C++ 版的一致性

C++ 版与 Python 版的内置示例输出逐字节相同（Lab1 共 938 个字符）。分别运行后比对：

```powershell
python Lab1\python\main.py > py.txt
.\Lab1\cpp\lab1.exe        > cpp.txt
fc.exe /b py.txt cpp.txt
```
