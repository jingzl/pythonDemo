# -*- coding: utf-8 -*-
"""
Created on Wed Jul  3 08:54:34 2019

@author: jingzl
"""

# pythonDemo3_DayDayUp.py

dayfactor = 0.01
dayup = pow( 1+dayfactor, 365 )
daydown = pow( 1-dayfactor, 365 )
print( "向上:{:.2f}，向下：{:.2f}".format( dayup, daydown ) )


dayup = 1.0
dayfactor = 0.01
for i in range(365):
    if i % 7 in [6,0]:
        dayup = dayup*(1-dayfactor)
    else:
        dayup = dayup*(1+dayfactor)
print( "力量：{:.2f}".format(dayup) )



def dayUp(df):
    dayup = 1
    for i in range(365):
        if i % 7 in [6,0]:
            dayup = dayup*(1-0.01)
        else:
            dayup = dayup*(1+df)
    return dayup

dayfactor = 0.01
while dayUp(dayfactor) < 37.78:
    dayfactor += 0.001
print("工作日的努力参数是：{:.3f}".format(dayfactor) )

####################
#weekStr = u"星期一星期二星期三星期四星期五星期六星期日"
#weekId = (int)(input("pls input week num(1-7):"))
#pos = (weekId -1) * 3
#print( weekStr[pos:pos+3])

####################
print( "1+1=2"+chr(10004))
print("python".center(20,"="))

print("{0:=^20}".format("PYTHON"))
print("{0:*>20}".format("BIT"))
print("{:10}".format("BIT"))

print("{0:,.2f}".format(12345.6789))
print("{0:b},{0:c},{0:d},{0:o},{0:x},{0:X}".format(425))
print("{0:e},{0:E},{0:f},{0:%}".format(3.14))


