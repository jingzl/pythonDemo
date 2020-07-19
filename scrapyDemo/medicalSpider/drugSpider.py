# -*- coding: utf-8 -*-
"""
Created on Thu Jul 16 2020
爬取相关药品信息:
国家药品监督管理局, www.nmpa.gov.cn
"""
__author__ = 'jingzl'
__version__ = '1.0'


import requests
import re
from bs4 import BeautifulSoup
import bs4

import urllib
import urllib.request
import urllib.parse


def main():
    url = "http://app1.nmpa.gov.cn/datasearchcnda/face3/base.jsp?tableId=32&tableName=TABLE32&title=%B9%FA%B2%FA%D2%A9%C6%B7%C9%CC%C6%B7%C3%FB&bcId=152904813882776432084296368957"

    opener = urllib.request.build_opener()
    opener.addheaders = [#('Host', 'www.nmpa.gov.cn'),
        ('User-Agent',"Mozilla/5.0(Windows NT 10.0; WOW64) AppleWebKit/537.36(KHTML, like Gecko) Chrome/69.0.3497.81 Safari/537.36  Maxthon / 5.3.8.2000"),
                         #('Accept', 'image / webp, image / apng, image / *, * / *;q = 0.8'),
                         #('Accept-Encoding', 'gzip, deflate'),
                         #('Accept-Language', 'zh-CN'),
                         #('Connection', 'keep-alive')
                         ]
    urllib.request.install_opener(opener)
    response = urllib.request.urlopen(url)
    html = response.read()

    path = "D:\\4-1.html"
    with open(path, 'wb') as f:
        f.write(html)
        f.close()
        print("文件保存成功")



    response.close()


if __name__ == '__main__':
    main()



