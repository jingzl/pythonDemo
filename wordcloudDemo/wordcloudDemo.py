# -*- coding: utf-8 -*-
# @Time    : 2020/12/10 15:07
# @Author  : jingzl
# @Email   : jingzl@hotmail.com
# @File    : 1.py
'''
词云DEMO
'''
__author__ = 'jingzl'
__version__ = '1.0'

import jieba
import wordcloud
#from scipy.misc import imread # 版本不对应
import imageio


def filterWords(t):
    # 需要根据实际业务数据来调整：新的词 和 忽略的词
    # 考虑需要排除过滤的词，比如一些虚词等
    #new_words = {'722项目','海大项目','JKW','水下人机协同','710','20公里声通','万米声通','20节点'}
    #for w in new_words:
    #    jieba.add_word(w)

    ls = jieba.lcut(t)

    words = []
    excludes = {'的', '及', '需要', '确认', '提交', '已', 'ok',
                'Monday', 'Tuesday', 'Wednesday', 'Thursday', 'Friday', 'Saturday', 'Sunday'}
    for i in range(len(ls)):
        if ls[i] in excludes:
            continue
        elif ls[i] in '!"#$%&^*+_-;:/?,.~`()@[\\]{|}，；： \n':  # 常见的中英文符号
            continue
        else:
            words.append(ls[i])
    return words


def main():
    # txt = "程序设计语言是计算机能够理解和识别用户操作意图的一种交互体系，它按照\
    # 特定规则组织计算机指令，使计算机能否自动进行各种运算处理"
    # txt = "life is short, you need python"
    # w = wordcloud.WordCloud( width=1000, font_path="msyh.ttc", height=700)
    # w.generate( " ".join(jieba.lcut(txt)))
    # w.to_file("pcwcloud.png")

    # f = open("SOT工作日志_2020.txt", "r", encoding="utf-8")
    f = open("2020年政府工作报告.txt", "r", encoding="utf-8")
    t = f.read()
    f.close()

    words = filterWords(t)
    txt = " ".join(words)

    # 设置需要掩码图形，白色背景
    #mask = imageio.imread("兔子.jpg")
    mask = imageio.imread("chinamap.jpg")
    w = wordcloud.WordCloud(font_path="msyh.ttc", width=1000, height=700, \
                            background_color="white", \
                            mask=mask)
    w.generate(txt)
    w.to_file("output.png")


if __name__ == '__main__':
    main()



