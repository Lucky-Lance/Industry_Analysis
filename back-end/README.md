# 后端安装与启动

### 环境配置
- 安装`FastAPI`和`Uvicorn`
  ```
  pip install fastapi
  pip install uvicorn[standard]
  ```

### 启动
- 执行下列命令
  ```
  uvicorn main:app --reload
  ```
- 查看API请访问[交互式 API 文档](http://127.0.0.1:8000/docs)