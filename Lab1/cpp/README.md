# 实验一：词法分析程序（C++17 实现）

> 导航：[仓库首页](../../README.md) · [实验一说明](../README.md) · 当前：C++ 实现 · [Python 实现](../python/README.md)

## 实验内容

把源程序的字符流切分为词法单元（Token）序列，每个单元由“类别 + 词素”组成；
遇到不属于任何合法词法单元的字符时报错。词类与正则式自定义，本实现支持的语言子集如下。

| 类别 | Token 类型 | 词素 / 正则式 |
|---|---|---|
| 关键字 | `IF` `THEN` `ELSE` `END` `REPEAT` `UNTIL` `READ` `WRITE` | `if` `then` `else` `end` `repeat` `until` `read` `write` |
| 标识符 | `ID` | `[a-zA-Z_][a-zA-Z0-9_]*` |
| 整数 | `NUM` | `[0-9]+` |
| 赋值 | `ASSIGN` | `:=` |
| 关系 | `RELOP` | `=` `<>` `>` `<` `>=` `<=` |
| 算术 | `PLUS` `MINUS` `TIMES` `OVER` | `+` `-` `*` `/` |
| 括号 | `LPAREN` `RPAREN` | `(` `)` |
| 分隔 | `SEMI` | `;` |
| 空白 | — | 空格、`\t`、`\n`、`\r`，跳过不输出 |

行为要点：

- 采用最长匹配与关键字回填：`if` → `IF`，`ifx` → `ID`；`>=` 不会被拆成 `>` 和 `=`；
- 空白字符只起分隔作用，不产生 Token；
- 遇到非法字符抛出词法错误，报告字符、位置（按字符计）、行号与列号。

## 设计与实现

每个词法类别对应一个 DFA，`recognizer.cpp` 中的函数直接模拟其状态转移：

| 识别器 | 对应自动机 | 说明 |
|---|---|---|
| `recognize_keyword_or_id` | 标识符 DFA（2 状态） | 首字符为字母或下划线，后续为字母、数字或下划线 |
| `recognize_multi_operator` | 运算符 DFA（向前看 1 个字符） | `:=`、`<>`、`<=`、`>=` 优先于单字符 |
| `recognize_number` | 整数 DFA（2 状态） | `[0-9]+` |
| `recognize_single_operator` | 单字符表 | `= - + * / ( ) ;` |

`tokenize` 按“标识符/关键字 → 双字符运算符 → 整数 → 单字符运算符”的优先级依次尝试，
某个识别器成功即前进并输出；全部失败则报词法错误。关键字表与单字符表分别用
`std::unordered_map` 实现。

## 文件

| 文件 | 说明 |
|---|---|
| `include/token.hpp` | 词法单元类型枚举 `TokenType` 与 `Token` 结构 |
| `include/recognizer.hpp` | 四个识别器与 `tokenize` 的接口 |
| `include/error.hpp` | `LexicalError` 异常（含位置、行、列） |
| `include/utf8.hpp`、`include/text_util.hpp` | UTF-8 码点计数、Python 风格 `repr`、控制台 UTF-8 |
| `src/recognizer.cpp` | 识别器与扫描循环 |
| `src/error.cpp` | 错误信息与行列号计算 |
| `src/token.cpp`、`src/utf8.cpp`、`src/text_util.cpp` | 类型名、UTF-8 工具、输出工具 |
| `src/main.cpp` | 演示入口（内置 7 组示例）与文件模式 |

## 编译

在实验根目录（本目录的上两级）用 g++ 编译：

```powershell
g++ -std=c++17 -O2 -Wall -Wextra -I Lab1/cpp/include -o Lab1/cpp/lab1.exe `
    Lab1/cpp/src/main.cpp Lab1/cpp/src/recognizer.cpp Lab1/cpp/src/token.cpp `
    Lab1/cpp/src/error.cpp Lab1/cpp/src/utf8.cpp Lab1/cpp/src/text_util.cpp
```

编译选项建议包含 `-Wall -Wextra -Wpedantic`，本实现无任何警告。

## 运行

```powershell
.\Lab1\cpp\lab1.exe
```

不带参数时运行 7 组内置示例，输出形如：

```text
基本表达式：'example_id := 123 + 45;'
(ID, "example_id")
(ASSIGN, ":=")
(NUM, "123")
(PLUS, "+")
(NUM, "45")
(SEMI, ";")
```

其中包含关键字与标识符、关系运算符、空白字符与非法字符的用例，最后一组示例的输出为：

```text
非法字符：'a := 10 @ 20'
词法错误：非法字符 '@'，位置 8（第 1 行，第 9 列）
```

## 文件模式

```powershell
.\Lab1\cpp\lab1.exe sample.txt
```

按文件内容输出词法单元序列；遇到非法字符时输出错误信息，退出码为 1。

## 与 Python 版的一致性

C++ 版与 Python 版的内置示例输出逐字节相同（Lab1 共 938 个字符）。可分别运行后比对：

```powershell
python Lab1\python\main.py > py.txt
.\Lab1\cpp\lab1.exe        > cpp.txt
fc.exe /b py.txt cpp.txt
```
