# 项目结构调整说明文档

## 1. 调整背景与目标

### 1.1 背景

原项目代码文件集中存放于根目录下，随着功能模块的增多，代码组织逐渐混乱：
- 所有源文件混在一起，难以快速定位功能代码
- 缺少清晰的模块划分，跨模块依赖关系不明确
- 数据文件与代码文件混杂，不利于数据管理和备份
- 测试文件与生产代码分离不够清晰

### 1.2 目标

1. **模块化**：按照功能职责划分模块，建立清晰的目录层次结构
2. **可维护性**：相关功能代码集中管理，减少跨目录依赖
3. **可读性**：制定统一的命名规范，明确各目录职责
4. **可扩展性**：为未来功能扩展预留清晰的模块空间

---

## 2. 调整前项目结构

```
travel-nav-system/
├── .gitignore
├── .gcovr.cfg
├── CONTRIBUTING.md
├── LICENSE
├── Makefile
├── README.md
├── SECURITY.md
├── main.c
├── main.h
├── global.c
├── global.h
├── graph.c
├── graph.h
├── travels.c
├── travels.h
├── password.c
├── password.h
├── input.c
├── menu.c
├── menu.h
├── graph_console.c
├── travels_console.c
├── user_console.c
├── userManager.c
├── userManager.h
├── stats.c
├── stats.h
├── backup.c
├── backup.h
├── graphParams.txt
├── graphVertex.txt
├── graphEdge.txt
├── user.txt
└── tests/
    ├── test_runner.c
    └── perf_benchmark.c
```

---

## 3. 调整后项目结构

```
travel-nav-system/
├── .gitignore
├── .gcovr.cfg
├── CONTRIBUTING.md
├── LICENSE
├── Makefile
├── README.md
├── SECURITY.md
├── main.c
├── main.h
├── src/
│   ├── core/
│   │   ├── global.c
│   │   ├── global.h
│   │   ├── graph.c
│   │   ├── graph.h
│   │   ├── travels.c
│   │   ├── travels.h
│   │   ├── password.c
│   │   ├── password.h
│   │   └── input.c
│   ├── console/
│   │   ├── menu.c
│   │   ├── menu.h
│   │   ├── graph_console.c
│   │   ├── travels_console.c
│   │   └── user_console.c
│   └── manager/
│       ├── userManager.c
│       ├── userManager.h
│       ├── stats.c
│       ├── stats.h
│       ├── backup.c
│       └── backup.h
├── data/
│   ├── graphParams.txt
│   ├── graphVertex.txt
│   ├── graphEdge.txt
│   └── user.txt
└── tests/
    ├── test_runner.c
    └── perf_benchmark.c
```

---

## 4. 文件迁移对照表

| 原路径 | 新路径 | 模块分类 |
|--------|--------|----------|
| `global.c/h` | `src/core/global.c/h` | 核心模块 |
| `graph.c/h` | `src/core/graph.c/h` | 核心模块 |
| `travels.c/h` | `src/core/travels.c/h` | 核心模块 |
| `password.c/h` | `src/core/password.c/h` | 核心模块 |
| `input.c` | `src/core/input.c` | 核心模块 |
| `menu.c/h` | `src/console/menu.c/h` | 控制台模块 |
| `graph_console.c` | `src/console/graph_console.c` | 控制台模块 |
| `travels_console.c` | `src/console/travels_console.c` | 控制台模块 |
| `user_console.c` | `src/console/user_console.c` | 控制台模块 |
| `userManager.c/h` | `src/manager/userManager.c/h` | 管理模块 |
| `stats.c/h` | `src/manager/stats.c/h` | 管理模块 |
| `backup.c/h` | `src/manager/backup.c/h` | 管理模块 |
| `graphParams.txt` | `data/graphParams.txt` | 数据文件 |
| `graphVertex.txt` | `data/graphVertex.txt` | 数据文件 |
| `graphEdge.txt` | `data/graphEdge.txt` | 数据文件 |
| `user.txt` | `data/user.txt` | 数据文件 |

---

## 5. 调整依据与原则

### 5.1 模块划分原则

1. **单一职责原则**：每个模块只负责一个明确的功能领域
2. **高内聚低耦合**：模块内部高度相关，模块间依赖最小化
3. **分层架构**：从核心到外围，层次清晰

