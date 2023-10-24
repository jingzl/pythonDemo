#include "ZNMapLineShape.h"
#include "CPStl.h "
#include "QuickFunc.h"
#include "ZNMapPolygonShape.h"
#include <WinSock.h>
#pragma comment(lib,"ws2_32")


ZNMapLineShape::ZNMapLineShape()
{
}

ZNMapLineShape::~ZNMapLineShape()
{
	safe_delete_array(mPointData1.array);

    for ( CP_VECTOR<ZNMapLine*>::iterator it=m_ZNMapLineAry.begin(); it!=m_ZNMapLineAry.end(); it++ )
    {
        ZNMapLine* lpMapLine = *it;
        if ( lpMapLine != NULL )
        {
            delete lpMapLine;
            lpMapLine = NULL;
        }
    }
    m_ZNMapLineAry.clear();
}

ZUINT32 ZNMapLineShape::skipDataBytes(const ZUINT8* dataPtr, ZUINT32 dataLength)
{
	const ZUINT8 *ptr = NULL;
	ZUINT32 remain, skipLength;
	ZUINT8 pointCount;
	ZINT32 readLength = -1;

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
		pointCount = *ptr;
		ptr ++;
		remain --;

		break_if(pointCount < 2);
		skipLength = (sizeof(ZINT8) * 2 * pointCount);
        ptr += skipLength;
		remain -= skipLength;

		break_if(dataPtr + dataLength < ptr);
		readLength = dataLength - remain;

	} while (0);

	return readLength;

}

ZINT32 ZNMapLineShape::readDataBytes(const ZUINT8* dataPtr, ZUINT32 dataLength)
{
	const ZUINT8 *ptr = NULL;
    ZINT8 *pointPtr = NULL, *tempPtr = NULL;

    ZUINT32 remain;
    ZUINT32 memLength;
    ZUINT32 pointIndex, pointCount;
    ZINT32 readLength = -1;
    ZBOOL needsFreePtr = ZTRUE;
    
    do {
        break_if(dataPtr == NULL || dataLength == 0);
        ptr = dataPtr;
        remain = dataLength;

        break_if( remain < 4 );
        mID = (ZUINT32)ntohl(*(ZUINT32*)ptr);
        ptr += 4;
        remain -= 4;

        break_if(remain < 1);
        pointCount = (ZUINT32)*ptr;
        ptr ++;
        remain --;

        break_if(pointCount < 2);
		if (pointPtr != NULL)
		{
			safe_delete_array(pointPtr);
		}

		memLength = sizeof(ZFLOAT32) * pointCount * 2;
        pointPtr = new ZINT8[pointCount * 2];
        break_if(pointPtr == NULL);
        tempPtr = pointPtr;
   
        for (pointIndex = 0; pointIndex < pointCount; pointIndex ++)
        {
            *tempPtr = (ZINT8)(*ptr);
            ptr += 1;
            tempPtr ++;
            *tempPtr = (ZINT8)(*ptr);
            ptr += 1;
            tempPtr ++;
        }

        ZNFloatSection LLPointData;
        LLPointData.array = NULL;
        LLPointData.num = 0;

        safe_delete_array(mPointData1.array);
		mPointData1.array = pointPtr;
        mPointData1.num = pointCount;
		convertStandBlockXY2LL(&mPointData1, &LLPointData, mBlockKind, mBlockX, mBlockY);

        int startIndex = 0;
        int idx = 0;
        for ( idx=2; idx<mPointData1.num; idx++ )
        {
            if ( mPointData1.array[idx*2]==mPointData1.array[(idx-1)*2] && 
                mPointData1.array[idx*2+1]==mPointData1.array[(idx-1)*2+1] )
            {
                ZNMapLine* lpLine = new ZNMapLine();
                for ( int k=0; k<(idx-startIndex); k++ )
                {
                    PZNMapPoint pLLPoint = new ZNMapPoint();
                    pLLPoint->lon = LLPointData.array[startIndex*2+2*k];
                    pLLPoint->lat = LLPointData.array[startIndex*2+2*k+1];
                    lpLine->pointary.push_back( pLLPoint );
                }
                m_ZNMapLineAry.push_back( lpLine );
                startIndex = idx + 1;
            }
        }
        if ( startIndex < (idx-1) )
        {
            ZNMapLine* lpLine = new ZNMapLine();
            for ( int k=0; k<(idx-startIndex); k++ )
            {
                PZNMapPoint pLLPoint = new ZNMapPoint();
                pLLPoint->lon = LLPointData.array[startIndex*2+2*k];
                pLLPoint->lat = LLPointData.array[startIndex*2+2*k+1];
                lpLine->pointary.push_back( pLLPoint );
            }
            m_ZNMapLineAry.push_back( lpLine );
        }

		safe_delete_array(mPointData1.array);
        safe_delete_array(LLPointData.array);
		pointPtr = NULL;

        remain -= (sizeof(ZUINT8) * 2 * pointCount);
        readLength = dataLength - remain;

	}while(0);

    return readLength;
}

//Modify by libin for line hump
#define ZNMAP_LINE_WIDTH_DIFF       (3)

