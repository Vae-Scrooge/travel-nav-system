# 测试报告

## 测试环境

| 项目 | 说明 |
|---|---|
| 操作系统 | Windows |
| 编译器 | `E:\Program Files\Dev-Cpp\MinGW64\bin\gcc.exe` |
| GCC 版本 | TDM-GCC 4.9.2 |
| 编译标准 | C99 |
| 编译参数 | `-std=c99 -Wall -Wextra -Wpedantic -Werror` |

## 编译验证

主程序严格编译命令：

```powershell
& "E:\Program Files\Dev-Cpp\MinGW64\bin\gcc.exe" -std=c99 -Wall -Wextra -Wpedantic -Werror main.c menu.c global.c graph.c travels.c userManager.c password.c stats.c -o build\toursys_latest.exe
```

结果：通过。

说明：`build\toursys.exe` 曾因 Windows 文件句柄占用无法覆盖写入，因此最终使用 `build\toursys_latest.exe` 完成验证。该问题属于目标文件占用，不是代码编译错误。

## 自动化测试

测试程序严格编译命令：

```powershell
& "E:\Program Files\Dev-Cpp\MinGW64\bin\gcc.exe" -std=c99 -Wall -Wextra -Wpedantic -Werror tests\test_runner.c global.c graph.c travels.c password.c stats.c -o build\test_runner_latest2.exe
```

测试运行命令：

```powershell
& "c:\Users\user\Desktop\toursys\build\test_runner_latest2.exe"
```

结果：通过，输出 `All tests passed.`

## 覆盖场景

当前自动化测试已覆盖：

1. 景点定位成功和失败。
2. 道路添加和非法道路拦截。
3. 邻接矩阵生成。
4. Floyd 最短路径计算。
5. Dijkstra 单源最短路径计算。
6. 景点新增、重命名、删除。
7. 道路新增、更新、删除。
8. PBKDF2-HMAC-SHA256 密码哈希生成和验证。
9. 旧明文密码兼容验证。
10. 系统启动、登录、注册、路径查询统计计数。
11. 统计数据保存、加载和报表输出。
12. DFS 遍历、边判断、入度环路检测。

## 端到端测试

端到端测试命令：

```powershell
"1 admin 123456 2 7 0" | & "c:\Users\user\Desktop\toursys\build\toursys_cov.exe"
```

覆盖流程：

1. 加载图数据。
2. 使用旧明文账号登录。
3. 查看景点邻接矩阵。
4. 查看访问统计报表。
5. 退出并保存统计。

结果：通过。

## 覆盖率说明

已清理旧覆盖率产物后，使用单元测试插桩版本重新生成覆盖率。

本地 `gcovr` 最新汇总结果：

| 文件 | 行覆盖率 |
|---|---:|
| `global.c` | 100% |
| `graph.c` | 92% |
| `input.c` | 95% |
| `password.c` | 95% |
| `stats.c` | 98% |
| `travels.c` | 93% |
| `userManager.c` | 93% |
| 汇总 | 94.2% |

当前项目已经建立总体行覆盖率、总体分支覆盖率和核心模块分项覆盖率硬门禁。最新覆盖率为：总体行覆盖率 94.2%，总体分支覆盖率 82.2%，且 `graph.c`、`travels.c`、`userManager.c`、`password.c`、`stats.c` 均超过 90% 行覆盖率目标。后续应继续推进端到端交互测试和性能基准回归测试。

## 测试中发现并修复的问题

1. `createGuideGraph` 中存在未使用变量，严格编译失败，已清理。
2. `statsLoad` 使用 `%9s` 读取键名导致 `pathQueries` 被截断，统计恢复失败，已改为 `%31s`。

## 结论

主程序和测试程序均已在指定 MinGW64 编译器下通过严格编译。核心业务测试和端到端流程验证通过。覆盖率目标和 CI 流程已建立，当前总体行覆盖率和总体分支覆盖率均已达到本阶段质量门禁。

## 2026-07-13 迭代更新

