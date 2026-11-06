# 实验二：语法分析程序（C++17 实现，LL(1)）

> 导航：[仓库首页](../../README.md) · [实验二说明](../README.md) · 当前：C++ 实现 · [Python 实现](../python/README.md)

## 实验内容

对词法分析得到的 Token 序列做语法分析，输出产生式序列与完整语法分析树。
本实现采用 **LL(1)** 方法：由预测分析表驱动，用显式栈完成无回溯的自顶向下分析。

| 项目 | 要求 |
|---|---|
| 输入 | 字符流（内部先经词法分析得到 Token 序列） |
| 输出 | 最左推导的产生式序列 + 完整语法分析树 |
| 文法 | 算术表达式文法（见下） |
| 错误处理 | 分析表查不到、终结符不匹配、输入多余，均给出 token 序号、期望符号与实际符号 |

### 文法

原始文法含左递归，消除左递归后为：

```text
E  -> T E'
E' -> + T E' | - T E' | ε
T  -> F T'
T' -> * F T' | / F T' | ε
F  -> ( E ) | id | num
```

### FIRST / FOLLOW 集

| 非终结符 | FIRST | FOLLOW |
|---|---|---|
| `E` | `( id num` | `) $` |
| `E'` | `+ - ε` | `) $` |
| `T` | `( id num` | `+ - ) $` |
| `T'` | `* / ε` | `+ - ) $` |
| `F` | `( id num` | `* / + - ) $` |

### LL(1) 预测分析表

| | ID | NUM | PLUS | MINUS | TIMES | OVER | LPAREN | RPAREN | EOF |
|---|---|---|---|---|---|---|---|---|---|
| **E** | E→TE' | E→TE' | | | | | E→TE' | | |
| **E'** | | | E'→+TE' | E'→-TE' | | | | E'→ε | E'→ε |
| **T** | T→FT' | T→FT' | | | | | T→FT' | | |
| **T'** | | | T'→ε | T'→ε | T'→\*FT' | T'→/FT' | | T'→ε | T'→ε |
| **F** | F→id | F→num | | | | | F→(E) | | |

### 文法假设

- 只处理表达式，不含赋值语句、`if`/`repeat` 语句与语句序列；
- 表达式必须完整，分析完须到达输入末尾；
- 采用“遇错即停”策略，一次报告第一个错误。

## 设计与实现

分析主循环（`src/parser.cpp` 的 `parse_expression`）：

1. 调用词法分析器得到 Token 序列，末尾追加 `EOF` 哨兵；
2. 栈中压入 `EOF` 与开始符号 `E`，并为 `E` 建立根结点；
3. 反复弹出栈顶：非终结符则查表 `M[X, a]`，把产生式右部逆序压栈并建立子结点；
   终结符则与当前输入比对，匹配成功就把词素写入结点并前移输入指针；
4. 栈底 `EOF` 弹出时，要求输入恰好读完，否则报语法错误。

语法树用 `std::unique_ptr` 管理子树，随根结点自动释放。打印采用先序递归 + 前缀累积，
非末子结点后继续画竖线，从而得到与 Python 版完全相同的树形。

## 文件

| 文件 | 说明 |
|---|---|
| `include/parsing_table.hpp` | LL(1) 预测分析表 `PARSING_TABLE` |
| `include/ast.hpp` | 语法树结点 `TreeNode` 与 `format_tree` |
| `include/parse_error.hpp` | `ParseError`（含期望/实际符号） |
| `include/parser.hpp` | `parse_expression` 接口与结果结构 |
| `src/parser.cpp` | 表驱动分析主循环 |
| `src/parsing_table.cpp` | 分析表与终止符判断 |
| `src/ast.cpp` | 树的格式化输出 |
| `src/parse_error.cpp` | 错误信息拼装 |
| `src/main.cpp` | 演示入口（内置 5 组示例）与文件模式 |

## 编译

在实验根目录（本目录的上两级）用 g++ 编译（需要同时编译 Lab1 的词法分析器）：

```powershell
g++ -std=c++17 -O2 -Wall -Wextra -I Lab1/cpp/include -I Lab2/cpp/include `
    -o Lab2/cpp/lab2.exe `
    Lab2/cpp/src/main.cpp Lab2/cpp/src/parser.cpp Lab2/cpp/src/parsing_table.cpp `
    Lab2/cpp/src/ast.cpp Lab2/cpp/src/parse_error.cpp `
    Lab1/cpp/src/recognizer.cpp Lab1/cpp/src/token.cpp Lab1/cpp/src/error.cpp `
    Lab1/cpp/src/utf8.cpp Lab1/cpp/src/text_util.cpp
```

## 运行

```powershell
.\Lab2\cpp\lab2.exe
```

输出形如：

```text
============================================================
例 2：乘法优先级
输入：'a + b * c'

产生式序列：
1. E → T E'
2. T → F T'
...

完整语法分析树：
E
├── T
│   ├── F
│   │   └── ID (a)
...
```

两个错误用例：

```text
例 4：语法错误：缺少右括号
输入：'(a + 3'
语法错误：第 5 个 token 期望 RPAREN，实际读到 EOF

例 5：词法错误：非法字符
输入：'a @ 3'
词法错误：非法字符 '@'，位置 2（第 1 行，第 3 列）
```

## 文件模式

```powershell
.\Lab2\cpp\lab2.exe sample.txt
```

分析文件中的表达式，输出产生式序列与语法分析树；出错时输出错误信息，退出码为 1。

## 与 Python 版的一致性

C++ 版与 Python 版的内置示例输出逐字节相同（Lab2 共 2957 个字符）。可分别运行后比对：

```powershell
python Lab2\python\main.py > py.txt
.\Lab2\cpp\lab2.exe        > cpp.txt
fc.exe /b py.txt cpp.txt
```
