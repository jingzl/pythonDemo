# -*- coding: utf-8 -*-
"""
Created on Sun Nov 17 2020
"""
__author__ = 'jingzl'
__version__ = '1.0'


from selenium import webdriver
import time
import datetime
import pandas as pd
import numpy as np


if __name__ == '__main__':
    # 1. 解析现有excel文件，分析URL
    # 2. 根据URL的目录特点，进行分类统计
    # 3. 根据URL的统计量倒序排列，优先处理量大的类型
    # 4. 分别将不同类型的URL爬取，并提取数据存储（每一种类型都需要单独分析，爬取）
    # 5. 根据不同类型分别输出新格式的excel
    # 6. 提供结果数据的最终合并操作

    # end
    print("all end".center(100, '-'))
