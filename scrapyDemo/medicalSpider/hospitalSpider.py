# -*- coding: utf-8 -*-
"""
Created on Thu Jul 16 2020
爬取医院医生及排班信息
"""
__author__ = 'jingzl'
__version__ = '1.0'


import re
from selenium import webdriver
from utils import *
import json
import time
import datetime
import pandas as pd
import numpy as np
import urllib.parse



def whuh_doctor():
    # 专家信息：http://www.whuh.com/doctorss/search.html（已包含知名专家信息）
    # 知名专家：http://www.whuh.com/doctorss/index/is_doc1/1.html
    option = None
    option = webdriver.ChromeOptions()
    option.add_argument(argument='headless')
    option.add_argument('--no-sandbox')

    try:
        print("开始处理[whuh-doctor]-{0}".format(datetime.datetime.now().strftime('%Y%m%d %H:%M:%S')).center(100, '-'))
        start = time.perf_counter()

        # 获取医生信息
        url = 'http://www.whuh.com/doctorss/search.html'
        browser = webdriver.Chrome(chrome_options=option)
        browser.get(url)
        s = browser.page_source.replace('amp;', '')
        # http://www.whuh.com/doctorss/index/sections_id/4.html
        m = re.findall(r"http://www.whuh.com/doctorss/index/sections_id/[0-9]*.html", s, re.M)
        dl = []
        print("共计{0}个科室页面".format(len(m)))
        for i in range(2): # len(m)
            print("--第{0}个科室:{1}".format(i, m[i]))
            browser2 = webdriver.Chrome(chrome_options=option)
            browser2.get(m[i])
            s2 = browser2.page_source.replace('amp;', '')
            m2 = re.findall(r"/doctorss/view/[0-9]*.html", s2, re.M)
            docturl_list = list(set(m2))

            # 处理分页信息
            yiipager_item = browser2.find_element_by_class_name('yiiPager')
            page_items = yiipager_item.find_elements_by_class_name('page')
            for p in range(1, len(page_items)): # 从2页开始
                a_item = page_items[p].find_element_by_tag_name('a')
                page_url = a_item.get_attribute('href')
                if len(page_url) <= 0:
                    continue
                browser_page = webdriver.Chrome(chrome_options=option)
                browser_page.get(page_url)
                s_page = browser_page.page_source.replace('amp;', '')
                p_docturl = re.findall(r"/doctorss/view/[0-9]*.html", s_page, re.M)
                docturl_list += list(set(p_docturl))
                browser_page.close()

            # http://www.whuh.com/doctorss/view/127.html
            print("--该科室下共计{0}个医生".format(len(docturl_list)))
            for j in range(len(docturl_list)): #
                print("----第{0}个医生：{1}".format(j, docturl_list[j]))
                detail_url = "http://www.whuh.com" + docturl_list[j]
                browser3 = webdriver.Chrome(chrome_options=option)
                browser3.get(detail_url)
                #s3 = browser3.page_source.replace('amp;', '')
                # 姓名
                item = browser3.find_element_by_class_name('zj_b1')
                doctname = item.text
                #print(doctname)
                # 所在科室  职称  专业专长
                item = browser3.find_element_by_class_name('zj_nr')
                doctinfo = item.text
                doctinfo_ary = doctinfo.split('\n')
                doctinfo_ks = doctinfo_ary[0].split('：')[-1]
                doctinfo_zc = doctinfo_ary[1].split('：')[-1]
                doctinfo_zy = doctinfo_ary[2].split('：')[-1]
                #print(doctinfo_ks, doctinfo_zc, doctinfo_zy)
                # 个人简介
                item = browser3.find_element_by_class_name('nr3')
                doctdesc = item.text
                #print(doctdesc)
                # 头像
                item = browser3.find_element_by_css_selector('.x.pt10')
                item_img = item.find_element_by_tag_name('img')
                doctimgurl = item_img.get_attribute('src')
                #print(doctimgurl)

                # 姓名 性别 科室 职称 头像地址 擅长 简介
                dl.append([doctname, '', doctinfo_ks, doctinfo_zc, doctimgurl, doctinfo_zy, doctdesc])
                browser3.close()
            browser2.close()
            time.sleep(0.1)
        browser.close()
        dur = time.perf_counter() - start
        print("总计爬取用时：{:.2f}s".format(dur))

        df = pd.DataFrame(dl, columns=['姓名', '性别', '科室', '职称', '头像地址', '擅长', '简介'], index=np.arange(len(dl)))
        # 医生信息写入csv文件
        df.to_csv('./output/doctor_whuh.csv', index=False, sep='|')
        print("处理完毕[whuh-doctor]-{0}".format(datetime.datetime.now().strftime('%Y%m%d %H:%M:%S')).center(100, '-'))

    except Exception as e:
        print(e)
        time.sleep(5)


def whuh_doctor_schedule(doctdate):
    # 门诊安排：http://www.whuh.com/help/menzheng.html
    print("开始处理[whuh-doctor-schedule]-{0}".format(datetime.datetime.now().strftime('%Y%m%d %H:%M:%S')).center(100, '-'))
    start = time.perf_counter()

    print(doctdate)

    print("处理完毕[whuh-doctor-schedule]-{0}".format(datetime.datetime.now().strftime('%Y%m%d %H:%M:%S')).center(100, '-'))


def whuh(hasdoctor, doctdate):
    # 武汉协和医院
    if hasdoctor:
        whuh_doctor()
    whuh_doctor_schedule(doctdate)


