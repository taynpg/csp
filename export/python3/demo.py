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