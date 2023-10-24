//
//  ZNMapTypes.m
//  ZDCNavi
//
//  Created by Satoshi Muraki on 08/12/11.
//  Copyright 2008 SONORAN BLUE. All rights reserved.
//

#include "ZNMapTypes.h"
#include "ZNAppConstants.h"


string kZNMapTileInfoBlockX = ("x");
string kZNMapTileInfoBlockY = ("y");
string kZNMapTileInfoBlockOrigin = ("o");

const ZUINT32 ZNMapBlockLongitude[kZNMapLevelCount] =
{
    28125,          // g
    112500,         // i
    450000,         // j
    1800000,        // k
    3600000,        // l
    7200000,        // m
    14400000,       // n
    28800000,	    // o
    115200000,      // p
    460800000       // q
};

const ZUINT32 ZNMapBlockLatitude[kZNMapLevelCount] =
{
    18750,          // g
    75000,          // i
    300000,         // j
    1200000,        // k
    2400000,        // l
    4800000,	    // m
    9600000,        // n
    19200000,	    // o
    76800000,       // p
    307200000       // q
};