def tjh_doctor():
    # 专家信息
    # 医疗科室 https://www.tjh.com.cn/Section/Index.aspx#title
    # 医技科室 https://www.tjh.com.cn/Section/Technology.aspx#title
    # 医疗中心 https://www.tjh.com.cn/Section/Center.aspx#title
    option = None
    option = webdriver.ChromeOptions()
    option.add_argument(argument='headless')
    option.add_argument('--no-sandbox')
    try:
        print("开始处理[tjh-doctor]-{0}".format(datetime.datetime.now().strftime('%Y%m%d %H:%M:%S')).center(100, '-'))
        start = time.perf_counter()
        sectionidx_url_list = ['https://www.tjh.com.cn/Section/Index.aspx#title',
                               'https://www.tjh.com.cn/Section/Technology.aspx#title',
                               'https://www.tjh.com.cn/Section/Center.aspx#title']
        section_url_list = []
        for i in range(len(sectionidx_url_list)):
            browser = webdriver.Chrome(chrome_options=option)
            browser.get(sectionidx_url_list[i])
            s = browser.page_source.replace('amp;', '')
            # /Section/IndexDoctorIntro.aspx?title=%e5%bf%83%e8%a1%80%e7%ae%a1%e5%86%85%e7%a7%91
            m = re.findall(r"/Section/IndexDoctorIntro.aspx\?title=[%a-zA-Z0-9]+", s, re.M)
            section_url_list += m
            browser.close()
        # 整理
        tmp_url_list = []
        for surl in section_url_list:
            surl = surl.upper()
            if surl not in tmp_url_list:
                tmp_url_list.append(surl)
        section_url_list = tmp_url_list
        dl = []
        print("共计{0}个科室页面".format(len(section_url_list)))
        for j in range(1): # len(section_url_list)
            print("--第{0}个科室:{1}".format(j, section_url_list[j]))
            browser2 = webdriver.Chrome(chrome_options=option)
            browser2.get('https://www.tjh.com.cn'+section_url_list[j])
            s2 = browser2.page_source.replace('amp;', '')
            m2 = re.findall(r"IndexDoctorIntroInfo.aspx\?id=[0-9]+", s2, re.M)
            docturl_list = m2
            browser2.close()
            # 科室名称
            doct_ks = urllib.parse.unquote(section_url_list[j].split('=')[-1])

            print("--该科室下共计{0}个医生".format(len(docturl_list)))
            for k in range(len(docturl_list)):
                print("----第{0}个医生：{1}".format(k, docturl_list[k]))
                detail_url = "https://www.tjh.com.cn/Section/" + docturl_list[k]
                browser3 = webdriver.Chrome(chrome_options=option)
                browser3.get(detail_url)

                # 姓名
                item = browser3.find_element_by_class_name('personInfo-title')
                doct_name = item.text

                # 照片 personIntro-header
                item = browser3.find_element_by_class_name('personIntro-header')
                item_img = item.find_element_by_tag_name('img')
                doct_imgurl = item_img.get_attribute('src')

                # 性别 职称
                item = browser3.find_element_by_class_name('personIntro-detail')
                item2 = item.find_elements_by_tag_name('p')
                doct_gender = item2[0].find_elements_by_tag_name('span')[0].text
                doct_zc = item2[2].text.split('：')[-1]

                # 擅长
                item = browser3.find_element_by_class_name('personAward')
                item2 = item.find_element_by_class_name('text')
                doct_zy = item2.text

                # 简介
                doct_desc = ''
                item = browser3.find_element_by_class_name('personNote')
                item2 = item.find_elements_by_tag_name('p')
                for n in range(1,len(item2)-1):
                    doct_desc += item2[n].text

                # 姓名 性别 科室 职称 头像地址 擅长 简介
                dl.append([doct_name, doct_gender, doct_ks, doct_zc, doct_imgurl, doct_zy,  doct_desc])
                browser3.close()
            time.sleep(0.1)

        dur = time.perf_counter() - start
        print("总计爬取用时：{:.2f}s".format(dur))

        df = pd.DataFrame(dl, columns=['姓名', '性别', '科室', '职称', '头像地址', '擅长', '简介'], index=np.arange(len(dl)))
        # 医生信息写入csv文件
        df.to_csv('./output/doctor_tjh.csv', index=False, sep='|')
        print("处理完毕[tjh-doctor]-{0}".format(datetime.datetime.now().strftime('%Y%m%d %H:%M:%S')).center(100, '-'))

    except Exception as e:
        print(e)
        time.sleep(5)


def tjh_doctor_schedule(doctdate):
    # 门诊安排
    print("开始处理[tjh-doctor-schedule]-{0}".format(datetime.datetime.now().strftime('%Y%m%d %H:%M:%S')).center(100, '-'))
    start = time.perf_counter()

    print(doctdate)

    print("处理完毕[tjh-doctor-schedule]-{0}".format(datetime.datetime.now().strftime('%Y%m%d %H:%M:%S')).center(100, '-'))


def tjh(hasdoctor, doctdate):
    # 同济医院
    if hasdoctor:
        tjh_doctor()
    tjh_doctor_schedule(doctdate)


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


                browser.close()

        except Exception as e:
            print(e)
            time.sleep(5)




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
    # 协和医院
    #whuh(hasdoctor, doctdate)
    # 同济医院
    tjh(hasdoctor, doctdate)


    # end
    print("all end".center(100, '-'))
