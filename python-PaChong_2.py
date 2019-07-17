# -*- coding: utf-8 -*-
"""
Created on Wed Jul 17 14:41:39 2019

@author: jingzl
"""

import requests
from bs4 import BeautifulSoup

r = requests.get( "https://python123.io/ws/demo.html" )
demo = r.text
soup = BeautifulSoup( demo, "html.parser" )
print( soup.title )
tag = soup.a
print( tag.parent.parent.name )
print( tag.attrs )
print( tag.attrs['class'])
print( type(tag) )
print( tag.string )
print( soup.p.string )

newsoup = BeautifulSoup( "<b><!--This is a comment--></b>\
                        <p>This is not a comment</p>", "html.parser" )
print( newsoup.b.string )
print( type(newsoup.b.string) )
print( newsoup.p.string )
print( type(newsoup.p.string) )

##
print( soup.title.parent )
print( soup.html.parent )
print( soup.parent )

print( soup.a.next_sibling )
print( soup.a.previous_sibling )

####
print( soup.prettify() )
print( soup.a.prettify() )

####
print(soup.find_all("p"))

import re
print( len(soup.find_all(string=re.compile('demo'))))

for link in soup.find_all('a'):
    print( link.get('href'))


