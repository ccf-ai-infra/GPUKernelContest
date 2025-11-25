#pragma once
#include <iostream>
#include <iomanip>
#include <string>

// ============================================================================
// 性能计算和显示工具
// ============================================================================

class PerformanceCalculator {
public:
    // ReduceSum性能计算
    struct ReduceSumMetrics {
        double throughput_gps;  // G elements/s
    };

    static ReduceSumMetrics calculateReduceSum(int size, float time_ms) {
        ReduceSumMetrics metrics;
        metrics.throughput_gps = (size / 1e9) / (time_ms / 1000.0);
        return metrics;
    }

    // SortPair性能计算
    struct SortPairMetrics {
        double throughput_gps;  // G elements/s  
    };

    static SortPairMetrics calculateSortPair(int size, float time_ms) {
        SortPairMetrics metrics;
        metrics.throughput_gps = (size / 1e9) / (time_ms / 1000.0);
        return metrics;
    }

    // TopkPair性能计算
    struct TopkPairMetrics {
        double throughput_gps;  // G elements/s
    };

    static TopkPairMetrics calculateTopkPair(int size, int k, float time_ms) {
        TopkPairMetrics metrics;
        metrics.throughput_gps = (size / 1e9) / (time_ms / 1000.0);
        return metrics;
    }
};

// ============================================================================
// 性能显示工具
// ============================================================================

class PerformanceDisplay {
public:
    // 显示ReduceSum性能表头
    static void printReduceSumHeader() {
        std::cout << "\nReduceSum 性能测试..." << std::endl;
        std::cout << "数据类型: float -> float" << std::endl;
        std::cout << "计算公式:" << std::endl;
        std::cout << "  吞吐量 = 元素数 / 时间(s) / 1e9 (G/s)" << std::endl;
        std::cout << std::setw(12) << "数据规模" << std::setw(15) << "时间(ms)" 
                  << std::setw(20) << "吞吐量(G/s)" << std::endl;
        std::cout << std::string(47, '-') << std::endl;
    }

    // 显示SortPair性能表头
    static void printSortPairHeader() {
        std::cout << "\nSortPair 性能测试..." << std::endl;
        std::cout << "数据类型: <float, uint32_t>" << std::endl;
        std::cout << "计算公式:" << std::endl;
        std::cout << "  吞吐量 = 元素数 / 时间(s) / 1e9 (G/s)" << std::endl;
        std::cout << std::setw(12) << "数据规模" << std::setw(15) << "升序(ms)" << std::setw(15) << "降序(ms)" 
                  << std::setw(16) << "升序(G/s)" << std::setw(16) << "降序(G/s)" << std::endl;
        std::cout << std::string(78, '-') << std::endl;
    }

    // 显示TopkPair性能表头  
    static void printTopkPairHeader() {
        std::cout << "\nTopkPair 性能测试..." << std::endl;
        std::cout << "数据类型: <float, uint32_t>" << std::endl;
        std::cout << "计算公式:" << std::endl;
        std::cout << "  吞吐量 = 元素数 / 时间(s) / 1e9 (G/s)" << std::endl;
    }

    static void printTopkPairDataHeader() {
        std::cout << std::setw(8) << "k值" << std::setw(15) << "升序(ms)" << std::setw(15) << "降序(ms)" 
                  << std::setw(16) << "升序(G/s)" << std::setw(16) << "降序(G/s)" << std::endl;
        std::cout << std::string(74, '-') << std::endl;
    }

    // 显示性能数据行
    static void printReduceSumData(int size, float time_ms, const PerformanceCalculator::ReduceSumMetrics& metrics) {
        std::cout << std::setw(12) << size << std::setw(15) << std::fixed << std::setprecision(3) 
                  << time_ms << std::setw(20) << std::setprecision(3) << metrics.throughput_gps << std::endl;
    }

    static void printSortPairData(int size, float asc_time, float desc_time, 
                                  const PerformanceCalculator::SortPairMetrics& asc_metrics,
                                  const PerformanceCalculator::SortPairMetrics& desc_metrics) {
        std::cout << std::setw(12) << size << std::setw(15) << std::fixed << std::setprecision(3) 
                  << asc_time << std::setw(15) << desc_time << std::setw(16) << std::setprecision(3) 
                  << asc_metrics.throughput_gps << std::setw(16) << desc_metrics.throughput_gps << std::endl;
    }

    static void printTopkPairData(int k, float asc_time, float desc_time,
                                  const PerformanceCalculator::TopkPairMetrics& asc_metrics,
                                  const PerformanceCalculator::TopkPairMetrics& desc_metrics) {
        std::cout << std::setw(8) << k << std::setw(15) << std::fixed << std::setprecision(3) 
                  << asc_time << std::setw(15) << desc_time << std::setw(16) << std::setprecision(3) 
                  << asc_metrics.throughput_gps << std::setw(16) << desc_metrics.throughput_gps << std::endl;
    }

    // 显示性能文件保存消息
    static void printSavedMessage(const std::string& filename) {
        std::cout << "\n性能结果已保存到: " << filename << std::endl;
    }
};