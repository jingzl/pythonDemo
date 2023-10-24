//
//  ZNMapTypesCustom.m
//  ZDCNavi2
//
//  Created by Satoshi Muraki on 09/06/12.
//  Copyright 2009 SONORAN BLUE. All rights reserved.
//
#include "ZNMapTypesCustom.h"

/*
    @abstract   
    @constant   kZNMapZoomScale
*/
//Modify by libin for PalmIphone (PalmCity Android Scale)
const ZFLOAT32 kZNMapZoomScale[kZNMapZoomScaleCount] = 
{
    1.647000,       // 1. g x2.0
    0.822860,       // 2. g x1.375
    0.411597,       // 3. g x0.875
    0.164679,       // 4. i x2.0
    0.082300,       // 5. i x1.0 //Modify by libin for PalmIphone (retina)
    0.041177,       // 6. j x2.0
    0.016471,       // 7. j x1.0
    0.008235,       // 8. k x2.0
    0.004118,       // 9. k x1.0

};

//Modify by libin for PalmIphone <<<<<<<<<<
/*
    g - 1.000000000 ( x2.0 - x1.00 ) ( 2.00000000 - 1.0000000 )
    h - 1.000000000 ( x1.5 - x0.75 ) ( N/A )
    i - 0.250000000 ( x4.0 - x1.00 ) ( 1.00000000 - 0.2500000 )
    j - 0.062500000 ( x4.0 - x1.00 ) ( 0.25000000 - 0.0625000 )
    k - 0.015625000 ( x3.0 - x1.00 ) ( N/A )
    l - 0.007812500 ( x8.0 - x1.00 ) ( 0.06250000 - 0.00781250 )
    n - 0.001953125 ( x4.0 - x1.00 ) ( 0.00781250 - 0.001953125 )
*/
//Modify by libin for PalmIphone
const bool kZNMapLevelValidation[kZNMapLevelCount] =
{
    0,          // g ( N/A )
    0,          // h ( N/A )
    1,          // i
    1,          // j
    1,          // k 
    1,          // l
    1,          // m
//Modify by libin for PalmIphone <<<<<<<<<< (TwoPointTouchStopToFixScale) Max 20KM
    0,          // n
    0           // o
//Modify by libin for PalmIphone >>>>>>>>>> (TwoPointTouchStopToFixScale)
};
//TODO: libin new add m,o, need debug
//Modify by libin for PalmIphone
// wqw edit 20120807 start
//const ZFLOAT32 kZNMapScalingMax[kZNMapLevelCount] =
//{
//    2.00000,    // g ( N/A )
//    1.00000,    // h ( N/A )
////Modify by libin for PalmIphone (TwoPointTouchStopToFixScale) Min 50m
//    1.647059,    // i
//    4.00000,    // j
//    4.00000,    // k
//    4.00000,    // l
//    4.00000,    // m
//    4.00000,    // n
//    4.00000     // o
//};
const ZFLOAT32 kZNMapScalingMax[kZNMapLevelCount] =
{
    2.000000,    // g ( N/A )
    2.000000,    // h ( N/A )
    //Modify by libin for PalmIphone (TwoPointTouchStopToFixScale) Min 50m
    10.000000,    // i
    4.000000,    // j
    2.499870,    // k
    2.000720,    // l
    2.000000,    // m
    4.000000,    // n
    4.000000    // o
};
// wqw edit 20120807 end 
//TODO: libin new add m,o, need debug
const ZFLOAT32 kZNMapScalingMin[kZNMapLevelCount] =
{
    1.00000,    // g ( N/A )
    1.00000,    // h ( N/A )
    1.00000,    // i
    1.00000,    // j
    1.00000,    // k ( N/A )
    1.00000,    // l
    1.00000,    // m
    1.00000,    // n
    1.00000     // o
};
//Modify by libin for PalmIphone >>>>>>>>>>

/*
    @abstract   
    @constant   kZNMapRenderingScale[1-4]

    const ZFLOAT32 kZNMapRenderingScale1 = 1.50;
    const ZFLOAT32 kZNMapRenderingScale2 = 1.00;
    const ZFLOAT32 kZNMapRenderingScale3 = 0.75;
    const ZFLOAT32 kZNMapRenderingScale4 = 0.50;
 */
const ZFLOAT32 kZNMapRenderingScale1 = 1.50; // 1.75
const ZFLOAT32 kZNMapRenderingScale2 = 1.00; // 1.50
const ZFLOAT32 kZNMapRenderingScale3 = 0.75; // 1.25
const ZFLOAT32 kZNMapRenderingScale4 = 0.50; // 1.00
