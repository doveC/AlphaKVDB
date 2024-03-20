# 使用跳表实现的轻量级KV存储引擎

- 采用SkipList作为底层数据结构，支持插入，删除，查询等常见操作
- 使用C++模版编程，支持自定义类型与自定义比较函数

# 存储引擎数据表现

## 插入操作

调表树高：18

| 调用次数（万次）|耗时（秒）|
|-------|---------------|
| 10    | 0.91206       |
| 50    | 4.85045       |
| 100   | 10.0155       |

每秒可处理写请求数（QPS）：10.96w

## 查询操作

跳表树高：18

| 调用次数（万次）|耗时（秒）|
|-------|---------------|
| 10    | 0.02469       |
| 50    | 0.22838       |
| 100   | 0.6642        |

每秒可处理写请求数（QPS）：405.02w