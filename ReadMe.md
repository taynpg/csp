# 一、csp 简介

csp-(cpp shi pan，C++式盘)，一个cmd命令行起盘工具，计划支持排盘奇门遁甲，大六壬，六爻。

因Me奇门用的较多，故此项目前期多以奇门为主，后续慢慢开发其他排盘，有兴趣的也可以参与贡献。

# 二、用法

## 2.1 使用 csp --help 查看帮助

示例图：

![示例图片](https://github.com/taynpg/csp/raw/main/images/usage.png)

## 2.2 支持时间范围

v1.3.1 以前（不含）仅支持：

开始日期：1900年01月31日（含）起有效。

结束日期：2098年12月31日（含）结束有效。

v1.3.1 版本及以后已加入寿星万年历，可以支持年跨度`-198~9999`。

开始日期：公元前198年01月01日（含）起有效。

结束日期：9999年12月31日（含）结束有效。


# 三、已实现功能

## 3.1 奇门遁甲

示例图：

![示例图片](https://github.com/taynpg/csp/raw/main/images/csp_qm_sjzpcjzr.png)

- 时家，转盘，超接置润法
- 时家，转盘，阴盘
- 时家，转盘，拆补

## 3.2 大六壬

- 暂无

## 3.3 六爻

- 暂无

# 四、构建

## 4.1 依赖：

- C++11语法

## 4.2 通用构建方法（跨平台）：

```shell
cd csp
mkdir build
cd build
cmake ..
cmake --build . --config release
```

## 4.3 手机端构建

环境：

- 安卓端可以使用termux（亲测可用）
- iPhone端可以使用iSh（暂未测试）

![termux](https://github.com/taynpg/csp/raw/main/images/csp_qm_sjzpcjzr_android.jpg)

# 五、最后

如果您发现了BUG或者有好的建议可以提issue或致邮件taynpg@163.com（欢迎您的任何想法和咨询）。
