# 更新日志

## v1.6.0（2026-10-19）

本版本以工程化重构为主：**命令行参数、排盘输出格式、JSON 结构均保持兼容**，但 C++ 接口有破坏性重命名。

### 一、工程结构

原来 `base/`、`zhcn/`、`qimen/`、`src/` 四套目录混放，其中 `base/` 已无人调用。现在收敛为标准的「公共头文件 + 实现 + 测试」布局：

| 旧位置 | 新位置 | 说明 |
| --- | --- | --- |
| `qimen/include/qimen.h` | `include/csp/qimen.hpp`、`include/csp/chart.hpp`、`include/csp/types.hpp` | 基类、排盘结果、基础类型拆分为三个头文件 |
| `qimen/src/qm_v1..v4.*` | `include/csp/qimen_variants.hpp` + `src/qimen/qimen_v1..v4.cpp` | 四个盘式共用一个头文件 |
| `qimen/src/qimen.cpp` | `src/qimen/qimen.cpp`（布局表与工厂）+ `src/qimen/calendar.cpp`（日历工具） | 日历接口从 `Qimen` 静态函数中剥离 |
| `zhcn/zh_lang.*` | `include/csp/zh.hpp` + `src/zh/names.cpp` | 改为返回 `std::string_view`，不再堆分配 |
| `src/print.*` | `include/csp/app.hpp` + `src/app/render_text.cpp` | 渲染与输出解耦，输出到字符串 |
| `src/jsonExport.*` | `src/app/render_json.cpp` | 同上，`render_json()` 返回字符串 |
| `src/qmuse.*` | `src/app/runner.cpp` | `CQimenUse` → `csp::Runner` |
| `src/csp_base.hpp` | `include/csp/app.hpp`、`include/csp/version.hpp`（CMake 生成） | 版本号单一来源 |
| `base/` | 已删除 | 无人调用，且 `judge()` 有静默返回错误的缺陷 |
| `export/fmt`（submodule） | 已删除 | 与 `3rd/fmt` 重复，且因包含路径顺序实际上从未被使用 |

新增：

- `csp_core` 静态库（可单独链接，供第三方或 Python 模块复用）；
- `tests/`：18 个用例、文本/JSON 快照测试，`ctest` 直接可跑；
- `.github/workflows/ci.yml`：Linux / macOS / Windows 三平台构建 + 测试；
- `tools/compare_json.py`：与上一版二进制做 JSON 逐字段对照（见第四节）；
- `LICENSE`（MIT，与 README、setup.py 声明一致）。

### 二、接口重命名

C++ 侧全部统一为：类型 `PascalCase`、函数与变量 `snake_case`、常量 `k` 前缀、无 `C` 前缀、无 `_` 尾缀。

| 旧 | 新 |
| --- | --- |
| `CQimenUse` | `csp::Runner` |
| `CmdParam` | `csp::Options` |
| `CZhData::gan/zhi/jx/bm/bs/jq/mon/lunar_day/num/gua/zs` | `csp::zh::heaven_stem/earth_branch/star/gate/deity/solar_term/month/lunar_day/numeral/trigram`（`zs` 随 `base/` 删除） |
| `CZhData::kw/mx` | `csp::zh::void_mark/horse_mark` |
| `QimenData` | `csp::Chart` |
| `JiaZi{yi,mi,di,hi}` | `csp::Pillars{year,month,day,hour}` |
| `QimenType::QM_TYPE_SJ_ZHIRUN/YIN/CHAIBU/MAOSHAN` | `QimenType::ZhiRun/YinPan/ChaiBu/MaoShan` |
| `Qimen::instance(type)` → `shared_ptr` | `Qimen::create(type)` → `unique_ptr` |
| `Qimen::get_result()`（按值返回） | `Qimen::chart()`（返回 `const Chart&`） |
| `Qimen::solar/lunar/from/next/between_days/between_secs/get_jq/jiazi/jiaziNoHour/mod` | `csp::cal::from_solar/from_lunar/to_solar/add_seconds/days_between/terms_around/four_pillars/four_pillars_without_hour/wrap` |
| `cal_dipan/cal_duty/cal_jiuxing/cal_bamen/cal_bashen/cal_tianpan/cal_angan/cal_other` | `arrange_earth_stems/resolve_duty/arrange_stars/arrange_gates/arrange_deities/arrange_heaven_stems/arrange_hidden_stems/resolve_void_and_horse` |
| `QimenV1::cal_ju`（各子类各写一份 `generate`） | `compute_ju()` + 基类模板方法 `generate()`，子类不再重复排盘流程 |

