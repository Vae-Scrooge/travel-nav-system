# 测试指南

## 1. 测试概述

项目包含完整的单元测试和集成测试，覆盖图管理、用户管理、路径规划、数据备份等核心功能。

## 2. 编译测试

```bash
# 编译测试程序
gcc -std=c11 -Wall -Wextra -g -DUNIT_TEST tests/test_runner.c global.c input.c graph.c travels.c userManager.c password.c stats.c backup.c -o build/test_runner.exe

# 运行测试
./build/test_runner.exe
```

## 3. 测试覆盖范围

| 模块 | 测试用例数 | 覆盖功能 |
|---|---|---|
| 图管理 | 15+ | 景点CRUD、道路CRUD、文件IO、矩阵转换 |
| 用户管理 | 12+ | 用户认证、权限管理、审计日志、密码修改 |
| 路径规划 | 8+ | Dijkstra、Floyd-Warshall、导游图生成、环路检测 |
| 数据备份 | 4+ | 创建、恢复、删除、列出备份 |
| 统计分析 | 3+ | 访问统计、报告生成 |
| 工具函数 | 2+ | 输入验证 |

## 4. 测试文件结构

```
tests/
├── test_runner.c      # 主测试程序
└── perf_benchmark.c   # 性能基准测试
```

## 5. 测试用例示例

### 5.1 图保存加载测试

```c
static void test_graph_file_io_and_corruption(void)
{
        ALGraph graph = create_sample_graph();
        ALGraph loaded;
        initGraph(&loaded);
        
        saveGraph(&graph);
        loadGraph(&loaded);
        
        assert(loaded.nodenum == graph.nodenum);
        assert(loaded.edgnum == graph.edgnum);
        
        freeGraph(&loaded);
        freeGraph(&graph);
}
```

### 5.2 用户密码修改测试

```c
static void test_user_change_password(void)
{
        const char *userPath = "test_user_change_pwd.txt";
        
        remove(userPath);
        assert(saveUserToFile(userPath, "testuser", "oldpass"));
        assert(validateUserInFile(userPath, "testuser", "oldpass", 0) == 1);
        
        assert(userChangePassword("testuser", "oldpass", "newpass"));
        assert(validateUserInFile(userPath, "testuser", "newpass", 0) == 1);
        
        remove(userPath);
}
```

## 6. 性能测试

```bash
# 编译性能测试
gcc -std=c11 -Wall -Wextra -g tests/perf_benchmark.c global.c input.c graph.c travels.c -o build/perf_benchmark.exe

# 运行性能测试
./build/perf_benchmark.exe
```