# -*- coding: utf-8
import ctypes  
ll = ctypes.cdll.LoadLibrary 
lib = ll("./TestLib/bin/TestLib.dll")  

sum = lib.add(1, 3)
print sum

str = lib.testStr("python123")
#print str

'''
import TestLib
data = "123456"
result = TestLib.Recognise(data)
print "the result is: "+ result

data2 = 123
resint = TestLib.RecogniseInt(data2)
print resint
'''
'''
import AdminJudge
filePath = "./distidx.dat"
dx = 116.1234
dy = 39.2345
res = AdminJudge.GetAdminInfo( filePath, dx, dy )
print res
'''

# ��������115.375000,39.416667;117.500000,41.083333
# ��ֱ�ţ�116.349007��39.936045��116.362429��39.945155

import DynMapTile
filePath = "C:\mcmdndata\��ͼͨ�Ƽ�(����)���޹�˾_zxt130822\��ȫ��v002"
z = 15
minx = 116.349007
miny = 39.936045
maxx = 116.362429
maxy = 39.945155
res = DynMapTile.GetMapTile( filePath, z, minx, miny, maxx, maxy )
print res











