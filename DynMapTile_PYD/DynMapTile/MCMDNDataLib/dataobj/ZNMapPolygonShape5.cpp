//
//  ZNMapPolygonShape2.m
//  ZDCNavi
//
//  Created by WangYingNa on 13/03/19.
//  Copyright 2013  All rights reserved.
//

//
//  ZNMapPolygonShape5.m
//  ZDCNavi
//
//  Created by Satoshi Muraki on 08/08/05.
//  Copyright 2008 SONORAN BLUE. All rights reserved.
//

#include "ZNMapPolygonShape.h"
#include "QuickFunc.h"

ZNMapPolygonShape5::~ZNMapPolygonShape5()
{
	//mPointCount = 0;
	safe_delete_array(mPointList.array);
	safe_delete_array(mLLPointList.array);
	safe_delete_array(mPolygonPoint.array);
	safe_delete_array(mPolygonInfoPtr);
	safe_delete_array(mSeparatorDataPtr);

	mSeparatorCount = 0;
	mPolygonCount = 0;
	
	for (CP_VECTOR<ZNFloatSection*>::iterator it = mLLPolygonPoint.begin(); it != mLLPolygonPoint.end(); it++)
	{
		ZNFloatSection *floatSec = *it;
		safe_delete_array(floatSec->array);
		safe_delete(floatSec);
	}
}

ZINT32 ZNMapPolygonShape5::skipDataBytes(const ZUINT8* dataPtr, ZUINT32 dataLength)
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

		// Read Point Count
		break_if(remain < 1);
		count = *ptr;
		ptr ++;
		remain --;

		// Skip Point Array
		skipLength = sizeof(ZINT8) * 2 * count;
		break_if(remain < skipLength);
		ptr += skipLength;
		remain -= skipLength;

		// Read Separator Count
		break_if(remain < 1);
		count = *ptr;
		ptr ++;
		remain --;

		// Skip Separator Array
		skipLength = sizeof(ZINT8) * 2 * count;
		break_if(remain < skipLength);
		ptr += skipLength;
		remain -= skipLength;

		break_if(dataPtr + dataLength < ptr);
		readLength = dataLength - remain;

	} while (0);

	return readLength;
}

ZINT32 ZNMapPolygonShape5::readDataBytes(const ZUINT8* dataPtr, ZUINT32 dataLength)
{

	const ZUINT8 *ptr = NULL;
	ZUINT32 remain;
	ZINT32 actualLength;
	ZINT32 readLength = -1;
	ZNFloatSection llPolygonPoint;

	do {
		break_if(dataPtr == NULL || dataLength == 0);

		ptr = dataPtr;
		remain = dataLength;

		actualLength = ZNMapPolygonReadPointFromDataBytes(ptr, remain, &mPointList);
		break_if(actualLength < 0);
		ptr += actualLength;
		remain -= actualLength;

		actualLength = ZNMapPolygonReadSeparatorFromDataBytes(ptr, remain, &mSeparatorCount, &mSeparatorDataPtr);
		break_if(actualLength < 0);
		ptr += actualLength;
		remain -= actualLength;

		break_if(!ZNMapPolygonCreateSeparatedPolygon(
			mPointList.num,
			mPointList.array,
			mSeparatorCount,
			mSeparatorDataPtr,
			&mPolygonCount,
			&mPolygonInfoPtr,
			&mPolygonPoint));

		break_if(dataPtr + dataLength < ptr);
		readLength = dataLength - remain;

        if (mPointList.num == 0)
        {
            llPolygonPoint.array = NULL;
            llPolygonPoint.num = 0;
            convertStandBlockXY2LL(&mPolygonPoint, &llPolygonPoint, mBlockKind, mBlockX, mBlockY);
            for (ZUINT32 index = 0; index < mPolygonCount; index ++)
            {
                ZNFloatSection *llPolygonPointSec = new ZNFloatSection;
                llPolygonPointSec->num = (mPolygonInfoPtr[index]).mCount;
                llPolygonPointSec->array = new ZFLOAT32[(mPolygonInfoPtr[index]).mCount*2];
                memmove_s(llPolygonPointSec->array, sizeof(ZFLOAT32) * mPolygonInfoPtr[index].mCount*2,
                    llPolygonPoint.array + mPolygonInfoPtr[index].mOffset*2, sizeof(ZFLOAT32) * mPolygonInfoPtr[index].mCount*2);
                mLLPolygonPoint.push_back(llPolygonPointSec);
            }
        }else{
            convertStandBlockXY2LL(&mPointList, &mLLPointList, mBlockKind, mBlockX, mBlockY);
        }	
	} while (0);

	safe_delete_array(llPolygonPoint.array);
	safe_delete_array(mPointList.array);
	safe_delete_array(mPolygonPoint.array);
	return readLength;
}
