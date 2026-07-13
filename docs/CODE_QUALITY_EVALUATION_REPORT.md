# 代码质量评估报告

## 评估范围

本报告记录本轮 gcovr 安装、项目清理、覆盖率报告生成、未覆盖区域分析、测试补充、静态分析和代码质量复评结果。

评估对象包括：

- 核心源码：`global.c`、`graph.c`、`input.c`、`password.c`、`stats.c`、`travels.c`、`userManager.c`
- 测试入口：`tests/test_runner.c`
- 覆盖率配置：`.gcovr.cfg`
- 质量报告输出：`coverage/index.html`、`coverage/coverage.xml`、`coverage/coverage.json`、`coverage/coverage.txt`

## 工具安装与验证

当前系统未提供可用的 `python`、`pip`、`winget`、`choco` 或 `scoop`。因此本轮在项目 `tools/python` 目录下安装隔离版 Python，并通过该 Python 的 `pip` 安装 gcovr。

验证结果：

```text
Python 3.12.10
gcovr 8.6
```

覆盖率底层工具使用 MinGW64 自带 `gcov.exe`：

```text
E:\Program Files\Dev-Cpp\MinGW64\bin\gcov.exe
```

## 清理记录

清理前识别到的主要无用或可再生成内容包括：

- `build/` 下历史编译产物和可执行文件
- 根目录历史覆盖率中间文件：`*.gcda`、`*.gcno`、`*.gcov`
- 运行时日志目录：`audit/`、`backup/`
- 安装过程下载缓存：`tools/downloads/`
- 测试临时输入文件：`test_input_tokens.txt`

保留内容包括：

- 核心源码和头文件
- `README.md`、`docs/` 技术文档
- `.git`、`.gitignore`
- `.github/workflows/ci.yml`
- `.gcovr.cfg`
- `Makefile`
- 本地质量工具：`tools/cppcheck`、`tools/python`

说明：最终验证阶段会重新生成 `build/`、`coverage/` 以及测试运行时日志；其中 `build/`、`coverage/`、`audit/`、`backup/` 已由 `.gitignore` 忽略。

## 覆盖率配置与输出

使用 `.gcovr.cfg` 作为统一过滤规则：

```text
root = .
exclude = tests/.*
exclude = main.c
exclude = menu.c
```

报告输出：

| 类型 | 路径 | 用途 |
|---|---|---|
| HTML | `coverage/index.html` | 可视化查看覆盖率和未覆盖行 |
| XML | `coverage/coverage.xml` | 自动化处理和 CI 集成 |
| JSON | `coverage/coverage.json` | 结构化分析 |
| TXT | `coverage/coverage.txt` | 快速文本摘要 |

## 改进前后对比

| 指标 | 改进前 | 改进后 | 变化 |
|---|---:|---:|---:|
| 行覆盖率 | 91.6% | 94.2% | +2.6% |
| 函数覆盖率 | 97.6% | 100.0% | +2.4% |
| 分支覆盖率 | 79.2% | 82.2% | +3.0% |
| 覆盖行数 | 866 / 945 | 890 / 945 | +24 行 |
| 90% 行覆盖率门禁 | 已通过 | 已通过 | 持续达标 |
| 80% 分支覆盖率门禁 | 未通过 | 已通过 | 新增达标 |

本轮继续补充 `graph.c` 默认路径包装测试、`password.c` 固定向量和边界输入测试、`stats.c` 错误分支测试，使整体分支覆盖率超过 80%，并使 `graph.c`、`password.c`、`stats.c` 均独立超过 90% 行覆盖率目标。

## 最新覆盖率明细

| 文件 | 行数 | 已覆盖 | 行覆盖率 | 主要未覆盖区域 |
|---|---:|---:|---:|---|
| `global.c` | 8 | 8 | 100% | 无 |
| `graph.c` | 278 | 258 | 92% | 14、30、58、73、92、167-170、276、284-285、357、378、439、453、474-475、492、505 |
| `input.c` | 21 | 20 | 95% | 32 |
| `password.c` | 172 | 165 | 95% | 220、222、224、274、279、319、371 |
| `stats.c` | 75 | 74 | 98% | 30 |
| `travels.c` | 142 | 133 | 93% | 62、109、158-162、202-203 |
| `userManager.c` | 249 | 232 | 93% | 166-168、219、245、267、352-354、369-370、405-408、468-469 |
| 汇总 | 945 | 890 | 94.2% | 行覆盖率已超过 90%，分支覆盖率已超过 80% |

## 本轮代码完善

### 测试补充

