# -*- coding: utf-8 -*-
"""
Created on Thu Jul 18 13:58:48 2019

@author: jingzl
"""

### 淘宝商品比价定向爬虫
import requests
import re

def getHTMLText( url ):
    try:
        r = requests.get( url, timeout=30 )
        r.raise_for_status()
        r.encoding = r.apparent_encoding
        return r.text
    except:
        return ""

def parsePage( ilt, html ):
    try:
        plt = re.findall( r'\"view_price\"\:\"[\d\.]*\"', html )
        tlt = re.findall( r'\"raw_title\"\:\".*?\"', html )
        for i in range( len(plt) ):
            price = eval( plt[i].split(':')[1] )
            title = eval( tlt[i].split(':')[1] )
            ilt.append( [price, title] )
    except:
        print("解析失败")
    
def printGoodsList( ilt):
    tplt = "{:4}\t{:8}\t{:16}"
    print( tplt.format("序号","价格","商品名称") )
    count = 0
    for g in ilt:
        count = count + 1
        print( tplt.format( count, g[0], g[1] ) )
    
def main():
    goods = '书包'
    depth = 1
    ## 分析淘宝搜索链接
    start_url = 'https://s.taobao.com/search?q=' + goods  
    infoList = []
    for i in range( depth ):
        try:
            url = start_url + '&s={0}'.format(44*i)
            html = getHTMLText( url )
            parsePage( infoList, html )
        except:
            continue
    printGoodsList( infoList )

main()

























