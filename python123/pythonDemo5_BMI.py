# -*- coding: utf-8 -*-
"""
Created on Thu Jul  4 15:58:28 2019

@author: jingzl
"""

# BMI

height,weight = eval(input("请输入身高和体重[逗号隔开]："))
bmi = weight / pow(height,2)
print("BMI数值为：{:.2f}".format(bmi))
who, nat = "",""
if bmi < 18.5:
    who,nat="偏瘦","偏瘦"
elif 18.5 <= bmi <= 24:
    who,nat="正常","正常"
elif 24 <= bmi <= 25:
    who,nat="正常","偏胖"
elif 25 <= bmi <= 28:
    who,nat="偏胖","偏胖"
elif 28 <= bmi < 30:
    who,nat="偏胖","肥胖"
else:
    who,nat="肥胖","肥胖"
print("BMI指标：国际'{0}',国内'{1}'".format(who,nat))




