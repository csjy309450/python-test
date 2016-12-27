#! usr/bin/python2.7
# -*-encoding=utf-8-*-

import numpy
import scipy
import matplotlib
import pandas
import sympy
import nose
"""
NumPy系统是Python的一种开源的数值计算扩展。这种工具可用来存储和处理大型矩阵，比Python自身的嵌套列表（nested list structure)结构要高效的多（该结构也可以用来表示矩阵（matrix））。据说NumPy将Python相当于变成一种免费的更强大的MatLab系统。

SciPy函数库在NumPy库的基础上增加了众多的数学、科学以及工程计算中常用的库函数。例如线性代数、常微分方程数值求解、信号处理、图像处理、稀疏矩阵等等。

matplotlib 是python最著名的绘图库，它提供了一整套和matlab相似的命令API，十分适合交互式地进行制图。

pandas 是基于 Numpy 构建的含有更高级数据结构和工具的数据分析包。

SymPy是Python的数学符号计算库，用它可以进行数学公式的符号推导。

nose Python测试工具
"""

print "numpy (", numpy.__version__, ")"
print "scipy (", scipy.__version__, ")"
print "matplotlib (", matplotlib.__version__, ")"
print "pandas (", pandas.__version__, ")"
print "sympy (", sympy.__version__, ")"
print "nose (", nose.__version__, ")"
