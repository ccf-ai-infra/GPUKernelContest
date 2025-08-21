#!/bin/bash

# TopkPair算法编译和运行脚本
# 用于参赛者只选择TopkPair题目时使用

# 加载公共配置
source "$(dirname "$0")/build_common.sh"

# 显示帮助信息
show_help() {
    echo "TopkPair算法编译和运行脚本"
    echo "用法: $0 [选项]"
    echo ""
    echo "选项:"
    echo "  --help              显示此帮助信息"
    echo "  --build-only        仅编译，不运行测试"
    echo "  --run MODE          编译并运行测试 (MODE: correctness|performance|all)"
    echo ""
    echo "示例:"
    echo "  $0                  # 编译并运行所有测试（默认行为）"
    echo "  $0 --build-only     # 仅编译"
    echo "  $0 --run correctness  # 编译并运行正确性测试"
    echo ""
}

# 解析命令行参数
RUN_MODE="all"  # 默认运行所有测试

while [[ $# -gt 0 ]]; do
    case $1 in
        --help)
            show_help
            exit 0
            ;;
        --build-only)
            RUN_MODE=""
            shift
            ;;
        --run)
            RUN_MODE="$2"
            shift 2
            ;;
        *)
            print_error "未知选项: $1"
            show_help
            exit 1
            ;;
    esac
done

# 编译TopkPair算法
if compile_algorithm "TopkPair" "topk_pair_algorithm.maca"; then
    # 如果指定了运行模式，则运行测试
    if [ -n "$RUN_MODE" ]; then
        echo ""
        run_single_test "TopkPair" "$RUN_MODE"
    fi
else
    exit 1
fi