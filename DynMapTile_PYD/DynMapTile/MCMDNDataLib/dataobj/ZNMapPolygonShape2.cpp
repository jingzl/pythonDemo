//
//  ZNMapPolygonShape2.cpp
//  ZDCNavi
//
//  Created by WangYingNa on 13/03/19.
//  Copyright 2013  All rights reserved.
//

#include "ZNMapPolygonShape.h"
#include "QuickFunc.h"


ZNMapPolygonShape2::~ZNMapPolygonShape2()
{
	mSeparatorCount = 0;
	mPolygonCount = 0;
	safe_delete_array(mPolygonInfoPtr);
	safe_delete_array(mSeparatorDataPtr);

	safe_delete_array(mPolygonPoint.array);
	safe_delete_array(mPointList.array);
	safe_delete_array(mLLPointList.array);

	for (CP_VECTOR<ZNFloatSection*>::iterator it = mLLPolygonPoint.begin(); it != mLLPolygonPoint.end(); it++)
	{
		ZNFloatSection *floatSec = *it;
		safe_delete_array(floatSec->array);
		safe_delete(floatSec);
	}
}

ZINT32 ZNMapPolygonShape2::skipDataBytes(const ZUINT8* dataPtr, ZUINT32 dataLength)
{
	const ZUINT8 *ptr = NULL;
	ZUINT32 remain;
	ZUINT32 skipLength;
	ZINT32 readLength = -1;
	ZUINT8 pointCount;

	do {
		break_if(dataPtr == NULL);
		break_if(dataLength == 0);
		ptr = dataPtr;
		remain = dataLength;

        break_if( remain < 4 );
        ptr += 4;
        remain -= 4;

		break_if(remain < 1);
		pointCount = *ptr;
		ptr ++;
		remain --;

		skipLength = sizeof(ZINT8) * 2 * pointCount;
		break_if(remain < skipLength);
        ptr += skipLength;
		remain -= skipLength;

		break_if(dataPtr + dataLength < ptr);
		readLength = dataLength - remain;

	} while (0);

	return readLength;
}

ZINT32 ZNMapPolygonShape2::readDataBytes(const ZUINT8* dataPtr, ZUINT32 dataLength)
{
	const ZUINT8 *ptr = NULL;
	ZUINT32 remain;
	ZINT32 actualLength;
	ZINT32 readLength = -1;
	ZUINT8 pointCount;
	ZBOOL didSplit = ZFALSE;
	ZNFloatSection llPolygonPoint;

	do {
		break_if(dataPtr == NULL);
		break_if(dataLength == 0);
		ptr = dataPtr;
		remain = dataLength;

		break_if(remain < 1);
		pointCount = *ptr;

		break_if(remain < (sizeof(ZINT8) * 2 * pointCount) + 1);
		didSplit = ZNMapPolygonSplitPolygon(pointCount, (const ZINT8*)(ptr + 1), &mSeparatorCount, &mSeparatorDataPtr);

		if (didSplit && 0 < mSeparatorCount)
		{
			ptr ++;
			remain --;

			break_if(!ZNMapPolygonCreateSeparatedPolygon8(pointCount, (const ZINT8*)ptr, mSeparatorCount, mSeparatorDataPtr, 
				&mPolygonCount, &mPolygonInfoPtr, &mPolygonPoint));

			actualLength = sizeof(ZINT8) * 2 * pointCount;
		}
		else
		{
			actualLength = ZNMapPolygonReadPointFromDataBytes(ptr, remain, &mPointList);
			break_if(actualLength < 0);
		}

		ptr += actualLength;
		remain -= actualLength;

		break_if(dataPtr + dataLength < ptr);
		readLength = dataLength - remain;

		if (mPointList.num == 0)
		{
			llPolygonPoint.array = NULL;
			convertStandBlockXY2LL(&mPolygonPoint, &llPolygonPoint, mBlockKind, mBlockX, mBlockY);
            if (mPolygonPoint.num > 0)
            {
                for (ZUINT32 index = 0; index < mPolygonCount; index ++)
                {
                    ZNFloatSection *llPolygonPointSec = new ZNFloatSection;
                    llPolygonPointSec->array = new ZFLOAT32[mPolygonInfoPtr[index].mCount*2];
                    memmove_s(llPolygonPointSec->array, sizeof(ZFLOAT32) * mPolygonInfoPtr[index].mCount*2,
                        llPolygonPoint.array + mPolygonInfoPtr[index].mOffset*2, sizeof(ZFLOAT32) * mPolygonInfoPtr[index].mCount*2);
                    llPolygonPointSec->num = mPolygonInfoPtr[index].mCount;
                    mLLPolygonPoint.push_back(llPolygonPointSec);
                }
            }
		}else
		{
			convertStandBlockXY2LL(&mPointList, &mLLPointList, mBlockKind, mBlockX, mBlockY);
		}

		//如果mPointList.num大于0，forecolor和bordercolor都要画一遍, 数据一样

	} while (0);

	mSeparatorCount = 0;
	safe_delete_array(mSeparatorDataPtr);
	safe_delete_array(mPolygonPoint.array);
	safe_delete_array(mPointList.array);
	safe_delete_array(llPolygonPoint.array);
	return readLength;
}

