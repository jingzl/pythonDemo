# -*- coding: utf-8 -*-
"""
Created on Thu Jul 16 2020
爬取医院医生及排班信息
"""
__author__ = 'jingzl'
__version__ = '1.0'


import re
from selenium import webdriver
import time
import datetime
import pandas as pd
import numpy as np
import urllib.parse
import json


def whuh_doctor():
    # 专家信息：http://www.whuh.com/doctorss/search.html（已包含知名专家信息）
    # 知名专家：http://www.whuh.com/doctorss/index/is_doc1/1.html
    print("开始处理[whuh-doctor]-{0}".format(datetime.datetime.now().strftime('%Y%m%d %H:%M:%S')).center(100, '-'))
    try:
        start = time.perf_counter()
        option = None
        option = webdriver.ChromeOptions()
        option.add_argument(argument='headless')
        option.add_argument('--no-sandbox')
        option.add_argument('start-maximized')
        # 获取医生信息
        url = 'http://www.whuh.com/doctorss/search.html'
        browser = webdriver.Chrome(chrome_options=option)
        browser.get(url)
        s = browser.page_source.replace('amp;', '')
        # http://www.whuh.com/doctorss/index/sections_id/4.html
        m = re.findall(r"http://www.whuh.com/doctorss/index/sections_id/[0-9]*.html", s, re.M)
        dl = []
        print("共计{0}个科室页面".format(len(m)))
        browser2 = webdriver.Chrome(chrome_options=option)
        # 可以通过调整下一个语句的range(m,n)来调整爬取哪些科室
        for i in range(len(m)):  #
            print("--第{0}个科室:{1}".format(i, m[i]))
            browser2.get(m[i])
            browser2.switch_to.window(browser2.current_window_handle)
            s2 = browser2.page_source.replace('amp;', '')
            m2 = re.findall(r"/doctorss/view/[0-9]*.html", s2, re.M)
            docturl_list = list(set(m2))

            # 处理分页信息
            if len(docturl_list) > 1:
                yiipager_item = browser2.find_element_by_class_name('yiiPager')
                page_items = yiipager_item.find_elements_by_class_name('page')
                browser_page = webdriver.Chrome(chrome_options=option)
                for p in range(1, len(page_items)): # 从2页开始
                    a_item = page_items[p].find_element_by_tag_name('a')
                    page_url = a_item.get_attribute('href')
                    if len(page_url) <= 0:
                        continue
                    browser_page.get(page_url)
                    browser_page.switch_to.window(browser_page.current_window_handle)
                    s_page = browser_page.page_source.replace('amp;', '')
                    p_docturl = re.findall(r"/doctorss/view/[0-9]*.html", s_page, re.M)
                    docturl_list += list(set(p_docturl))
                    time.sleep(1)
                browser_page.quit()

            # http://www.whuh.com/doctorss/view/127.html
            print("--该科室下共计{0}个医生".format(len(docturl_list)))
            browser3 = webdriver.Chrome(chrome_options=option)
            for j in range(len(docturl_list)):  #
                print("----第{0}个医生：{1}".format(j, docturl_list[j]))
                detail_url = "http://www.whuh.com" + docturl_list[j]
                browser3.get(detail_url)
                browser3.switch_to.window(browser3.current_window_handle)
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
                time.sleep(1)
            browser3.quit()
            time.sleep(1)
        browser2.quit()
        browser.quit()
        dur = time.perf_counter() - start
        print("总计爬取用时：{:.2f}s".format(dur))

        df = pd.DataFrame(dl, columns=['姓名', '性别', '科室', '职称', '头像地址', '擅长', '简介'], index=np.arange(len(dl)))
        # 医生信息写入文件
        df.to_excel('./output/doctor_whuh.xlsx', index=False)

    except Exception as e:
        print(e)
        time.sleep(5)

    print("处理完毕[whuh-doctor]-{0}".format(datetime.datetime.now().strftime('%Y%m%d %H:%M:%S')).center(100, '-'))


