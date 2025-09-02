# 题目：
## GPU高性能并行计算算法优化

要求参赛者通过一个或多个global kernel 函数（允许配套 device 辅助函数），实现高性能算法。

在正确性、稳定性前提下，比拼算法性能。


# 1. ReduceSum算法优化
```cpp
template <typename InputT = float, typename OutputT = float>
class ReduceSumAlgorithm {
public:
    // 主要接口函数 - 参赛者需要实现这个函数
    void reduce(const InputT* d_in, OutputT* d_out, int num_items, OutputT init_value) {
        // TODO
    }
};
```
其中

* 数据类型：InputT: float,  OutputT: float
* 系统将测试评估1M, 128M, 512M, 1G element number下的算法性能
* 假定输入d\_in数据量为num\_items


注意事项

* 累计误差不大于cpu double golden基准的0.5%
* 注意针对NAN和INF等异常值的处理



加分项

* 使用tensor core计算reduce
* 覆盖更全面的数据范围，提供良好稳定的性能表现


# 2. Sort Pair算法优化
```cpp
template <typename KeyType, typename ValueType>
class SortPairAlgorithm {
public:
    // 主要接口函数 - 参赛者需要实现这个函数
    void sort(const KeyType* d_keys_in, KeyType* d_keys_out,
              const ValueType* d_values_in, ValueType* d_values_out,
              int num_items, bool descending) {
                // TODO
              }
};
```
其中

* 数据类型：key: float, value: int32\_t
* 系统将测试评估1M, 128M, 512M, 1G element number下的算法性能
* 假定输入、输出的key和value的数据量一致，均为num\_items


注意事项

* 需要校验结果正确性
* 结果必须稳定排序


加分项

* 支持其他不同数据类型的排序，如half、double、int32_t等
* 覆盖更全面的数据范围，提供良好稳定的性能表现



# 3. Topk Pair算法优化
```cpp
template <typename KeyType, typename ValueType>
class TopkPairAlgorithm {
public:
    // 主要接口函数 - 参赛者需要实现这个函数
    void topk(const KeyType* d_keys_in, KeyType* d_keys_out,
              const ValueType* d_values_in, ValueType* d_values_out,
              int num_items, int k, bool descending) {
              // TODO
              }
};
```
其中

* 数据类型：key: float, value: int32\_t
* 系统将测试评估1M, 128M, 512M, 1G element number下的算法性能
* 假定输入的key和value的数据量一致，为num\_items；输出的key和value的数据量一致，为k
* k的范围：32，50，100，256，1024。k不大于num\_items


注意事项

* 结果必须稳定排序

加分项

* 支持其他不同数据类型的键值对，实现类型通用算法
* 覆盖更全面的数据范围，提供良好稳定的性能表现
