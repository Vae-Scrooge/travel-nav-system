# 修改说明

## 本次完善目标

针对旅游景点管理系统中已识别的稳定性、性能、数据校验和可维护性问题，完成了一轮集中修复与结构优化。改动重点覆盖图数据管理、路径规划、用户管理、主流程控制和资源释放。

## 主要变更

### 1. 图结构与数据持久化

- 新增 `initGraph`、`freeGraph`、`addEdge`、`freeMatrix` 等公共图操作函数。
- 修复邻接矩阵分配类型错误，避免 `double **` 使用错误大小分配内存。
- 修复重复转换邻接矩阵导致的内存泄漏。
- 加载图数据时增加参数、顶点、边的合法性校验。
- `locate` 找不到景点时改为返回 `-1`，避免错误映射到第一个景点。
- 修复导游图构建时错误修改原图的问题，确保导游图和原始景点网络相互独立。

影响范围：`global.c`、`global.h`、`graph.c`、`graph.h`。

### 2. 路径规划性能与正确性

- 修正 Floyd 最短路径算法的循环顺序，改为标准的 `k -> i -> j`。
- 增加不可达路径判断，避免 `INF` 参与无效路径更新。
- 新增基于邻接表的 Dijkstra 单源最短路径算法。
- 将用户的单次最短路径查询改为使用 Dijkstra，避免每次查询都执行全图最短路径计算。
- 增强路径查询中的景点名称校验和不可达提示。

影响范围：`travels.c`、`travels.h`。

### 3. 用户管理与输入安全

- 新增统一的 `readString` 和 `readInt` 输入函数。
- 替换用户登录、注册、主菜单、图录入中的不安全字符串读取。
- 修复 `validateUser` 文件打开失败时没有返回值的问题。
- 增加重复用户名注册校验。
- 优化菜单非法输入处理，避免误退出。

影响范围：`global.c`、`global.h`、`userManager.c`、`userManager.h`、`main.c`。

### 4. 主流程和资源管理

- 主程序启动时显式初始化原图和导游图。
- 退出系统前释放邻接表和邻接矩阵资源。
- 检测导游图环路前增加状态判断，避免未创建导游图时访问未初始化数据。
- 移除跨模块重复定义的全局变量，降低链接错误风险。

影响范围：`main.c`、`graph.c`、`travels.c`。

## 预期效果

- 最短路径查询性能提升，尤其适用于用户只查询两个景点之间路线的场景。
- 图数据加载、保存和导游图生成过程更稳定。
- 非法景点名、非法边权、错误菜单输入等场景具备更明确的处理逻辑。
- 降低缓冲区溢出、空返回、内存泄漏和原图被误修改的风险。
- 代码模块边界更清晰，后续扩展景点增删改、访问统计、用户持久化增强等功能更方便。

## 验证说明

- 已进行文本级检查，确认主要交互入口不再直接使用不安全的裸 `scanf("%s")`。
- 当前环境未安装 `gcc`、`clang` 或 MSVC `cl`，因此未能执行真实 C 编译验证。
- 建议在安装 C 编译器后执行完整编译，并重点验证：登录注册、图加载、图录入保存、导游路线生成、环路检测、最短路径查询。

## 工程化完善

### 1. 自动化测试体系

- 新增 `tests/test_runner.c`，覆盖景点定位、道路添加、邻接矩阵转换和 Floyd 最短路径计算等核心逻辑。
- 新增 `docs/TESTING.md`，明确单元测试、集成测试、端到端测试、性能测试和 80% 覆盖率目标。
- 新增持续集成配置 `.github/workflows/ci.yml`，用于自动执行构建、测试、覆盖率统计和静态检查。

### 2. 构建部署体系

- 新增 `Makefile`，提供 `build`、`test`、`coverage`、`dist`、`clean` 等标准目标。
- 新增 `docs/BUILD_DEPLOY.md`，说明环境隔离、构建产物管理、版本追溯和一键发布规划。
- 新增 `.gitignore`，排除构建产物、覆盖率文件、本地环境变量和备份目录。

### 3. 安全存储机制

- 新增 `docs/SECURITY.md`，说明当前明文密码风险、推荐的 bcrypt/Argon2/PBKDF2 哈希方案、密钥管理、备份恢复和传输安全要求。
- 明确当前项目未引入第三方加密库，不建议使用自制简化算法替代专用密码哈希算法。

