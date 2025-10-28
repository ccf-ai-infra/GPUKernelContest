# 子赛题三、GPU 算子优化挑战赛

欢迎参加 **GPU 算子优化挑战赛** 🎯！  
本比赛旨在通过优化深度学习框架中的核心计算模块，提升大模型的运行效率。在本仓库中，你可以提交你的优化代码、测试样例和使用说明。

---

## 🧠 比赛背景简介

随着大语言模型（LLM, 如 ChatGPT 等）的广泛使用，其在运行推理时对计算资源的要求越来越高，出现了所谓的 “三高” 问题：

- **高延迟**：响应慢
- **高显存占用**：消耗大量显存
- **高生态依赖**：对系统和软件依赖复杂

本次挑战赛正是为了解决这些问题，鼓励选手**优化 GPU 上的底层算子（即最基础的数学运算模块）**，提高模型推理效率。

---

## 🔧 挑战方向

本次挑战主要有两个技术方向：

### Test Time Scaling 算子优化  
- 对深度学习框架中的基础算子（如 PyTorch 或 PaddlePaddle 中的矩阵乘法）进行性能优化。
- **GEMM（General Matrix Multiplication）** 是大模型中的核心计算操作。  
- 目标是对其内核（Kernel）进行精细调优，提高执行效率。

👉 **项目目标：在不更换硬件的前提下，让推理速度提升 30%！**

---

## 🚀 快速上手

本竞赛旨在评估参赛者在GPU并行计算领域的算法优化能力。为了快速让参赛者进入比赛状态，我们提供了三个核心算法的高性能版本参考，供参赛选手不断优化性能：
- **ReduceSum**: 高精度归约求和
- **SortPair**: 键值对稳定排序
- **TopkPair**: 键值对TopK选择

