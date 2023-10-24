//
//  ZNMapPolygonShape2.m
//  ZDCNavi
//
//  Created by WangYingNa on 13/03/19.
//  Copyright 2013  All rights reserved.
//

#include "ZNMapPolygonShape.h"
#include "QuickFunc.h"


ZNMapPolygonShape4::~ZNMapPolygonShape4()
{
	mSeparatorCount = 0;
	safe_delete_array(mSeparatorDataPtr);
	safe_delete_array(mPolygonInfoPtr);
	safe_delete_array(mPolygonPoint.array);

	for (CP_VECTOR<ZNFloatSection*>::iterator it = mLLPolygonPoint.begin(); it != mLLPolygonPoint.end(); it++)
	{
		ZNFloatSection *floatSec = *it;
		safe_delete_array(floatSec->array);
		safe_delete(floatSec);
	}
}

ZINT32 ZNMapPolygonShape4::skipDataBytes(const ZUINT8* dataPtr, ZUINT32 dataLength)
{
	const ZUINT8 *ptr = NULL;
	ZUINT32 remain;
	ZINT32 skipLength;
	ZINT32 readLength = -1;
	ZUINT8 count;

	do {
		break_if(dataPtr == NULL);
		break_if(dataLength == 0);
		ptr = dataPtr;
		remain = dataLength;

        // ID
        break_if( remain < 4 );
        ptr += 4;
        remain -= 4;

		// Point Count
		break_if(remain < 1);
		count = *ptr;
		ptr ++;
		remain --;

		// Skip Point Array
		skipLength = sizeof(ZINT8) * 2 * count;
		break_if(remain < skipLength);
		ptr += skipLength;
		remain -= skipLength;

		// Separator Count
		break_if(remain < 1);
		count = *ptr;
		ptr ++;
		remain --;

		// Skip Separator Info Array
		skipLength = sizeof(ZINT8) * 2 * count;
		break_if(remain < skipLength);
		ptr += skipLength;
		remain -= skipLength;

		break_if(dataPtr + dataLength < ptr);
		readLength = dataLength - remain;

	} while (0);

	return readLength;
}

ZINT32 ZNMapPolygonShape4::readDataBytes(const ZUINT8* dataPtr, ZUINT32 dataLength)
{
	const ZUINT8 *ptr = NULL;
	//Modify by libin for PalmIphone
	const ZINT8 *pointPtr = NULL;
	ZUINT8 pointCount;
	ZUINT32 remain;
	ZINT32 actualLength;
	ZINT32 readLength = -1;
	ZNFloatSection llPolygonPoint;

	do {
		break_if(dataPtr == NULL || dataLength == 0);

		ptr = dataPtr;
		remain = dataLength;

		break_if(remain < 1);
		pointCount = *ptr;
		ptr ++;
		remain --;

		//Modify by libin for PalmIphone
		pointPtr = (const ZINT8*)ptr;
		actualLength = sizeof(ZINT16) * 2 * pointCount;
		break_if(remain < actualLength);
		ptr += actualLength;
		remain -= actualLength;

		actualLength = ZNMapPolygonReadSeparatorFromDataBytes(ptr, remain, &mSeparatorCount, &mSeparatorDataPtr);
		break_if(actualLength < 0);
		ptr += actualLength;
		remain -= actualLength;

		//Modify by libin for PalmIphone
		break_if(!ZNMapPolygonCreateSeparatedPolygon8(pointCount, pointPtr, mSeparatorCount, mSeparatorDataPtr, &mPolygonCount, &mPolygonInfoPtr, &mPolygonPoint));

		break_if(dataPtr + dataLength < ptr);
		readLength = dataLength - remain;

		if (mPolygonPoint.num > 0)
		{
			llPolygonPoint.array = NULL;
			convertStandBlockXY2LL(&mPolygonPoint, &llPolygonPoint, mBlockKind, mBlockX, mBlockY);
            for (ZUINT32 index = 0; index < mPolygonCount; index ++)
            {
                ZNFloatSection *llPolygonPointSec = new ZNFloatSection;
                llPolygonPointSec->num = mPolygonInfoPtr[index].mCount;
                llPolygonPointSec->array = new ZFLOAT32[mPolygonInfoPtr[index].mCount*2];
                memmove_s(llPolygonPointSec->array, sizeof(ZFLOAT32) * mPolygonInfoPtr[index].mCount*2,
                    llPolygonPoint.array + mPolygonInfoPtr[index].mOffset*2, sizeof(ZFLOAT32) * mPolygonInfoPtr[index].mCount*2);
                mLLPolygonPoint.push_back(llPolygonPointSec);
            }
		}

	
	} while (0);

	safe_delete_array(llPolygonPoint.array);
	safe_delete_array(mPolygonPoint.array);
	return readLength;
}