### 4. 功能完整性

- 新增 `docs/FUNCTIONAL_COMPLETENESS.md`，梳理已实现功能、待补齐功能、边缘场景和用户验收测试标准。

### 5. 代码规范化

- 新增 `docs/CODING_STANDARD.md`，覆盖命名、格式、注释、错误处理、模块边界、自动化检查和代码审查清单。
- 新增 `docs/PROJECT_PLAN.md`，明确五个阶段的里程碑、交付物、质量门禁和风险应对。

## 生产化增强

### 1. 真实编译验证

- 使用 `E:\Program Files\Dev-Cpp\MinGW64\bin\gcc.exe` 完成严格编译验证。
- 编译参数使用 `-std=c99 -Wall -Wextra -Wpedantic -Werror`。
- 清理 `createGuideGraph` 中未使用的历史变量，解决严格编译失败问题。

### 2. 自动化测试扩展

- 扩展 `tests/test_runner.c`，覆盖图 CRUD、Dijkstra、Floyd、DFS、环路检测、密码哈希、统计保存加载等核心场景。
- 新增 `docs/TEST_REPORT.md`，记录编译命令、测试命令、端到端流程和覆盖率说明。

### 3. 密码哈希存储

- 新增 `password.c`、`password.h`。
- 新注册用户使用 PBKDF2-HMAC-SHA256 格式保存密码。
- 登录验证兼容历史明文用户记录，避免旧数据立即失效。

### 4. 景点道路 CRUD

- 新增 `graphAddSpot`、`graphRenameSpot`、`graphDeleteSpot`、`graphAddOrUpdateRoad`、`graphDeleteRoad`、`manageGraphCrud`。
- 主菜单新增景点道路管理入口。
- 新增 `docs/API_DESIGN.md`，说明当前 C 接口和未来 RESTful API 映射关系。

### 5. 访问统计

- 新增 `stats.c`、`stats.h`。
- 支持统计系统启动、登录、注册、路径查询和景点访问次数。
- 主菜单新增访问统计报表入口。
- 修复 `statsLoad` 读取 `pathQueries` 键名被截断的问题。

## 迭代优化：覆盖率、安全与架构解耦

### 1. 测试覆盖率提升

- 新增 `graph_console.c`，将图管理菜单、手工建图、图打印和导游路线展示从 `graph.c` 中迁出。
- 新增 `input.c`，将控制台输入函数从 `global.c` 中迁出。
- 新增 `user_console.c`，将登录、注册和用户菜单交互从 `userManager.c` 中迁出。
- 新增 `graphSaveToFiles`、`graphLoadFromFiles`，支持对图数据文件进行可测试的路径化读写。
- 新增 `saveUserToFile`、`validateUserInFile`，支持用户管理失败路径和迁移逻辑测试。
- 扩展测试覆盖文件不存在、格式错误、图数据损坏、不可达路径、重复注册、登录失败和旧密码迁移。

验证结果：

- 主程序严格编译通过。
- 测试程序严格编译通过。
- 自动化测试输出 `All tests passed.`。
- 核心业务覆盖率提升至 81.29%。

说明：整体核心覆盖率已超过 80%；`graph.c`、`travels.c`、`userManager.c` 尚未达到单模块 90% 目标，后续仍需继续补充测试和拆分交互式路径查询。

### 2. 安全体系增强

- 旧明文用户在登录验证成功后自动迁移为 PBKDF2-HMAC-SHA256 哈希格式。
- 密码盐值生成改为优先使用系统级安全随机源。
- 保留历史明文兼容能力，避免旧用户无法登录。

后续建议：

- 接入经过审计的 bcrypt、Argon2 或 PBKDF2 第三方库。
- 增加迁移前备份、迁移日志和失败回滚机制。

### 3. 架构升级方案

- 新增 `docs/ITERATION_OPTIMIZATION_PLAN.md`，记录测试、安全、RESTful 服务层的分阶段实施方案。
- 当前判断：项目仍是本地控制台程序，RESTful API 应在确认远程访问或前后端分离需求后再引入独立服务层。

### 4. 静态分析

