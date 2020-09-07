# -*- coding: utf-8 -*-
"""
Created on Mon Aug 31 2020
Excel文件信息转换
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


def saveFile(df_zz, df_tz, df_fz):
    try:
        df_zz.to_excel(CONST_OUTPUT_PATH + '疾病症状_{}.xlsx'.format(datetime.datetime.now().strftime('%Y%m%d%H%M%S')), index=False)
        df_tz.to_excel(CONST_OUTPUT_PATH + '疾病体征_{}.xlsx'.format(datetime.datetime.now().strftime('%Y%m%d%H%M%S')), index=False)
        df_fz.to_excel(CONST_OUTPUT_PATH + '疾病辅助检查_{}.xlsx'.format(datetime.datetime.now().strftime('%Y%m%d%H%M%S')), index=False)
    except Exception as e:
        print(e)


def parseSheet(sheet, zz_ls, tz_ls, fz_ls):
    try:
        # 症状，体征，辅助
        diseaseName = sheet.cell_value(0, 1)
        print(diseaseName)
        ksName = sheet.cell_value(3, 1)
        print(ksName)
        flag = 0
        lastName = ''
        for i in range(sheet.nrows):
            row = sheet.row(i)
            name = row[0].value.strip()
            val = row[1].value
            if isinstance(val, str):
                val = val.strip()
            if len(name) <= 0 and len(val) <= 0:
                continue
            if name == '症状名称' and val == '症状属性值':
                flag = 1
                lastName = ''
            elif name == '体征名称' and val == '体征属性值':
                flag = 2
                lastName = ''
            elif name == '辅助检查名称' and val == '特征值':
                flag = 3
                lastName = ''

            if flag == 1 and name != '症状名称':
                if len(lastName) <= 0 and len(name) <= 0:
                    print('data error -- 症状名称')
                    continue
                if len(name) > 0 and len(val) > 0:
                    zz_ls.append([ksName, diseaseName, name, val])
                    lastName = name
                if len(name) <= 0 and len(val) > 0 and len(lastName) > 0:
                    zz_ls.append([ksName, diseaseName, lastName, val])
            elif flag == 2 and name != '体征名称':
                if len(lastName) <= 0 and len(name) <= 0:
                    print('data error -- 体征名称')
                    continue
                if len(name) > 0 and len(val) > 0:
                    tz_ls.append([ksName, diseaseName, name, val])
                    lastName = name
                if len(name) <= 0 and len(val) > 0 and len(lastName) > 0:
                    tz_ls.append([ksName, diseaseName, lastName, val])
            elif flag == 3 and name != '辅助检查名称':
                if len(lastName) <= 0 and len(name) <= 0:
                    print('data error -- 辅助检查名称')
                    continue
                if len(name) > 0 and len(val) > 0:
                    fz_ls.append([ksName, diseaseName, name, val])
                    lastName = name
                if len(name) <= 0 and len(val) > 0 and len(lastName) > 0:
                    fz_ls.append([ksName, diseaseName, lastName, val])

    except Exception as e:
        print(e)


def parseFile():
    try:
        # 症状，体征，辅助
        zz_ls = []
        tz_ls = []
        fz_ls = []
        fnames = os.listdir(CONST_DATA_PATH)
        for f in fnames:
            datafile = CONST_DATA_PATH + f
            excel = xlrd.open_workbook(datafile, encoding_override='utf-8')
            if excel:
                print(datafile)
                # all_sheet = excel.sheets()
                # print(all_sheet)
                sheet = excel.sheet_by_index(0)
                parseSheet(sheet, zz_ls, tz_ls, fz_ls)

        df_zz = pd.DataFrame(zz_ls, columns=['所属科室', '疾病名称', '症状名称', '症状属性值'], index=np.arange(len(zz_ls)))
        df_tz = pd.DataFrame(tz_ls, columns=['所属科室', '疾病名称', '体征名称', '体征属性值'], index=np.arange(len(tz_ls)))
        df_fz = pd.DataFrame(fz_ls, columns=['所属科室', '疾病名称', '辅助检查名称', '辅助检查属性值'], index=np.arange(len(fz_ls)))
        return df_zz, df_tz, df_fz

    except Exception as e:
        print(e)


def main():
    df_zz, df_tz, df_fz = parseFile()
    saveFile(df_zz, df_tz, df_fz)
    # end
    print("all end".center(100, '-'))


if __name__ == '__main__':
    main()

