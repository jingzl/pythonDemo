# -*- coding: utf-8 -*-
"""
Created on Mon Aug 31 2020
Excel文件信息转换
"""
__author__ = 'jingzl'
__version__ = '1.0'

import pandas as pd
import numpy as np
import xlrd

CONST_DATA_PATH = './data/'
CONST_OUTPUT_PATH = './output/'


def saveFile(df_zz, df_tz, df_fz):
    df_zz.to_excel(CONST_OUTPUT_PATH + '_1.xlsx', index=False)
    df_tz.to_excel(CONST_OUTPUT_PATH + '_2.xlsx', index=False)
    df_fz.to_excel(CONST_OUTPUT_PATH + '_3.xlsx', index=False)


def parseSheet(sheet):
    # 症状，体征，辅助
    zz_ls = []
    tz_ls = []
    fz_ls = []
    diseaseName = sheet.cell_value(0, 1)
    print(diseaseName)
    flag = 0
    lastName = ''
    for i in range(sheet.nrows):
        row = sheet.row(i)
        name = row[0].value.strip()
        val = row[1].value.strip()
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
                print('data error')
                continue
            if len(name) > 0 and len(val) > 0:
                zz_ls.append([diseaseName, name, val])
                lastName = name
            if len(name) <= 0 and len(val) > 0 and len(lastName) > 0:
                zz_ls.append([diseaseName, lastName, val])
        elif flag == 2 and name != '体征名称':
            if len(lastName) <= 0 and len(name) <= 0:
                print('data error')
                continue
            if len(name) > 0 and len(val) > 0:
                tz_ls.append([diseaseName, name, val])
                lastName = name
            if len(name) <= 0 and len(val) > 0 and len(lastName) > 0:
                tz_ls.append([diseaseName, lastName, val])
        elif flag == 3 and name != '辅助检查名称':
            if len(lastName) <= 0 and len(name) <= 0:
                print('data error')
                continue
            if len(name) > 0 and len(val) > 0:
                fz_ls.append([diseaseName, name, val])
                lastName = name
            if len(name) <= 0 and len(val) > 0 and len(lastName) > 0:
                fz_ls.append([diseaseName, lastName, val])

    df_zz = pd.DataFrame(zz_ls, columns=['疾病名称', '症状名称', '症状属性值'], index=np.arange(len(zz_ls)))
    df_tz = pd.DataFrame(tz_ls, columns=['疾病名称', '体征名称', '体征属性值'], index=np.arange(len(tz_ls)))
    df_fz = pd.DataFrame(fz_ls, columns=['疾病名称', '辅助检查名称', '辅助检查属性值'], index=np.arange(len(fz_ls)))
    return df_zz, df_tz, df_fz


def main():
    datafile = CONST_DATA_PATH + 'test.xlsx'
    excel = xlrd.open_workbook(datafile, encoding_override='utf-8')
    if excel:
        print(datafile)
        # all_sheet = excel.sheets()
        # print(all_sheet)
        sheet = excel.sheet_by_index(0)
        df_zz, df_tz, df_fz = parseSheet(sheet)
        saveFile(df_zz, df_tz, df_fz)


if __name__ == '__main__':
    main()

