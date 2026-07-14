# travel-nav-system 项目文档

## 项目简介

travel-nav-system 是一个基于 C 语言的旅游导航系统，提供景点管理、路线规划、导游路线生成等功能。

## 文档目录

| 文档 | 描述 |
|---|---|
| [PROJECT_OVERVIEW.md](PROJECT_OVERVIEW.md) | 项目概述、技术架构、功能模块、构建部署、代码规范、安全特性 |
| [FEATURE_GUIDE.md](FEATURE_GUIDE.md) | 功能指南与使用说明 |
| [API_REFERENCE.md](API_REFERENCE.md) | API 接口参考 |
| [TESTING_GUIDE.md](TESTING_GUIDE.md) | 测试指南 |
| [FEATURE_COMPARISON_REPORT.md](FEATURE_COMPARISON_REPORT.md) | 功能对比报告 |

## 快速开始

```bash
# 编译主程序
gcc -std=c11 -Wall -Wextra -g main.c menu.c global.c input.c graph.c graph_console.c travels.c travels_console.c userManager.c user_console.c password.c stats.c backup.c -o build/toursys.exe

# 运行测试
gcc -std=c11 -Wall -Wextra -g -DUNIT_TEST tests/test_runner.c global.c input.c graph.c travels.c userManager.c password.c stats.c backup.c -o build/test_runner.exe
./build/test_runner.exe

# 运行程序
./build/toursys.exe
```