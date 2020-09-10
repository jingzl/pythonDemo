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


import re
from selenium import webdriver
from selenium.webdriver import DesiredCapabilities
from selenium.webdriver.chrome.options import Options
from utils import *
import json
import time


'''
def main():
    option = None
    mysql_db = DataBase()
    # 配置文件中开启是否无头，生产阶段关闭
    if if_headless():
        option = webdriver.ChromeOptions()
        option.add_argument(argument='headless')
        option.add_argument('--no-sandbox')
        option.add_argument(
            'Mozilla/5.0(Windows NT 10.0; WOW64) AppleWebKit/537.36(KHTML, like Gecko) Chrome/69.0.3497.81 Safari/537.36  Maxthon / 5.3.8.2000'
            #'Mozilla/5.0 (Linux; Android 6.0; Nexus 5 Build/MRA58N) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/74.0.3729.157 Mobile Safari/537.36'
         )
        option.add_argument('blink-settings=imagesEnabled=false')  # 不加载图片, 提升速度

    for i in range(1, 2):  # 470 遍历469个一级目录网页
        try:
            browser = webdriver.Chrome(chrome_options=option)
            #url_1 = 'http://app1.nmpa.gov.cn/datasearchcnda/face3/base.jsp?tableId=32&tableName=TABLE32&title=%B9%FA%B2%FA%D2%A9%C6%B7%C9%CC%C6%B7%C3%FB&bcId=152904813882776432084296368957&tableId=32&State=1&bcId=152904813882776432084296368957&State=1&tableName=TABLE32&State=1&viewtitleName=COLUMN302&State=1&viewsubTitleName=COLUMN303,COLUMN299&State=1&curstart=' + str(
            #    i) + '&State=1&tableView=%25E5%259B%25BD%25E4%25BA%25A7%25E8%258D%25AF%25E5%2593%2581%25E5%2595%2586%25E5%2593%2581%25E5%2590%258D&State=1'
            #url_1 = 'https://www.baidu.com/'

            #url_1 = 'http://app1.nmpa.gov.cn/datasearchcnda/face3/base.jsp?tableId=25&tableName=TABLE25&title=%B9%FA%B2%FA%D2%A9%C6%B7&bcId=152904713761213296322795806604'
            #url_1 = 'http://app1.nmpa.gov.cn/datasearchcnda/face3/dir.html?type=yp'
            #url_1 = 'http://www.nmpa.gov.cn/WS04/CL2042/'
            url_1 = 'file:///C:/jingzl/1.html'

            browser.get(url_1)
            #print(url_1)

            s = browser.page_source.replace('amp;', '')
            print(s)
            m = re.findall(r'content.jsp\?tableId=32&tableName=TABLE32&tableView=国产药品商品名&Id=\d+', s, re.M)

            browser.close()
            print(m)

            for j in range(1): # len(m)
                url_2 = 'http://app1.nmpa.gov.cn/datasearchcnda/face3/' + m[j]
                browser = webdriver.Chrome(chrome_options=option)
                browser.get(url_2)
                print(url_2)
                print(browser.page_source)
                #sql = "insert into t_gcypspm(c_bh, dt_insertTime, c_url, b_content,c_json, c_page) VALUES (REPLACE(UUID(),\"-\",\"\"), sysdate(), %s,%s,%s,%s)"
                #mysql_db.exetcute_sql(sql, [url_2, browser.page_source, parse2json(browser.page_source),
                #                            str(i) + '_' + str(j + 1)])
                # pickle.loads(s) 可用该方法将乱码汉字转换
                browser.close()

        except Exception as e:
            print(e)
            time.sleep(5)


def parse2json(html):
    # 批准文号        pzwh
    # 批准文号备注    pzwhbz
    # 产品名称        cpmc
    # 英文名称        ywmc
    # 商品名          spm
    # 生产单位        scdw
    # 规格            gg
    # 剂型            jx
    # 产品类别        cplb
    # 批准日期        pzrq
    # 原批准文号      ypzwh
    # 药品本位码      ypbwm
    # 药品本位码备注  ypbwmbz
    # 生产地址        scdz
    
    # 初始化，避免取不到的情况下为空值
    result_json = dict()
    # 批准文号
    reg_dict = dict()

    reg_dict['pzwh'] = r"批准文号</td>\s*<td.*>(.*)</td></tr>"
    reg_dict['pzwhbz'] = r"批准文号备注</td>\s*<td.*>(.*)</td></tr>"
    reg_dict['cpmc'] = r"产品名称</td>\s*<td.*>(.*)</td></tr>"
    reg_dict['ywmc'] = r"英文名称</td>\s*<td.*>(.*)</td></tr>"
    reg_dict['spm'] = r"商品名</td>\s*<td.*>(.*)</td></tr>"
    reg_dict['scdw'] = r"生产单位</td>\s*<td.*><a.*>(.*)</a></td></tr>"
    reg_dict['gg'] = r"规格</td>\s*<td.*>(.*)</td></tr>"
    reg_dict['jx'] = r"剂型</td>\s*<td.*>(.*)</td></tr>"
    reg_dict['cplb'] = r"产品类别</td>\s*<td.*>(.*)</td></tr>"
    reg_dict['pzrq'] = r"批准日期</td>\s*<td.*>(.*)</td></tr>"
    reg_dict['ypzwh'] = r"原批准文号</td>\s*<td.*>(.*)</td></tr>"
    reg_dict['ypbwm'] = r"药品本位码</td>\s*<td.*>(.*)</td></tr>"
    reg_dict['ypbwmbz'] = r"药品本位码备注</td>\s*<td.*>(.*)</td></tr>"
    reg_dict['scdz'] = r"生产地址</td>\s*<td.*>(.*)</td></tr>"
    for i, v in reg_dict.items():
        reg_search = re.search(v, html)
        if reg_search is not None:
            result_json[i] = reg_search.group(1)
        else:
            result_json[i] = ''
    return json.dumps(result_json, ensure_ascii=False)
'''

# 国产药品商品名
def GCYPSPM():
    option = None
    option = webdriver.ChromeOptions()
    option.add_argument('--headless')
    option.add_argument('--no-sandbox')
    option.add_argument('--disable-gpu')
    option.add_argument('disable-dev-shm-usage')
    option.add_argument("--window-size=800x600")
    option.add_argument("start-maximised")

    capabilities = DesiredCapabilities.CHROME.copy()
    capabilities['acceptSslCerts'] = True
    capabilities['acceptInsecureCerts'] = True
    capabilities['marionette'] = False

    browser = webdriver.Chrome(chrome_options=option, desired_capabilities=capabilities)
    browser.implicitly_wait(10)

    url = "http://app1.nmpa.gov.cn/data_nmpa/face3/base.jsp?tableId=32&tableName=TABLE32&title=%B9%FA%B2%FA%D2%A9%C6%B7%C9%CC%C6%B7%C3%FB&bcId=152904813882776432084296368957"

    browser.get(url)
    s = browser.page_source.replace('amp;', '')
    m = re.findall(r"javascript:commitForECMA", s, re.M)
    print(len(m))


def GCYPSPM2():
    #browser = webdriver.Firefox()
    #url = "http://app1.nmpa.gov.cn/data_nmpa/face3/base.jsp?tableId=32&tableName=TABLE32&title=%B9%FA%B2%FA%D2%A9%C6%B7%C9%CC%C6%B7%C3%FB&bcId=152904813882776432084296368957"

    #browser.get(url)
    #s = browser.page_source.replace('amp;', '')
    pass







def main():
    GCYPSPM()


if __name__ == '__main__':
    main()



