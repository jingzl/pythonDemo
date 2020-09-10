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
# 分页次数
CONST_NEXTPAGE = 9


def pageQuery(keyword, targetsite, browser):
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
        qtitle = item.text
        if len(qtitle) <= 0:
            continue
        # print(item.text)
        # 内容
        qcontent = ''
        item = item_result.find_element_by_class_name('c-line-clamp2')
        item2 = item.find_elements_by_tag_name('span')
        if not item2:
            continue
        if not item2[1]:
            continue
        qcontent = item2[1].text
        if len(qcontent) <= 0:
            continue
        # print(qcontent)
        # keyword targetsite 提问 回答
        dl.append([keyword, targetsite, qtitle, qcontent])
    return dl


def baiduQuery(keyword, targetsite):
    option = None
    option = webdriver.ChromeOptions()
    option.add_argument(argument='headless')
    option.add_argument('--no-sandbox')
    option.add_argument('start-maximized')

    try:
        url = 'http://m.baidu.com/s?word=site:'+targetsite+'+'+keyword
        browser = webdriver.Chrome(chrome_options=option)
        browser.get(url)
        dl = []
        dl = pageQuery(keyword, targetsite, browser)
        browser.quit()
        browser2 = webdriver.Chrome(chrome_options=option)
        for i in range(1, CONST_NEXTPAGE):
            url2 = "http://m.baidu.com/s?pn={0}&word=site:{1}+{2}".format(i*10, targetsite, keyword)
            browser2.get(url2)
            browser2.switch_to.window(browser2.current_window_handle)
            dl += pageQuery(keyword, targetsite, browser2)
            time.sleep(1)
        browser2.quit()
        # 去重
        tmp_list = []
        for item in dl:
            if item not in tmp_list:
                tmp_list.append(item)
        dl = tmp_list
        return dl

    except Exception as e:
        print(e)
        time.sleep(5)


def query(keyword_list):
    print("开始处理[query]-{0}".format(datetime.datetime.now().strftime('%Y%m%d %H:%M:%S')).center(100, '-'))
    print("共计[{0}]个目标站，[{1}]个关键字".format(len(CONST_TARGETSITE), len(keyword_list)))
    start = time.perf_counter()

    for i in range(len(keyword_list)):  #
        keyword = keyword_list[i].strip()
        if len(keyword) <= 0:
            continue
        print("--keyword-{0}".format(keyword))
        dl = []
        for j in range(len(CONST_TARGETSITE)):  #
            targetsite = CONST_TARGETSITE[j]
            print("----site-{0}".format(targetsite))
            dl += baiduQuery(keyword, targetsite)
        df = pd.DataFrame(dl, columns=['keyword', 'targetsite', '提问', '回答'], index=np.arange(len(dl)))
        # 医生信息写入excel文件
        df.to_excel('./output/qa_{0}_{2}_{1}.xlsx'.format(keyword, datetime.datetime.now().strftime('%Y%m%d'), i), index=False)

    dur = time.perf_counter() - start
    print("总计爬取用时：{:.2f}s".format(dur))
    print("处理完毕[query]-{0}".format(datetime.datetime.now().strftime('%Y%m%d %H:%M:%S')).center(100, '-'))


if __name__ == '__main__':
    # 解析爬虫配置
    keyword_list = []
    with open('./conf/keyword.txt', 'r', encoding='UTF-8') as f:
        keyword_list = f.read().splitlines()
    query(keyword_list)

    # end
    print("all end".center(100, '-'))
