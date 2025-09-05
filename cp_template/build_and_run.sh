#!/bin/bash

# GPU高性能并行计算算法优化竞赛 - 统一编译和运行脚本
# 整合了所有算法的编译、运行和公共配置

# ============================================================================
# 公共配置和工具函数
# ============================================================================

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
COMPILER_FLAGS=${COMPILER_FLAGS:-"-O3 -std=c++17 --extended-lambda -DRUN_FULL_TEST"} 

# ***** 这里是关键修改点1：头文件目录 *****
# 现在头文件在 utils/ 目录下
HEADER_DIR=${HEADER_DIR:-utils} 

# ***** 这里是关键修改点2：源文件目录 *****
# 现在源文件在 ./ 目录下
SOURCE_CODE_DIR=${SOURCE_CODE_DIR:-} 

BUILD_DIR=${BUILD_DIR:-build}

# 编译单个算法的通用函数
# 参数: $1=算法名称, $2=源文件名（不含路径）
compile_algorithm() {
    local algo_name="$1"
    local source_file_name="$2" # 例如 "reduce_sum_algorithm.maca"
    local target_file="$BUILD_DIR/test_${algo_name,,}"  # 转换为小写
    
    print_info "编译 $algo_name 算法..."
    
    # 创建构建目录
    mkdir -p "$BUILD_DIR"
    
    # ***** 这里是关键修改点3：编译命令 *****
    # -I$HEADER_DIR 用于告诉编译器头文件在哪里
    # $SOURCE_CODE_DIR/$source_file_name 用于指定要编译的源文件的完整路径
    local compile_cmd="$COMPILER $COMPILER_FLAGS -I$HEADER_DIR $source_file_name -o $target_file"
    
    print_info "执行: $compile_cmd"
    
    if $compile_cmd; then
        print_success "$algo_name 编译完成!"
        echo ""
        echo "运行测试:"
        echo "   ./$target_file [correctness|performance|all]"
        return 0
    else
        print_error "$algo_name 编译失败!"
        return 1
    fi
}

# 显示编译配置信息
show_build_config() {
    print_info "编译配置:"
    echo "   COMPILER: $COMPILER"
    echo "   COMPILER_FLAGS: $COMPILER_FLAGS"
    echo "   HEADER_DIR: $HEADER_DIR" # 显示头文件目录
    echo "   SOURCE_CODE_DIR: $SOURCE_CODE_DIR" # 显示源文件目录
    echo "   BUILD_DIR: $BUILD_DIR"
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

# ============================================================================
# 主脚本逻辑
# ============================================================================

# 显示帮助信息 (整合了所有选项)
show_help() {
    echo "GPU算法竞赛统一编译和运行脚本"
    echo "用法: $0 [选项]"
    echo ""
    echo "选项:"
    echo "  --help              显示帮助信息"
    echo "  --build-only        仅编译所有算法，不运行测试"
    echo "  --run_reduce [MODE] 编译并运行ReduceSum算法测试 (MODE: correctness|performance|all, 默认all)"
    echo "  --run_sort [MODE]   编译并运行SortPair算法测试 (MODE: correctness|performance|all, 默认all)"
    echo "  --run_topk [MODE]   编译并运行TopkPair算法测试 (MODE: correctness|performance|all, 默认all)"
    echo ""
    echo "示例:"
    echo "  $0                  # 编译并运行所有测试（默认行为）"
    echo "  $0 --build-only     # 仅编译所有算法"
    echo "  $0 --run_sort performance # 编译并运行SortPair性能测试"
    echo ""
}

# 解析命令行参数
RUN_MODE="run_all"  # 默认为编译并运行所有测试
ALGO_TO_RUN=""      # 记录要运行的单个算法
SINGLE_ALGO_TEST_MODE="all" # 单个算法的测试模式

while [[ $# -gt 0 ]]; do
    case $1 in
        --help)
            show_help
            exit 0
            ;;
        --build-only)
            RUN_MODE="build_only"
            shift
            ;;
        --run_reduce)
            RUN_MODE="run_single"
            ALGO_TO_RUN="ReduceSum"
            if [[ -n "$2" && "$2" != --* ]]; then
                SINGLE_ALGO_TEST_MODE="$2"
                shift
            fi
            shift
            ;;
        --run_sort)
            RUN_MODE="run_single"
            ALGO_TO_RUN="SortPair"
            if [[ -n "$2" && "$2" != --* ]]; then
                SINGLE_ALGO_TEST_MODE="$2"
                shift
            fi
            shift
            ;;
        --run_topk)
            RUN_MODE="run_single"
            ALGO_TO_RUN="TopkPair"
            if [[ -n "$2" && "$2" != --* ]]; then
                SINGLE_ALGO_TEST_MODE="$2"
                shift
            fi
            shift
            ;;
        *)
            print_error "未知选项: $1"
            show_help
            exit 1
            ;;
    esac
