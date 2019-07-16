# -*- coding: utf-8 -*-
"""
Created on Fri Jul 12 12:02:24 2019

@author: jingzl
"""

# wordcloud

import jieba
import wordcloud

#txt = "程序设计语言是计算机能够理解和识别用户操作意图的一种交互体系，它按照\
#特定规则组织计算机指令，使计算机能否自动进行各种运算处理"
#txt = "life is short, you need python"
#w = wordcloud.WordCloud( width=1000, font_path="msyh.ttc", height=700)
#w.generate( " ".join(jieba.lcut(txt)))
#w.to_file("pcwcloud.png")

from scipy.misc import imread
mask = imread("chinamap.jpg")
f = open("新时代中国特色社会主义.txt", "r", encoding="utf-8")
#f = open("关于实施乡村振兴战略的意见.txt", "r", encoding="utf-8")
t = f.read()
f.close()
ls = jieba.lcut(t)
txt = " ".join(ls)
w = wordcloud.WordCloud( font_path="msyh.ttc", width=1000, height=700, \
                        background_color="white", \
                        mask = mask)
w.generate(txt)
w.to_file("grwordcloud3.png")






