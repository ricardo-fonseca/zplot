#!/usr/bin/env python3

""" Generates the colortables.h file

The color tables are copied from the CMOcean color tables 
from Matplotlib.

For more information on the CMOcean tables visit
https://matplotlib.org/cmocean/
"""

import sys
import cmocean
import matplotlib
import numpy as np

def export(table,name):
    x = np.linspace( start = 0.0, stop = 1.0, num = 256 )

    rgba = table( x )

    argb_i8 = np.empty((256,3),dtype = np.uint8)
    argb_i8[:,0] = rgba[:,0] * 255
    argb_i8[:,1] = rgba[:,1] * 255
    argb_i8[:,2] = rgba[:,2] * 255

    print("const uint8_t ct_%s[] = {"%name)
    for i in range(64):
        print("    ",end='')
        for j in range(4):
            idx = i*4+j
            if (idx<255):
                end = ','
            else:
                end = ''
            print("%#04x,%#04x,%#04x"%(argb_i8[idx,0],argb_i8[idx,1],argb_i8[idx,2]), end = end )
        print("")
    print("};")


header = """
#ifndef __COLORTABLES__
#define __COLORTABLES__

#include <stdint.h>
"""
print(header)

ctnames = cmocean.cm.cmapnames

tableNames = []
tableEnum = []

for name in ctnames:
    table = getattr( cmocean.cm, name )
    if (isinstance(table,matplotlib.colors.LinearSegmentedColormap)):
        print("")
        export( table, name )
        tableNames.append("ct_%s"%name)
        tableEnum.append(name.upper())

print("")
print("enum color_table{")
for i in range(0,len(tableEnum),8):
    print("    ", end='')
    if ( i + 8 > len(tableEnum) ):
        stop = len(tableEnum)
    else:
        stop = i + 8
    for j in range( i, stop ):
        if (j < len(tableEnum) - 1):
            print("%s"%tableEnum[j], end=",")
        else:
            print("%s"%tableEnum[j], end="")
    print("")
print('};')
        

print("")
print("const uint8_t * ct_values[] = {")
for i in range(0,len(tableNames),8):
    print("    ", end='')
    if ( i + 8 > len(tableNames) ):
        stop = len(tableNames)
    else:
        stop = i + 8
    for j in range( i, stop ):
        if (j < len(tableNames) - 1):
            print("%s"%tableNames[j], end=",")
        else:
            print("%s"%tableNames[j], end="")
    print("")
print('};')

print("")
print("#endif")
