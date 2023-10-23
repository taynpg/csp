# 1.csp 简介

csp-(cpp shi pan，C++式盘)，一个cmd命令行起盘工具，计划支持排盘奇门遁甲，大六壬，六爻。

因Me奇门用的较多，故此项目前期多以奇门为主，后续慢慢开发其他排盘，有兴趣的也可以参与贡献。

# 2.杂聊

在学习易学相关内容的过程中，绝大多数情况下，不管何门派都离不开式盘的排布，市面上也有很多的软件可以实现此类功能。但是对于Me来说，软件一来下载麻烦，二来界面过于臃肿复杂，还有不少登录广告等，三是不能在任意地方使用，比如我日常使用windows，出门携带mac，偶尔玩玩linux，手机端之前用的iPhone，现在是Android。据我观察，对易学感兴趣的人群中程序员占比不小，大家开发的工具各有特点，有web的、app的、桌面的等。

不知道大家是否像我一样喜欢**大道至简**。于是有了开发一个cmd命令的念头，初次开发的是一个简易的奇门遁甲 [cqm 时家奇门遁甲排盘本地排盘工具(gitee)](https://gitee.com/taynpg/cqm)、[cqm 时家奇门遁甲排盘本地排盘工具(github)](https://github.com/taynpg/cqm)，日常使用也就够了，但是有不少兄弟们联系我希望新增一些功能，比如我写的是奇门时家转盘，他们希望新增时家拆补，诸葛盘，阴盘。当然还有希望新增六爻，六壬的。原本我只是在cqm的基础上改，但是我发现当初写代码时因为没有考虑到这么多情况，因为基本没法改，不少地方完全不一样，代码也没有做兼容操作，于是就新起此项目，做了底层兼容，例如奇门遁甲，现在可以很轻松的新增几乎任何一种排盘方式。

与cqm不同，csp转用cpp，本项目无gitee托管。

# 3.已实现功能

## 奇门遁甲

示例图：

![示例图片](https://github.com/taynpg/csp/raw/main/images/csp_qm_sjzpcjzr.png)

- 时家，转盘，超接置润法

## 大六壬

- 暂无

## 六爻

- 暂无

# 4.构建

依赖：

- C++11语法
- Boost库（program_options、locale）

构建方法：

1.windows、macos、linux构建

在根目录CMakeLists.txt中的`set(APEEND_THID_LIB_DIR)`中添加你自己的Boost所在的目录。

```shell
cd csp
mkdir build
cd build
cmake .. (可选-G)
cmake --build . --config release
```

2.手机端构建

环境：

- 安卓端可以使用termux（亲测可用）
- iPhone端可以使用iSh（暂未测试）

![termux](https://github.com/taynpg/csp/raw/main/images/csp_qm_sjzpcjzr_android.jpg)

构建：方法与电脑端一致。

# 5.最后

如果您发现了BUG或者有好的建议可以提issue或致邮件taynpg@163.com（欢迎您的任何想法和咨询）。