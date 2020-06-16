# -*- coding: utf-8 -*-
"""
Created on Tue Jun 25 14:14:50 2019

@author: jingzl
"""
import numpy as np
import pandas as pd


#### Series类型
b = pd.Series( [9,8,7,6], ['a','b','c','d'])
print b
print b.index
'''
print b.values

print b[1], b['b']

print b[:3]
print b[b>b.median()]
print np.exp(b)

print 'c' in b
print 0 in b
print b.get( 'f', 100 )

a = pd.Series([1,2,3],['c','d','e'])
print a+b

## .name属性
b.name = 'Series对象'
b.index.name = '索引列'
print b
'''

#### DataFrame类型
d = pd.DataFrame( np.arange(10).reshape(2,5) )
print d

dl = { 'one':[1,2,3,4], 'two':[9,8,7,6]}
d = pd.DataFrame(dl, index=['a','b','c','d'])
print d


c= d.sort_index( axis=1, ascending=False )
print c

c = d.sort_values( 'b', axis=1, ascending=False )
print c


b = pd.DataFrame( np.arange(20).reshape(4,5), index=['c','a','d','b'])
print b

c = b.sort_values( 2, ascending=False )
print c

c = c.sort_values( 'a', axis=1, ascending=False )
print c


print c.describe()

print type(c.describe())














