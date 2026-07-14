# API 参考

## 1. 图管理接口

### 1.1 初始化与清理

```c
void initGraph(ALGraph *graph);           // 初始化图
void freeGraph(ALGraph *graph);           // 释放图内存
```

### 1.2 景点操作

```c
int graphAddSpot(ALGraph *graph, const char *name);               // 添加景点
int graphRenameSpot(ALGraph *graph, const char *oldName, const char *newName); // 修改景点名称
int graphDeleteSpot(ALGraph *graph, const char *name);            // 删除景点
int graphUpdateSpotInfo(ALGraph *graph, const char *name, const char *description, double ticketPrice, const char *openTime); // 更新景点信息
void graphPrintSpotInfo(ALGraph *graph, const char *name);        // 打印景点信息
```

### 1.3 道路操作

```c
int addEdge(ALGraph *graph, int from, int to, int length);        // 添加边
int deleteEdge(ALGraph *graph, int from, int to);                 // 删除边
int getlength(ALGraph graph, int from, int to);                   // 获取边长度
int isedg(ALGraph graph, char *v1, char *v2);                     // 判断是否有边
```

### 1.4 文件操作

```c
void saveGraph(ALGraph *graph);                                   // 保存图到文件
void loadGraph(ALGraph *graph);                                   // 从文件加载图
int graphSaveToFiles(const ALGraph *graph, const char *paramsPath, const char *vertexPath, const char *edgePath); // 保存到指定文件
int graphLoadFromFiles(ALGraph *graph, const char *paramsPath, const char *vertexPath, const char *edgePath); // 从指定文件加载
```

## 2. 用户管理接口

### 2.1 用户认证

```c
int validateUserInFile(const char *filepath, const char *username, const char *password, int *role); // 验证用户
int saveUserToFile(const char *filepath, const char *username, const char *password); // 保存用户
```

### 2.2 用户管理

```c
const char *userManagerGetCurrentUser(void);                      // 获取当前用户
int userChangePassword(const char *username, const char *oldPassword, const char *newPassword); // 修改密码
void userManagerPrintMigrationAudit(const char *userPath, int limit); // 打印迁移审计
void userManagerPrintAdminAudit(const char *userPath, int limit);     // 打印管理员审计
```

## 3. 路径规划接口

### 3.1 最短路径

```c
void transToMatrix(ALGraph *graph);                               // 转换为邻接矩阵
void shortPath(ALGraph graph, int pathMatrix[][MAXNUM], double shortpath[][MAXNUM]); // Floyd-Warshall算法
void printPath(ALGraph graph, int pathMatrix[][MAXNUM], double shortpath[][MAXNUM], int i, int j); // 打印路径
int dijkstraShortestPath(ALGraph graph, int start, int end, double *distance, int prev[]); // Dijkstra算法
```

### 3.2 导游路线

```c
void createGuideGraph(ALGraph *graph, ALGraph *guidgraph);        // 创建导游图
void createGuideGraphEX(ALGraph *graph, ALGraph *guidgraph, int n); // 创建导游图（扩展）
int islooptest(ALGraph graph);                                    // 环路检测
void DFSTraverse(ALGraph graph);                                  // 深度优先遍历
int exportGuideRoute(ALGraph graph, const char *filename);        // 导出导游路线
```

## 4. 统计分析接口

```c
void statsRecordSpotAccess(ALGraph *graph, const char *spotName); // 记录景点访问
void statsRecordPathQuery(void);                                   // 记录路径查询
void statsPrintReport(ALGraph *graph);                            // 打印统计报告
```

## 5. 数据备份接口

```c
int backupCreate(const ALGraph *graph, const char *userPath);     // 创建备份
int backupRestore(const char *backupName, ALGraph *graph, const char *userPath); // 恢复备份
void backupList(void);                                            // 列出备份
int backupDelete(const char *backupName);                         // 删除备份
```

## 6. 工具函数

```c
int locate(ALGraph graph, const char *e);                         // 查找顶点索引
int getEdgeNum(ALGraph *graph, const char *name);                 // 获取边数
int readInt(int *value);                                          // 读取整数
int readString(char *buffer, size_t maxLen);                      // 读取字符串
```