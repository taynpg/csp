import os
from setuptools import setup
from setuptools.dist import Distribution

# 声明这是一个平台相关的包（包含二进制扩展）
class BinaryDistribution(Distribution):
    def has_ext_modules(self):
        return True

# 编译好的模块文件路径（根据你的实际路径调整）
MODULE_FILE = "bin/csp_qm.cp313-win_amd64.pyd"

setup(
    name="csp_qm",
    version="1.5.0",
    author="taynpg",
    description="A ShiJia QimenPan Cli Tool.",
    long_description=open("README.md").read(),
    long_description_content_type="text/markdown",
    # 无需查找 Python 包（如果没有其他 Python 文件）
    packages=[],  
    # 直接指定二进制模块文件
    data_files=[("", [MODULE_FILE])],  
    include_package_data=True,
    distclass=BinaryDistribution,
    install_requires=[],
    python_requires=">=3.6",
    options={
        "bdist_wheel": {
            "python_tag": "cp313",      # 指定 Python 标签
            "plat_name": "win_amd64",   # 指定平台标签
        }
    }
)