done

if [ "$RUN_MODE" = "build_only" ]; then
    print_info "开始编译所有算法..."
else
    print_info "开始编译并运行所有算法..."
fi
print_info "工作目录: $(pwd)"
print_info "编译时间: $(date '+%Y-%m-%d %H:%M:%S')"
show_build_config

# 清理构建目录
if [ -d "$BUILD_DIR" ]; then
    print_info "清理现有构建目录: $BUILD_DIR"
    rm -rf "$BUILD_DIR"
fi

# 核心逻辑：根据 RUN_MODE 执行操作
case "$RUN_MODE" in
    "build_only")
        print_info "编译所有算法..."
        
        # 直接调用 compile_algorithm 函数
        print_info "[1/3] 编译ReduceSum..."
        if ! compile_algorithm "ReduceSum" "reduce_sum_algorithm.maca"; then
            print_error "ReduceSum编译失败"
            exit 1
        fi
        
        print_info "[2/3] 编译SortPair..."
        if ! compile_algorithm "SortPair" "sort_pair_algorithm.maca"; then
            print_error "SortPair编译失败"
            exit 1
        fi
        
        print_info "[3/3] 编译TopkPair..."
        if ! compile_algorithm "TopkPair" "topk_pair_algorithm.maca"; then
            print_error "TopkPair编译失败"
            exit 1
        fi
        
        print_success "所有算法编译完成!"
        echo ""
        echo "可执行文件:"
        echo "  $BUILD_DIR/test_reducesum    - ReduceSum算法测试"
        echo "  $BUILD_DIR/test_sortpair     - SortPair算法测试"
        echo "  $BUILD_DIR/test_topkpair     - TopkPair算法测试"
        echo ""
        echo "使用方法:"
        echo "  ./$BUILD_DIR/test_reducesum [correctness|performance|all]"
        echo "  ./$BUILD_DIR/test_sortpair [correctness|performance|all]"
        echo "  ./$BUILD_DIR/test_topkpair [correctness|performance|all]"
        ;;
        
    "run_all")
        print_info "编译并运行所有算法测试..."
        
        # 直接调用 compile_algorithm 和 run_single_test 函数
        print_info "[1/3] ReduceSum..."
        if compile_algorithm "ReduceSum" "reduce_sum_algorithm.maca"; then
            run_single_test "ReduceSum" "all"
        else
            exit 1
        fi
        
        print_info "[2/3] SortPair..."
        if compile_algorithm "SortPair" "sort_pair_algorithm.maca"; then
            run_single_test "SortPair" "all"
        else
            exit 1
        fi
        
        print_info "[3/3] TopkPair..."
        if compile_algorithm "TopkPair" "topk_pair_algorithm.maca"; then
            run_single_test "TopkPair" "all"
        else
            exit 1
        fi
        
        print_success "所有测试完成!"
        ;;
        
    "run_single")
        print_info "编译并运行 ${ALGO_TO_RUN} 测试 (模式: ${SINGLE_ALGO_TEST_MODE})..."
        local source_file_name=""
        case "$ALGO_TO_RUN" in
            "ReduceSum") source_file_name="reduce_sum_algorithm.maca" ;;
            "SortPair") source_file_name="sort_pair_algorithm.maca" ;;
            "TopkPair") source_file_name="topk_pair_algorithm.maca" ;;
        esac

        if compile_algorithm "$ALGO_TO_RUN" "$source_file_name"; then
            run_single_test "$ALGO_TO_RUN" "$SINGLE_ALGO_TEST_MODE"
        else
            exit 1
        fi
        ;;
esac