def whuh_doctor_schedule(doctdate):
    # 门诊安排：http://www.whuh.com/help/menzheng.html
    print("开始处理[whuh-doctor-schedule]-{0}".format(datetime.datetime.now().strftime('%Y%m%d %H:%M:%S')).center(100, '-'))
    try:
        start = time.perf_counter()
        print(doctdate)
        option = None
        option = webdriver.ChromeOptions()
        option.add_argument(argument='headless')
        option.add_argument('--no-sandbox')
        option.add_argument('start-maximized')
        browser = webdriver.Chrome(chrome_options=option)
        browser.implicitly_wait(10)
        paiban_dl = []
        for k in range(1, 3):
            # 指定日期的全部数据
            url = 'http://www.whuh.com/searchmz/indexcopy.html?OutpDate={}&DeptName=&datatype=json&page={}'.format(doctdate, k)
            browser.get(url)
            s = browser.page_source.replace('amp;', '')
            res = browser.find_element_by_tag_name('body').text
            res_json = json.loads(res)
            dl = res_json['list']
            # 日期 时间 门诊 科室 医师 职称 院区 位置
            for i in range(len(dl)):
                yq = ''
                if dl[i]['yqdm'] == '1300':
                    yq = '西院门诊'
                if dl[i]['yqdm'] == '1000' and dl[i]['Kinds'] == '肿瘤门诊':
                    yq = '肿瘤门诊'
                if dl[i]['yqdm'] == '1000' and dl[i]['Kinds'] != '肿瘤门诊':
                    yq = '本部门诊'
                paiban_dl.append([dl[i]['OutpDate'], dl[i]['TimeInterval'], dl[i]['TopDept'], dl[i]['SuperDept'], dl[i]['DoctorName'], dl[i]['titles'], yq, dl[i]['DeptAddress']])
        browser.quit()
        dur = time.perf_counter() - start
        print("总计爬取用时：{:.2f}s".format(dur))
        df = pd.DataFrame(paiban_dl, columns=['日期', '时间', '门诊', '科室', '医师', '职称', '院区', '位置'], index=np.arange(len(paiban_dl)))
        # 医生信息写入文件
        df.to_excel('./output/paiban_whuh_'+doctdate+'.xlsx', index=False)

    except Exception as e:
        print(e)
        time.sleep(5)

    print("处理完毕[whuh-doctor-schedule]-{0}".format(datetime.datetime.now().strftime('%Y%m%d %H:%M:%S')).center(100, '-'))


def tjh_doctor():
    print("开始处理[tjh-doctor]-{0}".format(datetime.datetime.now().strftime('%Y%m%d %H:%M:%S')).center(100, '-'))
    try:
        start = time.perf_counter()
        # 专家信息
        # 医疗科室 https://www.tjh.com.cn/Section/Index.aspx#title
        # 医技科室 https://www.tjh.com.cn/Section/Technology.aspx#title
        # 医疗中心 https://www.tjh.com.cn/Section/Center.aspx#title
        option = None
        option = webdriver.ChromeOptions()
        option.add_argument(argument='headless')
        option.add_argument('--no-sandbox')
        option.add_argument('start-maximized')
        sectionidx_url_list = ['https://www.tjh.com.cn/Section/Index.aspx#title',
                               'https://www.tjh.com.cn/Section/Technology.aspx#title',
                               'https://www.tjh.com.cn/Section/Center.aspx#title']
        section_url_list = []
        browser = webdriver.Chrome(chrome_options=option)
        for i in range(len(sectionidx_url_list)):
            browser.get(sectionidx_url_list[i])
            s = browser.page_source.replace('amp;', '')
            # /Section/IndexDoctorIntro.aspx?title=%e5%bf%83%e8%a1%80%e7%ae%a1%e5%86%85%e7%a7%91
            m = re.findall(r"/Section/IndexDoctorIntro.aspx\?title=[%a-zA-Z0-9]+", s, re.M)
            section_url_list += m
        browser.quit()
        # 整理
        tmp_url_list = []
        for surl in section_url_list:
            surl = surl.upper()
            if surl not in tmp_url_list:
                tmp_url_list.append(surl)
        section_url_list = tmp_url_list
        dl = []
        print("共计{0}个科室页面".format(len(section_url_list)))
        browser2 = webdriver.Chrome(chrome_options=option)
        # 可以通过调整下一个语句的range(m,n)来调整爬取哪些科室
        for j in range(1):  # len(section_url_list)
            print("--第{0}个科室:{1}".format(j, section_url_list[j]))
            browser2.get('https://www.tjh.com.cn'+section_url_list[j])
            browser2.switch_to.window(browser2.current_window_handle)
            s2 = browser2.page_source.replace('amp;', '')
            m2 = re.findall(r"IndexDoctorIntroInfo.aspx\?id=[0-9]+", s2, re.M)
            docturl_list = m2
            # 科室名称
            doct_ks = urllib.parse.unquote(section_url_list[j].split('=')[-1])

            print("--该科室下共计{0}个医生".format(len(docturl_list)))
            browser3 = webdriver.Chrome(chrome_options=option)
            for k in range(len(docturl_list)):
                print("----第{0}个医生：{1}".format(k, docturl_list[k]))
                detail_url = "https://www.tjh.com.cn/Section/" + docturl_list[k]
                browser3.get(detail_url)
                browser3.switch_to.window(browser3.current_window_handle)

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
                for n in range(1, len(item2)-1):
                    doct_desc += item2[n].text

                # 姓名 性别 科室 职称 头像地址 擅长 简介
                dl.append([doct_name, doct_gender, doct_ks, doct_zc, doct_imgurl, doct_zy,  doct_desc])
                time.sleep(1)
            browser3.quit()
            time.sleep(1)
        browser2.quit()
        dur = time.perf_counter() - start
        print("总计爬取用时：{:.2f}s".format(dur))

        df = pd.DataFrame(dl, columns=['姓名', '性别', '科室', '职称', '头像地址', '擅长', '简介'], index=np.arange(len(dl)))
        # 医生信息写入csv文件
        df.to_excel('./output/doctor_tjh.xlsx', index=False)

    except Exception as e:
        print(e)
        time.sleep(5)

    print("处理完毕[tjh-doctor]-{0}".format(datetime.datetime.now().strftime('%Y%m%d %H:%M:%S')).center(100, '-'))


