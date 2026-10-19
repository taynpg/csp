# csp

[![CI](https://github.com/taynpg/csp/actions/workflows/ci.yml/badge.svg)](https://github.com/taynpg/csp/actions/workflows/ci.yml)

**csp**（cpp shi pan，C++ 式盘）是一个命令行奇门遁甲起盘工具，同时也可以作为 C++ 库嵌入其它程序。

## 特点

- 单机本地运行，使用过程无需联网；
- 跨平台：只要编译器支持 C++17 即可构建（Windows / Linux / macOS / 手机端）；
- 支持时间范围宽（公元 2 年 - 9998 年）；
- 终端输出与 JSON 输出，便于人工阅读或交给程序、AI 二次处理；
- 可导出为 Python 模块（pybind11）。

## 已实现的排盘方式

| `-t` | 盘式 | 定局 |
| --- | --- | --- |
| 1 | 时家转盘，超接置润法 | 超接置闰推演年历 |
| 2 | 时家转盘，阴盘 | 年支 + 农历月 + 农历日 + 时支 |
| 3 | 时家转盘，拆补法 | 符头 + 所在节气 |
| 4 | 时家转盘，茅山法 | 交节后的天数定三元 |

# 一、构建

## 1.1 依赖

- 支持 C++17 的编译器（GCC / Clang / MSVC 均可）；
- CMake 3.16 及以上。

第三方库（tyme4cpp、fmt、nlohmann/json、CLI11）已随源码提供，无需联网。

## 1.2 构建与测试

```shell
git clone https://github.com/taynpg/csp
cd csp
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
ctest --test-dir build --output-on-failure   # 可选：跑单元测试与快照测试
```

产物：`build/csp`（Windows 为 `build/csp.exe`）。

常用 CMake 选项：

| 选项 | 默认 | 说明 |
| --- | --- | --- |
| `CSP_BUILD_TESTS` | `ON` | 构建单元测试与快照测试 |
| `CSP_PYTHON_EXPORT` | `OFF` | 构建 Python 扩展模块 |
| `CMAKE_BUILD_TYPE` | `Release` | 构建类型 |

## 1.3 Python 模块

需要先初始化 pybind11 子模块：

```shell
git clone --recursive https://github.com/taynpg/csp
cd csp
cmake -B build -DCSP_PYTHON_EXPORT=ON
cmake --build build --parallel
```

产物为 `csp_qm` 扩展模块（Windows 下是 `.pyd`，需要与 `libstdc++-6.dll` 等运行时库放在一起）。

## 1.4 打包

```shell
cmake --build build --parallel
cpack --config build/CPackConfig.cmake -B dist
```

生成器按平台自动选择：

| 平台 | 生成器 | 产物 |
| --- | --- | --- |
| Windows | `NSIS64`、`ZIP` | `csp-v1.6.0-msvc.x86_64.64Bit.exe`（安装包，装完自动加 PATH）、同名 `.zip`（解压即用） |
| Linux | `DEB`、`TGZ` | `csp_1.6.0_amd64.deb`（`dpkg -i` 直接装）、`csp-v1.6.0-gnu.x86_64.64Bit.tar.gz` |
| macOS | `TGZ` | `csp-v1.6.0-clang.universal.tar.gz`（Intel + Apple Silicon 通用） |

Windows 上打 NSIS 安装包需要 `makensis`（CI 里由 `choco install nsis` 提供）。

推送 `v*` 标签会触发 [`.github/workflows/release.yml`](.github/workflows/release.yml)：三平台并行构建 → 跑测试 → 打包 → 汇总 `SHA256SUMS.txt` → 创建 GitHub Release 并上传附件；也可以在 Actions 页面手动触发（填一个已存在的标签）。

# 二、命令行用法

```shell
csp -t 1 -d 2026-10-19-8-25-0
```

| 参数 | 说明 |
| --- | --- |
| `-t, --type` | 盘式类型（必填），取值 1-4 |
| `-d, --date` | 排盘时间，格式 `年-月-日-时-分-秒`，缺省为当前时间 |
| `--ju` | 手动指定局数，`0` 为自动；范围 -9 到 9，负数为阴遁 |
| `-z, --zone` | 与本地时间的时差（小时），默认 0 |
| `-a, --angan` | 暗干排法：`0` 门原宫位地盘、`1` 入值使门 |
| `-j, --json` | 导出 JSON 文件到当前目录 |
| `--no-color` | 强制输出纯文本（重定向到文件或管道时自动关闭颜色） |
| `-v, --version` | 显示版本 |

示例：

```shell
# 当前时间排盘
csp -t 1

# 指定时间、手动定局（阴遁五局）、导出 JSON
csp -t 3 -d 1986-5-29-12-0-0 --ju -5 -j

# 不输出颜色并保存（重定向时会自动去掉颜色）
csp -t 2 -d 2026-10-19-8-25-0 > pan.txt
```

![demo](https://github.com/taynpg/csp/raw/main/images/demo.png)

> Windows 编码提示：程序内部一律使用 UTF-8。写控制台时转成宽字符（`WriteConsoleW`），
> 所以 cmd/PowerShell 窗口里中文正常；重定向到文件或管道时按原始 UTF-8 字节写出，
> 文件内容也是 UTF-8。只有在 PowerShell 5.1 里把输出**管道或重定向**给别的命令时，
> 它才会按本地代码页（GBK）解码子进程输出，此时先执行一次
> `[Console]::OutputEncoding = [Text.Encoding]::UTF8`。

# 三、作为 C++ 库使用

```cmake
add_subdirectory(csp)
target_link_libraries(your_target PRIVATE csp_core)
```

```cpp
#include <csp/app.hpp>
#include <csp/qimen.hpp>

// 方式一：直接排盘，读取结构化结果
auto qimen = csp::Qimen::create(csp::QimenType::ZhiRun);
if (!qimen->generate(csp::DateTime(2026, 10, 19, 8, 25, 0), 0)) {
    return 1;
}
const csp::Chart& chart = qimen->chart();
// chart.ju           局数
// chart.yin_dun      是否阴遁
// chart.stars        九星（宫位 -> 星）
// chart.gates        八门
// chart.deities      八神
// chart.earth_stem   地盘
// chart.heaven_stem  天盘
// chart.hidden_stem  暗干
// chart.void_palace  旬空宫位
// chart.horse_palace 马星宫位

// 方式二：走应用层，拿到渲染好的文本或 JSON
csp::Options options;
options.type = csp::QimenType::ZhiRun;
options.time = csp::DateTime(2026, 10, 19, 8, 25, 0);

csp::Result result;
if (csp::prepare_result(options, result)) {
    std::string text = csp::render_text(result.chart, options.type, false);
    std::string json = csp::render_json(result);
}
```

宫位下标（`0`-`8`）的定义：

```
------- ------- -------
|  4   |   3  |   2  |
------- ------- -------
|  5   |   8  |   1  |
------- ------- -------
|  6   |   7  |   0  |
------- ------- -------
```

# 四、Python 用法

```python
import csp_qm

param = csp_qm.Options()          # 等价于旧名 csp_qm.CmdParam()
param.year, param.mon, param.day = 2023, 10, 7
param.hour, param.type = 15, 1

runner = csp_qm.Runner()          # 等价于旧名 csp_qm.CQimenUse()
runner.run(param)                 # 打印排盘结果，并把自动定局的局数回填到 param.ju

print(param.ju)

# 也可以不打印，直接拿文本或 JSON
text = csp_qm.render(param)
data = csp_qm.render_json(param)
```

# 五、JSON 输出

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
        }
    ],
    "基本信息": {
        "三元": "处暑下元",
        "上一节气": { "名称": "立秋", "时间": "2026-08-07 19:42:43" },
        "下一节气": { "名称": "处暑", "时间": "2026-08-23 10:18:49" },
        "五不遇时": false,
        "值使门": "伤门",
        "值符星": "天冲",
        "公历": "2026-08-31 13:07:16",
        "农历": { "年": 2026, "日": "十九", "时辰": "未", "月": "七", "闰月": false },
        "四柱": { "年柱": "丙午", "日柱": "丁丑", "时柱": "丁未", "月柱": "丙申" },
        "局数": 7,
        "工具": "csp",
        "当日节气": "处暑",
        "版本": "v1.6.0",
        "盘式名称": "时家转盘拆补法",
        "盘式类型": 3,
        "阴阳遁": "阴遁"
    },
    "旬空": {
        "年柱": ["寅", "卯"],
        "日柱": ["申", "酉"],
        "时柱": ["寅", "卯"],
        "月柱": ["辰", "巳"]
    }
}
```

# 六、目录结构

```
include/csp/        公共头文件
  types.hpp           基础类型、常量、枚举
  chart.hpp           排盘结果 Chart
  calendar.hpp        日历工具（tyme4cpp 的薄封装）
  qimen.hpp           排盘基类
  qimen_variants.hpp  四种盘式
  zh.hpp              中文名称表
  app.hpp             Options / Result / 渲染 / Runner
  version.hpp.in      版本号模板（CMake 生成 version.hpp）
