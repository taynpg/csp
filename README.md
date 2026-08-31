# 一、csp 简介

csp-(cpp shi pan，C++式盘)，一个cmd命令行奇门遁甲起盘工具。

## 特点

- 单机本地、使用过程中无需联网。
- 跨平台（即本项目编译成二进制可执行文件仅需要一个`支持C++17标准`的`编译器`）。
- 支持时间范围广（`9998年跨度`）。
- 支持手机移动端。

本项目会长期维护，欢迎您的`PR`。

**拉取项目请使用：**

```shell
git clone https://github.com/taynpg/csp
```

**如果您还需要制作python包**

同时使用`-DCSP_PYTHON_EXPORT=ON`开启。

```shell
git clone --recursive https://github.com/taynpg/csp
```

# 二、用法

## 2.1 使用

请使用`--help`查看帮助信息。

![help](https://github.com/taynpg/csp/raw/main/images/help.png)

下面是一个示例排盘（暗干目前支持2种排法）：

![demo](https://github.com/taynpg/csp/raw/main/images/demo.png)

关于`Python`的使用，本项目支持导出为`whl`模块供python使用，不过仅仅是导出了排盘接口。设定好参数（参数与csp命令行所需参数一致）打印排盘结果与
使用csp类似，没有开放更多内部接口的使用，鉴于Python版本众多，本项目仅提供一版的`whl`安装包。

```python
import csp_qm  # 导入安装的包

def test_qimen_pan():

    print("\n=== 测试 csp-qm 包 ===")

    # 1. 创建排盘参数
    param = csp_qm.CmdParam()
    param.year = 2023
    param.mon = 10
    param.day = 7
    param.hour = 15
    param.type = 1

    print("\n执行排盘...")
    qm = csp_qm.CQimenUse()

    # 这里打印结果
    qm.run(param)

    # 4. 检查结果
    print("\n排盘完成！")
    print("自动计算 is_auto:", param.is_auto)
    print("局数 ju:", param.ju)

if __name__ == "__main__":
    test_qimen_pan()
```

## 2.2 支持时间范围

公元`0002-01-01`至`9998-12-31`。

# 三、已实现功能

- 时家，转盘，超接置润法
- 时家，转盘，阴盘
- 时家，转盘，拆补
- 时家，转盘，茅山

# 记录

- 2026-8-31，v1.5.5版本开始，改成源码和执行集均为UTF-8编码。

- 2026-8-31，v1.5.5版本开始，支持导出结果为Json，给AI或者程序用。

- 2025-09-04，已完成`v1.5.0`版本，该版本为预览版。

- 2025-09-03, tyme4cpp日历库已出，csp的`v1.5`大版本开始开发。

**以下是v1.5前的声明：**

截止到目前`v1.4.6`版本，不少用户反馈有月干支错误的BUG，经查实确实存在，属于日历库的问题。现在在等 [tyme日历库](https://6tail.cn/tyme.html)出`cpp`版本，**后续将使用tyme库替换现有的2个日历库进行修正**。

csp在未来使用`tyme`日历库替换现有日历库以后将发布`1.5`版本并恢复使用cmake构建，csp计划将永久停留在`v1.5`版本，也就是说，csp将不再新增更多的排盘方式（除非有pull request合并请求），但csp**仍然会保持永久性维护**，仅限于处理后续可能遇到的`BUG`或者排盘错误的问题，尽可能保持高度的稳定和准确，并会伴有`v1.5.x`无限制的小版本更新发布。

## 导出结果示例

```json
{
    "九宫": [
        {
            "九星": "天任",
            "五行": "金",
            "八神": "腾蛇",
            "八门": "死门",
            "地盘": "己",
            "天盘": "乙",
            "天禽寄宫": false,
            "宫位": "乾六宫",
            "暗干": "癸",
            "空亡": false,
            "马星": false
        },
         ...
    ],
    "基本信息": {
        "三元": "处暑下元",
        "上一节气": {
            "名称": "立秋",
            "时间": "2026-08-07 19:42:43"
        },
        "下一节气": {
            "名称": "处暑",
            "时间": "2026-08-23 10:18:49"
        },
        "五不遇时": false,
        "值使门": "伤门",
        "值符星": "天冲",
        "公历": "2026-08-31 13:07:16",
        "农历": {
            "年": 2026,
            "日": "十九",
            "时辰": "未",
            "月": "七",
            "闰月": false
        },
        "四柱": {
            "年柱": "丙午",
            "日柱": "丁丑",
            "时柱": "丁未",
            "月柱": "丙申"
        },
        "局数": 7,
        "工具": "csp",
        "当日节气": "处暑",
        "版本": "v1.5.5",
        "盘式名称": "时家转盘拆补法",
        "盘式类型": 3,
        "阴阳遁": "阴遁"
    },
    "旬空": {
        "年柱": [
            "寅",
            "卯"
        ],
        "日柱": [
            "申",
            "酉"
        ],
        "时柱": [
            "寅",
            "卯"
        ],
        "月柱": [
            "辰",
            "巳"
        ]
    }
}
```

# 四、构建

## 4.1 依赖：

- 支持C++17语法编译器

## 4.2 构建方法：

拉取项目请保证网络畅通（可访问`github`）。

```shell
$ git clone https://github.com/taynpg/csp
$ cd csp
$ cmake -Bbuild -DCMAKE_BUILG_TYPE=Release
$ cmake --build build --config Release
```

## 4.3 手机端构建

**由于软件排版格式上几乎完全一致，`Git`页面图片展示效果不一定是最新的，避免频繁更新图片导致仓库大占用。**

### （1）安卓端可以使用termux（亲测可用）

![termux](https://github.com/taynpg/csp/raw/main/images/csp_qm_sjzpcjzr_android.jpg)

### （2）iPhone端可以使用iSh（亲测可用）

![iSh](https://github.com/taynpg/csp/raw/main/images/ish.png)

# 五、最后

如果您发现了BUG或者有好的建议可以提`issue`或致邮件`taynpg@163.com`（欢迎您的任何想法和咨询）。
