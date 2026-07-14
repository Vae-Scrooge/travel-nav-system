# 贡献指南

欢迎为 travel-nav-system 项目做出贡献！本指南旨在帮助新贡献者快速了解项目贡献方式，确保贡献过程顺畅高效。

## 目录

- [行为准则](#行为准则)
- [开发环境配置](#开发环境配置)
- [代码贡献流程](#代码贡献流程)
- [提交规范](#提交规范)
- [代码风格要求](#代码风格要求)
- [测试要求](#测试要求)
- [文档贡献](#文档贡献)

## 行为准则

### 我们的承诺

为了营造一个开放和友好的环境，我们作为贡献者和维护者承诺让每个人都能无骚扰地参与我们的项目和社区。

### 我们的标准

有助于创建积极环境的行为包括：

- 使用友好和包容的语言
- 尊重不同的观点和经验
- 优雅地接受建设性批评
- 关注对社区最有利的事情
- 对其他社区成员表现出同理心

## 开发环境配置

### 系统要求

- **操作系统**：Windows、macOS 或 Linux
- **编译器**：GCC 4.9+ 或 Clang
- **构建工具**：Make
- **测试工具**：gcov/gcovr（覆盖率分析）

### Windows 环境配置

```powershell
# 安装 MinGW64
# 下载地址：https://sourceforge.net/projects/mingw-w64/

# 设置环境变量
$env:Path = "E:\Program Files\Dev-Cpp\MinGW64\bin;" + $env:Path

# 验证安装
gcc --version
make --version
```

### Linux 环境配置

```bash
# Ubuntu/Debian
sudo apt-get update
sudo apt-get install -y gcc make gcovr cppcheck

# CentOS/RHEL
sudo yum install -y gcc make gcovr cppcheck
```

### macOS 环境配置

```bash
# 使用 Homebrew
brew install gcc make gcovr cppcheck
```

### 项目构建

```bash
# 克隆项目
git clone https://github.com/<your-username>/travel-nav-system.git
cd travel-nav-system

# 编译主程序
make build

# 运行测试
make test

# 覆盖率分析
make coverage
```

## 代码贡献流程

### 1. Fork 项目

在 GitHub 上点击 "Fork" 按钮创建项目的个人副本。

### 2. 创建功能分支

```bash
# 确保本地仓库是最新的
git fetch upstream
git checkout main
git merge upstream/main

# 创建功能分支
git checkout -b feature/<feature-name>
```

### 3. 开发和测试

- 编写代码实现功能
- 运行测试确保代码正确
- 检查代码风格符合规范

### 4. 提交代码

```bash
# 添加修改的文件
git add <file-name>

# 提交代码（遵循提交规范）
git commit -m "feat: 添加新功能描述"
```

### 5. 推送分支

```bash
git push origin feature/<feature-name>
```

### 6. 创建 Pull Request

在 GitHub 上创建 Pull Request，填写完整的描述信息。

### 7. 代码审查

- 维护者会审查你的代码
- 根据反馈进行修改
- 修改后再次提交

### 8. 合并代码

审查通过后，维护者会将你的代码合并到主分支。

## 提交规范

### 提交类型

| 类型 | 说明 |
|------|------|
| `feat` | 新功能（feature） |
| `fix` | 修复 bug |
| `docs` | 文档更新 |
| `style` | 代码风格调整（不影响代码逻辑） |
| `refactor` | 重构（既不新增功能，也不修复 bug） |
| `perf` | 性能优化 |
| `test` | 测试相关 |
| `chore` | 构建过程或辅助工具的变动 |
| `ci` | CI/CD 配置变动 |

### 提交格式

```
<类型>(<范围>): <描述>

<详细说明>

<相关Issue链接>
```

### 示例

```
feat(graph): 添加景点重命名功能

- 实现 graphRenameSpot 函数
- 更新图数据结构
- 添加单元测试

Closes #123
```

## 代码风格要求

### 命名规范

| 类型 | 规则 | 示例 |
|------|------|------|
| 函数 | 小驼峰或动宾结构 | `initGraph`、`freeGraph`、`addEdge` |
| 变量 | 小驼峰 | `roadList`、`nodeIndex` |
| 常量 | 全大写，下划线分隔 | `MAXNUM`、`INF`、`HASH_SIZE` |
| 结构体 | 首字母大写 | `ALGraph`、`CNode`、`HNode` |
| 文件 | 小写，下划线分隔 | `graph.c`、`user_manager.c` |

### 格式规范

- 使用 4 个空格进行缩进
- 每行代码不超过 80 个字符
- 函数之间空两行
- 代码块之间空一行

### 注释规范

- 函数开头需要注释说明功能、参数和返回值
- 复杂逻辑需要添加注释说明
- 变量声明时可以添加注释说明用途
- 注释使用中文

### 错误处理

- 所有函数参数需要进行合法性检查
- 返回值需要明确的错误码或返回状态
- 错误信息需要清晰明确

## 测试要求

### 单元测试

- 新增功能必须添加单元测试
- 测试代码放在 `tests/` 目录下
- 测试函数命名格式：`test_<功能名称>`

### 测试覆盖率

- 新增代码的行覆盖率应达到 90% 以上
- 分支覆盖率应达到 80% 以上
- 核心模块的覆盖率要求在 CI 中强制执行

### 测试命令

```bash
# 运行所有测试
make test

# 生成覆盖率报告
make coverage

# 查看覆盖率详情
gcovr --config .gcovr.cfg --html-details coverage/report.html
```

## 文档贡献

### 文档类型

| 文档 | 说明 |
|------|------|
| `README.md` | 项目简介和快速开始 |
| `docs/PROJECT_OVERVIEW.md` | 项目概述和技术架构 |
| `docs/FEATURE_GUIDE.md` | 功能指南和使用说明 |
| `docs/API_REFERENCE.md` | API 接口参考 |
| `docs/TESTING_GUIDE.md` | 测试指南 |

### 文档规范

- 使用 Markdown 格式编写
- 语言使用中文
- 代码块需要添加语言标识
- 表格使用标准 Markdown 格式

## 问题反馈

如果在贡献过程中遇到问题，可以通过以下方式反馈：

- 在 GitHub 上创建 Issue
- 发送邮件给项目维护者
- 在社区讨论区提问

## 贡献者名单

感谢所有为项目做出贡献的开发者！

---

**最后更新**：2026-07-14
