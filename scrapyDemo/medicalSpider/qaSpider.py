# -*- coding: utf-8 -*-
"""
Created on Thu Jul 16 2020
爬取 m.baidu.com 相关医疗问答
"""
__author__ = 'jingzl'
__version__ = '1.0'

import re
from selenium import webdriver
import time
import datetime
import pandas as pd
import numpy as np


# 目标站
CONST_TARGETSITE = ['youlai.cn','mfk.com','ydf.com','baikemy.com','myzx.cn','sytown.cn','yxys.com','miaoshou.com',
                    'pule.com','vodjk.com','fh21.com.cn','bohe.cn']


def baiduQuery(keyword, targetsite):
    option = None
    option = webdriver.ChromeOptions()
    option.add_argument(argument='headless')
    option.add_argument('--no-sandbox')

    try:
        url = 'http://m.baidu.com/s?word=site:'+targetsite+'+'+keyword
        browser = webdriver.Chrome(chrome_options=option)
        browser.get(url)
        s = browser.page_source.replace('amp;', '')
        dl = []
        item_results = browser.find_elements_by_css_selector('.c-result.result')
        for i in range(len(item_results)):
            item_result = item_results[i]
            audio_tag = item_result.find_elements_by_css_selector('.c-gap-left-middle.c-color-link.c-font-normal')
            if (len(audio_tag) <= 0):
                continue
            # 标题
            item = item_result.find_element_by_class_name('c-title-text')
            if not item:
                continue
            print(item.text)
            # 内容
            #item = item_result.find_element_by_class_name('')





    except Exception as e:
        print(e)
        time.sleep(5)


def query(keyword_list):
    print("开始处理[query]-{0}".format(datetime.datetime.now().strftime('%Y%m%d %H:%M:%S')).center(100, '-'))
    print("共计[{0}]个目标站，[{1}]个关键字".format(len(CONST_TARGETSITE), len(keyword_list)))

    start = time.perf_counter()
    for i in range(1): # len(keyword_list)
        keyword = keyword_list[i].strip()
        if (len(keyword) <= 0):
            continue
        print("--keyword-{0}".format(keyword))
        for j in range(1): # len(CONST_TARGETSITE)
            targetsite = CONST_TARGETSITE[j]
            print("----site-{0}".format(targetsite))
            baiduQuery(keyword,targetsite)

    dur = time.perf_counter() - start
    print("总计爬取用时：{:.2f}s".format(dur))
    print("处理完毕[query]-{0}".format(datetime.datetime.now().strftime('%Y%m%d %H:%M:%S')).center(100, '-'))


def getHTMLText(url):
    try:
        r = requests.get( url, timeout=30 )
        r.raise_for_status()
        r.encoding = r.apparent_encoding
        return r.text
    except:
        return ""


def fillUnivList( ulist, html ):
    myAttrs = {'class':'results'}
    soup = BeautifulSoup(html)
    # soup = BeautifulSoup(html)
    ulist = soup.find_all(name='div',attrs=myAttrs)
    '''
    for tr in soup.find( 'tbody' ).children:
        if isinstance( tr, bs4.element.Tag ):
          tds = tr('td')
          ulist.append([tds[0].string, tds[1].string, tds[3].string])
    '''



def main():
    uinfo = []
    #url = "http://m.baidu.com/ssid=c2156c6a7a3530364e12/s?word=site%3Ayoulai.cn+%E7%B3%96%E5%B0%BF%E7%97%85%E8%B6%B3&sa=tb&ts=5343508&t_kt=0&ie=utf-8&rsv_t=7de61UzIHwLNAhxszrzb83X1WbW02ZqxCbXg2Mhg9rLxl5s0Jx6P&rsv_pq=11715403022720906433&sugid=12439476823165841995&rqlang=zh&rsv_sug4=4287&oq=site%3Ayoulai.cn%2B%E7%B3%96%E5%B0%BF%E7%97%85%E8%B6%B3"
    # url = "http://m.baidu.com/ssid=c2156c6a7a3530364e12/s?pn=10&usm=1&word=site%3Ayoulai.cn+%E7%B3%96%E5%B0%BF%E7%97%85%E8%B6%B3&sa=np&rsv_pq=12218146811932497451&rsv_t=7336GooOmbKC9C78yQwKPjp3gOiEkAcWGv4DEe6lDkvrmNZRA0Ci&rqid=12218146811932497451&params_ssrt=smarty"
    #url = "http://m.baidu.com/s?word=site%3Ayoulai.cn+%E8%85%B9%E7%97%9B"
    url = "http://m.baidu.com/s?pn=30&usm=1&word=site%3Ayoulai.cn%20%E8%85%B9%E7%97%9B"
    # html = getHTMLText( url )

    fakeHeaders = {
        'User-Agent': "Mozilla/5.0(Windows NT 10.0; WOW64) AppleWebKit/537.36(KHTML, like Gecko) Chrome/69.0.3497.81 Safari/537.36  Maxthon / 5.3.8.2000"}

# Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; AcooBrowser; .NET CLR 1.1.4322; .NET CLR 2.0.50727)

    opener = urllib.request.build_opener()
    opener.addheaders = [('Host', 'm.baidu.com'),
        ('User-Agent',"Mozilla/5.0(Windows NT 10.0; WOW64) AppleWebKit/537.36(KHTML, like Gecko) Chrome/69.0.3497.81 Safari/537.36  Maxthon / 5.3.8.2000"),
                         #('Accept', 'image / webp, image / apng, image / *, * / *;q = 0.8'),
                         #('Accept-Encoding', 'gzip, deflate'),
                         #('Accept-Language', 'zh-CN'),
                         #('Connection', 'keep-alive')
                         ]
    urllib.request.install_opener(opener)
    response = urllib.request.urlopen(url)
    html = response.read()


    path = "D:\\2-1.html"
    with open(path, 'wb') as f:
        f.write(html)
        f.close()
        print("文件保存成功")



    soup = BeautifulSoup(html)
    myAttrs = {'class': 'c-result result', 'srcid': 'www_normal'}  # h5_mobile
    uinfo = soup.find_all(name='div', attrs=myAttrs)

    for result in uinfo:
        #print( result )
        #print(type(result))

        # 获取标题
        title_tag = result.find(name='span', attrs={'class': 'c-title-text'})
        if not title_tag:
            continue
        title = title_tag.get_text()

        # 查找是否有收听按钮
        audio_tag = result.find(name='div', attrs={'aria-label':re.compile(r'收听医生语音(\s\w+)?')})
        if not audio_tag:
            continue
        # print( audio_tag )

        # 获得内容
        txt_tag = result.find(name='div', attrs={'role': 'text'})
        if not txt_tag:
            continue
        spanls = txt_tag.find_all(name='span')
        if not spanls:
            continue
        if not spanls[1]:
            continue
        txt = spanls[1].get_text()

        print("{0}----{1}".format(title, txt))

        print('------------------------------------------------------------------')


    response.close()

    # fillUnivList( uinfo, html )
    #print( uinfo )

    '''
    path = "C:\\jingzl\\1.html"
    with open(path, 'wb') as f:
        f.write(html)
        f.close()
        print("文件保存成功")
    '''

    # fillUnivList( uinfo, html )
    # printUnivList( uinfo, 20 )  # 20 univs


if __name__ == '__main__':
    # 解析爬虫配置
    keyword_list = []
    with open('./conf/keyword.txt', 'r', encoding='UTF-8') as f:
        keyword_list = f.read().splitlines()
    query(keyword_list)

    # end
    print("all end".center(100, '-'))



