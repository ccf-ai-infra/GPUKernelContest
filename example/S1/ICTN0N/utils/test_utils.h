#pragma once
#include <vector>
#include <random>
#include <algorithm>
#include <mc_runtime.h>
#include <maca_fp16.h>
#include <iostream>
#include <chrono>
#include <cmath>

// 引入模块化头文件
#include "yaml_reporter.h"
#include "performance_utils.h"

// ============================================================================
// 测试配置常量
// ============================================================================
#ifndef RUN_FULL_TEST
const int TEST_SIZES[] = {1000000, 134217728}; // 1M, 128M, 512M, 1G
#else
const int TEST_SIZES[] = {1000000, 134217728, 536870912, 1073741824}; // 1M, 128M, 512M, 1G
#endif

const int NUM_TEST_SIZES = sizeof(TEST_SIZES) / sizeof(TEST_SIZES[0]);

// 性能测试重复次数
constexpr int WARMUP_ITERATIONS = 5;
constexpr int BENCHMARK_ITERATIONS = 10;


// ============================================================================
// 错误检查宏
// ============================================================================
#define MACA_CHECK(call) \
    do { \
        mcError_t error = call; \
        if (error != mcSuccess) { \
            std::cerr << "MACA error at " << __FILE__ << ":" << __LINE__ \
                      << " - " << mcGetErrorString(error) << std::endl; \
            exit(1); \
        } \
    } while(0)

// ============================================================================
// 测试数据生成器
// ============================================================================
class TestDataGenerator {
private:
    std::mt19937 rng;
    
public:
    TestDataGenerator(uint32_t seed = 42) : rng(seed) {}
    
    // 生成随机float数组
    std::vector<float> generateRandomFloats(int size, float min_val = -1000.0f, float max_val = 1000.0f) {
        std::vector<float> data(size);
        std::uniform_real_distribution<float> dist(min_val, max_val);
        for (int i = 0; i < size; i++) {
            data[i] = dist(rng);
        }
        return data;
    }
    
    // 生成随机half数组
    std::vector<half> generateRandomHalfs(int size, float min_val = -100.0f, float max_val = 100.0f) {
        std::vector<half> data(size);
        std::uniform_real_distribution<float> dist(min_val, max_val);
        for (int i = 0; i < size; i++) {
            data[i] = __float2half(dist(rng));
        }
        return data;
    }
    
    // 生成随机uint32_t数组
    std::vector<uint32_t> generateRandomUint32(int size) {
        std::vector<uint32_t> data(size);
        for (int i = 0; i < size; i++) {
            data[i] = static_cast<uint32_t>(i); // 使用索引作为值，便于验证稳定排序
        }
        return data;
    }
    
    // 生成随机int64_t数组
    std::vector<int64_t> generateRandomInt64(int size) {
        std::vector<int64_t> data(size);
        for (int i = 0; i < size; i++) {
            data[i] = static_cast<int64_t>(i);
        }
        return data;
    }
    
    // 生成包含NaN和Inf的测试数据 (half版本)
    std::vector<half> generateSpecialHalfs(int size) {
        std::vector<half> data = generateRandomHalfs(size, -10.0f, 10.0f);
        if (size > 100) {
            data[10] = __float2half(NAN);
            data[20] = __float2half(INFINITY);
            data[30] = __float2half(-INFINITY);
        }
        return data;
    }
    
    // 生成包含NaN和Inf的测试数据 (float版本)
    std::vector<float> generateSpecialFloats(int size) {
        std::vector<float> data = generateRandomFloats(size, -10.0f, 10.0f);
        if (size > 100) {
            data[10] = NAN;
            data[20] = INFINITY;
            data[30] = -INFINITY;
        }
        return data;
    }
};

// ============================================================================
// 性能测试工具
// ============================================================================
class PerformanceMeter {
private:
    mcEvent_t start, stop;
    
public:
    PerformanceMeter() {
        MACA_CHECK(mcEventCreate(&start));
        MACA_CHECK(mcEventCreate(&stop));
    }
    
