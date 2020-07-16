# -*- coding: utf-8 -*-
"""
Created on Thu Jul 16 2020
爬取 m.baidu.com 相关医疗问答
"""
__author__ = 'jingzl'
__version__ = '1.0'

import requests
from bs4 import BeautifulSoup
import bs4

def getHTMLText(url):
    try:
        r = requests.get( url, timeout=30 )
        r.raise_for_status()
        r.encoding = r.apparent_encoding
        return r.content
    except:
        return ""




def main():
    uinfo = []
    url = "http://m.baidu.com/ssid=c2156c6a7a3530364e12/s?word=site%3Ayoulai.cn+%E7%B3%96%E5%B0%BF%E7%97%85%E8%B6%B3&sa=tb&ts=5343508&t_kt=0&ie=utf-8&rsv_t=7de61UzIHwLNAhxszrzb83X1WbW02ZqxCbXg2Mhg9rLxl5s0Jx6P&rsv_pq=11715403022720906433&sugid=12439476823165841995&rqlang=zh&rsv_sug4=4287&oq=site%3Ayoulai.cn%2B%E7%B3%96%E5%B0%BF%E7%97%85%E8%B6%B3"
    html = getHTMLText( url )

    path = "C:\\jingzl\\1.html"
    with open(path, 'wb') as f:
        f.write(html)
        f.close()
        print("文件保存成功")

    # fillUnivList( uinfo, html )
    # printUnivList( uinfo, 20 )  # 20 univs


if __name__ == '__main__':
    main()

