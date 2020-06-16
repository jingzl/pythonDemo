# -*- coding: utf-8 -*-
"""
Created on Fri Jun 28 10:23:50 2019

@author: jingzl
"""
#coding:utf-8

## 温度转换程序
import sys
reload(sys)
sys.setdefaultencoding('utf8')

TempStr = input(u"请输入带有符号的温度值：")
if TempStr[-1] in [ 'F', 'f' ]:
    C = ( eval(TempStr[0:-1]) - 32 ) / 1.8
    print(r"转换后的温度是{:.2f}C".format(C))
elif TempStr[-1] in [ 'C', 'c' ]:
    F = 1.8 * eval(TempStr[0:-1]) + 32
    print(r"转换后的温度是{:.2f}F".format(F))
else:
    print(u"输入格式错误")
   


