# -*- coding: utf-8 -*-
"""
Created on Thu Jul 18 10:32:55 2019

@author: jingzl
"""

## re库使用

import re

# search
match = re.search( r'[1-9]\d{5}', 'BIT 100081')
if match:
    print( match.group(0) )


# match
match = re.match( r'[1-9]\d{5}', '100081 BIT 100082' )
if match:
    print( match.group(0) )

# findall
ls = re.findall( r'[1-9]\d{5}', 'BIT100081 TSU100084' )
if ls:
    print( ls )

# split
ls = re.split( r'[1-9]\d{5}', 'BIT100081 TSU100084' )
if ls:
    print( ls )
    
# finditer
for m in re.finditer( r'[1-9]\d{5}', 'BIT100081 TSU100084' ):
    if m:
        print( m.group(0) )
    
# sub
str = re.sub( r'[1-9]\d{5}', ':zipcode', 'BIT100081 TSU100084' )
print( str )
    

pat = re.compile( r'[1-9]\d{5}' )
rst = pat.search( 'BIT100081 TSU100084' )
if rst:
    print( rst.group(0))

# match
m = re.search( r'[1-9]\d{5}', 'BIT100081 TSU100084' )
print( m.string )
print( m.re )
print( m.group(0) )
print( m.span() )


m = re.search( r'PY.*N', 'PYANBNCNDN' )
print( m.group(0) )
m = re.search( r'PY.*?N', 'PYANBNCNDN' )
print( m.group(0) )




















    