### 解耦调整

本轮将控制台交互和核心业务进一步拆分：

1. 新增 `graph_console.c`，承接图管理菜单、手工建图、图打印和导游路线展示。
2. 新增 `input.c`，承接 `readString`、`readInt` 输入函数。
3. 新增 `user_console.c`，承接登录、注册和用户菜单交互。
4. `graph.c` 保留图结构、图 CRUD、矩阵转换、文件读写和导游图业务构建。
5. `userManager.c` 保留用户保存、验证和旧明文密码迁移。

### 新增测试场景

本轮自动化测试新增：

1. 图参数文件不存在。
2. 图参数文件格式错误。
3. 顶点文件出现重复景点。
4. 边文件格式错误。
5. 边文件引用不存在景点。
6. 边长度为负数。
7. Dijkstra 不可达路径。
8. 用户重复注册。
9. 用户不存在和密码错误。
10. 旧明文密码登录后自动迁移为哈希。
11. 系统安全随机盐值基本差异检查。

### 最新验证命令

主程序严格编译：

```powershell
& "E:\Program Files\Dev-Cpp\MinGW64\bin\gcc.exe" -std=c99 -Wall -Wextra -Wpedantic -Werror main.c menu.c global.c input.c graph.c graph_console.c travels.c userManager.c user_console.c password.c stats.c -o build\toursys_iter5.exe
```

测试程序严格编译：

```powershell
& "E:\Program Files\Dev-Cpp\MinGW64\bin\gcc.exe" -std=c99 -Wall -Wextra -Wpedantic -Werror tests\test_runner.c global.c input.c graph.c travels.c userManager.c password.c stats.c -o build\test_runner_iter4.exe
```

结果：主程序和测试程序均通过，测试输出 `All tests passed.`。

### 最新覆盖率

本轮覆盖率统计聚焦核心业务模块，不将 `input.c`、`graph_console.c`、`user_console.c` 计入核心覆盖率。

| 文件 | 行覆盖率 |
|---|---:|
| `global.c` | 100.00% |
| `graph.c` | 75.82% |
| `travels.c` | 75.60% |
| `userManager.c` | 74.32% |
| `password.c` | 90.12% |
| `stats.c` | 97.33% |
| 汇总 | 81.29% |

结论：核心业务整体覆盖率已达到 80% 以上；但 `graph.c`、`travels.c`、`userManager.c` 尚未达到单模块 90% 目标，后续需要继续拆分交互式路径查询、补充图保存失败路径和用户迁移失败路径测试。

### 静态分析

本地尝试执行 `cppcheck --version`，结果显示当前环境未安装 `cppcheck`。本轮已使用 GCC 严格编译参数作为基础静态检查；专用静态分析应在安装 `cppcheck` 后或通过 CI 环境继续执行。

## 2026-07-13 最终补充验证

### 单元测试拆分与补充

本轮继续围绕 `graph.c`、`travels.c`、`userManager.c` 拆分单职责测试用例。新增和强化场景包括：

1. 图 CRUD 参数异常、重复景点、边更新、边删除和边索引重排。
2. 图文件加载格式错误、额外字段、未知景点、负权边和保存失败。
3. Dijkstra 空输出参数、起点终点相同和不可达路径。
4. `printPath` 非法索引和不可达路径输出。
5. 用户保存空参数、重复注册、文件不可写、畸形记录跳过。
6. 旧明文密码迁移的空参数、过长路径、源文件缺失、目标用户缺失、临时文件无法写入和保留其他用户记录。

### 最终验证命令

测试程序严格编译：

```powershell
& "E:\Program Files\Dev-Cpp\MinGW64\bin\gcc.exe" -DUNIT_TEST -std=c99 -Wall -Wextra -Wpedantic -Werror tests\test_runner.c global.c input.c graph.c travels.c userManager.c password.c stats.c -o build\test_runner_final3.exe
```

主程序严格编译：

