# -*- coding: utf-8


# 北京区域：115.375000,39.416667;117.500000,41.083333
# 西直门：116.349007，39.936045；116.362429，39.945155

import DynMapTile
filePath = "C:\mcmdndata\易图通科技(北京)有限公司_zxt130822\新全国v002"
# 15
z = 16
minx = 116.349007
miny = 39.936045
maxx = 116.362429
maxy = 39.945155
res = DynMapTile.GetMapTile( filePath, z, minx, miny, maxx, maxy )
print res











