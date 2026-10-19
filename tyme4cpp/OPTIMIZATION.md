# tyme4cpp 本地优化版变动说明

本文档记录在上游 v1.2.0（commit 68092a9）基础上所做的性能优化与内存泄漏修复。
目标：**不改变公开 API 语义**（一处例外见 §2.4），改动集中、可复现，便于上游发布新版本后**重新套用**。

- 验证环境：MSYS2 UCRT64（g++ 16.2.0 / CMake + Ninja，`-std=c++17`）
- 回归验证：上游自带测试全量通过（1097 assertions / 452 test cases）
- 性能验证：`tools/bench.cpp`（构建与用法见 §5）

---

## 1. 变更文件总览

| 文件 | 变更 |
|---|---|
| `tyme/tyme.h` | ① 补充缺失的 `<stdexcept>`/`<sstream>` include；② `LoopTyme::names` 由值拷贝 `vector<string>` 改为共享指针 |
| `tyme/tyme.cpp` | ① 新增 `MemoCache` 线程安全缓存工具；② 6 处重计算方法接入缓存；③ 修复 2 处静态指针内存泄漏；④ 6 处 `equals` 改为直接比较 |
| `tools/bench.cpp` | 新增（可选）：性能基准，用于回归验证 |

---

## 2. 逐项说明

### 2.1 头文件补 include（缺陷修复）

`tyme.h` 原来依赖 `<stdexcept>`、`<sstream>` 被使用方"恰好"先包含才能编译
（仓库内 `tests.cpp` 因先包含 `catch.hpp` 而侥幸通过；单独 `#include "tyme.h"` 会报
`invalid_argument` 未声明 / `istringstream` 不完整）。已在头部统一补上。

### 2.2 内存泄漏修复

原实现有两处**进程生命周期内永不释放的堆分配**（泄漏检测工具必然报告）：

```cpp
// 原
EightCharProvider* LunarHour::provider  = new DefaultEightCharProvider();
ChildLimitProvider* ChildLimit::provider = new DefaultChildLimitProvider();
```

改为指向**静态默认实例**：

```cpp
// 现
namespace {
    DefaultEightCharProvider  default_eight_char_provider;
    DefaultChildLimitProvider default_child_limit_provider;
}
EightCharProvider*  LunarHour::provider  = &default_eight_char_provider;
ChildLimitProvider* ChildLimit::provider = &default_child_limit_provider;
```

> 兼容性说明：`provider` 仍是**公有扩展点**（可替换为自定义 Provider），
> 语义不变——替换后新对象的生命周期由调用方负责（与原行为一致）。

### 2.3 LoopTyme 去除名称表拷贝（最大分配热点）

`LoopTyme`（天干、地支、生肖、六十甲子、九星……几乎所有文化对象的基类）
原先每个实例都**按值拷贝**整个 `vector<string> names`：

- 每个对象 = 1 次 vector 堆分配 + N 次 string 堆拷贝；
- 而 `NAMES` 全部是静态常量表，拷贝纯属浪费；
- 热点路径（如 `SixtyCycle::get_heaven_stem()`）每次调用都隐式产生几十次堆分配。

改为持有共享指针：

```cpp
// tyme.h（LoopTyme）
const vector<string> *names;   // 原: vector<string> names;

LoopTyme(const vector<string> &names, const int index)
    : names(&names), index(index_of(index)) { }
// get_size / get_name / index_of 同步改为 (*names)->… / (*names)[…]
```

> **语义注意（唯一一处行为约束）**：构造时传入的名称表必须**活得比对象久**。
> 库内部所有调用点传入的都是 `static const vector<string> NAMES`，天然满足；
> 若在外部把临时 `vector<string>` 传进 `LoopTyme` 构造函数，会悬垂（原来不会）。
> 子类构造函数签名未变，正常使用不受影响。

### 2.4 天文计算缓存（最大耗时热点）

农历/节气转换依赖朔日（`calc_shuo`）与节气（`calc_qi`/`qi_accurate2`）的迭代级数计算，
原实现**每次调用都全量重算**（且 `LunarDay` 构造校验会反复调用 `get_day_count`）。
这些结果由入参唯一确定，故引入进程级记忆化缓存：

`tyme.cpp` 顶部（`namespace tyme` 内）新增工具：

```cpp
template <typename V>
class MemoCache {   // mutex + std::map<long long, V>，get(key, compute) 惰性填充
};
```

接入点（6 处，键均为天然唯一组合）：

