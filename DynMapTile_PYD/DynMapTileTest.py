# -*- coding: utf-8


# ��������115.375000,39.416667;117.500000,41.083333
# ��ֱ�ţ�116.349007��39.936045��116.362429��39.945155

import DynMapTile
filePath = "C:\mcmdndata\��ͼͨ�Ƽ�(����)���޹�˾_zxt130822\��ȫ��v002"
# 15
z = 16
minx = 116.349007
miny = 39.936045
maxx = 116.362429
maxy = 39.945155
res = DynMapTile.GetMapTile( filePath, z, minx, miny, maxx, maxy )
print res











