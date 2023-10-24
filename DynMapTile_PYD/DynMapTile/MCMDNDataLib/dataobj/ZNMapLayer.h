#pragma once
#include "ZNMapDataDefine.h"
#include "CPStl.h"

#define Scale1kmDownBound (0.32) 
#define Scale1kmUpBound   (0.33) 

class ZNMapLayer
{
public:
	ZUINT8                     mNumber;
	ZUINT8                     mKind;
	ZUINT8                     mFlags;
	ZUINT32                    mShapeOffset;
	ZUINT16                    mShapeCount;

	ZNMAPDATAREP *mMapDataRep;
public:
	ZNMapLayer(void);
	~ZNMapLayer(void);
};
