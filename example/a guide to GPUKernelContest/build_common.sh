#!/bin/bash

# GPU算法竞赛公共编译配置
# 被各个独立编译脚本调用

# 设置颜色
RED='\033[0;31m'
GREEN='\033[0;32m'
BLUE='\033[0;34m'
YELLOW='\033[0;33m'
NC='\033[0m' # No Color

# 打印函数
print_info() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

print_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

# 编译配置 - 可通过环境变量自定义
COMPILER=${COMPILER:-mxcc}

#COMPILER_FLAGS=${COMPILER_FLAGS:--O3 -std=c++17 --extended-lambda} # not run all test for easy debug
COMPILER_FLAGS=${COMPILER_FLAGS:--O3 -std=c++17 --extended-lambda -DRUN_FULL_TEST} 

INCLUDE_DIR=${INCLUDE_DIR:-src}
BUILD_DIR=${BUILD_DIR:-build}

# 编译单个算法的通用函数
# 参数: $1=算法名称, $2=源文件名
compile_algorithm() {
    local algo_name="$1"
    local source_file="$2"
    local target_file="$BUILD_DIR/test_${algo_name,,}"  # 转换为小写
    
    print_info "编译 $algo_name 算法..."
    
    # 创建构建目录
    mkdir -p "$BUILD_DIR"
    
    # 编译命令
    local compile_cmd="$COMPILER $COMPILER_FLAGS -I$INCLUDE_DIR src/$source_file -o $target_file"
    
    print_info "执行: $compile_cmd"
    
    if $compile_cmd; then
        print_success "$algo_name 编译完成!"
        echo ""
        echo "运行测试:"
        echo "  ./$target_file [correctness|performance|all]"
        return 0
    else
        print_error "$algo_name 编译失败!"
        return 1
    fi
}

# 显示编译配置信息
show_build_config() {
    print_info "编译配置:"
    echo "  COMPILER: $COMPILER"
    echo "  COMPILER_FLAGS: $COMPILER_FLAGS"
    echo "  INCLUDE_DIR: $INCLUDE_DIR"
    echo "  BUILD_DIR: $BUILD_DIR"
    echo ""
}

# 运行单个测试
run_single_test() {
    local algo_name="$1"
    local test_mode="${2:-all}"
    local test_file="$BUILD_DIR/test_${algo_name,,}"
    
    if [ -f "$test_file" ]; then
        print_info "运行 $algo_name 测试 (模式: $test_mode)..."
        "./$test_file" "$test_mode"
        return $?
    else
        print_error "$algo_name 测试程序不存在: $test_file"
        return 1
    fi
}
