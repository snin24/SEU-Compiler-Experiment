# 实验二：语法分析程序（Python 实现，LL(1)）

> 导航：[仓库首页](../../README.md) · [实验二说明](../README.md) · [C++ 实现](../cpp/README.md) · 当前：Python 实现

## 实验内容

对词法分析得到的 Token 序列做语法分析，输出最左推导的产生式序列与语法分析树。
本实现是实验二的原始参考实现，采用 LL(1) 预测分析，文法与输出格式与 C++ 版一致。

| 项目 | 要求 |
|---|---|
| 输入 | 字符流（内部先经词法分析得到 Token 序列） |
| 输出 | 产生式序列 + 语法分析树 |
| 文法 | 算术表达式文法（见下） |
| 错误处理 | 分析表查不到、终结符不匹配、输入多余时给出错误信息 |

### 文法

```text
E  -> T E'
E' -> + T E' | - T E' | ε
T  -> F T'
T' -> * F T' | / F T' | ε
F  -> ( E ) | id | num
```

该文法满足 LL(1) 条件，完整的 FIRST/FOLLOW 集与预测分析表见
[实验二说明](../README.md)。

### 输出

- **产生式序列**：按最左推导的展开顺序编号输出；
- **语法分析树**：缩进树形输出，终结符带词素（如 `ID (a)`），空产生式显示 `epsilon`。

## 文件

| 文件 | 说明 |
|---|---|
| `__init__.py` | 包标记，使本目录代码可作为 `Lab2.python.*` 导入 |
| `main.py` | 演示入口，运行 5 组示例（基本加法、乘法优先级、多行与括号、缺少右括号、非法字符） |
| `utils/parser_table.py` | LL(1) 预测分析表与文法（`E/T/F` 及带撇的非终结符） |
| `utils/syntax_tree.py` | 表驱动分析主循环、语法树构造与树形格式化 |
| `utils/error.py` | `ParseError`：记录出错 token、期望符号与实际符号 |

## 运行

在实验根目录（本目录的上两级）下：

```powershell
python Lab2\python\main.py
```

输出示例：

```text
============================================================
例 1：基本加法
输入：'a + 3'

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

## 复用实验一的词法分析器

`utils/syntax_tree.py` 通过 `from Lab1.python.utils.tokenize import tokenize` 复用实验一的词法分析器，
`main.py` 通过 `sys.path` 让实验根目录进入模块搜索路径。

目录结构为 `Lab1/python`、`Lab2/python`，Python 代码按标准包组织：`Lab1.python.utils`、
`Lab2.python.utils`（各级目录下的 `__init__.py` 为包标记）。相对原实现，只调整了
`main.py` 中 `sys.path` 的上溯层级（一行）与导入语句中的包名，算法代码与输出不变。

## 与 C++ 版的一致性

C++ 版与 Python 版的内置示例输出逐字节相同（Lab2 共 2957 个字符）。分别运行后比对：

```powershell
python Lab2\python\main.py > py.txt
.\Lab2\cpp\lab2.exe        > cpp.txt
fc.exe /b py.txt cpp.txt
```
