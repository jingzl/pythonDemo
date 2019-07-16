# -*- coding: utf-8 -*-
"""
Created on Sat Jun 29 18:41:57 2019

@author: jingzl
"""

import turtle
turtle.setup( 650, 350, 200, 200 )
turtle.penup()
turtle.fd(-250)
turtle.pendown()
turtle.pensize(25)
turtle.pencolor("purple")
turtle.seth(-40)
for i in range(4):
    turtle.circle(40,80)
    turtle.circle(-40,80)
turtle.circle(40, 80/2)
turtle.fd(40)
turtle.circle(16,180)
turtle.fd(40*2/3)
turtle.done()




