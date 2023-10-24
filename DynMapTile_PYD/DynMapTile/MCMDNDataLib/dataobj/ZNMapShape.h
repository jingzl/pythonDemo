#pragma once
//#include "ZNMapLayer.h"
#include "ZNMapTypes.h"
#include "DataType.h"

class ZNMapShape
{
public:

public:
	ZNMapShape();
	~ZNMapShape();
};

class ZNMapTextureShape:public ZNMapShape
{
public:
	ZBOOL decode();
public:
	ZNMapTextureShape();
	~ZNMapTextureShape();
};

extern ZNFloatSection* patternPointsForKind(ZUINT8 kind, const ZFLOAT32* points, ZUINT32 pointCount);
extern ZNFloatSection* patternPoints(const ZFLOAT32 *points, ZUINT32 pointCount, const ZFLOAT32 *pattern, ZUINT32 patternCount);