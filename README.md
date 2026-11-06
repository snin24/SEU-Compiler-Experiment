# 编译原理实验

本仓库是《编译原理》课程的实验材料，包含两个实验：

| 实验 | 题目 | 说明文档 |
|---|---|---|
| 实验一 | 词法分析程序 | [Lab1/README.md](Lab1/README.md) |
| 实验二 | 语法分析程序（LL(1)） | [Lab2/README.md](Lab2/README.md) |

每个实验都提供两份等价的实现：

- **C++17**：重写实现，位于 `Lab1/cpp/`、`Lab2/cpp/`；
- **Python**：原始参考实现，位于 `Lab1/python/`、`Lab2/python/`，算法代码未作修改。

两版程序的内置示例输出逐字节一致。

## 阅读指引

1. 先看实验目录下的 `README.md`，了解该实验的题目、要求与总体设计；
2. 再按需进入 `cpp/README.md` 或 `python/README.md`，查看该语言下的实现细节、编译运行方法与输出示例。

| 实验 | 实验说明 | C++ 实现 | Python 实现 |
|---|---|---|---|
| 实验一 | [Lab1/README.md](Lab1/README.md) | [Lab1/cpp/README.md](Lab1/cpp/README.md) | [Lab1/python/README.md](Lab1/python/README.md) |
| 实验二 | [Lab2/README.md](Lab2/README.md) | [Lab2/cpp/README.md](Lab2/cpp/README.md) | [Lab2/python/README.md](Lab2/python/README.md) |

## 目录结构

```text
实验/
├── README.md                  本文件：仓库总览与实验跳转
├── Lab1/                      实验一：词法分析程序
│   ├── README.md              实验说明（题目、要求、两份实现的跳转）
│   ├── cpp/                   C++17 实现，README.md 为详细说明
│   └── python/                Python 参考实现，README.md 为详细说明
└── Lab2/                      实验二：语法分析程序（LL(1)）
    ├── README.md              实验说明（题目、文法、两份实现的跳转）
    ├── cpp/                   C++17 实现，README.md 为详细说明
    └── python/                Python 参考实现，README.md 为详细说明

```

## 快速开始

编译命令见各语言目录的 README：[Lab1/cpp](Lab1/cpp/README.md)、[Lab2/cpp](Lab2/cpp/README.md)。
两个实验都已带有编译好的可执行文件，可直接运行：

```powershell
.\Lab1\cpp\lab1.exe          # 实验一：词法分析器
.\Lab2\cpp\lab2.exe          # 实验二：语法分析器
```

Python 版：

```powershell
python Lab1\python\main.py
python Lab2\python\main.py
```

## 环境要求

| 项目 | 要求 |
|---|---|
| C++ | 支持 C++17 的 g++（开发环境为 MSYS2 UCRT64 的 g++ 15.2.0） |
| Python | Python 3.8 及以上（无需第三方库） |
