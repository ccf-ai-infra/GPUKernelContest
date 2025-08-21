#!/bin/bash

# GPU高性能并行计算算法优化竞赛 - 统一编译脚本
# 调用各个独立的build_and_run脚本来编译所有算法

# 加载公共配置
source "$(dirname "$0")/build_common.sh"

# 显示帮助信息
show_help() {
    echo "GPU算法竞赛统一编译和运行脚本"
    echo "用法: $0 [选项]"
    echo ""
    echo "选项:"
    echo "  --help              显示帮助信息"
    echo "  --build-only        仅编译所有算法，不运行测试"
    echo "  --run_reduce        编译并运行ReduceSum算法测试"
    echo "  --run_sort          编译并运行SortPair算法测试"
    echo "  --run_topk          编译并运行TopkPair算法测试"
    echo ""
    echo "示例:"
    echo "  $0                  # 编译并运行所有测试（默认行为）"
    echo "  $0 --build-only     # 仅编译所有算法"
    echo "  $0 --run_sort       # 编译并运行SortPair测试"
    echo ""
    echo "单独算法脚本:"
    echo "  ./build_and_run_reduce_sum.sh   # 仅处理ReduceSum"
    echo "  ./build_and_run_sort_pair.sh    # 仅处理SortPair"
    echo "  ./build_and_run_topk_pair.sh    # 仅处理TopkPair"
    echo ""
}

# 解析命令行参数
RUN_MODE="run_all"  # 默认为编译并运行所有测试

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
            RUN_MODE="run_reduce"
            shift
            ;;
        --run_sort)
            RUN_MODE="run_sort"
            shift
            ;;
        --run_topk)
            RUN_MODE="run_topk"
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

# 根据运行模式执行相应操作
case "$RUN_MODE" in
    "build_only")
        print_info "编译所有算法..."
        
        print_info "[1/3] 编译ReduceSum..."
        ./build_and_run_reduce_sum.sh --build-only
        if [ $? -ne 0 ]; then
            print_error "ReduceSum编译失败"
            exit 1
        fi
        
        print_info "[2/3] 编译SortPair..."
        ./build_and_run_sort_pair.sh --build-only
        if [ $? -ne 0 ]; then
            print_error "SortPair编译失败"
            exit 1
        fi
        
        print_info "[3/3] 编译TopkPair..."
        ./build_and_run_topk_pair.sh --build-only
        if [ $? -ne 0 ]; then
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
        
        print_info "[1/3] ReduceSum..."
        ./build_and_run_reduce_sum.sh --run all
        
        print_info "[2/3] SortPair..."
        ./build_and_run_sort_pair.sh --run all
        
        print_info "[3/3] TopkPair..."
        ./build_and_run_topk_pair.sh --run all
        
        print_success "所有测试完成!"
        ;;
        
    "run_reduce")
        print_info "编译并运行ReduceSum测试..."
        ./build_and_run_reduce_sum.sh --run all
        ;;
        
    "run_sort")
        print_info "编译并运行SortPair测试..."
        ./build_and_run_sort_pair.sh --run all
        ;;
        
    "run_topk")
        print_info "编译并运行TopkPair测试..."
        ./build_and_run_topk_pair.sh --run all
        ;;
esac