`Chart` 的成员也一并正规化，例如 `jiuxr→stars`、`jiuxp→stars_origin`、`bamenr→gates`、`bashenr→deities`、`dp→earth_stem`、`tp→heaven_stem`、`angan→hidden_stem`、`jigong→lodging_palace`、`maxing→horse_palace`、`kongw→void_palace`、`jieq→solar_term`、`yuejiang_→moon_general`、`dt_→time`。

### 三、缺陷修复

1. **阴盘手动定局忽略阴阳**：`-t 2 --ju -5` 原本输出「阳遁五局」，与 1/3/4 号盘式不一致。原因是 `QimenV2::generate()` 在 `set_and_check()` 之后又把 `is_yin` 无条件改成了 `false`。**该修复会改变 type 2 手动负局数的排盘结果**（其余情形不变）。
2. **茅山法三元恒为下元**：`QimenV4::cal_yuan()` 用 `uint32_t` 常量与 `int` 相减，比较结果恒为假，于是永远返回「下元」。改为有符号比较后，按交节后天数正常分上/中/下元。**该修复会改变 type 4 的排盘结果**，属于预期内的行为变更。
3. **超接置闰法忽略推演失败**：`cal_ju()` 丢弃了 `inference()` 的返回值，推演失败时局数仍为 0 却继续排盘。现在会返回失败。
4. **年历累积脏数据**：`year_datas_` 只增不减，同一实例多次 `generate()` 会读到错位的日期。现在每次排盘先清空，并新增回归测试。
5. **重复赋值**：`dzcong_[2] = 8;` 连写两次（其余为 8↔2、5↔11 的两组对冲），删掉多余的一行；该表已改名为 `horse_branch` 并注明用途。
6. **无效自赋值**：`print.cpp` 中的 `szYuan = szYuan;`、`parse()` 中未使用的变量、`Qimen::generate()` 静默 `return false` 等清理。
7. **日期参数不校验**：`-d 2020-13-45-99-99-99` 现在会在参数层被拒绝并给出中文提示，不再依赖底层异常。
8. **线程不安全**：`Qimen::now_time()` 使用 `std::localtime`，改为 `localtime_s` / `localtime_r`。
9. **Windows 重定向丢输出**：原 `u8ConsoleBuf` 在被重定向时仍然调用 `WriteConsoleW`，而 `WriteConsoleW` 在文件/管道句柄上必然失败且返回值被忽略，于是 `csp --help > help.txt` 得到的是空文件（v1.5.5 同样存在）。现在的 `U8ConsoleBuf` 是标准 `std::streambuf` 适配层：连控制台时把 UTF-8 转宽字符写出，被重定向时**原样转发给原来的 rdbuf**，字节一个不改；同时覆盖 `std::cerr` / `std::clog`。非 Windows 平台不安装任何 buf，直接使用标准流。
10. **暗干越界读写（内存越界，会污染其它字段）**：`gen_angan_v1()` 里 `get_index()` 查不到元素时返回 `-1`，代码却直接拿它当下标用：
    - 旬首仪落中宫时，`bamenp[8]` 只是占位值，`get_index(bamenr, 8, ...)` 返回 `-1`，随后 `dp[-1]`、`anggan[-1] = ...` 都是越界访问。实测结果是**暗干出现重复天干**（合法排法不可能重复），同一盘的其它字段也可能被写坏；
    - `angan[get_index(angan, 9, 0)] = 9` 也一样：序列里没有「甲」时就是 `angan[-1] = 9`，这一写正好落到 `wuxing[8]`，于是 **`-a 1` 时中宫的「五行」被写成 1 或 2**，JSON 里表现为 `"五行": "一"` / `"二"`。
    现在所有下标使用前都做有效性判断，旬首仪落中宫时按寄宫（坤二）取值使门。**该修复会改变 `-a 1` 的排盘结果**（旧结果是越界后的随机值）。
11. **茅山法月末日期崩溃**：`QimenV4::cal_ju()` 用「上个月 + 当前日」去构造时间，12-31 会构造出 `2100-11-31`，tyme 抛异常而进程直接 `terminate`（v1.5.5 同样崩溃）。现在把日期夹到上月最后一天，并在 `Runner` 里兜住日历库异常，改为返回错误信息。
12. **`-a 1` 与旬首仪落中宫的组合**：四个盘式共用同一条暗干实现，上述越界对 1/2/3/4 号盘式同时存在。
13. **Windows 控制台下帮助信息乱码**：CLI11 把 UTF-8 字节直接写 `std::cout`，简体中文 Windows 的控制台按本地代码页（936/GBK）解释这些字节，于是 `--help`、`-v` 和参数错误提示全是乱码（排盘正文不受影响——它走的是 `WriteConsoleW`）。现在 CLI11 的输出先收进字符串，统一经 `console` 层「UTF-8 → 宽字符 → `WriteConsoleW`」输出；控制台下的 `\n` 会补成 CRLF，避免旧版控制台把行首吞掉。同时错误提示改走 stderr，重定向 stdout 时不会和盘面混在一起。
14. **MSVC `/W4` 告警**：CI 的 Windows 作业对内嵌的 `tyme.cpp` 报 3 处告警，现已清零——`God::get_day_gods` 与 `Taboo::get_taboos` 的循环变量把 `size_t` 写成了 `unsigned long`（C4267），改为 `size_t`；`SolarTime::get_phase` 的局部变量 `month` 遮蔽了基类 `MonthUnit::month`（C4458），改名 `lunar_month`。三处均属写法问题，排盘结果不变。

