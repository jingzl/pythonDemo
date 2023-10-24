#pragma once
#include "ZNMapLineShape.h"
#include "ZNMapTypes.h"
#include "ZNMapShape.h"
#include "DataType.h"


class ZNMapLinePolygonShape : public ZNMapLineShape
{
public:
    ZNMapLinePolygonShape(char blockKind, ZUINT32 blockX, ZUINT32 blockY)
    {
        mBlockKind = blockKind;
        mBlockX = blockX;
        mBlockY = blockY;
        mPointData1.array = NULL;
        mPointData1.num = 0;
    };
    ~ZNMapLinePolygonShape(void);

public:
	ZINT32 readDataBytes(const ZUINT8* dataPtr, ZUINT32 dataLength);

};

extern ZNFloatSection*
floatPointDataFromPoints(const ZINT8 *points, ZUINT32 pointCount);

static ZNFloatSection*
polygonPointDataFromFloatPoints(ZFLOAT32 lineWidth, const ZFLOAT32 *points, ZUINT32 pointCount);

static ZNFloatSection*
polygonPointJointedDataFromPoints(ZFLOAT32 lineWidth, const ZINT8 *points, ZUINT32 pointCount);

static ZBOOL
twoPolygonPointJointedDataFromPoints(ZFLOAT32 lineWidth1, ZFLOAT32 lineWidth2, const ZINT8 *points, ZUINT32 pointCount, ZNFloatSection **outData1, ZNFloatSection **outData2);