def tjh_doctor_schedule(doctdate, yuanqu, haobie):
    # 门诊安排
    print("开始处理[tjh-doctor-schedule]-{0}".format(datetime.datetime.now().strftime('%Y%m%d %H:%M:%S')).center(100, '-'))
    try:
        start = time.perf_counter()
        print(doctdate)
        option = None
        option = webdriver.ChromeOptions()
        option.add_argument(argument='headless')
        option.add_argument('--no-sandbox')
        option.add_argument('start-maximized')
        browser = webdriver.Chrome(chrome_options=option)
        browser.implicitly_wait(10)
        # https://www.tjh.com.cn/Menzhen/Arrange1.aspx?week=1&yuanqu=0&haobie=0&riqi=20200821&ksdaima=
        # 院区 & 号别
        yuanqu_ls = [0, 1, 2]  # 主院区 光谷院区 中法新城院区
        haobie_ls = [0, 1, 2]  # 专家门诊 知名/综合专家门诊 普通门诊
        yq_ls = []
        hb_ls = []
        if yuanqu not in yuanqu_ls:
            yq_ls = yuanqu_ls
        else:
            yq_ls.append(yuanqu)
        if haobie not in haobie_ls:
            hb_ls = haobie_ls
        else:
            hb_ls.append(haobie)
        paiban_dl = []
        for i in yq_ls:
            for j in hb_ls:
                url = "https://www.tjh.com.cn/Menzhen/Arrange1.aspx?yuanqu={0}&haobie={1}&riqi={2}&ksdaima=".format(i, j, doctdate)
                print(url)
                browser.get(url)
                browser.switch_to.window(browser.current_window_handle)
                mztb_item = browser.find_element_by_id('mztb')
                if not mztb_item:
                    continue
                items = mztb_item.find_elements_by_class_name('table-list')
                if not items or len(items) <= 0:
                    continue
                btnall_item = browser.find_element_by_id('btnAll')
                if btnall_item:
                    browser.execute_script("arguments[0].click();", btnall_item)
                print("院区-{} 号别-{}的排班共计：{}".format(i, j, len(items)))
                for k in range(len(items)):
                    td_item = items[k].find_elements_by_tag_name('td')
                    # 日期 时间 门诊 科室 医师 职称 院区 位置 状态
                    paiban_time = td_item[0].text
                    if not paiban_time:
                        continue
                    paiban_mz = td_item[1].text
                    paiban_ks = td_item[2].text
                    paiban_ys = td_item[3].text
                    paiban_zc = td_item[4].text
                    paiban_yq = td_item[5].text
                    paiban_wz = td_item[6].text
                    paiban_zt = td_item[7].text
                    paiban_dl.append([doctdate, paiban_time, paiban_mz, paiban_ks, paiban_ys, paiban_zc, paiban_yq, paiban_wz, paiban_zt])
                time.sleep(0.5)

        browser.quit()
        dur = time.perf_counter() - start
        print("总计爬取用时：{:.2f}s".format(dur))

        df = pd.DataFrame(paiban_dl, columns=['日期', '时间', '门诊', '科室', '医师', '职称', '院区', '位置', '状态'], index=np.arange(len(paiban_dl)))
        # 医生信息写入文件
        df.to_excel('./output/paiban_tjh_' + doctdate + '.xlsx', index=False)

    except Exception as e:
        print(e)
        time.sleep(5)

    print("处理完毕[tjh-doctor-schedule]-{0}".format(datetime.datetime.now().strftime('%Y%m%d %H:%M:%S')).center(100, '-'))


def main():
    # 爬取医生信息
    # 协和医院
    whuh_doctor()
    # 同济医院
    # tjh_doctor()

    # 爬取排班信息
    doctdate = "20200822"  # 指定排班日期，必须是今日或往后六天
    # 协和医院
    # whuh_doctor_schedule(doctdate)
    # 同济医院
    # yuanqu = [0, 1, 2]  # 主院区 光谷院区 中法新城院区
    # haobie = [0, 1, 2]  # 专家门诊 知名/综合专家门诊 普通门诊
    # 如果为-1，则全部处理
    yuanqu = 0
    haobie = 0
    #tjh_doctor_schedule(doctdate, yuanqu, haobie)

    # end
    print("all end".center(100, '-'))


if __name__ == '__main__':
    main()
