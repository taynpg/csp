#!/usr/bin/env python3
"""对照两个 csp 可执行文件的排盘 JSON 输出，逐字段定位差异。

用法：
    python tools/compare_json.py <旧版exe> <新版exe> [--random N] [--seed S]

默认跑一组定向用例（4 盘式 × 24 个日期 + 4 盘式 × 8 组参数），
加 --random N 再追加 N 个随机日期（每个日期 4 盘式 × 两种暗干排法）。

约定：`基本信息.版本` 一律忽略（两版必然不同），其余字段任何差异都会打印出来。
"""

import argparse
import json
import os
import random
import shutil
import subprocess
import sys
import tempfile

DATES = [
    "2026-10-19-8-30-0", "1986-5-29-12-0-0", "2000-1-1-0-0-0", "1900-2-28-23-59-59",
    "2024-2-10-12-0-0", "2100-12-31-23-0-0", "1999-12-31-23-30-0", "2020-4-4-15-0-0",
    "2-1-1-0-0-0", "9998-12-31-23-59-59", "2023-3-22-12-0-0", "2026-3-15-5-30-0",
    "1970-1-1-0-0-0", "2033-12-22-12-0-0", "2017-7-15-3-0-0", "2008-8-8-8-8-8",
    "1950-6-1-18-0-0", "2025-11-7-11-0-0", "1995-1-30-23-0-0", "2019-2-5-0-30-0",
    "2044-2-29-12-0-0", "2026-10-10-12-0-0", "2026-10-20-12-0-0", "2026-10-30-12-0-0",
]

EXTRA = [
    ["-a", "1"], ["--ju", "5"], ["--ju", "-5"], ["--ju", "1"],
    ["--ju", "-9"], ["-z", "8"], ["-z", "-3"], ["-a", "1", "--ju", "-7"],
]

IGNORED = ("基本信息.版本", "基本信息.工具")


def run(exe, args, workdir):
    for name in os.listdir(workdir):
        os.remove(os.path.join(workdir, name))
    proc = subprocess.run([exe] + args + ["-j"], cwd=workdir, capture_output=True)
    files = [f for f in os.listdir(workdir) if f.endswith(".json")]
    if not files:
        return None, proc.returncode
    with open(os.path.join(workdir, files[0]), encoding="utf-8") as handle:
        return json.load(handle), proc.returncode


def diff(old, new, path=""):
    out = []
    if isinstance(old, dict) and isinstance(new, dict):
        for key in old:
            if key in new:
                out += diff(old[key], new[key], f"{path}.{key}")
            else:
                out.append(f"{path}.{key}: 仅旧版有")
        for key in new:
            if key not in old:
                out.append(f"{path}.{key}: 仅新版有 = {new[key]!r}")
        return out
    if isinstance(old, list) and isinstance(new, list):
        if len(old) != len(new):
            out.append(f"{path}: 长度 {len(old)} != {len(new)}")
        for i, (a, b) in enumerate(zip(old, new)):
            out += diff(a, b, f"{path}[{i}]")
        return out
    if old != new:
        if path.lstrip(".") in IGNORED:
            return []
        out.append(f"{path}: {old!r} != {new!r}")
    return out


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("old")
    parser.add_argument("new")
    parser.add_argument("--random", type=int, default=0, help="追加的随机日期数量")
    parser.add_argument("--seed", type=int, default=20261019)
    parser.add_argument("--verbose", action="store_true", help="打印全部差异行")
    args = parser.parse_args()

    cases = []
    for qimen_type in (1, 2, 3, 4):
        for date in DATES:
            cases.append(["-t", str(qimen_type), "-d", date])
        for extra in EXTRA:
            cases.append(["-t", str(qimen_type), "-d", "2026-10-19-8-30-0"] + extra)

    if args.random:
        rng = random.Random(args.seed)
        for _ in range(args.random):
            date = "{}-{}-{}-{}-{}-{}".format(
                rng.randint(1901, 2099), rng.randint(1, 12), rng.randint(1, 28),
                rng.randint(0, 23), rng.randint(0, 59), rng.randint(0, 59))
            for qimen_type in (1, 2, 3, 4):
                for style in ("0", "1"):
                    cases.append(["-t", str(qimen_type), "-d", date, "-a", style])

    root = tempfile.mkdtemp()
    old_dir = os.path.join(root, "old")
    new_dir = os.path.join(root, "new")
    os.makedirs(old_dir)
    os.makedirs(new_dir)

    same = 0
    crash_old = 0
    problems = 0
    try:
        for case in cases:
            old_json, rc_old = run(args.old, case, old_dir)
            new_json, rc_new = run(args.new, case, new_dir)

            if rc_new != 0:
                print(f"[新版失败] {' '.join(case)} rc={rc_new}")
                problems += 1
                continue
            if rc_old != 0:
                crash_old += 1
                print(f"[旧版崩溃] {' '.join(case)}  → 新版正常出盘")
                continue
            if old_json is None or new_json is None:
                print(f"[无输出]   {' '.join(case)}")
                problems += 1
                continue

            lines = diff(old_json, new_json)
            if not lines:
                same += 1
                if args.verbose:
                    print(f"[一致]     {' '.join(case)}")
                continue
            problems += 1
            print(f"\n### {' '.join(case)}")
            for line in lines[:30 if args.verbose else 8]:
                print("   ", line)
            if len(lines) > (30 if args.verbose else 8):
                print(f"    ... 共 {len(lines)} 处")
    finally:
        shutil.rmtree(root, ignore_errors=True)

    print("\n" + "=" * 64)
    print(f"用例 {len(cases)}：一致 {same}，有差异 {problems - crash_old}，旧版崩溃 {crash_old}")
    if crash_old:
        print("旧版崩溃的用例请人工确认新版输出是否合理（v1.5.5 在茅山法月末日期会抛异常）")
    return 1 if problems else 0


if __name__ == "__main__":
    sys.exit(main())
