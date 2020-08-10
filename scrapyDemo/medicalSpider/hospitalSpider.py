# -*- coding: utf-8 -*-
"""
Created on Thu Jul 16 2020
爬取医院医生及排班信息
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
from utils import *
import json
import time


'''
def main():
    print("hello")
    url = "https://www.tjh.com.cn/Menzhen/Arrange1.aspx?week=0&yuanqu=0&haobie=0&riqi=20200718&ksdaima=#title"
    url = "https://www.tjh.com.cn/Menzhen/Arrange1.aspx?week=1&yuanqu=0&haobie=0&riqi=20200719&ksdaima=#title"

    url = "https://www.tjh.com.cn/Menzhen/ZuanJia_details.aspx?id=302001&mzDaima=#title"
    url = "https://www.tjh.com.cn/Menzhen/ZuanJia_details.aspx?id=851001&mzDaima=#title"


    url = "http://www.whuh.com/doctorss/search.html"
    url = "http://www.whuh.com/doctorss/index/is_doc1/1.html"
    url = "http://www.whuh.com/help/menzheng.html"


    return
'''


def whuh_doctor():
    # 专家信息：http://www.whuh.com/doctorss/search.html（已包含知名专家信息）
    # 知名专家：http://www.whuh.com/doctorss/index/is_doc1/1.html
    option = None
    option = webdriver.ChromeOptions()
    option.add_argument(argument='headless')
    option.add_argument('--no-sandbox')

    try:
        # 获取医生信息
        url = 'http://www.whuh.com/doctorss/search.html'
        browser = webdriver.Chrome(chrome_options=option)
        browser.get(url)
        s = browser.page_source.replace('amp;', '')
        # print(s)
        # print(len(m))
        # print(m[0])
        # http://www.whuh.com/doctorss/index/sections_id/4.html
        m = re.findall(r"http://www.whuh.com/doctorss/index/sections_id/[0-9]*.html", s, re.M)
        print(len(m))
        print(m[0])
        for i in range(len(m)):
            browser2 = webdriver.Chrome(chrome_options=option)
            browser2.get(m[i])
            s2 = browser2.page_source.replace('amp;', '')
            m2 = re.findall(r"/doctorss/view/[0-9]*.html", s2, re.M)
            docturl_list = list(set(m2))
            # print(len(docturl_list))
            # print(docturl_list[0])
            # http://www.whuh.com/doctorss/view/127.html
            for j in range(len(docturl_list)):
                detail_url = "http://www.whuh.com" + docturl_list[j]
                browser3 = webdriver.Chrome(chrome_options=option)
                browser3.get(detail_url)
                s3 = browser3.page_source.replace('amp;', '')

                browser3.close()

            browser2.close()
            break

        browser.close()

    except Exception as e:
        print(e)
        time.sleep(5)



def whuh_doctor_schedule(doctdate):
    # 门诊安排：http://www.whuh.com/help/menzheng.html
    print(doctdate)



def whuh(hasdoctor， doctdate):
    # 武汉协和医院
    if hasdoctor:
        whuh_doctor()
    whuh_doctor_schedule(doctdate)


def main():
    option = None
    mysql_db = DataBase()
    # 配置文件中开启是否无头，生产阶段关闭
    if if_headless():
        option = webdriver.ChromeOptions()
        option.add_argument(argument='headless')
        option.add_argument('--no-sandbox')

    for i in range(1, 2):  #
        try:
            browser = webdriver.Chrome(chrome_options=option)

            url_1 = 'https://www.tjh.com.cn/Menzhen/Arrange1.aspx?week=0&yuanqu=0&haobie=0&riqi=20200722&ksdaima=#title'

            browser.get(url_1)
            s = browser.page_source.replace('amp;', '')
            #print(s)

            #'<p style="cursor:pointer;" onclick="getUrl("王琳","902001")"><b>王琳</b></p>'

            #print(s)
            #m = re.findall(r'<p style="cursor:pointer;" onclick="getUrl("\s","\d")\s+', s, re.M)
            m = re.findall(r"getUrl\('[\u4E00-\u9FA5]+','[0-9]*'\)", s, re.M)
            #print(m[0])  # getUrl('吴杰','851001')

            #m = browser.find_elements_by_class_name('table-list')
            #m0 = m[0].find_element_by_tag_name('p').get_attribute('onclick')
            #print(m0)
            #print(m)

            browser.close()

            for j in range(1): # len(m)
                print(m[0])
                doctid = re.findall("\d+", m[0], re.M)

                print(doctid[0])
                # 需要判断医生ID是否存在，避免重复爬取

                url_2 = 'https://www.tjh.com.cn/Menzhen/ZuanJia_details.aspx?id='+doctid[0]+'&mzDaima='
                print(url_2)
                browser = webdriver.Chrome(chrome_options=option)
                browser.get(url_2)
                #print(browser.page_source)


                # 姓名、性别、职务、职称、头像、擅长、个人简介、排版信息
                item = browser.find_element_by_class_name('personInfo-title')
                doct_name = item.text
                print(doct_name)

                item = browser.find_element_by_class_name('personIntro-detail')
                item2 = item.find_elements_by_tag_name('p')
                print(item2[0].text)
                print(item2[1].text)
                print(item2[2].text)

                item = browser.find_element_by_class_name('personAward')
                item2 = item.find_element_by_class_name('text')
                print(item2.text)

                item = browser.find_element_by_class_name('personNote')
                item2 = item.find_elements_by_tag_name('p')
                #print(item2[0].text)
                print(item2[1].text)
                print(item2[2].text)

                item = browser.find_element_by_class_name('personArrange')
                item2 = item.find_elements_by_tag_name('tr')
                for k in range(1,len(item2)):
                    item3 = item2[k].find_elements_by_tag_name('td')
                    print(item3[0].text)
                    print(item3[1].text)
                    print(item3[2].text)
                    print(item3[3].text)





                '''
                sql = "insert into t_gcypspm(c_bh, dt_insertTime, c_url, b_content,c_json, c_page) VALUES (REPLACE(UUID(),\"-\",\"\"), sysdate(), %s,%s,%s,%s)"
                mysql_db.exetcute_sql(sql, [url_2, browser.page_source, parse2json(browser.page_source),
                                            str(i) + '_' + str(j + 1)])
                # pickle.loads(s) 可用该方法将乱码汉字转换
                '''


                browser.close()

        except Exception as e:
            print(e)
            time.sleep(5)


def parse2json(html):
    '''
    批准文号        pzwh
    批准文号备注    pzwhbz
    产品名称        cpmc
    英文名称        ywmc
    商品名          spm
    生产单位        scdw
    规格            gg
    剂型            jx
    产品类别        cplb
    批准日期        pzrq
    原批准文号      ypzwh
    药品本位码      ypbwm
    药品本位码备注  ypbwmbz
    生产地址        scdz
    '''
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




if __name__ == '__main__':

    # (\(^"[\u4E00-\u9FA5]+^",^"[0-9]*^"\))
    #str = '<p style="cursor:pointer;" onclick="getUrl("王琳","902001")"><b>王琳</b></p>'
    #m = re.findall(r'getUrl\("[\u4E00-\u9FA5]+","[0-9]*"\)', str, re.M)
    #print(m[0])

    #s = "getUrl('吴杰','851001')"
    #doctid = re.findall("\d+", s, re.M)
    #print(doctid[0])


    #s = '<p class="personInfo-title">吴杰</p>'
    #doctname = re.findall('[\u4E00-\u9FA5]+', s, re.M)
    #print(doctname[0])

    #s = '<img src="/plmnhytf12f3/1/专家照片/2018/100209  王琳.jpg">'
    #doctimg = re.findall('plmnhytf12f3', s, re.M)
    #print(doctimg)

    # 通过配置文件获取相关参数：
    # 是否爬取医生信息、排班的日期
    hasdoctor = True
    doctdate = "20200810"
    whuh(hasdoctor， doctdate)

    #main()