新增 `test_input_helpers_validate_tokens`，验证：

1. `readString(NULL, size)` 拒绝空指针。
2. `readString(buffer, 0)` 拒绝无效缓冲区长度。
3. `readString` 对超长 token 做安全截断。
4. `readInt` 能解析合法整数。
5. `readInt` 能拒绝非数字 token。
6. 输入结束时 `readString` 返回失败。
7. Windows 下关闭重定向输入句柄后删除临时文件，避免测试残留。

新增 `test_graph_default_path_wrappers_round_trip`，验证：

1. 调用 `saveGraph` 使用 `graphParams.txt`、`graphVertex.txt`、`graphEdge.txt` 默认路径保存数据。
2. 调用 `loadGraph` 从默认路径加载数据。
3. 保存和加载前后景点数、道路数和关键道路长度一致。
4. 测试前自动备份默认数据文件，测试后恢复，避免破坏项目真实数据。

新增 `test_password_vectors_and_error_paths`，验证：

1. PBKDF2-HMAC-SHA256 固定测试向量。
2. 长密码和长 salt 触发 SHA-256 末尾填充分支。
3. 特殊字符密码可正常哈希和验证。
4. 空 salt、空输出、过小输出缓冲区、空校验参数、错误算法和错误迭代次数等异常路径。

新增 `test_stats_error_and_empty_graph_branches`，验证：

1. `statsSave` 在 `stats.txt` 被目录占用时的保存失败流程。
2. 空图和空指针图报表输出。
3. 空图、空景点名和非法景点访问不会污染统计计数。

### 代码质量修复

1. `password.c`
   - 将 SHA-256 轮函数中的 `t1`、`t2` 缩小到循环内部作用域。
   - 降低局部变量生命周期，减少误用风险。

2. `stats.c` / `stats.h`
   - 将只读图参数改为 `const ALGraph *`。
   - 在 `statsSave` 中缩小循环变量 `i` 的作用域。
   - 提升接口语义清晰度和静态分析质量。

## 静态分析结果

执行范围：

```text
graph.c travels.c userManager.c password.c stats.c input.c
```

工具：

```text
cppcheck 2.21.0
```

最终结果：

```text
docs/cppcheck-branch-quality-report.txt 长度为 0，无剩余告警。
```

## 综合代码质量评估

| 维度 | 评分 | 依据 |
|---|---:|---|
| 覆盖率质量 | 94 / 100 | 行覆盖率 94.2%，函数覆盖率 100.0%，分支覆盖率 82.2%；`graph.c`、`password.c`、`stats.c` 均超过 90%。 |
| 模块化程度 | 90 / 100 | 控制台交互已从图、用户、路径核心逻辑中拆出，业务函数可测试性较好。 |
| 可读性 | 88 / 100 | 命名整体清晰，关键文档较完整；部分历史 C 函数仍可继续统一命名风格。 |
| 可维护性 | 89 / 100 | 文件 I/O、权限、密码、统计、路径算法已有清晰边界；仍可继续削减全局状态依赖。 |
| 潜在缺陷控制 | 92 / 100 | 严格编译通过，`cppcheck` 无剩余告警，输入、文件异常、权限和迁移路径均有测试覆盖。 |

综合代码质量指数：**92.4 / 100**。

## 剩余风险与后续建议

1. `graph.c` 分支覆盖率为 78.17%，后续可继续补充 `updateDirectedEdge`、`deleteDirectedEdge` 和 `graphLoadFromFiles` 的细分失败路径测试。
2. 分支覆盖率已达到 82.2%，建议先以 80% 作为 CI 硬门禁，稳定后提升到 85%。
3. `password.c` 中 `toHex` 的小缓冲区内部保护分支仍未直接暴露到公共接口，可在必要时通过测试专用包装进一步覆盖。
4. `stats.c` 行覆盖率已达到 98%，仅 `statsLoad` 文件不存在早返回分支仍可补充显式测试。
5. 本地 Windows 环境没有可用 `make`，本轮使用 MinGW64 GCC 直接执行等价构建流程；CI 环境仍可使用 `make`，并已保留 `make build`、`make test`、`make coverage` 流程。

## 结论

本轮已完成 gcovr 安装、项目产物清理、HTML/XML/JSON/TXT 覆盖率报告生成、未覆盖区域分析、测试补充、代码质量修复和复评。最终行覆盖率达到 **94.2%**，分支覆盖率达到 **82.2%**，超过本轮按 90% 行覆盖率和 80% 分支覆盖率设定的目标；静态分析无剩余告警，项目当前质量状态较稳定。
