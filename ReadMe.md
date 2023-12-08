# 1.csp 简介

csp-(cpp shi pan，C++式盘)，一个cmd命令行起盘工具，计划支持排盘奇门遁甲，大六壬，六爻。

因Me奇门用的较多，故此项目前期多以奇门为主，后续慢慢开发其他排盘，有兴趣的也可以参与贡献。

# 2.已实现功能

## 奇门遁甲

示例图：

![示例图片](https://github.com/taynpg/csp/raw/main/images/csp_qm_sjzpcjzr.png)

- 时家，转盘，超接置润法
- 时家，转盘，阴盘

## 大六壬

- 暂无

## 六爻

- 暂无

# 4.构建

依赖：

- C++11语法

构建方法：

```shell
cd csp
mkdir build
cd build
cmake ..
cmake --build . --config release
```

3.手机端构建

环境：

- 安卓端可以使用termux（亲测可用）
- iPhone端可以使用iSh（暂未测试）

![termux](https://github.com/taynpg/csp/raw/main/images/csp_qm_sjzpcjzr_android.jpg)

构建：方法与MacOS和Linux一致。

# 5.最后

如果您发现了BUG或者有好的建议可以提issue或致邮件taynpg@163.com（欢迎您的任何想法和咨询）。
