/*
 *  ZNTextureTypes.h
 *  ZDCNavi
 *
 *  Created by Satoshi Muraki on 09/04/18.
 *  Copyright 2009 SONORAN BLUE. All rights reserved.
 *
 */
/*
    ZNTextureTypes.h
    
    テクスチャの表示に利用される構造体の定義
*/
#include "DataType.h"
/*
    @struct     ZNTexturePosition
    @abstract   テクスチャのビットマップに描画されているイメージの矩形情報
    @member     vertices[4][2]
    @member     coords[4][2]
*/
struct ZNTexturePosition
{
    ZFLOAT32   vertices[4][2];
    ZFLOAT32   coords[4][2];
};
typedef struct ZNTexturePosition ZNTexturePosition;



