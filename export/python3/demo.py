import csp_qm  # 导入安装的包

def test_qimen_pan():
    
    print(dir(csp_qm))
    """测试奇门排盘功能"""
    print("\n=== 测试 csp-qm 包 ===")
    
    # 1. 创建排盘参数
    param = csp_qm.CmdParam()
    param.year = 2023
    param.mon = 10
    param.day = 7
    param.hour = 15
    param.type = 1
    
    # 2. 打印参数
    print("排盘参数：", param)
    
    # 3. 执行排盘
    print("\n执行排盘...")
    qm = csp_qm.CQimenUse()
    qm.run(param)
    
    # 4. 检查结果
    print("\n排盘完成！检查参数更新：")
    print("自动计算 is_auto:", param.is_auto)
    print("局数 ju:", param.ju)

if __name__ == "__main__":
    test_qimen_pan()
