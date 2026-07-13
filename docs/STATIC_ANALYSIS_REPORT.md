# cppcheck 静态分析报告

## 分析范围

本次静态分析聚焦以下三个源代码文件：

1. `graph.c`
2. `travels.c`
3. `userManager.c`

## 工具与版本

| 项目 | 说明 |
|---|---|
| 工具 | cppcheck |
| 版本 | 2.21.0 |
| 安装位置 | `tools/cppcheck/PFiles/Cppcheck/cppcheck.exe` |
| 分析标准 | C99 |
| 平台参数 | `--platform=win64` |
| 检查级别 | `--enable=all --check-level=exhaustive` |

## 执行命令

```powershell
& "c:\Users\user\Desktop\toursys\tools\cppcheck\PFiles\Cppcheck\cppcheck.exe" --enable=all --check-level=exhaustive --std=c99 --platform=win64 --force --inline-suppr --template=gcc --suppress=missingIncludeSystem --suppress=unusedFunction --suppress=staticFunction --suppress=checkersReport graph.c travels.c userManager.c 2> docs\cppcheck-target-report.txt
```

## 发现与修复

本轮分析过程中已修复以下问题：

1. `graph.c` 中只读图参数补充 `const` 修饰，包括 `saveGraph`、`graphSaveToFiles`、`getEdgeNum`、`createGuideGraphEX` 相关路径。
2. `travels.c` 中修复 `printPath` 先访问数组再校验索引的未定义行为风险。
3. `travels.c` 中修复 `dijkstraShortestPath` 在校验 `prev` 前写入数组的空指针风险。
4. `graph.c` 中修复 `getEdgeNum` 在校验 `graph` 前解引用指针的空指针风险。
5. `travels.c` 中消除 `path` 参数遮蔽全局变量的问题，改为 `pathMatrix`。
6. 多处邻接表只读遍历指针改为 `const CNode *`。
7. 多处循环变量下沉到最小作用域，降低误用风险。

## 最终结果

最终 `docs/cppcheck-target-report.txt` 文件长度为 0，表示在当前抑制项范围内，`graph.c`、`travels.c`、`userManager.c` 无剩余 cppcheck 告警。

说明：`missingIncludeSystem`、`unusedFunction`、`staticFunction` 主要由 Windows 系统头文件和定向扫描单个源文件导致，已作为上下文型噪声抑制；本次修复集中处理真实代码缺陷、未定义行为风险和可维护性问题。