    ~PerformanceMeter() {
        mcEventDestroy(start);
        mcEventDestroy(stop);
    }
    
    void startTiming() {
        MACA_CHECK(mcEventRecord(start));
    }
    
    float stopTiming() {
        MACA_CHECK(mcEventRecord(stop));
        MACA_CHECK(mcEventSynchronize(stop));
        float milliseconds = 0;
        MACA_CHECK(mcEventElapsedTime(&milliseconds, start, stop));
        return milliseconds;
    }
};

// ============================================================================
// 正确性验证工具
// ============================================================================
template<typename T>
bool compareArrays(const std::vector<T>& a, const std::vector<T>& b, double tolerance = 1e-6) {
    if (a.size() != b.size()) return false;
    
    for (size_t i = 0; i < a.size(); i++) {
        if constexpr (std::is_same_v<T, half>) {
            float fa = __half2float(a[i]);
            float fb = __half2float(b[i]);
            if (std::isnan(fa) && std::isnan(fb)) continue;
            if (std::isinf(fa) && std::isinf(fb) && (fa > 0) == (fb > 0)) continue;
            if (std::abs(fa - fb) > tolerance) return false;
        } else if constexpr (std::is_floating_point_v<T>) {
            if (std::isnan(a[i]) && std::isnan(b[i])) continue;
            if (std::isinf(a[i]) && std::isinf(b[i]) && (a[i] > 0) == (b[i] > 0)) continue;
            if (std::abs(a[i] - b[i]) > tolerance) return false;
        } else {
            if (a[i] != b[i]) return false;
        }
    }
    return true;
}

// CPU参考实现 - 稳定排序
template<typename KeyType, typename ValueType>
void cpuSortPair(std::vector<KeyType>& keys, std::vector<ValueType>& values, bool descending) {
    std::vector<std::pair<KeyType, ValueType>> pairs;
    for (size_t i = 0; i < keys.size(); i++) {
        pairs.emplace_back(keys[i], values[i]);
    }
    
    if (descending) {
        std::stable_sort(pairs.begin(), pairs.end(), 
            [](const auto& a, const auto& b) { return a.first > b.first; });
    } else {
        std::stable_sort(pairs.begin(), pairs.end());
    }
    
    for (size_t i = 0; i < pairs.size(); i++) {
        keys[i] = pairs[i].first;
        values[i] = pairs[i].second;
    }
}

// CPU参考实现 - TopK
template<typename KeyType, typename ValueType>
void cpuTopkPair(const std::vector<KeyType>& keys_in, const std::vector<ValueType>& values_in,
                 std::vector<KeyType>& keys_out, std::vector<ValueType>& values_out,
                 int k, bool descending) {
    std::vector<std::pair<KeyType, ValueType>> pairs;
    for (size_t i = 0; i < keys_in.size(); i++) {
        pairs.emplace_back(keys_in[i], values_in[i]);
    }
    
    if (descending) {
        std::stable_sort(pairs.begin(), pairs.end(), 
            [](const auto& a, const auto& b) { return a.first > b.first; });
    } else {
        std::stable_sort(pairs.begin(), pairs.end());
    }
    
    keys_out.resize(k);
    values_out.resize(k);
    for (int i = 0; i < k; i++) {
        keys_out[i] = pairs[i].first;
        values_out[i] = pairs[i].second;
    }
}

// CPU参考实现 - ReduceSum (使用double精度)
template<typename InputT>
double cpuReduceSum(const std::vector<InputT>& data, double init_value) {
    double sum = init_value;
    for (const auto& val : data) {
        if constexpr (std::is_same_v<InputT, half>) {
            float f_val = __half2float(val);
            if (!std::isnan(f_val)) {
                sum += static_cast<double>(f_val);
            }
        } else {
            if (!std::isnan(val)) {
                sum += static_cast<double>(val);
            }
        }
    }
    return sum;
}
