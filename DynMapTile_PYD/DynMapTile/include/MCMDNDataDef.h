#ifndef _MCMDNDATADEF_H_
#define _MCMDNDATADEF_H_



const int LEVEL_NUM = 10;
const int ZNMapBlockLongitude4[LEVEL_NUM] = 
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

const int ZNMapBlockLatitude4[LEVEL_NUM] = 
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


//const int MESH_BASE_COORX     = 100800000; //( 257040000*1000/2550 );     // 最左下角基准横坐标
//const int MESH_BASE_COORY     = 4800000; //( 12240000*1000/2550  );     // 最左下角基准纵坐标

// 以南纬90，西经180为原点坐标
const int MESH_BASE_COORX       = -648000000; // 180 * 3600000;
const int MESH_BASE_COORY       = -324000000; // 90 * 3600000;


#define TRANS_TO_SECOND1000(x)          (int)(x*3600000) // 1/1000秒单位
#define REVTRANS_TO_SECOND1000(x)       (x/3600000.0) 

#endif