- 本地尝试执行 `cppcheck`，当前环境未安装该工具。
- 本轮以 GCC 严格编译作为基础静态检查，专用静态分析需在安装 `cppcheck` 后或 CI 中继续执行。

## 2026-07-13 测试拆分与 cppcheck 专项修复

### 1. 单元测试补充

- 继续拆分 `graph.c`、`travels.c`、`userManager.c` 的测试用例，确保单个测试只验证一个功能点或异常条件。
- 为图文件保存、加载、CRUD、路径不可达、非法索引和用户管理失败路径补充断言。
- 新增 `UNIT_TEST` 条件编译入口，用于验证旧明文密码迁移的内部异常路径。
- `Makefile` 的测试编译流程增加 `-DUNIT_TEST`，保证本地和 CI 测试行为一致。

### 2. 覆盖率结果

- 使用 MinGW64 GCC 覆盖率插桩重新生成 `gcov` 结果。
- 最终核心覆盖率提升至 92.35%。
- 重点文件覆盖率：
  - `graph.c`: 90.65%
  - `travels.c`: 93.66%
  - `userManager.c`: 94.87%

### 3. cppcheck 静态分析

- 下载并安装 `cppcheck 2.21.0` 到 `tools/cppcheck`。
- 对 `graph.c`、`travels.c`、`userManager.c` 执行定向静态分析。
- 修复空指针风险、数组越界访问风险、变量遮蔽、变量作用域过大和缺失 `const` 修饰等问题。
- 最终 `docs/cppcheck-target-report.txt` 长度为 0，目标三文件无剩余告警。
- 新增 `docs/STATIC_ANALYSIS_REPORT.md`，记录工具版本、执行命令、修复项和最终结论。

### 4. 验证结果

- 主程序严格编译通过：`-std=c99 -Wall -Wextra -Wpedantic -Werror`。
- 测试程序严格编译通过：`-DUNIT_TEST -std=c99 -Wall -Wextra -Wpedantic -Werror`。
- 测试运行退出码为 `0`。

## 2026-07-13 系统改进与优化实施

### 1. 文档一致性

- 更新 `README.md`，同步最新测试覆盖率、功能限制、旧密码迁移状态和工程化文档链接。
- 同步 `docs/TEST_REPORT.md`、`docs/ITERATION_OPTIMIZATION_PLAN.md`、`docs/COVERAGE_GATE_PLAN.md` 和 `docs/PROJECT_EVALUATION_REPORT.md` 中的覆盖率数据。

### 2. 覆盖率门禁

- 修改 `.github/workflows/ci.yml`，移除覆盖率检查中的容错逻辑。
- 新增 `docs/COVERAGE_GATE_PLAN.md`，明确整体 80% 和核心模块 90% 的分阶段门禁计划。

### 3. 编码统一

- 在 `main.c` 中为 Windows 控制台配置 UTF-8 输入输出编码。
- 修复主菜单中的历史乱码文本。
- 新增 `docs/ENCODING_STANDARD.md`，明确源码、文档、数据文件和日志文件统一使用 UTF-8 无 BOM。

### 4. 用户迁移安全

- 旧明文密码迁移前自动在 `backup/user/` 下创建时间戳备份。
- 新增迁移日志 `backup/user/migration.log`，记录迁移状态、影响记录数和耗时。
- 迁移替换原文件失败时，尝试从备份回滚。
- 补充迁移辅助函数和异常路径测试。

### 5. 权限系统

- 新增 `UserRole`、当前用户记录和 `userCanManageGraph` 权限判断。
- 登录成功后记录当前用户角色。
- 创建或替换景点图、维护景点道路前增加管理员权限检查。
- 新增 `docs/PERMISSION_MATRIX.md`。

### 6. 服务层架构

- 新增 `docs/SERVICE_ARCHITECTURE.md`，明确只有存在多端访问需求时才启动服务层建设。
- 文档中定义了未来服务层分层原则、候选接口契约和错误码建议。

### 7. 性能基准

- 新增 `tests/perf_benchmark.c`，用于 Dijkstra 最短路径查询性能基准。
- `Makefile` 新增 `perf` 目标。
- 新增 `docs/PERFORMANCE_BASELINE.md`，记录测试场景、阈值和本地基准结果。

### 8. 最新验证

