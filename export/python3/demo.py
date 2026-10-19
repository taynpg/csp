import csp_qm  # 导入安装的包


def test_qimen_pan():
    print("\n=== 测试 csp-qm 包 ===")

    # 1. 创建排盘参数（Options 的旧名是 CmdParam）
    param = csp_qm.Options()
    param.year = 2023
    param.mon = 10
    param.day = 7
    param.hour = 15
    param.type = 1

    print("\n执行排盘...")
    qm = csp_qm.Runner()  # 旧名是 CQimenUse

    # 打印结果（Runner 会把自动定局的局数回填到 param.ju）
    qm.run(param)

    print("\n排盘完成！")
    print("自动计算 is_auto:", param.is_auto)
    print("局数 ju:", param.ju)

    # 只想拿数据、不想打印时，可以这样：
    param2 = csp_qm.Options()
    param2.year, param2.mon, param2.day, param2.hour = 2023, 10, 7, 15
    param2.type = 3
    text = csp_qm.render(param2)        # 终端文本（是否着色由 param2.color 控制）
    data = csp_qm.render_json(param2)   # JSON 文本
    print(text.splitlines()[6])
    print(data.splitlines()[0])


if __name__ == "__main__":
    test_qimen_pan()