### 四、输出与行为

以 `D:/Program/csp-v1.5.5-msvc.x86_64.64Bit/bin/csp.exe`（官方 v1.5.5 二进制）为基准做了两轮对照：

- 定向 125 例（4 盘式 × 24 个日期 + 手动局数/时差/暗干/闰月/边界年）；
- 随机 1600 例（200 个日期 × 4 盘式 × 两种暗干排法），逐字段比对 JSON。

除版本号与下列**预期内的修复**外，输出与 v1.5.5 一致：

| 差异 | 出现范围 | 原因 |
| --- | --- | --- |
| 中宫「五行」为「一/二」→「土」 | 所有 `-a 1` 用例 | 第 10 条的越界写 |
| 暗干出现重复天干 → 互不相同的八个天干 | 旬首仪落中宫的盘（约占十分之一） | 第 10 条的越界读 |
| 阳遁 → 阴遁 | `-t 2 --ju <负数>` | 第 1 条 |
| 三元由恒为下元 → 按交节天数 | `-t 4` 约四成盘面 | 第 2 条 |
| 崩溃 → 正常出盘 | `-t 4` 且日期为月末（如 12-31） | 第 11 条 |

- JSON 结构与字段名不变；
- 新增 `--no-color`，便于重定向或二次处理；
- 新增 `-v/--version`；
- 参数越界、局数超范围等错误现在返回非 0 退出码；
- 帮助、版本与错误提示在 Windows 控制台下不再乱码（走宽字符输出），错误提示写到 stderr；
- Python 模块保留旧名字：`csp_qm.CmdParam` ≡ `csp_qm.Options`，`csp_qm.CQimenUse` ≡ `csp_qm.Runner`；新增 `csp_qm.render(options)` 与 `csp_qm.render_json(options)`；`Runner.run()` 后会把自动定局的局数回填到参数对象。

### 五、内部改进

- 排盘流程改为模板方法：`generate()` 统一负责校验、定局、8 个排盘步骤，四个盘式只实现 `compute_ju()`；
- 四柱在一次排盘中只算一次（`Qimen::pillars()`），原来 `cal_dipan`/`cal_duty`/`cal_jiuxing`/`cal_bamen` 各自重复计算；
- 渲染与落盘解耦：`render_text()` / `render_json()` 返回字符串，`Runner` 负责输出，测试可直接比对；
- 局数表从「四位整数 + `%10`/`/1000` 解包」改为显式的 `JuPattern{upper, middle, lower, yang}`；
- 命中 `-Wall -Wextra -Wpedantic` 零告警。

### 六、发布产物

推到 `v1.6.0` 标签后由 GitHub Actions 自动构建（三平台并行、构建前跑完整测试）：

| 平台 | 文件 | 说明 |
| --- | --- | --- |
| Windows x64 | `csp-v1.6.0-msvc.x86_64.64Bit.exe` | NSIS 安装包，装完自动加入 PATH，带卸载项 |
| Windows x64 | `csp-v1.6.0-msvc.x86_64.64Bit.zip` | 解压即用 |
| Linux x86_64 | `csp_1.6.0_amd64.deb` | `sudo dpkg -i` 或 `sudo apt install ./csp_1.6.0_amd64.deb` |
| Linux x86_64 | `csp-v1.6.0-gnu.x86_64.64Bit.tar.gz` | 解压后把 `csp` 放进 `PATH` |
| macOS | `csp-v1.6.0-clang.universal.tar.gz` | 同时支持 Intel 与 Apple Silicon，未签名 |

另附 `SHA256SUMS.txt`。可执行文件为动态链接，仅依赖系统 libc（Linux 侧 `-static-libstdc++ -static-libgcc`），Linux 构建在 Ubuntu 22.04 上完成（glibc ≥ 2.35）。

---

## v1.5.5

- 源码与输出统一为 UTF-8；
- 支持 `-j/--json` 导出排盘结果。

## v1.5.0

- 使用 [tyme4cpp](https://github.com/6tail/tyme4cpp) 替换旧日历库，修正月柱干支；
- 恢复 CMake 构建。