### 5.2 目录职责定义

| 目录 | 职责说明 | 使用场景 |
|------|----------|----------|
| `src/core/` | 核心数据结构与算法实现 | 图数据结构、路径规划算法、密码安全、全局工具函数 |
| `src/console/` | 控制台用户界面交互 | 菜单展示、命令解析、用户输入输出处理 |
| `src/manager/` | 业务逻辑管理 | 用户管理、统计分析、数据备份恢复 |
| `data/` | 应用运行时数据文件 | 图参数、顶点、边数据、用户账户数据 |
| `tests/` | 单元测试与性能测试 | 功能验证、性能基准测试 |

### 5.3 命名规范

#### 5.3.1 文件命名

- 核心模块：`<功能名>.c/h`（如 `graph.c`, `travels.h`）
- 控制台模块：`<功能名>_console.c`（如 `graph_console.c`）
- 管理模块：`<功能名>Manager.c/h`（如 `userManager.c`）

#### 5.3.2 目录命名

- 使用小写字母，单词间用下划线分隔
- 采用复数形式表示集合（如 `tests/`, `data/`）

---

## 6. 依赖关系说明

### 6.1 模块依赖方向

```
main.c
├── src/core/       (核心依赖)
├── src/console/    (界面依赖)
└── src/manager/    (业务依赖)

src/console/
└── src/core/       (依赖核心数据结构)

src/manager/
└── src/core/       (依赖核心工具函数)

tests/
├── src/core/       (测试核心模块)
└── src/manager/    (测试管理模块)
```

### 6.2 关键依赖变更

| 文件 | 变更前依赖 | 变更后依赖 |
|------|-----------|-----------|
| `main.c` | `backup.h` | `src/core/global.h`, `src/core/graph.h`, `src/core/travels.h`, `src/console/menu.h`, `src/manager/userManager.h`, `src/manager/stats.h`, `src/manager/backup.h` |
| `src/console/graph_console.c` | `graph.h`, `travels.h`, `userManager.h` | `../core/global.h`, `../core/graph.h`, `../core/travels.h`, `../manager/userManager.h` |
| `src/manager/userManager.c` | `password.h`, `stats.h` | `../core/password.h`, `stats.h` |
| `tests/test_runner.c` | `../graph.h`, `../password.h`, `../stats.h`, `../travels.h`, `../userManager.h`, `../backup.h` | `../src/core/graph.h`, `../src/core/password.h`, `../src/manager/stats.h`, `../src/core/travels.h`, `../src/manager/userManager.h`, `../src/manager/backup.h` |

---

## 7. Makefile 调整说明

### 7.1 编译选项变更

```makefile
# 变更前
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -g

# 变更后
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -g -Isrc/core -Isrc/console -Isrc/manager
```

### 7.2 源文件路径更新

所有源文件路径更新为新的目录结构，使用相对路径引用。

### 7.3 数据文件路径更新

```makefile
# 变更前
cp graphParams.txt graphVertex.txt graphEdge.txt user.txt $(BUILD_DIR)/dist/

# 变更后
cp data/graphParams.txt data/graphVertex.txt data/graphEdge.txt data/user.txt $(BUILD_DIR)/dist/
```

---

## 8. 验证结果

### 8.1 编译验证

- ✅ 主程序编译成功：`build/toursys.exe`
- ✅ 测试程序编译成功：`build/test_runner.exe`

### 8.2 测试验证

```
All tests passed.
```

所有单元测试通过，功能完整性得到验证。

### 8.3 功能验证

- ✅ 图数据加载与保存
- ✅ 用户注册与登录
- ✅ 路径规划算法
- ✅ 数据备份与恢复
- ✅ 统计功能

---

## 9. 后续优化建议

1. **头文件统一管理**：考虑创建 `src/include/` 目录统一管理公共头文件
2. **构建系统升级**：考虑使用 CMake 替代 Makefile，支持跨平台构建
3. **模块接口文档**：为每个模块添加接口说明文档
4. **持续集成**：配置 CI/CD 流程，自动编译和测试
5. **日志系统**：引入结构化日志系统，便于问题排查