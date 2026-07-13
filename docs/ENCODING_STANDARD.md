# 编码规范与乱码治理方案

## 统一标准

| 对象 | 标准 |
|---|---|
| 源码文件 | UTF-8，无 BOM |
| Markdown 文档 | UTF-8，无 BOM |
| 数据文件 | UTF-8，无 BOM |
| 控制台输出 | UTF-8 |
| 日志文件 | UTF-8 |

## 控制台配置

Windows 环境下，程序启动时调用 `SetConsoleOutputCP(CP_UTF8)` 和 `SetConsoleCP(CP_UTF8)`，使控制台输入输出优先使用 UTF-8。

Linux/macOS 环境下，依赖终端 `LANG` / `LC_ALL` 设置为 UTF-8，例如：

```bash
export LANG=C.UTF-8
```

## 文件保存要求

1. 新增 `.c`、`.h`、`.md`、`.txt` 文件时统一保存为 UTF-8，无 BOM。
2. 不再新增 GBK、ANSI 或混合编码文件。
3. 历史乱码文本应优先替换为清晰英文或标准中文。
4. 若源文件中需要中文字符串，必须确认编译器和运行终端均按 UTF-8 处理。

## 历史数据治理

1. `user.txt`、`graphVertex.txt`、`graphEdge.txt`、`graphParams.txt` 应统一按 UTF-8 读取和保存。
2. 历史日志文件如存在乱码，迁移前应先备份，再转换编码。
3. 密码哈希、盐值和系统标识字段仅使用 ASCII 字符，避免编码影响校验。

## 验证清单

1. 启动程序后菜单无乱码。
2. 景点名称包含中文时，保存和重新加载后内容一致。
3. `backup/user/migration.log` 可用 UTF-8 编辑器正常查看。
4. CI 中不新增非 UTF-8 编码文件。

## 当前状态

本轮已修复主菜单、路径环路输出和核心头文件中的历史乱码，并在主程序中增加 Windows UTF-8 控制台配置。`graph.c` 中已禁用的历史旧实现已移除，减少无效代码和乱码注释残留。

当前源码和主要文档已通过以下检查：

```bash
rg -n "<replacement-character>|<mojibake-pattern>" --glob "*.c" --glob "*.h" --glob "*.md" --glob "*.txt"
```

后续如新增中文内容，应继续保持 UTF-8 无 BOM，并在提交前执行同类检查。
