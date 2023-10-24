/*
    ZNMapTypes.h
*/

#ifndef __ZNMapTypes_h__
#define __ZNMapTypes_h__
#include <string>
#include <vector>
#include "DataType.h"
using namespace  std;

/*#if defined(__cplusplus)
extern "C"
{
#endif*/
#include  "ZNGeometry.h"
#include  "ZNMapTypesCustom.h"

#define kZNMapBlockDataWidth            1020
#define kZNMapBlockDataHeight           1020

#define kZNMapBlockPixel                816
#define kZNMapBlockScaleXf              1.25
#define kZNMapBlockScaleYf              1.00
#define kZNMapBlockPixelWidth           1020
#define kZNMapBlockPixelHeight          816

#define kZNMapBlockDataWidth100         (kZNMapBlockDataWidth*100)
#define kZNMapBlockDataHeight100        (kZNMapBlockDataHeight*100)
#define kZNMapBlockScaleX100            125
#define kZNMapBlockScaleY100            100
#define kZNMapBlockPixelWidth4          (kZNMapBlockPixelWidth*4)
#define kZNMapBlockPixelHeight4         (kZNMapBlockPixelHeight*4)    

/*m/ms */
#define UNIT_Y_TW	0.0308
/*m/ms */
#define UNIT_X_TW	0.0280
	
/*
    @abstract   ZNMapTileGetInfoArray 
    @constant   kZNMapTileInfoBlockX 
    @constant   kZNMapTileInfoBlockY 
    @constant   kZNMapTileInfoBlockOrigin 
*/
extern string kZNMapTileInfoBlockX;
extern string kZNMapTileInfoBlockY;
extern string kZNMapTileInfoBlockOrigin;

/*
    @abstract  

    @constant   kZNMapBlockLongitude4 
    @constant   kZNMapBlockLatitude4   
*/
extern const ZUINT32 ZNMapBlockLongitude[kZNMapLevelCount];
extern const ZUINT32 ZNMapBlockLatitude[kZNMapLevelCount];

enum
{
    kZNMapLayerType_Auto                = 0,
    kZNMapLayerType_Daytime             = 1,
    kZNMapLayerType_Nighttime           = 2
};

enum
{
    kZNMapPartKind_Layer                = 1,
    kZNMapPartKind_Shape                = 2,
    kZNMapPartKind_Text                 = 16,
    kZNMapPartKind_End                  = 0
};

enum
{
    kZNMapLayerKind_Polygon             = 1,
    kZNMapLayerKind_Polyline            = 2,
    kZNMapLayerKind_Sign                = 3
};

enum
{
    kZNMapShapeKind_Text1               = 8,
    kZNMapShapeKind_Text2               = 9,
    kZNMapShapeKind_Text3               = 10,
    kZNMapShapeKind_Text4               = 11,
    kZNMapShapeKind_Text5               = 12,
    kZNMapShapeKind_Polygon1            = 16,
    kZNMapShapeKind_Polygon2            = 17,
    kZNMapShapeKind_Polygon3            = 18,
    kZNMapShapeKind_Polygon4            = 20,
    kZNMapShapeKind_Polygon5            = 21,
    kZNMapShapeKind_Polygon6            = 22,
    kZNMapShapeKind_Sign1               = 32,
    kZNMapShapeKind_Sign2               = 33,
    kZNMapShapeKind_Sign3               = 34,
    kZNMapShapeKind_Sign4               = 35,
    kZNMapShapeKind_LineGraph           = 81
};

/* 
    kZNMapShapeTextHorizontalFlag    
    kZNMapShapeTextBasedCenterFlag    (0:右上or左下 1:中心)
*/    
enum
{
    kZNMapShapeTextHorizontalFlag       = 1L << 7,
    kZNMapShapeTextBasedCenterFlag      = 1L << 6
};

enum
{
    kZNMapShapeSign4HorizontalFlag      = 1L << 6,
    kZNMapShapeSign4LevelFlag           = 7L << 3,
    kZNMapShapeSign4PlaceFlag           = 7L << 0
};

struct ZNMapTextLineInfo
{
    ZUINT16             mOffset;
    ZUINT8              mLength;
};
typedef struct ZNMapTextLineInfo ZNMapTextLineInfo;

struct ZNMapTextData
{
    ZUINT32             mID;
    ZUINT8              mKind;
    ZINT8               mBaseX;
    ZINT8               mBaseY;
    ZUINT8              mFlags;
    ZUINT8              mRotate;
    ZUINT8              mFontSize;
};
typedef struct ZNMapTextData ZNMapTextData;

/*
mID 存储POI的FeatCode
mNumber 存储POI的SortCode简化处理之后的值
*/
struct ZNMapSignData 
{
    ZUINT32             mID;
    ZINT8               mBaseX;
    ZINT8               mBaseY;
    ZUINT16             mNumber;
};
typedef struct ZNMapSignData ZNMapSignData;

struct ZNMapSignDataDigit
{
    ZUINT32             mID;
    ZINT8               mBaseX;
    ZINT8               mBaseY;
    ZUINT16             mNumber;
    ZUINT16             mDigit;
};
typedef struct ZNMapSignDataDigit ZNMapSignDataDigit;

struct ZNMapSignDataText
{
    ZUINT32             mID;
    ZINT8               mBaseX;
    ZINT8               mBaseY;
    ZUINT16             mNumber;
    ZUINT8              mFlags;
};
typedef struct ZNMapSignDataText ZNMapSignDataText;

struct ZNMapPolygonInfo
{
    ZUINT16            mOffset;
    ZUINT16            mCount;
};
typedef struct ZNMapPolygonInfo ZNMapPolygonInfo;

struct ZNMapSeparatorInfo
{
    ZUINT8             mStartIndex;
    ZUINT8             mEndIndex;
};
typedef struct ZNMapSeparatorInfo ZNMapSeparatorInfo;

#pragma pack ()


typedef struct _tag_ZNMapPoint
{
    ZFLOAT32 lon;
    ZFLOAT32 lat;

    _tag_ZNMapPoint():lon(0.0f),lat(0.0f)
    {
    }
    ~_tag_ZNMapPoint()
    {}

} ZNMapPoint, *PZNMapPoint;

typedef struct _tag_ZNMapLine
{
    vector<PZNMapPoint> pointary;

    _tag_ZNMapLine()
    {
    }
    ~_tag_ZNMapLine()
    {
        for ( vector<PZNMapPoint>::iterator it=pointary.begin(); it!=pointary.end(); it++ )
        {
            PZNMapPoint lpZNMapPoint = *it;
            if ( lpZNMapPoint != NULL )
            {
                delete lpZNMapPoint;
                lpZNMapPoint = NULL;
            }
        }
        pointary.clear();
    }

} ZNMapLine, *PZNMapLine;




/*#if defined(__cplusplus)
}
#endif*/

#endif /* __ZNMapTypes_h__ */