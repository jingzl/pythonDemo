# -*- coding: utf-8 -*-
"""
Created on Tue Jul 16 11:34:26 2019

@author: jingzl
"""

import requests

def getHTMLText(url):
    try:
        r = requests.get( url, timeout = 30 )
        r.raise_for_status()
        r.encoding = r.apparent_encoding
        return r.text
    except:
        return "爬取失败"
    
#if __name__ == "__main__":
#    url = "http://www.baidu.com"
#    print( getHTMLText(url) )

# 爬取JD页面
#if __name__ == "__main__":
#    url = "https://item.jd.com/2967929.html"
#    print( getHTMLText(url)[:1000] )


# 爬取亚马逊商品页面
url = "https://www.amazon.cn/gp/product/B01M8L5Z3Y"
try:
    kv = {"user-agent":"Mozilla/5.0"}
    r = requests.get( url, headers=kv )
    r.raise_for_status()
    r.encoding = r.apparent_encoding
    print( r.text[1000:2000] )
except:
    print( "爬取失败" )

    
# 百度 360 的搜索关键词提交
keyword = "python"
try:
    kv = { 'wd':keyword }
    r = requests.get( "http://www.baidu.com/s", params=kv )
    print( r.request.url )
    r.raise_for_status()
    print( len(r.text) )
except:
    print( "爬取失败" )
    
    
## 爬取网络图片并存储
import requests
import os

root = "d://pics//"
url = "http://image.ngchina.com.cn/2019/0710/20190710011326785.jpg"
path = root + url.split('/')[-1]
try:
    if not os.path.exists(root):
        os.mkdir(root)
    if not os.path.exists(path):
        r = requests.get(url)
        r.raise_for_status()
        with open( path, 'wb' ) as f:
            f.write( r.content )
            f.close()
            print( "文件保存成功" )
    else:
        print("文件已存在")
except:
    print( "爬取失败" )


## IP地址归属地自动查询
import requests

url = "http://m.ip138.com/ip.asp?ip="
try:
    r = requests.get( url + '119.139.199.61' )
    r.raise_for_status()
    r.encoding = r.apparent_encoding
    print( r.text[-500:])
except:
    print("爬取失败")

    