```powershell
& "E:\Program Files\Dev-Cpp\MinGW64\bin\gcc.exe" -std=c99 -Wall -Wextra -Wpedantic -Werror main.c menu.c global.c input.c graph.c graph_console.c travels.c travels_console.c userManager.c user_console.c password.c stats.c -o build\toursys_final3.exe
```

测试运行结果：退出码为 `0`，断言测试全部通过。

### 最终覆盖率

使用 `--coverage -O0` 重新编译并运行测试后，`gcov` 汇总结果如下：

| 文件 | 行覆盖率 |
|---|---:|
| `global.c` | 100.00% |
| `graph.c` | 90.65% |
| `travels.c` | 93.66% |
| `userManager.c` | 93.98% |
| `password.c` | 90.12% |
| `stats.c` | 97.33% |
| 汇总 | 92.57% |

结论：整体覆盖率超过 80% 目标，且本轮重点文件 `graph.c`、`travels.c`、`userManager.c` 均达到核心模块 90% 以上覆盖率目标。

### cppcheck 静态分析

已安装并验证 `cppcheck 2.21.0`，位置为：

```text
tools/cppcheck/PFiles/Cppcheck/cppcheck.exe
```

定向分析命令：

```powershell
& "c:\Users\user\Desktop\toursys\tools\cppcheck\PFiles\Cppcheck\cppcheck.exe" --enable=all --check-level=exhaustive --std=c99 --platform=win64 --force --inline-suppr --template=gcc --suppress=missingIncludeSystem --suppress=unusedFunction --suppress=staticFunction --suppress=checkersReport graph.c travels.c userManager.c 2> docs\cppcheck-target-report.txt
```

结果：`docs/cppcheck-target-report.txt` 文件长度为 0，目标三文件无剩余 cppcheck 告警。详细说明见 `docs/STATIC_ANALYSIS_REPORT.md`。

## 2026-07-13 分支门禁优化验证

### 新增测试场景

本轮针对低覆盖模块继续补充测试：

1. `graph.c`：新增 `saveGraph`、`loadGraph` 默认路径包装集成测试，验证 `graphParams.txt`、`graphVertex.txt`、`graphEdge.txt` 默认路径保存和加载正确性，并在测试前后自动备份与恢复真实数据文件。
2. `password.c`：新增 PBKDF2-HMAC-SHA256 固定测试向量、长密码、长 salt、特殊字符密码、空参数、过小输出缓冲区、错误算法和错误迭代次数测试。
3. `stats.c`：新增保存失败、空图报表、空指针图报表、非法景点访问和空参数访问测试。

### 最新验证命令

本地 Windows 环境没有可用 `make`，本轮继续使用 MinGW64 GCC 执行与 Makefile 等价的构建命令；CI 环境仍保持 `make build`、`make test`、`make coverage` 流程。

覆盖率验证命令：

```powershell
& "E:\Program Files\Dev-Cpp\MinGW64\bin\gcc.exe" -DUNIT_TEST -std=c99 -Wall -Wextra -Wpedantic -Werror --coverage -O0 tests\test_runner.c global.c input.c graph.c travels.c userManager.c password.c stats.c --coverage -o build\test_runner_branch_quality.exe
& ".\build\test_runner_branch_quality.exe"
gcovr --config .gcovr.cfg --gcov-executable gcov --html-details coverage\index.html --xml-pretty --xml coverage\coverage.xml --json-pretty --json coverage\coverage.json --txt coverage\coverage.txt --print-summary --fail-under-line 90 --fail-under-branch 80
```

### 最新覆盖率

| 文件 | 行覆盖率 |
|---|---:|
| `global.c` | 100% |
| `graph.c` | 92% |
| `input.c` | 95% |
| `password.c` | 95% |
| `stats.c` | 98% |
| `travels.c` | 93% |
| `userManager.c` | 93% |
| 汇总 | 94.2% |

总体函数覆盖率为 **100.0%**，总体分支覆盖率为 **82.2%**。本轮已通过 `--fail-under-line 90` 和 `--fail-under-branch 80` 双门禁。
