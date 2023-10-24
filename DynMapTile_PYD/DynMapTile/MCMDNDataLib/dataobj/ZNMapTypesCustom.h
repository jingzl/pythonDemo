//
//  ZNMapTypesCustom.h
//  ZDCNavi2
//
//  Created by Satoshi Muraki on 09/06/12.
//  Copyright 2009 SONORAN BLUE. All rights reserved.
//

//Modify by libin for PalmIphone <<<<<<<<<<
/*
    @enum      
*/
#pragma once
#include "DataType.h"

enum
{
    kZNMapKind_g                        = 0x67,     // 
    kZNMapKind_i                        = 0x69,     // 
    kZNMapKind_j                        = 0x6a,     // 
    kZNMapKind_k                        = 0x6b,     // 
    kZNMapKind_l                        = 0x6c,     // 
    kZNMapKind_m                        = 0x6d,     //
    kZNMapKind_n                        = 0x6e,     // 
    kZNMapKind_o                        = 0x6f,     //
    kZNMapKind_p                        = 0x70,
    kZNMapKind_q                        = 0x71
};

//Modify by libin for PalmIphone
/*
    @enum    
*/
enum 
{
    kZNMapLevel_g                       = 0,        //
    kZNMapLevel_i                       = 1,        // 
    kZNMapLevel_j                       = 2,        // 
    kZNMapLevel_k                       = 3,        // 
    kZNMapLevel_l                       = 4,        // 
    kZNMapLevel_m                       = 5,        //
    kZNMapLevel_n                       = 6,        // 
    kZNMapLevel_o                       = 7,        //
    kZNMapLevel_p                       = 8,
    kZNMapLevel_q                       = 9
};
#define kZNMapLevelCount                10           // 
//Modify by libin for PalmIphone >>>>>>>>>>

/*
    @abstract
    @constant   kZNMapLevelValidation
    @constant   kZNMapScalingMax
    @constant   kZNMapScalingMin
*/
extern const bool  kZNMapLevelValidation[kZNMapLevelCount];
extern const ZFLOAT32 kZNMapScalingMax[kZNMapLevelCount];
extern const ZFLOAT32 kZNMapScalingMin[kZNMapLevelCount];

/*
    @abstract  
    @constant   kZNMapZoomScale
*/
//Modify by libin for PalmIphone (PalmCity Android Scale)
#define kZNMapZoomScaleCount            9
extern const ZFLOAT32 kZNMapZoomScale[kZNMapZoomScaleCount];

/*
    @abstract 
    @constant   kZNMapRenderingScale[1-4]
 */
extern const ZFLOAT32 kZNMapRenderingScale1;
extern const ZFLOAT32 kZNMapRenderingScale2;
extern const ZFLOAT32 kZNMapRenderingScale3;
extern const ZFLOAT32 kZNMapRenderingScale4;

/*
    @abstract   ZNTextureGroup
    @constant   kZNMapTextureTextWidth
    @constant   kZNMapTextureTextHeight
    @constant   kZNMapTextureSignWidth
    @constant   kZNMapTextureSignWidth
 */
#define kZNMapTextureTextWidth          512.0
#define kZNMapTextureTextHeight         64.0
#define kZNMapTextureSignWidth          256.0
#define kZNMapTextureSignHeight         128.0

/*
    @abstract  
    @constant   
*/
#define kZNMapDrawBackgroundLineWidthExtend     1.0
