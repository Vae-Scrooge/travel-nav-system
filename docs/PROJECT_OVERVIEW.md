# 项目概述

## 1. 项目简介

travel-nav-system 是一个基于 C 语言的旅游导航系统，提供景点管理、路线规划、导游路线生成等功能。系统采用模块化设计，分为图管理、用户管理、路径规划、统计分析等模块。

## 2. 技术架构

### 2.1 技术栈

| 项目 | 技术 | 版本 |
|---|---|---|
| 编程语言 | C | C11 |
| 编译器 | GCC | 4.9+ |
| 数据结构 | 邻接表 | - |
| 算法 | Dijkstra、Floyd-Warshall、DFS | - |

### 2.2 模块划分

```
travel-nav-system/
├── main.c          # 主程序入口
├── menu.c          # 菜单系统
├── global.h/c      # 全局数据结构
├── graph.h/c       # 图管理核心
├── graph_console.c # 图管理控制台
├── travels.h/c     # 路径规划
├── travels_console.c # 路径规划控制台
├── userManager.h/c # 用户管理
├── user_console.c  # 用户管理控制台
├── password.c      # 密码安全
├── stats.c         # 统计分析
├── backup.c        # 数据备份
└── input.c         # 输入处理
```

### 2.3 核心数据结构

```c
typedef struct stCNode {
    int index;           // 邻接点索引
    int length;          // 边长度
    struct stCNode *next;
} CNode;

typedef struct stHNode {
    char data[10];       // 景点名称
    char description[100]; // 景点描述
    double ticketPrice;  // 门票价格
    char openTime[20];   // 开放时间
    CNode *first;        // 邻接表头指针
} HNode, RoadList[MAXNUM];

typedef struct {
    RoadList roadlist;   // 邻接表
    int nodenum;         // 顶点数
    int edgnum;          // 边数
} ALGraph;
```

## 3. 功能模块

| 模块 | 功能 | 状态 |
|---|---|---|
| 用户管理 | 注册、登录、权限管理、修改密码 | ✅ |
| 景点管理 | 添加、删除、修改景点信息 | ✅ |
| 道路管理 | 添加、删除、修改道路 | ✅ |
| 路径规划 | 最短路径查询、导游路线生成 | ✅ |
| 统计分析 | 访问统计、使用报告 | ✅ |
| 数据备份 | 创建、恢复、删除备份 | ✅ |

## 4. 构建与部署

### 4.1 编译命令

```bash
# 编译主程序
gcc -std=c11 -Wall -Wextra -g main.c menu.c global.c input.c graph.c graph_console.c travels.c travels_console.c userManager.c user_console.c password.c stats.c backup.c -o build/toursys.exe

# 编译测试程序
gcc -std=c11 -Wall -Wextra -g -DUNIT_TEST tests/test_runner.c global.c input.c graph.c travels.c userManager.c password.c stats.c backup.c -o build/test_runner.exe

# 使用 Makefile
make build
make test
make clean
```

### 4.2 CI/CD

项目使用 GitHub Actions 进行持续集成，配置文件位于 `.github/workflows/ci.yml`。

## 5. 代码规范

### 5.1 命名规范

| 类型 | 规则 | 示例 |
|---|---|---|
| 函数 | 使用小驼峰或动宾结构 | `initGraph`、`freeGraph` |
| 变量 | 使用清晰英文名 | `nodeCount`、`edgeLength` |
| 常量 | 使用全大写加下划线 | `MAXNUM`、`INF` |
| 结构体 | 使用名词 | `ALGraph`、`CNode` |
| 文件 | 按模块命名 | `graph.c`、`userManager.c` |

### 5.2 格式规范

- 使用 4 个空格缩进
- 每个函数只做一类事情，建议不超过 80 行
- 条件判断必须使用大括号
- 指针声明风格保持一致：`ALGraph *graph`
- 头文件必须使用 include guard

## 6. 安全特性

### 6.1 密码存储

- 使用 PBKDF2-HMAC-SHA256 哈希算法
- 每个新用户生成独立盐值
- 密码校验时只比较哈希结果，不保存明文密码
- 旧明文用户在登录验证成功后自动迁移为哈希格式

### 6.2 数据保护

- 数据文件具备备份和恢复机制
- 敏感数据的访问和修改需要可追踪
- 管理员操作记录写入审计日志

### 6.3 权限控制

- 角色权限管理（管理员/普通用户）
- 删除操作需要确认
- 密码最小长度为 6 位