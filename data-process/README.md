# 数据预处理部分

> ftc 2022-05-03

* 使用环境

  > 本人是在ubuntu20.04 g++ cpp17环境下运行代码的，代码是跨平台的，不过不知道windows下运行会不会出现问题

* 使用方法：

  ```shell
  cmake .
  make
  ./main
  ```

* 使用说明

  > 首先确保`Node.csv`、`Link.csv`在当前文件夹。
  >
  > main.cpp里面有两个函数，首先运行`divideGraph()`，这个函数会生成`./output`这个文件夹，然后写入文件，命名格式为`"%08d-Link-%08d.csv"%(子图序号,这个子图有多少个节点)`和`"%08d-Node-%08d.csv"%(子图序号,这个子图有多少个节点)`。其中`xxx-Link-xxx.csv`存储的是子图里的边，格式与`Link.csv`完全一致，`xxx-Node-xxx.csv`存储的是子图节点，格式与`Node.csv`完全一致。采取联通分量分割算法，把两边都不含黑灰产业的边断掉。
  >
  > 其次运行`analyseTheBiggest()`，这个函数分析最大子图——`./output/00000055-Link-00172977.csv`和`./output/00000055-Node-00172977.csv`。首先给子图中的边赋值，边`A->B`复值为`10*{A是否包含黑产}+2*{A的黑产个数}`。其次给节点赋值，节点权值等于所有入边权值之和。把所有节点权值计算完成后，找到权值最高的那一个（权值15538），记为`root`。采取dfs算法，对`root`周边0\~5轮能遍历到的节点做记录，并输出。
  >
  > 函数`subGraphToJsonForAnalyse()`负责把图转化成json，json格式见`index.html`或者https://echarts.apache.org/examples/zh/editor.html?c=graph-webkit-dep

* 后续开发预期

  > 与前端小组成员做交互，提供特定数据格式的接口。

