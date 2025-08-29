# GPU 高性能并行计算算法优化竞赛

## 🎯 竞赛概述

本竞赛旨在评估参赛者在GPU并行计算领域的算法优化能力。参赛者可选择实现三个核心算法的高性能版本：
- **ReduceSum**: 高精度归约求和
- **SortPair**: 键值对稳定排序
- **TopkPair**: 键值对TopK选择

## 🚀 快速开始

### 编译和测试

#### 1. 全量编译和运行
```bash
# 编译并运行所有算法测试（默认行为）
./build_and_run.sh

# 仅编译所有算法，不运行测试
./build_and_run.sh --build-only

# 编译并运行单个算法测试
./build_and_run.sh --run_reduce   # ReduceSum算法
./build_and_run.sh --run_sort     # SortPair算法
./build_and_run.sh --run_topk     # TopkPair算法
```

#### 2. 单独编译和运行
```bash
# 编译并运行ReduceSum算法（默认行为）
./build_and_run_reduce_sum.sh

# 仅编译ReduceSum算法，不运行测试
./build_and_run_reduce_sum.sh --build-only

# 编译并运行SortPair正确性测试
./build_and_run_sort_pair.sh --run correctness

# 编译并运行TopkPair性能测试
./build_and_run_topk_pair.sh --run performance
```

#### 3. 手动运行测试
```bash
./build/test_reducesum [correctness|performance|all]
./build/test_sortpair [correctness|performance|all]
./build/test_topkpair [correctness|performance|all]
```

## 📝 参赛指南

### 实现位置
参赛者需要在以下文件中替换Thrust实现：
- `src/reduce_sum_algorithm.maca` - 替换Thrust归约求和
- `src/sort_pair_algorithm.maca` - 替换Thrust稳定排序
- `src/topk_pair_algorithm.maca` - 替换Thrust TopK选择

### 算法要求
见competition_parallel_algorithms.md

## 📊 性能评测

### 测试流程
1. **Warmup**: 5次预热运行
2. **Benchmark**: 10次正式测试取平均
3. **数据规模**: 1M, 128M, 512M, 1G elements
4. **评估指标**: 吞吐量(G/s)

### 性能指标计算

#### ReduceSum
- **数据类型**: float → float
- **吞吐量**: elements / time(s) / 1e9 (G/s)

#### SortPair
- **数据类型**: <float, uint32_t>
- **吞吐量**: elements / time(s) / 1e9 (G/s)

#### TopkPair  
- **数据类型**: <float, uint32_t>
- **吞吐量**: elements / time(s) / 1e9 (G/s)

### 性能结果文件
每个算法会生成详细的YAML性能分析文件：
- `reduce_sum_performance.yaml` - ReduceSum性能数据
- `sort_pair_performance.yaml` - SortPair性能数据
- `topk_pair_performance.yaml` - TopkPair性能数据

这些文件包含：
- 算法信息和数据类型
- 计算公式说明
- 各数据规模的详细性能数据
- 升序/降序分别统计（适用时）

## 📁 项目结构

```
├── build_and_run.sh                # 统一编译和运行脚本（默认编译+运行所有算法）
├── build_common.sh                  # 公共编译配置和函数
├── build_and_run_reduce_sum.sh     # ReduceSum独立编译和运行脚本
├── build_and_run_sort_pair.sh      # SortPair独立编译和运行脚本
├── build_and_run_topk_pair.sh      # TopkPair独立编译和运行脚本
├── competition_parallel_algorithms.md  # 详细题目说明
├── src/                            # 算法实现和工具文件
│   ├── reduce_sum_algorithm.maca     # 1. ReduceSum测试程序
│   ├── sort_pair_algorithm.maca      # 2. SortPair测试程序
│   ├── topk_pair_algorithm.maca      # 3. TopkPair测试程序
│   ├── test_utils.h               # 测试工具和CPU参考实现
│   ├── yaml_reporter.h            # YAML性能报告生成器
│   └── performance_utils.h        # 性能测试工具
├── final_results/reduce_sum_results.yaml  #ReduceSum性能数据
├── final_results/sort_pair_results.yaml   #替换Thrust稳定排序
└── final_results/topk_pair_results.yaml   #TopkPair性能数据         
```

## 🔧 开发工具

### 编译选项
```bash
# 默认编译命令
mxcc -O3 -std=c++17 --extended-lambda -Isrc

### 自动化测试
```bash
# 查看所有选项
./build.sh --help

# 运行所有测试并生成YAML报告
./build.sh --run_all

### 环境变量配置

| 变量 | 默认值 | 说明 |
|--------|--------|------|
| `COMPILER` | `mxcc` | CUDA编译器路径 |
| `COMPILER_FLAGS` | `-O3 -std=c++17 --extended-lambda` | 编译标志 |
| `INCLUDE_DIR` | `src` | 头文件目录 |
| `BUILD_DIR` | `build` | 构建输出目录 |

### 调试模式

## 📋 提交清单

在提交前请确保：
- [ ] 所有算法通过正确性测试
- [ ] 性能测试可以正常运行
- [ ] 代码注释清晰，说明优化策略
- [ ] 无内存泄漏或运行时错误
- [ ] 生成完整测试报告
- [ ] 在函数实现注释中说明创新点

# 提交时包含以下文件
# - final_results/reduce_sum_results.yaml
# - final_results/sort_pair_results.yaml
# - final_results/topk_pair_results.yaml
```

## 🤝 技术支持

如有技术问题，请：
1. 查看详细错误信息和GPU状态
2. 确认环境配置正确
3. 检查内存使用是否超限
4. 验证算法逻辑和数据类型

---

**祝您在竞赛中取得优异成绩！** 🏆