[三个核心算法赛题模板](https://gitlink.org.cn/ccf-ai-infra/GPUKernelContest/tree/main/cp_template)

### 📥 选手赛题准备

1. 点击 **[创建赛题](https://gitlink.org.cn/ccf-ai-infra/GPUKernelContest/issues/new)** ，并记录赛题的ID
2. 算力平台启动一个MACA3.0.0.4+PyTorch2.4.0的容器算力，详细步骤参考： **[算力平台使用说明](https://ai.gitee.com/docs/compute/container)**
3. 用ssh或者vscode进入容器环境，Clone自己Fork的仓库
   ```bash
   git clone https://gitlink.com/gitlinkuseid/GPUKernelContest.git
   ```
   > 备注：`gitlinkuseid` 替换为您的URL。
4. 进入参赛项目，创建以赛题ID为名称的目录。例如：[赛题3](https://gitlink.org.cn/ccf-ai-infra/GPUKernelContest/issues/3)
   ```bash
   # 进入Clone的仓库
   cd GPUKernelContest
   # 创建以赛题ID为名称的目录
   mkdir -p S1/3
   ```
   ```
   # 创建后的目录结构如下:
   GPUKernelContest
   ├── cp_template(说明：赛目模板目录)
   ├── S1(说明：第一季比赛名)
   │   ├── 3(说明：以自己创建赛题ID命名目录存放自己需要提交的内容)
   ```
4. Fork仓库并初始化比赛环境（三个核心算法题优化赛题以外自定义的赛题需有入口run.sh脚本，供CI自动测试验证）
   1. 拷贝赛题样例`cp_template`到赛题`3`目录
   ```bash
   # cp -r cp_template/* S1/3
   ```
   2. 拷贝后的目录结构如下：
   ```
   ├── S1(说明：第一季比赛名)
   │   ├── 3(说明：以自己创建赛题ID命名目录存放自己需要提交的内容)
   |   |   ├── utils
   │   |   ├── reduce_sum_algorithm.maca
   │   |   ├── run.sh（说明：作为CI自动测试验证的入口）
   |   |   └── sort_pair_algorithm.maca
   |   |   └── topk_pair_algorithm.maca
   │   └── ……
        ```

### 编译和测试

选手赛题目录内提供了编译、测试的脚本，供选手熟悉比赛环境，步骤如下：

```bash
# ！！！注意参赛选手需要根据自己的赛题ID进入自己完成题目的目录！！！！
cd S1/3
```

#### 1. 编译和运行

编译并运行所有算法测试（默认行为），如下：
```bash
./run.sh
```

单个或几个赛题测试验证，修改`run.sh`脚本，详细如下：
```bash
#!/bin/bash

# 单个赛题测试验证(ReduceSum算法)
./build_and_run.sh --run_reduce
```

编译运行单个ReduceSum测试如下：
```bash
./run.sh  # ReduceSum算法
```

#### 2. 手动运行测试

```bash
# 仅编译所有算法，不运行测试
./build_and_run.sh --build-only

# 单个运行不同算法的测试
./build/test_reducesum [correctness|performance|all]
./build/test_sortpair [correctness|performance|all]
./build/test_topkpair [correctness|performance|all]
```

对于如何提交可参考：[如何贡献](https://gitlink.org.cn/ccf-ai-infra/GPUKernelContest/tree/main/how-to-contribute.md)

### ✅ 参赛要求：
- 提交内容必须可以在MACA软件上运行。
- 所提交的优化代码将由主办方审核，**需成功合并（Merge）到赛事官方仓库，才算有效提交。**

### 📦 提交内容包含：
- 算子优化后的代码
- 可运行的测试用例
- 使用说明文档

---

## 📈 评分机制

每次合并的提交会按以下规则评分，[mcTileLang](https://gitee.com/metax-maca/mcTileLang)详见[mcTileLang](docs/Tilelang/TileLang.md)：

### 🎯 基础得分（Level）：
| 等级 | 内容描述 | 分值 |
|------|----------|------|
| Level 1 | 优化一个 PyTorch或Paddle 算子 / 验证[mcTileLang](https://gitee.com/metax-maca/mcTileLang)的docs文件夹下的文档并提交验证结果的截图到对应的issue | 5 分 |
| Level 2 | 融合优化 2~9 个算子 / 迁移[mcTileLang](https://gitee.com/metax-maca/mcTileLang)的docs文件夹下的文档已有的与CUDA相关的文档到MACA，并给对应的文档提交PR | 10 分 |
| Level 3 | 为[mcTileLang](https://gitee.com/metax-maca/mcTileLang)的docs/deeplearning_operators文件夹下未编写文档的算子提交算子解读的PR/补充和修复已有文档 | 20 分 |
| Level 4 | 含 MMA（多维矩阵乘）融合算子 / 用于大模型推理的复杂融合算子 / 给开源仓库[mcTileLang](https://gitee.com/metax-maca/mcTileLang)提交example文件夹下的代码PR | 50 分 |
| 合并至MACA开源项目仓库的每个PR | 参考：[mcTVM](https://github.com/metax-maca/mcTVM),[mcTileLang](https://gitee.com/metax-maca/mcTileLang) | 50 分 |

> 注释事项，非AI Infra组下的项目PR需在赛题Issue中提供合并记录，并确保和参赛时使用邮箱一致的提交邮箱方为有效。

### ✨ 加分项：
| 内容 | 分值 |
|------|------|
| 代码规范、清晰 | +10 分 |
| 性能优化明显 | +10 分 |
| 记录优化过程、说明模型来源 | +20 分 |
| 使用 LLM Prompt 自动生成代码及样例 | +20 分 |

**注意：** 加分项只针对于在基础得分相同的情况下通过加分项来区分不同的排名和后续优秀选手的参考。

---

## 🏆 排名机制

1. 评委评分从高到低排序
2. **评估规则：** 取前 12 名作为最终获奖选手
3. 若基础得分相同：
  - 加分项多者优先
  - 提交数量多者优先
  - 提交时间早者优先
4. 当同一参赛选手在本赛题有多个赛题的提交时，多个赛题计算累计得分

---

## 📚 参考MACA开源项目仓库

你可以参考以下项目仓库，了解算子开发与提交格式。如果为[GitHub](https://github.com/orgs/MetaX-MACA/repositories)、[Gitee](https://gitee.com/organizations/metax-maca/projects)仓库里面的项目提出一个好的Issue可以获得算力券的激励。
如：
- [mcTVM](https://github.com/MetaX-MACA/mcTVM)
- [FlashMLA](https://github.com/MetaX-MACA/FlashMLA)
- [mcEigen](https://github.com/MetaX-MACA/mcEigen)
- [mcPytorch](https://github.com/MetaX-MACA/mcPytorch)

---

## 💡 术语解释

- **算子（Operator）**：指深度学习框架中的基本计算模块，例如矩阵乘法、卷积等。
- **GEMM**：全称 General Matrix Multiplication，一种用于矩阵计算的核心算法，是大模型中的基础运算。
- **MMA**：Matrix Multiply-Accumulate，多维矩阵乘加运算，适用于复杂计算加速。
- **LLM**：Large Language Model，大语言模型，如 GPT、BERT 等。
- **推理（Inference）**：模型训练完成后，用来“预测”或“使用”的过程。
- **Prompt**：用于引导大模型生成特定内容的输入提示词。
- **PR（Pull Request）**：在 Git 仓库中提交你的修改请求，供维护者审查后合并。

---

## 📬 联系与帮助

如需帮助或有疑问，请联系主办方或在项目中发起 Issue。

祝你挑战顺利，优化出更快的大模型推理体验！🚀
