# -*- coding: utf-8 -*-
"""
Created on Wed Jun 19 10:37:00 2019

@author: jingzl
"""

import numpy as np
import matplotlib.pyplot as plt
#import matplotlib

def f(t):
    return np.exp(-t)*np.cos(2*np.pi*t)

a = np.arange(0.0,5.0,0.02)

plt.subplot(211)
plt.plot( a, f(a) )

plt.subplot(2,1,2)
plt.plot( a, np.cos(2*np.pi*a), 'r--' )

plt.show()

#plt.plot( [3,3,4,5,2] )
#plt.ylabel( "grade" )
#plt.savefig( "test", dpi=600 )
#plt.show()


#### 风格测试
a = np.arange(10)
plt.plot(a, a*1.5, 'r--', a, a*2.5, 'go-', a, a*3.5, '*', a, a*4.5, 'b-.' )
plt.show()

####中文测试
a = np.arange(0.0, 5.0, 0.02)
plt.xlabel( u'横轴:时间', fontproperties='SimHei', fontsize=20 )
plt.ylabel( u'纵轴:振幅', fontproperties='SimHei', fontsize=20 )
plt.plot( a, np.cos(2*np.pi*a), "r--" )
plt.show()

#### 文本显示
a = np.arange(0.0, 5.0, 0.02)
plt.plot( a, np.cos(2*np.pi*a), 'r--' )

plt.xlabel( u'横轴:时间', fontproperties='SimHei', fontsize=20 )
plt.ylabel( u'纵轴:振幅', fontproperties='SimHei', fontsize=20 )
plt.title( r'正弦波实例 $y=cos(2\pi x)$', fontproperties='SimHei', fontsize=25 )

#plt.text( 2, 1, r'$\mu=100$', fontsize=15 )
plt.annotate( r'$\mu=100$', xy=(2,1), xytext=(3,1.5), 
             arrowprops=dict(facecolor='black', shrink=0.1, width=2) )

plt.axis( [-1, 6, -2, 2] )
plt.grid( True )

plt.show()


#### subplot2grid

#### 图表绘制函数
labels = ['frogs', 'Dogs', 'Hogs', 'Logs']
sizes = [15,30,45,10]
explode = (0,0.1,0,0)
plt.pie( sizes, explode=explode, labels=labels, autopct='%1.1f%%', 
        shadow=False, startangle=90)
#plt.axis('equal') #打开则为正圆形饼图
plt.show()
 

#### 直方图
np.random.seed(0)
mu, sigma = 100, 20  #均值和标准差
a = np.random.normal( mu, sigma, size=100 )
plt.hist( a, 20, normed=1, histtype='stepfilled', facecolor='b', alpha=0.75)
plt.title('Histogram')
plt.show()













