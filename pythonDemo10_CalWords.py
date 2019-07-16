# -*- coding: utf-8 -*-
"""
Created on Wed Jul 10 08:57:16 2019

@author: jingzl
"""

# 文本词频统计

#d = {"中国":"北京", "美国":"华盛顿", "英国":"伦敦" }
#"中国" in d
#len(d)
#
#d = {}
#d["a"]=1; d["b"] = 2

# CalHamlet
def getText():
    txt = open("hamlet.txt", "r").read()
    txt = txt.lower()
    for ch in '!"#$%&^*+_-;:/?,.~`()@[\\]{|}':
        txt = txt.replace( ch, " " )
    return txt

hamletTxt = getText()
words = hamletTxt.split()
counts = {}
for word in words:
    counts[word] = counts.get(word, 0) + 1
items = list(counts.items())
items.sort( key=lambda x:x[1], reverse = True )
for i in range(10):
    word, count = items[i]
    print("{0:<10}{1:>5}".format( word, count ))
    

# CalThreeKingdoms
import jieba
txt = open("threekingdoms.txt","r",encoding="utf-8").read()
excludes = {"将军","却说","荆州","二人","不可","不能","如此"}
words = jieba.lcut(txt)
counts = {}
for word in words:
    if len(word) == 1:
        continue
    elif word == "诸葛亮" or word == "孔明曰":
        rword = "孔明"
    elif word == "关公" or word == "云长":
        rword = "关羽"
    elif word == "玄德" or word == "玄德曰":
        rword = "刘备"
    elif word == "孟德" or word == "丞相曰":
        rword = "曹操"
    else:
        rword = word
    counts[rword] = counts.get( rword, 0 ) + 1

for word in excludes:
    del counts[word]
items = list( counts.items() )
items.sort( key=lambda x:x[1], reverse=True )
for i in range(10):
    word, count = items[i]
    print("{0:<10}{1:>5}".format(word, count))
    
    





