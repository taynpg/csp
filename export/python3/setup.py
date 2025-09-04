from setuptools import setup, find_packages

setup(
    name="csp_qm",
    version="1.5.0",
    author="taynpg",
    author_email="taynpg@163.com",
    description="A cli tool for Qimen.",
    long_description=open('README.md').read(),
    long_description_content_type="text/markdown",
    url="https://github.com/taynpg/csp",
    classifiers=[
        "Programming Language :: Python :: 3",
        "Operating System :: OS Independent",
    ],
    license="MIT",  # 改用SPDX license表达式
    include_package_data=True,
    python_requires='>=3.6',
    packages=["csp_qm"],
    options={
        "bdist_wheel": {
            "python_tag": "cp313",      # 指定 Python 标签
            "plat_name": "win_amd64",   # 指定平台标签
        }
    }
)