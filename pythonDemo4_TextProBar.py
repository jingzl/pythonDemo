# -*- coding: utf-8 -*-
"""
Created on Thu Jul  4 11:37:32 2019

@author: jingzl
"""

# pythonDemo3_TextProBar.py

import time


# v1
scale = 10
print("-----执行开始-----")
for i in range(scale+1):
    a = '*' * i
    b = '.' * (scale-i)
    c = (i/scale) * 100
    print("{:^3.0f}%[{}->{}]".format(c,a,b))
    time.sleep(0.1)
print("-----执行结束-----")


#
for i in range(101):
    print("\r{:3}%".format(i), end="")
    time.sleep(0.1)

# v2
scale = 50
print("执行开始".center(scale//2,'-'))
start = time.perf_counter()
for i in range(scale+1):
    a = '*' * i
    b = '.' * (scale-i)
    c = (i/scale) * 100
    dur = time.perf_counter()- start
    print("\r{:^3.0f}%[{}->{}]{:.2f}s".format(c,a,b,dur),end='')
    time.sleep(0.1)
print("\n" + "执行结束".center(scale//2,'-'))

