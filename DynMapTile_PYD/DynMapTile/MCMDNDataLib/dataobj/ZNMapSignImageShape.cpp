#include "ZNMapSignImageShape.h"
#include "QuickFunc.h"
#include <WinSock.h>
#include "ZNMapUtility.h"
#pragma comment(lib,"ws2_32")



ZNMapSignImageShape::ZNMapSignImageShape(void)
{
};

ZNMapSignImageShape::~ZNMapSignImageShape(void)
{
}

ZINT32 ZNMapSignImageShape::readDataBytes1(const ZUINT8* dataBytes, ZUINT32 dataLength)
{
	const ZUINT8 *ptr = NULL;
	ZUINT32 remain = 0;
	ZINT32 readLength = -1;
	

	do {
		break_if(dataBytes == NULL);
		break_if(dataLength == 0);
		ptr = dataBytes;
		remain = dataLength;

        // ID
        break_if( remain < 4 );
        mSignData.mID = (ZUINT32)ntohl(*(ZUINT32*)ptr);
        ptr += 4;
        remain -= 4;

		break_if( remain < 1 );
		mSignData.mBaseX = *ptr;
		ptr += 1;
		remain -= 1;

		break_if( remain < 1 );
		mSignData.mBaseY = *ptr;
		ptr += 1;
		remain -= 1;

		break_if(remain < 2);
		mSignData.mNumber = (ZUINT16)ntohs(*(ZUINT16*)ptr);
        ptr += 2;
        remain -= 2;

		mCenterOffset.x = mSignData.mBaseX;
		mCenterOffset.y = mSignData.mBaseY;

		ZINT32 minX, minY, maxX, maxY; 
		ZUINT8 level = ZNMapLevelFromKind(mBlockKind);
		ZNMapGetLLOfBlockRect(level, mBlockX, mBlockY, minX, minY, maxX, maxY);
		mLLCenterOffset.x = (((mCenterOffset.x + 128) *(ZNMapBlockLongitude[level]/4.0))/255.00  + minX)/3600000.0;
		mLLCenterOffset.y = (((mCenterOffset.y + 128) *(ZNMapBlockLatitude[level]/4.0))/255.00  + minY)/3600000.0;

		break_if(dataBytes + dataLength < ptr);
		readLength = dataLength - remain;

	} while (0);

	return readLength;
}
