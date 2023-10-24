//
//  ZNMapPolygonShape2.m
//  ZDCNavi
//
//  Created by WangYingNa on 13/03/19.
//  Copyright 2013  All rights reserved.
//

#include "ZNMapPolygonShape.h"
#include "QuickFunc.h"

ZNMapPolygonShape3::~ZNMapPolygonShape3()
{
	mPenUpCount = 0;
	mSeparatorCount = 0;
	mPolygonCount = 0;
	safe_delete_array(mPointList.array);
	safe_delete_array(mLLPointList.array);
	safe_delete_array(mPenUpListPtr);
	safe_delete(mSeparatorDataPtr);
	safe_delete(mPolygonInfoPtr);
	safe_delete_array(mPolygonPoint.array);
	safe_delete_array(mPenUpListPtr);

	for (CP_VECTOR<ZNFloatSection*>::iterator it = mLLPolygonPoint.begin(); it != mLLPolygonPoint.end(); it++)
	{
		ZNFloatSection *floatSec = *it;
		safe_delete_array(floatSec->array);
		safe_delete(floatSec);
	}

	for (CP_VECTOR<ZNFloatSection*>::iterator it = mPenUpList.begin(); it != mPenUpList.end(); it++)
	{
		ZNFloatSection *intSec = *it;
		safe_delete_array(intSec->array);
		safe_delete(intSec);
	}
};

ZINT32 ZNMapPolygonShape3::skipDataBytes(const ZUINT8* dataPtr, ZUINT32 dataLength)
{
	const ZUINT8 *ptr = NULL;
	ZUINT32 remain;
	ZUINT32 skipLength;
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
		break_if( remain < 1 );
		count = *ptr;
		ptr ++;
		remain --;

		// Vector Point Array
		skipLength = sizeof(ZINT8) * 2 * count;
		break_if(remain < skipLength);
		ptr += skipLength;
		remain -= skipLength;

		// Pen-Up Count
		break_if(remain < 1);
		count = *ptr;
		ptr ++;
		remain --;

		// Index Array
		skipLength = sizeof(ZUINT8) * count;
		break_if(remain < skipLength);
		ptr += skipLength;
		remain -= skipLength;

		break_if(dataPtr + dataLength < ptr);
		readLength = dataLength - remain;

	} while (0);

	return readLength;
}

ZINT32 ZNMapPolygonShape3::readDataBytes(const ZUINT8* dataPtr, ZUINT32 dataLength)
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

		// Read Point Count
		break_if(remain < 1);
		pointCount = *ptr;

		break_if(pointCount == 0);
		break_if(remain < 1 + sizeof(ZINT8) * 2 * pointCount);

		didSplit = ZNMapPolygonSplitPolygon(pointCount, (const ZINT8*)(ptr + 1), &mSeparatorCount, &mSeparatorDataPtr);

		actualLength = ZNMapPolygonReadPointFromDataBytes(ptr, remain, &mPointList);
		break_if(actualLength < 0);
		ptr += actualLength;
		remain -= actualLength;

		if (didSplit)
		{
			ZNMapPolygonCreateSeparatedPolygon(mPointList.num, mPointList.array, mSeparatorCount, mSeparatorDataPtr, &mPolygonCount, &mPolygonInfoPtr, &mPolygonPoint);
		}

		actualLength = ZNMapPolygonReadPenUpFromDataBytes(ptr, remain, &mPenUpCount, &mPenUpListPtr);
		break_if(actualLength < 0);
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
        if (mPenUpCount != 0)
        {
            ZUINT32 start , end;
            start = end = 0 ;
            for (ZUINT32 index = 0; index < mPenUpCount; index++)
            {
                end = mPenUpListPtr[index];
                break_if(end == 0);
                end--;

                if (start < end)
                {
                    ZNFloatSection *llPenupPointSec = new ZNFloatSection;
                    llPenupPointSec->num =end - start + 1;
                    llPenupPointSec->array = new ZFLOAT32[llPenupPointSec->num*2];
                    memmove_s(llPenupPointSec->array, sizeof(ZFLOAT32) * llPenupPointSec->num*2,
                        mLLPointList.array +start*2, sizeof(ZFLOAT32) * llPenupPointSec->num*2);
                    mLLPolygonPoint.push_back(llPenupPointSec);
                }
                start = end + 1;
            }
        }
	}

	} while (0);

	mSeparatorCount = 0;
	safe_delete(mSeparatorDataPtr);
	safe_delete_array(llPolygonPoint.array);
	safe_delete_array(mPointList.array);
	safe_delete_array(mPolygonPoint.array);
	safe_delete_array(mPenUpListPtr);
	return readLength;
}
