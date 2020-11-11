# -*- coding: utf-8 -*-
"""
Created on Sun Nov 8 2020
爬取 m.baidu.com 相关医疗问答
http://m.baidu.com/s?word=%E9%98%BF%E6%89%98%E4%BC%90%E4%BB%96%E6%B1%80%E7%9A%84%E5%89%AF%E4%BD%9C%E7%94%A8%E6%9C%89%E5%93%AA%E4%BA%9B&sa=tb&ts=1490330&t_kt=0&ie=utf-8&rsv_t=24caNQm8KstfgYGEYvu1yCT1gWPpL5D%252FEhTrpPu1HD63ff1EMN4a&rsv_pq=7788586972734363406&ss=100&sugid=5206738781083846848&rqlang=zh&rsv_sug4=8047&inputT=6235&oq=%E9%98%BF%E6%89%98%E4%BC%90%E4%BB%96%E6%B1%80
https://m.baidu.com/bh/m/detail/qr_8637588506800154377
https://m.baidu.com/bh/m/detail/qr_8637588506800154377

"""
__author__ = 'jingzl'
__version__ = '1.0'

import os
import re
from selenium import webdriver
import time
import datetime
import pandas as pd
import numpy as np
import xlrd
import argparse
parser = argparse.ArgumentParser()
parser.description = 'please enter parameters ...'
parser.add_argument('-q', '--query', help='爬取', dest='argQ', action='store_true')
args = parser.parse_args()

# 目标站
# m.baidu.com

# 分页次数
CONST_NEXTPAGE = 2


def pageQuery(keyword, browser):
    dl = []
    qtitle = ''
    qtxt = ''
    url2 = ''
    item_results = browser.find_elements_by_css_selector('.c-result.result')
    for i in range(len(item_results)):
        item_result = item_results[i]
        try:
            item_result_content = item_result.find_element_by_class_name('c-result-content')
            item_article = item_result_content.find_element_by_tag_name('article')
            item_section = item_result_content.find_element_by_tag_name('section')
            item_zj = item_section.find_element_by_css_selector('.lg-inner.c-gap-bottom-small')
            item_title = item_zj.find_element_by_class_name('c-title')
            qtitle = item_title.text
            if u'专家回答' not in qtitle:
                continue
            print(qtitle)

            # top1
            # c-touchable-feedback c-touchable-feedback-no-default tts-b-item
            item_top = item_section.find_element_by_css_selector('.c-touchable-feedback.c-touchable-feedback-no-default.tts-b-item')

            # 问题 tts-b-hl
            item_q = item_section.find_element_by_class_name('tts-b-hl')
            qtxt = item_q.text
            print(qtxt)

            # 回答 c-line-clamp2
            item_a = item_section.find_element_by_class_name('c-line-clamp2')
            atxt = item_a.text
            print(atxt)

            # 医院级别 c-label-gray-outline

            # 详情页链接 rl-link-href
            #durl = item_top.get_attribute('rl-link-href')
            #print(durl)
            item_durl = item_top.click()
            time.sleep(1)
            browser.switch_to.window(browser.window_handles[0])
            url2 = browser.current_url
            print(url2)

        except Exception as e:
            #print(e)
            #time.sleep(1)
            continue

        # print(qcontent)
        # keyword targetsite 提问 回答
        #dl.append([keyword, targetsite, qtitle, qcontent])

    dl.append([keyword, qtitle, qtxt, url2])

    return dl


def baiduQuery(keyword):
    option = None
    option = webdriver.ChromeOptions()
    #option.add_argument(argument='headless')
    option.add_argument('--no-sandbox')
    option.add_argument('start-maximized')

    dl = []
    try:
        url = 'http://m.baidu.com/s?word='+keyword
        browser = webdriver.Chrome(chrome_options=option)
        browser.implicitly_wait(10)
        browser.get(url)
        dl = pageQuery(keyword, browser)
        browser.quit()
        '''
        browser2 = webdriver.Chrome(chrome_options=option)
        for i in range(1, CONST_NEXTPAGE):
            url2 = "http://m.baidu.com/s?pn={0}&word=site:{1}+{2}".format(i*10, targetsite, keyword)
            browser2.get(url2)
            browser2.switch_to.window(browser2.current_window_handle)
            dl += pageQuery(keyword, browser2)
            time.sleep(1)
        browser2.quit()
        # 去重
        tmp_list = []
        for item in dl:
            if item not in tmp_list:
                tmp_list.append(item)
        dl = tmp_list
        '''
        return dl

    except Exception as e:
        print(e)
        time.sleep(1)
        return dl


def query(keyword_config):
    print("开始处理[query]-{0}".format(datetime.datetime.now().strftime('%Y%m%d %H:%M:%S')).center(100, '-'))
    start = time.perf_counter()

    keyword_list = []
    with open(keyword_config, 'r', encoding='UTF-8') as f:
        keyword_list = f.read().splitlines()
    print("共计[{0}]个关键字".format(len(keyword_list)))
    dl = []
    for i in range(1):  # len(keyword_list)
        keyword = keyword_list[i].strip()
        if len(keyword) <= 0:
            continue
        print("--keyword-{0}".format(keyword))
        dl += baiduQuery(keyword)

    df = pd.DataFrame(dl, columns=['keyword', '提问', '回答', '链接'], index=np.arange(len(dl)))
    #df = df.drop_duplicates(subset=['提问'], keep='first')
    # 医生信息写入excel文件
    df.to_excel('./output/qa2_{0}.xlsx'.format(datetime.datetime.now().strftime('%Y%m%d')), index=False)

    dur = time.perf_counter() - start
    print("总计爬取用时：{:.2f}s".format(dur))
    print("处理完毕[query]-{0}".format(datetime.datetime.now().strftime('%Y%m%d %H:%M:%S')).center(100, '-'))


def combinedata(qa_path):
    try:
        qa_ls = []
        fnames = os.listdir(qa_path)
        for f in fnames:
            datafile = qa_path + f
            excel = xlrd.open_workbook(datafile, encoding_override='utf-8')
            if excel:
                print(datafile)
                # all_sheet = excel.sheets()
                # print(all_sheet)
                sheet = excel.sheet_by_index(0)
                for i in range(1,sheet.nrows):
                    row = sheet.row(i)
                    v0 = row[0].value
                    v1 = row[1].value
                    v2 = row[2].value
                    v3 = row[3].value
                    qa_ls.append([v0, v1, v2, v3])
        df_qa = pd.DataFrame(qa_ls, columns=['keyword', 'targetsite', '提问', '回答'], index=np.arange(len(qa_ls)))  # ,
        df_qa = df_qa.drop_duplicates(subset=['提问'], keep='first')
        qa_file = './output/' + 'qa合并_{}.xlsx'.format(datetime.datetime.now().strftime('%Y%m%d%H%M%S'))
        df_qa.to_excel(qa_file, index=False)
        print('----{}'.format(qa_file))

    except Exception as e:
        print(e)


if __name__ == '__main__':
    query('./conf/querykeyword.txt')
    '''
    if args.argQ:
        query('./conf/keyword2.txt')
    elif args.argC:
        combinedata('./output/qa2/')
    else:
        print("invalid parameter, please check -h")
    '''
    # end
    print("all end".center(100, '-'))
