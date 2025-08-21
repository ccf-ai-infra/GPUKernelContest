#pragma once
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <chrono>
#include <iomanip>
#include <sstream>

// ============================================================================
// YAML性能报告生成器
// ============================================================================

class YAMLPerformanceReporter {
public:
    struct PerformanceData {
        std::string algorithm;
        std::string input_type;
        std::string output_type;
        std::string key_type;
        std::string value_type;
        std::vector<std::map<std::string, std::string>> metrics;
    };

    // 创建性能数据条目
    static std::map<std::string, std::string> createEntry() {
        return std::map<std::string, std::string>();
    }

    // 生成ReduceSum性能YAML
    static void generateReduceSumYAML(const std::vector<std::map<std::string, std::string>>& perf_data, 
                                      const std::string& filename = "reduce_sum_performance.yaml") {
        std::ofstream yaml_file(filename);
        
        // 写入头部信息
        writeHeader(yaml_file, "ReduceSum算法性能测试结果");
        
        // 算法信息
        yaml_file << "algorithm: \"ReduceSum\"\n";
        yaml_file << "data_types:\n";
        yaml_file << "  input: \"float\"\n";
        yaml_file << "  output: \"float\"\n";
        
        // 计算公式
        yaml_file << "formulas:\n";
        yaml_file << "  throughput: \"elements / time(s) / 1e9 (G/s)\"\n";
        
        // 性能数据
        yaml_file << "performance_data:\n";
        for (const auto& data : perf_data) {
            yaml_file << "  - data_size: " << data.at("data_size") << "\n";
            yaml_file << "    time_ms: " << formatFloat(data.at("time_ms")) << "\n";
            yaml_file << "    throughput_gps: " << formatFloat(data.at("throughput_gps")) << "\n";
            yaml_file << "    data_type: \"" << data.at("data_type") << "\"\n";
        }
        
        yaml_file.close();
    }

    // 生成SortPair性能YAML
    static void generateSortPairYAML(const std::vector<std::map<std::string, std::string>>& perf_data,
                                     const std::string& filename = "sort_pair_performance.yaml") {
        std::ofstream yaml_file(filename);
        
        // 写入头部信息
        writeHeader(yaml_file, "SortPair算法性能测试结果");
        
        // 算法信息
        yaml_file << "algorithm: \"SortPair\"\n";
        yaml_file << "data_types:\n";
        yaml_file << "  key_type: \"float\"\n";
        yaml_file << "  value_type: \"uint32_t\"\n";
        
        // 计算公式
        yaml_file << "formulas:\n";
        yaml_file << "  throughput: \"elements / time(s) / 1e9 (G/s)\"\n";
        
        // 性能数据
        yaml_file << "performance_data:\n";
        for (const auto& data : perf_data) {
            yaml_file << "  - data_size: " << data.at("data_size") << "\n";
            yaml_file << "    ascending:\n";
            yaml_file << "      time_ms: " << formatFloat(data.at("asc_time_ms")) << "\n";
            yaml_file << "      throughput_gps: " << formatFloat(data.at("asc_throughput_gps")) << "\n";
            yaml_file << "    descending:\n";
            yaml_file << "      time_ms: " << formatFloat(data.at("desc_time_ms")) << "\n";
            yaml_file << "      throughput_gps: " << formatFloat(data.at("desc_throughput_gps")) << "\n";
            yaml_file << "    key_type: \"" << data.at("key_type") << "\"\n";
            yaml_file << "    value_type: \"" << data.at("value_type") << "\"\n";
        }
        
        yaml_file.close();
    }

    // 生成TopkPair性能YAML
    static void generateTopkPairYAML(const std::vector<std::map<std::string, std::string>>& perf_data,
                                     const std::string& filename = "topk_pair_performance.yaml") {
        std::ofstream yaml_file(filename);
        
        // 写入头部信息
        writeHeader(yaml_file, "TopkPair算法性能测试结果");
        
        // 算法信息
        yaml_file << "algorithm: \"TopkPair\"\n";
        yaml_file << "data_types:\n";
        yaml_file << "  key_type: \"float\"\n";
        yaml_file << "  value_type: \"uint32_t\"\n";
        
        // 计算公式
        yaml_file << "formulas:\n";
        yaml_file << "  throughput: \"elements / time(s) / 1e9 (G/s)\"\n";
        
        // 性能数据
        yaml_file << "performance_data:\n";
        for (const auto& data : perf_data) {
            yaml_file << "  - data_size: " << data.at("data_size") << "\n";
            yaml_file << "    k_value: " << data.at("k_value") << "\n";
            yaml_file << "    ascending:\n";
            yaml_file << "      time_ms: " << formatFloat(data.at("asc_time_ms")) << "\n";
            yaml_file << "      throughput_gps: " << formatFloat(data.at("asc_throughput_gps")) << "\n";
            yaml_file << "    descending:\n";
            yaml_file << "      time_ms: " << formatFloat(data.at("desc_time_ms")) << "\n";
            yaml_file << "      throughput_gps: " << formatFloat(data.at("desc_throughput_gps")) << "\n";
            yaml_file << "    key_type: \"" << data.at("key_type") << "\"\n";
            yaml_file << "    value_type: \"" << data.at("value_type") << "\"\n";
        }
        
        yaml_file.close();
    }

private:
    // 写入YAML文件头部
    static void writeHeader(std::ofstream& file, const std::string& title) {
        file << "# " << title << "\n";
        file << "# 生成时间: ";
        
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        file << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
        file << "\n\n";
    }

    // 格式化浮点数
    static std::string formatFloat(const std::string& value) {
        try {
            double d = std::stod(value);
            std::ostringstream oss;
            oss << std::fixed << std::setprecision(6) << d;
            return oss.str();
        } catch (...) {
            return value;
        }
    }
};