# -*- coding: utf-8 -*-
"""
Created on Sun Nov 17 2020
"""
__author__ = 'jingzl'
__version__ = '1.0'


import os
import pandas as pd
import numpy as np
import xlrd
import time
import datetime

CONST_DATA_PATH = './data/'
CONST_OUTPUT_PATH = './output/'


def logError(url):
    print('error: ' + url)


def outputURL(url1, url2):
    print(url1[0]+'_'+url1[1])
    print(url2)


def parseTypeForURL(typeFile, files):
    try:
        typels = []
        typeExcel = xlrd.open_workbook(CONST_DATA_PATH+typeFile, encoding_override='utf-8')
        if typeExcel:
            sheet = typeExcel.sheet_by_index(0)
            for i in range(sheet.nrows):
                row = sheet.row(i)
                typeurl = row[0].value.strip()
                typels.append(typeurl)

        for i in range(len(files)):
            datafile = CONST_DATA_PATH + files[i]
            excel = xlrd.open_workbook(datafile, encoding_override='utf-8')
            if excel:
                sheet = excel.sheet_by_index(0)
                for j in range(sheet.nrows):
                    row = sheet.row(j)
                    url = row[0].value.strip()
                    try:
                        url = url.split(' ')[0]
                        url1 = url.split('//')
                        url2 = url1[1].split('/')
                        url3 = url1[0] + '//' + url2[0] + '/' + url2[1]
                        if url3 in typels:
                            outputURL(url2, url)
                    except Exception as e:
                        print(e)
                        logError(url)
                        continue

    except Exception as e:
        print(e)


def parseSheet(sheet):
    type_list = []
    for i in range(sheet.nrows):
        row = sheet.row(i)
        url = row[0].value.strip()
        #print(url)
        # https://m.120ask.com/shilu/038cvwlcawkim7p9
        try:
            url1 = url.split('//')
            url2 = url1[1].split('/')
            url3 = url1[0] + '//' + url2[0] + '/' + url2[1]
            type_list.append(url3)
        except Exception as e:
            print(e)
            logError(url)
            continue
    return type_list


def parseFileForType(files):
    try:
        dl = []
        for i in range(len(files)):
            datafile = CONST_DATA_PATH + files[i]
            excel = xlrd.open_workbook(datafile, encoding_override='utf-8')
            if excel:
                print(datafile)
                # all_sheet = excel.sheets()
                # print(all_sheet)
                sheet = excel.sheet_by_index(0)
                dl += parseSheet(sheet)
        res = pd.value_counts(dl)
        dt = {'分类': res.index, '个数': res.values}
        df = pd.DataFrame(dt)
        df.to_excel(CONST_OUTPUT_PATH + '分类统计_{0}.xlsx'.format(datetime.datetime.now().strftime('%Y%m%d%H%M%S')),
            index=False)
    except Exception as e:
        print(e)


if __name__ == '__main__':
    # 解析原始数据统计分类
    firstFiles = ['v1-120ask.xlsx', 'v2-120ask.xlsx', 'v3-120ask.xlsx']
    #parseFileForType(firstFiles)

    # 确认要爬取的分类，并根据分类文件拆分URL
    typeFile = '分类统计_20201119_FINAL.xlsx'
    parseTypeForURL(typeFile, firstFiles)

    # 3. 根据URL的统计量倒序排列，优先处理量大的类型
    # 4. 分别将不同类型的URL爬取，并提取数据存储（每一种类型都需要单独分析，爬取）
    # 5. 根据不同类型分别输出新格式的excel
    # 6. 提供结果数据的最终合并操作

    # end
    print("all end".center(100, '-'))
