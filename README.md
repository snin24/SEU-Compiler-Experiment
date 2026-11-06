# 编译原理实验

> **持续更新中**：本仓库随课程进度不断补充。

本仓库是东南大学计算机科学与技术专业《编译原理》课程的实验材料。每个实验都提供两份等价的实现：

- **C++17**：AI重写实现，位于各实验的 `cpp/`；
- **Python**：原始参考实现，位于各实验的 `python/`。

两版程序的内置示例输出一致。

## 实验进度

| 实验 | 题目 | 状态 | 说明文档 |
|---|---|---|---|
| 实验一 | 词法分析程序 | 已完成 | [Lab1/README.md](Lab1/README.md) |
| 实验二 | 语法分析程序（LL(1)） | 已完成 | [Lab2/README.md](Lab2/README.md) |
| 实验三及以后 | 按课程进度补充 | 待更新 | — |

## 阅读指引

1. 实验目录下的 `README.md`，记录实验的题目、要求与总体设计；
2. 实验分别用 C++ 和 Python 实现相同功能。在 `cpp/README.md` 或 `python/README.md` 中查看实现细节、编译运行方法与输出示例。

| 实验 | 实验说明 | C++ 实现 | Python 实现 |
|---|---|---|---|
| 实验一 | [Lab1/README.md](Lab1/README.md) | [Lab1/cpp/README.md](Lab1/cpp/README.md) | [Lab1/python/README.md](Lab1/python/README.md) |
| 实验二 | [Lab2/README.md](Lab2/README.md) | [Lab2/cpp/README.md](Lab2/cpp/README.md) | [Lab2/python/README.md](Lab2/python/README.md) |
| 后续实验 | 待补充 | 待补充 | 待补充 |

## 目录结构

```text
实验/
├── README.md                  本文件：仓库总览、实验跳转与更新说明
├── Lab1/                      实验一：词法分析程序
│   ├── README.md              实验说明（题目、要求、两份实现的跳转）
│   ├── assets/                状态机（DFA）图片
│   ├── cpp/                   C++17 实现，README.md 为详细说明
│   └── python/                Python 参考实现，README.md 为详细说明
├── Lab2/                      实验二：语法分析程序（LL(1)）
│   ├── README.md              实验说明（题目、文法、两份实现的跳转）
│   ├── cpp/                   C++17 实现，README.md 为详细说明
│   └── python/                Python 参考实现，README.md 为详细说明
└── ......                     持续更新后续实验
```

## 环境要求

| 项目 | 要求 |
|---|---|
| C++ | 支持 C++17 的 g++（开发环境为 MSYS2 UCRT64 的 g++ 15.2.0） |
| Python | Python 3.8 及以上（无需第三方库） |
