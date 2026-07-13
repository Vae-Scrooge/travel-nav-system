# GitHub Actions 推送后 CI 失败分析与修复记录

## 1. 错误表现

失败链接：

`https://github.com/Vae-Scrooge/travel-nav-system/actions/runs/29237695664/job/86776133897`

页面显示：

- 工作流：`CI`
- 作业：`build-and-test`
- 触发提交：`Merge GitHub main history`
- 失败步骤：`Build`
- 错误注解：`Process completed with exit code 2.`
- 附带警告：`actions/checkout@v4` 运行时涉及 Node.js 20 弃用提示，该提示不是本次失败的直接原因。

功能中断位置为 CI 的构建阶段，后续 `Test`、`Coverage` 和 `Static analysis` 无法形成可信的完整流水线结果。

## 2. 触发条件

触发条件如下：

- 向 GitHub 仓库 `Vae-Scrooge/travel-nav-system` 的 `main` 分支推送提交。
- GitHub Actions 使用 `.github/workflows/ci.yml` 中的 `Build` 步骤执行 `make build`。
- Ubuntu Runner 通过 GNU Make 解析仓库根目录 `Makefile`。
- `Makefile` 中部分规则命令使用空格缩进，而 GNU Make 要求规则命令必须以制表符 Tab 开头。
- `BUILD_DIR := build` 后，目录目标 `$(BUILD_DIR):` 与伪目标 `build:` 实际同名，导致 Makefile 存在目标覆盖和循环依赖风险。

## 3. 影响范围

影响范围包括：

- 所有推送到 `main` 分支后自动触发的 CI 构建。
- 所有拉取请求触发的 CI 构建。
- 依赖 CI 状态判断项目可交付性的开发者和评审人员。

本问题不直接影响本地手动 GCC 编译，也不改变运行时业务功能；但会阻断远程自动化验证，因此属于工程交付阻塞问题。

## 4. 日志与状态信息

已收集到的信息：

- GitHub Actions 公开页面显示 `Build` 步骤失败，退出码为 `2`。
- GitHub 作业日志下载接口在未提供仓库管理员权限时返回 `403 Must have admin rights to Repository`，因此无法通过公开接口直接下载完整日志。
- 本地仓库 `main` 与远程 `origin/main` 在失败时均指向提交 `f22ceffc970ad55b04b999ca12d9fd81561c916a`。
- 本地检查 `Makefile` 前导缩进发现：
  - `test` 目标第一条命令为空格缩进。
  - `coverage` 目标 `gcov` 命令为空格缩进。
  - `perf` 目标两条命令为空格缩进。
- 本地 `mingw32-make` 进一步暴露同名目标问题：
  - `warning: overriding recipe for target 'build'`
  - `warning: ignoring old recipe for target 'build'`
  - `Circular build <- build dependency dropped.`

## 5. 根本原因

根本技术原因是 `Makefile` 中存在两类工程配置缺陷：

1. 规则命令缩进不统一，部分命令使用空格而不是 Tab。
2. 目录目标 `$(BUILD_DIR):` 展开后与伪目标 `build:` 同名，导致 Make 解析时覆盖规则，并使 `build/` 目录创建逻辑不可靠。

GNU Make 在读取 Makefile 时会解析整个文件，而不是只解析当前目标的命令行。因此，即使 CI 当前执行的是 `make build`，后续目标中出现空格缩进的命令行也可能导致 Makefile 解析失败，并返回退出码 `2`。同名目标问题还会导致 `make build` 未可靠创建 `build/` 目录，进一步中断编译输出。

## 6. 修复方案

已实施修复：

- 将 `Makefile` 中所有规则命令缩进统一为 Tab。
- 移除 `$(BUILD_DIR):` 目录目标，改为在 `build`、`test` 和 `perf` 目标中显式执行 `mkdir -p $(BUILD_DIR)`。
- 将默认编译器显式设置为 `gcc`，避免本地 Make 使用不存在的内置默认 `cc`。
- 将 `.gitignore` 中 Dev-C++ 生成文件规则从固定文件名改为通配规则：
  - `*.dev`
  - `*.layout`

该修复不改变业务代码、不改变测试目标、不降低覆盖率门禁。

## 7. 验证方法

修复后需要验证：

1. 严格编译主程序。
2. 编译并运行测试程序。
3. 编译并运行性能基准。
4. 推送到 GitHub 后确认新的 Actions 作业不再在 `Build` 步骤因 Makefile 解析失败中断。

本地验证结果：

- `mingw32-make -n build`、`mingw32-make -n test`、`mingw32-make -n coverage`、`mingw32-make -n perf` 均可正常展开命令，不再出现目标覆盖、循环依赖或 Makefile 解析错误。
- 使用 MinGW64 GCC 直接严格编译主程序通过。
- 使用 MinGW64 GCC 编译并运行单元测试通过。
- 使用 MinGW64 GCC 编译并运行性能基准通过。

说明：当前 Windows 环境中的 `mingw32-make` 没有可用的 Unix `mkdir` 命令，因此本地不直接执行完整 `make build`；GitHub Actions 使用 Ubuntu Runner，具备 `mkdir -p`、`rm -rf`、`gcov` 等 POSIX 工具。

## 8. 后续建议

- 如需进一步消除 Node.js 运行时弃用警告，可在 GitHub Actions 后续版本中升级相关官方 Action 版本。
- 建议未来修改 Makefile 后优先在 Linux 或 CI 环境执行 `make build`、`make test`、`make coverage`。
