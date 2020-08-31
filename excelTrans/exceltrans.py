# -*- coding: utf-8 -*-
"""
Created on Mon Aug 31 2020
Excel文件信息转换
"""
__author__ = 'jingzl'
__version__ = '1.0'


import xlrd

CONST_DATA_PATH = './data/'
CONST_OUTPUT_PATH = './output/'


def saveFile(df_zz, df_tz, df_fz):

    pass


def parseSheet(sheet):
    # 症状，体征，辅助
    zz_ls = []
    tz_ls = []
    fz_ls = []
    diseaseName = sheet.cell_value(0, 1)
    print(diseaseName)
    for i in range(sheet.nrows):
        row = sheet.row(i)
        # 症状名称
        if row[0].value == '症状名称' and row[1].value == '症状属性值':
            print(row[0].value)

        # 体征名称
        if row[0].value == '体征名称' and row[1].value == '体征属性值':
            print(row[0].value)


        # 辅助检查名称
        if row[0].value == '辅助检查名称' and row[1].value == '特征值':
            print(row[0].value)




    # df_zz =

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

