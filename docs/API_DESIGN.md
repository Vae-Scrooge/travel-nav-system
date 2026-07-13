# 景点道路接口设计说明

## 当前实现边界

当前项目是 C 语言控制台程序，不包含 HTTP 服务框架，因此本轮实现的是景点和道路 CRUD 的业务接口与控制台菜单入口，而不是实际网络 RESTful API。

已落地的 C 语言接口如下：

| 操作 | C 接口 | 说明 |
|---|---|---|
| 新增景点 | `graphAddSpot` | 创建景点 |
| 修改景点 | `graphRenameSpot` | 修改景点名称 |
| 删除景点 | `graphDeleteSpot` | 删除景点并同步删除关联道路 |
| 新增或更新道路 | `graphAddOrUpdateRoad` | 创建或更新两个景点之间的道路 |
| 删除道路 | `graphDeleteRoad` | 删除两个景点之间的道路 |
| 控制台管理入口 | `manageGraphCrud` | 提供交互式管理菜单 |

## RESTful 映射建议

如果后续引入 HTTP 服务层，可按以下方式映射：

| HTTP 方法 | 路径 | 对应能力 |
|---|---|---|
| `GET` | `/spots` | 查询景点列表 |
| `POST` | `/spots` | 新增景点 |
| `PUT` | `/spots/{name}` | 修改景点名称 |
| `DELETE` | `/spots/{name}` | 删除景点 |
| `GET` | `/roads` | 查询道路列表 |
| `POST` | `/roads` | 新增道路 |
| `PUT` | `/roads/{from}/{to}` | 修改道路长度 |
| `DELETE` | `/roads/{from}/{to}` | 删除道路 |
| `GET` | `/stats` | 查询访问统计报表 |

## 参数验证要求

1. 景点名称不能为空。
2. 景点名称不能重复。
3. 景点数量不能超过 `MAXNUM`。
4. 道路两端景点必须存在。
5. 道路不能连接同一个景点。
6. 道路长度必须为正整数。
7. 删除景点时必须同步删除所有关联道路。

## 错误处理建议

| 场景 | RESTful 状态码建议 |
|---|---|
| 参数非法 | `400 Bad Request` |
| 景点不存在 | `404 Not Found` |
| 景点重复 | `409 Conflict` |
| 超过最大容量 | `409 Conflict` |
| 服务器内部错误 | `500 Internal Server Error` |

## 后续落地方案

若需要真正提供 RESTful API，建议新增独立服务层，例如：

1. 使用 C Web 框架封装当前图模块。
2. 或将核心图逻辑迁移为库，由 Python、Go、Java 或 Node.js 服务调用。
3. 保留当前 `graph.c` 中的 CRUD 业务函数作为领域层，HTTP 层只负责参数解析和响应转换。
