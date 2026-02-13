# TileLang Puzzle 练习指南

本指南将帮助你通过 TileLang Puzzle 练习，掌握 TileLang 编程基础。

---

## 什么是 TileLang Puzzle？

TileLang Puzzles 是一系列编程练习，旨在帮助你学习 [TileLang](https://github.com/tile-ai/tilelang)——一种用于开发高性能深度学习算子的领域特定语言（DSL）。

练习从简单的示例开始，逐步过渡到现代算子如 GEMM 和 FlashAttention，帮助你全面理解 TileLang 的设计原理。

---

## 环境准备

### 前置要求

确保你已经完成了 [mcTileLang 源码安装](./mctilelang-install.md)。

### 验证环境

运行以下命令检查 TileLang 是否安装正确：

```bash
python -c "import tilelang; print(tilelang.__version__)"
```

---

## 获取 Puzzle 代码

### 克隆仓库

```bash
git clone https://github.com/tile-ai/tilelang-puzzles.git
cd tilelang-puzzles
```

### 目录结构

```
tilelang-puzzles/
├── puzzles/          # Puzzle 题目（需要你来完成）
├── ans/              # 参考答案
├── scripts/          # 辅助脚本
└── README.md
```

---

## Puzzle 列表

共有 10 个 Puzzle，难度递增：

| 编号 | 名称 | 描述 | 难度 |
|------|------|------|------|
| 01 | copy | 张量拷贝 | ⭐ |
| 02 | vector-add | 向量加法 | ⭐ |
| 03 | outer-vec-add | 外积向量加法 | ⭐ |
| 04 | backward-op | 反向传播算子 | ⭐ |
| 05 | reduce-sum | 归约求和 | ⭐ |
| 06 | softmax | Softmax 计算 | ⭐⭐ |
| 07 | scalar-flash-attn | 标量 Flash Attention | ⭐⭐ |
| 08 | matrix | 矩阵运算 | ⭐⭐ |
| 09 | conv | 卷积运算 | ⭐⭐ |
| 10 | dequant-mm | 反量化矩阵乘法 | ⭐⭐⭐ |

---

## 如何练习

### 1. 运行 Puzzle

每个 Puzzle 都是一个独立的可执行脚本：

```bash
# 运行 Puzzle 01
python3 puzzles/01-copy.py
```

### 2. 查看参考答案

如果遇到困难，可以参考 `ans/` 目录下的实现：

```bash
# 查看 Puzzle 01 的参考答案
python3 ans/01-copy.py
```

### 3. 理解代码

每个 Puzzle 文件里面包含：
- **问题描述**：注释中说明需要实现的功能
- **待填充代码**：标记为 `# TODO` 的部分
- **测试代码**：验证你的实现是否正确

---

## 提交方式

将完成的 Puzzle 提交到本仓库的 `spring_tilelang/submissions/YOUR_ID/` 目录下。

提交目录结构示例：
```
spring_tilelang/
└── submissions/
    └── YOUR_ID/
        ├── puzzles/           # 完成的 Puzzle 代码
        ├── screenshots/       # 运行截图
        └── notes.md           # 学习笔记
```

---

## 学习建议

### 循序渐进

1. **从简单开始**：先完成 01-03 号 Puzzle，熟悉 TileLang 语法
2. **理解原理**：不要直接看答案，先自己思考实现方案
3. **对比学习**：完成后再对比参考答案，学习更优写法
4. **记录笔记**：记录每个 Puzzle 的核心概念和技巧

### 核心概念

在练习过程中，重点关注以下概念：

- **Tile**：TileLang 的核心抽象，表示数据块
- **Kernel**：GPU 上执行的函数
- **Schedule**：调度策略，影响性能
- **Memory Layout**：内存布局，影响数据访问效率

### 调试技巧

- 使用 `print` 输出中间结果
- 从小规模数据开始测试
- 对比参考答案的输出来定位问题

---

## 参考资源

- [TileLang 官方仓库](https://github.com/tile-ai/tilelang)
- [TileLang Puzzles 仓库](https://github.com/tile-ai/tilelang-puzzles)
- [Triton Puzzles](https://github.com/srush/Triton-Puzzles) - 类似的 Triton 练习
- [LeetGPU](https://leetgpu.com/) - GPU 编程练习平台

---

## 常见问题

### Q: 运行 Puzzle 时报错怎么办？

A: 首先检查 TileLang 是否正确安装，然后检查 GPU 是否可用。可以运行 `python3 scripts/check_tilelang_env.py` 检查环境。

### Q: 如何优化我的实现？

A: 可以尝试：
- 调整 Tile 大小
- 优化内存访问模式
- 使用更高效的算法

---

祝你练习愉快，早日掌握 TileLang！
