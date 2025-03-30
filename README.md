# 一、csp 简介

csp-(cpp shi pan，C++式盘)，一个cmd命令行起盘工具，计划支持排盘奇门遁甲，大六壬，六爻。

## 特点

- 单机本地、使用过程中无需联网。
- 跨平台（即本项目编译成二进制可执行文件仅需要一个`支持C++11标准`的`编译器`）。
- 支持时间范围广（`一万年跨度`）。
- 支持手机移动端。

本项目会长期维护，欢迎您的`PR`。

**拉取项目请使用：**

```shell
git clone https://github.com/taynpg/csp
```

# 二、用法

示例图（示例图不一定是最新的，具体看文字功能介绍）：

![示例图片](https://github.com/taynpg/csp/raw/main/images/usage.png)

## 2.2 支持时间范围

v1.3.1 以前（不含）仅支持：

开始日期：1900年01月31日（含）起有效。

结束日期：2098年12月31日（含）结束有效。

v1.3.1 版本及以后已加入寿星万年历，可以支持年跨度`-198~9999`。

开始日期：公元前198年01月01日（含）起有效。

结束日期：9999年12月31日（含）结束有效。


# 三、已实现功能

- 时家，转盘，超接置润法
- 时家，转盘，阴盘
- 时家，转盘，拆补
- 时家，转盘，茅山

# 四、构建

## 4.1 依赖：

- 支持C++11语法编译器

## 4.2 构建方法：

拉取项目请保证网络畅通（可访问`github`）。

```shell
$ git clone https://github.com/taynpg/csp
$ cs csp
$ cmake -Bbuild
$ cmake --build build --config Release
```

## 4.3 手机端构建

环境：

- 安卓端可以使用termux（亲测可用）

![termux](https://github.com/taynpg/csp/raw/main/images/csp_qm_sjzpcjzr_android.jpg)

- iPhone端可以使用iSh（亲测可用）

![iSh](https://github.com/taynpg/csp/raw/main/images/ish.png)

# 五、最后

如果您发现了BUG或者有好的建议可以提`issue`或致邮件`taynpg@163.com`（欢迎您的任何想法和咨询）。
