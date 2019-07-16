# -*- coding: utf-8 -*-
"""
Created on Tue Jul  9 07:55:43 2019

@author: jingzl
"""

# KochDraw.py 科赫雪花绘制

import turtle

def koch( size, n ):
    if n == 0:
        turtle.fd( size )
    else:
        for angle in [0, 60, -120, 60]:
            turtle.left( angle )
            koch( size/3, n-1 )

def main():
    turtle.setup( 600, 600 )
    turtle.penup()
    turtle.goto(-200,100)
    turtle.pendown()
    turtle.pensize(2)
    level = 4
    koch( 400, level )  # 3阶科赫曲线
    turtle.right(120)
    koch( 400, level )
    turtle.right(120)
    koch( 400, level )
    turtle.hideturtle()
    turtle.done()
    
main()