- 主程序、测试程序和性能基准程序均通过严格编译。
- 单元测试退出码为 `0`。
- 最新覆盖率：总体 92.57%，`graph.c` 90.65%，`travels.c` 93.66%，`userManager.c` 93.98%。
- `cppcheck` 定向分析报告长度为 0。

## 2026-07-13 改进建议落实与最终回归

### 1. 编码与乱码治理

- 清理核心源码、头文件和菜单输出中的历史乱码注释与残留文本。
- 移除 `graph.c` 中已禁用的历史交互实现，减少无效旧代码和乱码残留。
- 复跑乱码扫描，未发现替换字符或典型乱码模式。

### 2. CI 覆盖率门禁

- CI 保持总体行覆盖率 80% 硬门禁。
- 新增 P0 核心模块 90% 分模块硬门禁：
  - `graph.c`
  - `travels.c`
  - `userManager.c`

### 3. 权限、审计与交互体验

- 用户角色从“用户名规则”升级为 `user.txt` 第三字段角色规则。
- 管理员操作写入 `audit/admin_actions.log`。
- 帮助菜单增加迁移审计和管理员操作审计查询入口。
- 图替换、景点删除和道路删除等关键操作增加确认机制。

### 4. 性能基准扩展

- 扩展 Dijkstra 性能基准，覆盖链式图、随机稀疏图、密集图和不可达图。
- 基准输出增加可达性、估算内存、总 CPU 时间和平均查询耗时。

### 5. 最终验证

- 主程序严格编译通过：`-std=c99 -Wall -Wextra -Wpedantic -Werror`。
- 测试程序严格编译通过并运行成功，退出码为 `0`。
- 性能基准程序严格编译通过并运行成功。
- `cppcheck 2.21.0` 对 `graph.c`、`travels.c`、`userManager.c` 的定向分析无剩余告警。
- 最新覆盖率记录：总体 92.57%，`graph.c` 90.65%，`travels.c` 93.66%，`userManager.c` 93.98%。

## 2026-07-13 分支覆盖率门禁与低覆盖模块补强

### 1. 测试补充

- 新增 `graph.c` 默认路径包装集成测试，覆盖 `saveGraph` 和 `loadGraph` 对 `graphParams.txt`、`graphVertex.txt`、`graphEdge.txt` 的保存与加载流程。
- 默认路径测试会在执行前备份真实数据文件，执行后恢复，避免破坏项目根目录数据。
- 新增 `password.c` 固定测试向量、长密码、长 salt、特殊字符密码和异常参数测试。
- 新增 `stats.c` 保存失败、空图报表和非法景点访问分支测试。

### 2. CI 门禁

- 在 CI 覆盖率检查中新增整体分支覆盖率 80% 硬门禁。
- 将 `password.c` 和 `stats.c` 纳入 90% 行覆盖率分模块门禁。

### 3. 最新验证结果

- 主程序严格编译通过。
- 覆盖率插桩测试程序严格编译通过并运行成功。
- `gcovr 8.6` 生成 HTML、XML、JSON 和文本覆盖率报告。
- 最新覆盖率：总体行覆盖率 94.2%，函数覆盖率 100.0%，分支覆盖率 82.2%。
- 单文件覆盖率：`graph.c` 92%，`password.c` 95%，`stats.c` 98%。
- `cppcheck 2.21.0` 扩展扫描 `graph.c`、`travels.c`、`userManager.c`、`password.c`、`stats.c`、`input.c`，无剩余告警。

## 2026-07-13 最终收尾审查与仓库清理

### 1. 项目审查

- 新增 `docs/FINAL_REVIEW_REPORT.md`，记录最终结构评估、验证结果、清理范围和剩余边界。
- 复核源码、测试、文档、CI 和数据样例，未发现阻塞交付的重大功能缺陷、性能问题或安全隐患。

### 2. 文件清理

- 删除历史压缩包 `toursys.rar`。
- 清理本地构建产物、覆盖率产物、运行日志、备份目录和本地工具链目录。
- 补充 `.gitignore`，忽略 `tools/`、安装包和压缩包等非源码交付内容。

### 3. 最终验证

- 主程序严格编译通过。
- 单元测试通过。
- 覆盖率门禁通过：总体行覆盖率 94.2%，总体分支覆盖率 82.2%。
- 静态分析目标文件无告警。
