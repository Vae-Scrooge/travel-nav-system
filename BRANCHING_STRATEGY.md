# 分支管理策略

本文件描述 travel-nav-system 项目的分支管理策略，基于 Git Flow 工作流进行调整和优化。

## 目录

- [分支类型](#分支类型)
- [分支命名规范](#分支命名规范)
- [分支创建流程](#分支创建流程)
- [分支合并流程](#分支合并流程)
- [分支保护规则](#分支保护规则)
- [标签管理](#标签管理)
- [分支生命周期](#分支生命周期)

## 分支类型

### 主分支

| 分支名称 | 用途 | 保护级别 |
|---------|------|---------|
| `main` | 生产环境代码，包含稳定的已发布版本 | 🔒 完全保护 |
| `develop` | 开发环境主分支，包含最新的开发代码 | 🔒 需要PR审查 |

### 辅助分支

| 分支类型 | 用途 | 保护级别 |
|---------|------|---------|
| `feature/*` | 功能开发分支，用于开发新功能 | ❌ 无保护 |
| `bugfix/*` | Bug 修复分支，用于修复生产环境问题 | ❌ 无保护 |
| `release/*` | 发布分支，用于准备新版本发布 | 🔒 需要PR审查 |
| `hotfix/*` | 紧急修复分支，用于修复生产环境紧急问题 | 🔒 需要PR审查 |

## 分支命名规范

### 功能分支

```
feature/<feature-name>
feature/<issue-number>-<feature-name>
```

示例：
- `feature/add-graph-export`
- `feature/123-add-user-permission`

### Bug 修复分支

```
bugfix/<bug-description>
bugfix/<issue-number>-<bug-description>
```

示例：
- `bugfix/fix-dijkstra-crash`
- `bugfix/456-fix-memory-leak`

### 发布分支

```
release/<version-number>
```

示例：
- `release/v1.0.0`
- `release/v1.1.0`

### 紧急修复分支

```
hotfix/<version-number>-<issue-description>
```

示例：
- `hotfix/v1.0.1-critical-security`
- `hotfix/v1.0.2-data-loss`

## 分支创建流程

### 从 develop 创建功能分支

```bash
# 确保 develop 分支是最新的
git checkout develop
git pull origin develop

# 创建功能分支
git checkout -b feature/add-graph-export

# 推送分支到远程
git push -u origin feature/add-graph-export
```

### 从 develop 创建 Bug 修复分支

```bash
# 确保 develop 分支是最新的
git checkout develop
git pull origin develop

# 创建 Bug 修复分支
git checkout -b bugfix/fix-dijkstra-crash

# 推送分支到远程
git push -u origin bugfix/fix-dijkstra-crash
```

### 从 develop 创建发布分支

```bash
# 确保 develop 分支是最新的
git checkout develop
git pull origin develop

# 创建发布分支
git checkout -b release/v1.0.0

# 更新版本号和 CHANGELOG
# ...

# 推送分支到远程
git push -u origin release/v1.0.0
```

### 从 main 创建紧急修复分支

```bash
# 确保 main 分支是最新的
git checkout main
git pull origin main

# 创建紧急修复分支
git checkout -b hotfix/v1.0.1-critical-security

# 推送分支到远程
git push -u origin hotfix/v1.0.1-critical-security
```

## 分支合并流程

### 功能分支合并到 develop

1. 创建 Pull Request 从 `feature/*` 到 `develop`
2. 至少一个维护者审查通过
3. CI/CD 测试通过
4. 合并到 develop

```bash
# 在 develop 分支上合并
git checkout develop
git pull origin develop
git merge --no-ff feature/add-graph-export
git push origin develop

# 删除本地和远程分支
git branch -d feature/add-graph-export
git push origin --delete feature/add-graph-export
```

### Bug 修复分支合并到 develop

1. 创建 Pull Request 从 `bugfix/*` 到 `develop`
2. 至少一个维护者审查通过
3. CI/CD 测试通过
4. 合并到 develop

```bash
git checkout develop
git pull origin develop
git merge --no-ff bugfix/fix-dijkstra-crash
git push origin develop

git branch -d bugfix/fix-dijkstra-crash
git push origin --delete bugfix/fix-dijkstra-crash
```

### 发布分支合并到 main 和 develop

1. 创建 Pull Request 从 `release/*` 到 `main`
2. 至少一个维护者审查通过
3. CI/CD 测试通过
4. 合并到 main
5. 打标签
6. 合并到 develop（包含发布期间的修复）

```bash
# 合并到 main
git checkout main
git pull origin main
git merge --no-ff release/v1.0.0
git push origin main

# 打标签
git tag -a v1.0.0 -m "Release v1.0.0"
git push origin v1.0.0

# 合并到 develop
git checkout develop
git pull origin develop
git merge --no-ff release/v1.0.0
git push origin develop

# 删除发布分支
git branch -d release/v1.0.0
git push origin --delete release/v1.0.0
```

### 紧急修复分支合并到 main 和 develop

1. 创建 Pull Request 从 `hotfix/*` 到 `main`
2. 至少一个维护者审查通过
3. CI/CD 测试通过
4. 合并到 main
5. 打标签
6. 合并到 develop

```bash
# 合并到 main
git checkout main
git pull origin main
git merge --no-ff hotfix/v1.0.1-critical-security
git push origin main

# 打标签
git tag -a v1.0.1 -m "Hotfix v1.0.1"
git push origin v1.0.1

# 合并到 develop
git checkout develop
git pull origin develop
git merge --no-ff hotfix/v1.0.1-critical-security
git push origin develop

# 删除紧急修复分支
git branch -d hotfix/v1.0.1-critical-security
git push origin --delete hotfix/v1.0.1-critical-security
```

## 分支保护规则

### main 分支

| 规则 | 配置 |
|------|------|
| 需要 PR 审查 | ✅ 启用 |
| 至少审查人数 | 1 人 |
| 需要状态检查通过 | ✅ 启用 |
| 禁止强制推送 | ✅ 启用 |
| 禁止直接推送 | ✅ 启用 |
| 需要线性历史 | ✅ 启用 |

### develop 分支

| 规则 | 配置 |
|------|------|
| 需要 PR 审查 | ✅ 启用 |
| 至少审查人数 | 1 人 |
| 需要状态检查通过 | ✅ 启用 |
| 禁止强制推送 | ✅ 启用 |
| 禁止直接推送 | ✅ 启用 |
| 需要线性历史 | ✅ 启用 |

### release/* 分支

| 规则 | 配置 |
|------|------|
| 需要 PR 审查 | ✅ 启用 |
| 至少审查人数 | 1 人 |
| 需要状态检查通过 | ✅ 启用 |
| 禁止强制推送 | ✅ 启用 |

### hotfix/* 分支

| 规则 | 配置 |
|------|------|
| 需要 PR 审查 | ✅ 启用 |
| 至少审查人数 | 1 人 |
| 需要状态检查通过 | ✅ 启用 |
| 禁止强制推送 | ✅ 启用 |

## 标签管理

### 标签命名规范

```
v<major>.<minor>.<patch>
v<major>.<minor>.<patch>-<pre-release>
```

示例：
- `v1.0.0`
- `v1.0.1`
- `v1.1.0`
- `v2.0.0-alpha.1`
- `v2.0.0-beta.2`

### 标签创建流程

1. 在 `main` 分支上创建标签
2. 添加标签描述
3. 推送标签到远程

```bash
# 创建标签
git tag -a v1.0.0 -m "Release v1.0.0"

# 推送标签
git push origin v1.0.0
```

### 标签删除流程

```bash
# 删除本地标签
git tag -d v1.0.0

# 删除远程标签
git push origin :v1.0.0
```

## 分支生命周期

### 功能分支

- **创建**：从 develop 分支创建
- **活跃期**：开发功能，定期推送
- **合并**：合并到 develop 分支
- **删除**：合并后删除本地和远程分支

### Bug 修复分支

- **创建**：从 develop 分支创建
- **活跃期**：修复 bug，定期推送
- **合并**：合并到 develop 分支
- **删除**：合并后删除本地和远程分支

### 发布分支

- **创建**：从 develop 分支创建
- **活跃期**：准备发布，修复发布相关问题
- **合并**：合并到 main 和 develop 分支
- **删除**：合并后删除本地和远程分支

### 紧急修复分支

- **创建**：从 main 分支创建
- **活跃期**：修复紧急问题
- **合并**：合并到 main 和 develop 分支
- **删除**：合并后删除本地和远程分支

## 工作流程图

```
main ------------------------------*------------------------*----------
     \                            / \                      / \
      \                          /   \                    /   \
       \                        /     \                  /     \
        hotfix/*              /       \                /       \
               \             /         \              /         \
                *-----------           *------------*           *
                                   release/*
                                    /       \
                                   /         \
                          develop *-----------*------------------*------
                               /             \
                              /               \
                       feature/*           bugfix/*
                             *                 *
```

## 最佳实践

### 提交频率

- 每天至少提交一次代码
- 每个小功能完成后提交
- 修复一个 bug 后提交

### 分支大小

- 功能分支应该在 1-2 周内完成
- 如果功能太大，拆分为多个小功能
- 每个 PR 的代码量不宜超过 500 行

### 冲突处理

- 定期从上游分支拉取最新代码
- 在合并前解决所有冲突
- 使用 rebase 保持线性历史

### 代码审查

- 审查前确保代码通过测试
- 审查时关注代码质量和安全性
- 至少需要一个审查者批准

---

**最后更新**：2026-07-14
