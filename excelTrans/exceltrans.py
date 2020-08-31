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
        print( row[0].value, row[1].value )


        # 体征名称


        # 辅助检查名称




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