src/
  qimen/              定局与排盘实现
  app/                参数处理、文本渲染、JSON 渲染、终端输出
  zh/                 名称表实现
  main.cpp            命令行入口
tests/                单元测试与快照（ctest）
tools/
  compare_json.py     与上一版二进制做 JSON 逐字段对照
```

# 七、开发

```shell
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure
```

- 代码风格见 `.clang-format`（`clang-format -i <file>`）；
- 改动排盘或渲染后，若确认输出变化符合预期，可用 `CSP_UPDATE_SNAPSHOTS=1` 重新生成快照；
- 历史版本与接口变更见 [CHANGELOG.md](CHANGELOG.md)。

### 回归验证方法

重构或修改排盘算法后，建议与上一版的官方二进制做「同参数、逐字段」对照，而不是只依赖单元测试：

```shell
# 128 个定向用例：4 盘式 × 24 个日期 + 4 盘式 × 8 组参数（手动局数 / 时差 / 暗干 / 负局数）
python tools/compare_json.py /path/to/v1.5.5/csp.exe ./build/csp.exe

# 再加 200 个随机日期（每日期 4 盘式 × 两种暗干排法，共 1600 盘）
python tools/compare_json.py /path/to/v1.5.5/csp.exe ./build/csp.exe --random 200
```

脚本会递归比对两份 JSON 的每个字段（只忽略 `基本信息.版本`），打印差异位置；
遇到旧版崩溃的用例会单独列出，人工确认新版输出是否合理。
v1.6.0 就是这样比对出 101/128 完全一致、其余 27 例均为预期修复的，差异逐条记在 CHANGELOG。

# 八、其它

- 支持范围：公元 `0002-01-01` 至 `9998-12-31`；
- 手机端：安卓可用 termux，iPhone 可用 iSh 直接编译（见 `images/`）。

![termux](https://github.com/taynpg/csp/raw/main/images/csp_qm_sjzpcjzr_android.jpg)

# 九、许可证

[MIT](LICENSE)。欢迎提 issue 或 PR。
