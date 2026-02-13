# mcTileLang 源码编译教程

本教程将指导你完成 mcTileLang 的源码编译安装。

> **环境说明**
> - 硬件：曦云 C500
> - PyTorch：2.8
> - Python：3.12
> - Maca：3.3.0.4

---

## 0. 必选：安装 cmake

```bash
apt-get update && apt-get install -y cmake
```

---

## 1. 安装依赖

### 1.1 基础依赖包

```bash
pip install z3-solver>=4.13.0 cython psutil cloudpickle tqdm
```

### 1.2 从 Maca PyPI 安装 PyTorch 相关包

```bash
pip install apex dropout_layer_norm flash_attn fused_dense_lib \
    rotary_emb torch torchaudio torchvision triton \
    xentropy_cuda_lib xformers \
    -i https://repos.metax-tech.com/r/maca-pypi/simple \
    --trusted-host repos.metax-tech.com
```

### 1.3 安装 tvm-ffi 依赖

```bash
pip install torch-c-dlpack-ext
```

---

## 2. 从源码构建

### 2.1 克隆代码

```bash
git clone https://gitee.com/metax-maca/mcTileLang/
cd mcTileLang
```

### 2.2 初始化子模块

```bash
git submodule update --init --recursive
```

### 2.3 配置 git（首次使用需要）

```bash
git config --global user.email "you@example.com"
git config --global user.name "Your Name"
```

### 2.4 必备构建工具

```bash
apt-get install -y cmake patchelf
```

### 2.5 CMake 配置

```bash
USE_MACA=ON cmake -B build
```

### 2.6 编译

```bash
make -C build -j 32
```

> 注：`-j 32` 表示使用 32 个线程并行编译，可根据 CPU 核心数调整

### 2.7 安装 tvm-ffi

```bash
cd 3rdparty/tvm/3rdparty/tvm-ffi && pip install . && cd -
```

---

## 3. 运行验证

### 3.1 添加 tilelang 到 PYTHONPATH

```bash
export PYTHONPATH=/path/to/mcTileLang
```

将 `/path/to/mcTileLang` 替换为实际的 mcTileLang 路径，例如：

```bash
export PYTHONPATH=/root/mcTileLang
```

### 3.2 验证导入

```bash
python -c "import tilelang; print(tilelang.__version__)"
```

### 3.3 运行简单示例

```bash
python examples/quickstart.py
```

---

## 4. 任务提交

完成安装后，请提交以下材料：

1. **安装成功截图**：运行 `python -c "import tilelang; print(tilelang.__version__)"` 的输出截图
2. **示例运行截图**：运行 `python examples/quickstart.py` 的输出截图
3. **遇到的问题与解决方案**（如有）：记录安装过程中遇到的问题及解决方法

将上述材料打包提交到本仓库的 `spring_tilelang/submissions/YOUR_ID/` 目录下。

提交目录结构示例：
```
spring_tilelang/
└── submissions/
    └── YOUR_ID/
        ├── screenshots/       # 安装成功截图
        ├── notes.md           # 学习文档（可选）
        └── README.md          # 说明文档
```

---

## 常见问题

| 问题 | 解决方案 |
|------|---------|
| `cmake: command not found` | `apt-get install -y cmake` |
| `patchelf not found` | `apt-get install -y patchelf` |
| git 身份验证错误 | 配置 `git config --global user.email` 和 `user.name` |
| pip 权限警告 | 可忽略，或使用虚拟环境 |

---

## 参考资源

- [mcTileLang 官方仓库](https://gitee.com/metax-maca/mcTileLang)
- [mcTileLang 文档](https://gitee.com/metax-maca/mcTileLang/tree/main/docs)
- [TileLang 官方仓库](https://github.com/tile-ai/tilelang)