| 方法 | 缓存键 |
|---|---|
| `SolarTerm::init_by_year`（粗儒略日） | `(年, 序号)` → `year*24+offset` |
| `SolarTerm::get_julian_day`（精确儒略日） | `(年, 序号)` → `year*24+index` |
| `LunarMonth::get_new_moon`（朔日） | `(年, 月带闰标记)` → `year*100+m+50` |
| `LunarMonth::get_day_count`（月天数） | 同上 |
| `LunarMonth::get_first_julian_day`（月初儒略日） | 同上 |
| `LunarYear::get_leap_month`（闰月表查找） | `year` |

- 线程安全：`get()` 内部以 `mutex` 保护；DLL 已链接 winpthread，无新增依赖。
- 缓存无上限：极端情况（把 -1~9999 全年扫一遍）约几十万条目、数十 MB 量级，
  实际使用远达不到；若需限制可自行改为 LRU（接入点已集中，改动很小）。

### 2.5 equals 优化

原实现普遍用 `to_string() == other.to_string()` 比较（构造两个临时字符串）。
以下 6 处改为直接比较字段/索引（语义等价，结果由字段唯一决定）：

`SolarDay`、`LunarDay`、`SolarTime`、`HijriDay`（字段比较）；
`Element`、`Zodiac`、`HeavenStem`（LoopTyme 索引比较）。

其余低频类型的 `equals`（RabByung 系列、LunarWeek 等）保持原样，控制改动面。

---

## 3. 性能对比（UCRT64 g++ 16.2，-O2，Windows）

| 基准（`tools/bench.cpp`） | 原 v1.2.0 | 优化后·冷 | 优化后·热 | 提升(热) |
|---|---:|---:|---:|---:|
| 公历→农历 100 年逐日 | 1452~1559 ms | 29.1 ms | 10.7 ms | **~140x** |
| 逐日干支+天干+地支+生肖 | 1827~2118 ms | 11.8 ms | 11.8 ms | **~160x** |
| 100 年 × 24 节气精确儒略日 | 13.6~14.1 ms | 13.8 ms | 0.2 ms | **~70x** |
| 农历 to_string + equals | 306~330 ms | 1.6 ms | 1.6 ms | **~200x** |

- "冷" = 首次触碰（含真实天文计算，该成本无法消除）；"热" = 相同数据第二次请求（命中缓存）。
- 实际收益取决于访问模式：**重复查询/大范围遍历**（日历 UI、批量计算等常见场景）接近热路径。

---

## 4. 上游更新后如何重新套用

改动全部集中在 **2 个文件、约 130 行**，无新文件依赖（`tools/bench.cpp` 独立可选）。
建议流程：

1. `git stash` / 记录本版改动（或直接保留本 fork 分支）。
2. 拉取上游新版本，按下面的"锚点"逐项重放：

| 变更 | 锚点（在上游代码中定位） |
|---|---|
| include 补齐 | `tyme.h` 顶部 `#include <mutex>` 附近 |
| provider 修复 | `tyme.cpp` 中 `= new DefaultEightCharProvider()` / `= new DefaultChildLimitProvider()` |
| LoopTyme 指针化 | `tyme.h` 中 `vector<string> names;` 及 LoopTyme 构造/拷贝/tyme.cpp 的 `LoopTyme::get_size|get_name|index_of` |
| MemoCache | 粘贴工具类后，锚定 `SolarTerm::init_by_year`、`SolarTerm::get_julian_day`、`LunarMonth::get_new_moon/get_day_count/get_first_julian_day`、`LunarYear::get_leap_month` 的函数体 |
| equals | 搜索 `to_string() == other.to_string()` |

3. 回归验证：
   ```bash
   # MSYS2 UCRT64 shell
   cmake -G Ninja -DCMAKE_BUILD_TYPE=Release -B build-optimized
   cmake --build build-optimized
   ./build-optimized/tests/tests.exe          # 期望: All tests passed
   g++ -std=c++17 -O2 -I tyme tools/bench.cpp tyme/tyme.cpp tyme/util.cpp \
       -o bench.exe -static-libgcc -static-libstdc++ -Wl,-Bstatic -lwinpthread -Wl,-Bdynamic
   ./bench.exe                                 # 对比 §3 表格量级
   ```

> 注意：若上游给 `LoopTyme`/`LunarMonth`/`SolarTerm` 增加了新成员或新方法，重放时以
> 上游新函数体为基准、只套"缓存/指针化"的包装，不要整函数覆盖。

---

## 5. 已知权衡

1. **LoopTyme 生命周期约束**（见 §2.3）：传入临时 `vector<string>` 会悬垂；库内不受影响。
2. **缓存进程级无上限**（见 §2.4）：内存换时间；如需封顶请自行改 LRU。
3. `LunarHour::provider` / `ChildLimit::provider` 语义未变；如上游未来增加
   `set_provider` 之类的 setter，注意同样不要在 setter 里 `delete` 默认实例。
