#include "ZNMapSignTextShape.h"
#include "ZNTextureTypes.h"
#include <WinSock.h>
#include "QuickFunc.h"
#include "ZNMapUtility.h"
#pragma comment(lib,"ws2_32")


ZNMapSignTextShape::ZNMapSignTextShape()
{
    mFlag = 0;
	bIsDigit = ZTRUE;
}

ZNMapSignTextShape::~ZNMapSignTextShape()
{
    m_textArray.clear();
}

ZINT32 ZNMapSignTextShape::readDataBytes2(const ZUINT8*dataBytes, ZUINT32 dataLength, wstring& text)
{
	const ZUINT8 *ptr = NULL;
	ZUINT32 remain;
	ZINT32 readLength = -1;

	do {
		break_if(dataBytes == NULL);
		break_if(dataLength == 0);
		ptr = dataBytes;
		remain = dataLength;

        break_if( remain < 4 );
        m_mapSignDigitData.mID = (ZUINT32)ntohl(*(ZUINT32*)ptr);
        ptr += 4;
        remain -= 4;

		break_if(remain < 1);
		m_mapSignDigitData.mBaseX = *ptr;
		ptr ++;
		remain --;

		break_if(remain < 1);
		m_mapSignDigitData.mBaseY = *ptr;
		ptr ++;
		remain --;

		break_if(remain < 2);
		m_mapSignDigitData.mNumber = ntohs(*(ZUINT16*)ptr);
		ptr += 2;
		remain -= 2;

		break_if(remain < 2);
		m_mapSignDigitData.mDigit = ntohs(*(ZUINT16*)ptr);
		ptr += 2;
		remain -= 2;

		break_if(dataBytes + dataLength < ptr);
		readLength = dataLength - remain;

	} while (0);

	mPoint.x = m_mapSignDigitData.mBaseX;
	mPoint.y = m_mapSignDigitData.mBaseY;
	

	ZINT32 minX, minY, maxX, maxY; 
	ZUINT8 level = ZNMapLevelFromKind(mBlockKind);
	ZNMapGetLLOfBlockRect(level, mBlockX, mBlockY, minX, minY, maxX, maxY);
	mLLPoint.x = (((mPoint.x + 128) *(ZNMapBlockLongitude[level]/4.0))/255.00  + minX)/3600000.0;
	mLLPoint.y = (((mPoint.y + 128) *(ZNMapBlockLatitude[level]/4.0))/255.00  + minY)/3600000.0;
	return readLength;
}

ZINT32 ZNMapSignTextShape::readDataBytes3(const ZUINT8*dataBytes, ZUINT32 dataLength, wstring& text)
{
	ZNMapTextLineInfo textLineInfo;
	const ZUINT8 *ptr = NULL;
	ZUINT32 remain;
	ZINT32 readLength = -1;

	do {
		bIsDigit = FALSE;
		break_if(dataBytes == NULL || dataLength == 0);
		ptr = dataBytes;
		remain = dataLength;

        break_if( remain < 4 );
        m_mapSignTextData.mID = (ZUINT32)ntohl(*(ZUINT32*)ptr);
        ptr += 4;
        remain -= 4;

		break_if(remain < 1);
		m_mapSignTextData.mBaseX = *ptr;
		ptr ++;
		remain --;

		break_if(remain < 1);
		m_mapSignTextData.mBaseY = *ptr;
		ptr ++;
		remain --;

		break_if(remain < 2);
		m_mapSignTextData.mNumber = ntohs(*(ZUINT16*)ptr);
		ptr += 2;
		remain -= 2;

		break_if(remain < 1);
		m_mapSignTextData.mFlags = *ptr;
		ptr ++;
		remain --;

		break_if(remain < 2);
		textLineInfo.mOffset = ntohs(*(ZUINT16*)ptr);
		ptr += 2;
		remain -= 2;

		break_if(remain < 1);
		textLineInfo.mLength = *ptr;
		ptr ++;
		remain --;

		break_if(text.length() < (ZUINT32)textLineInfo.mOffset + (ZUINT32)textLineInfo.mLength);
		wstring textString = text.substr(textLineInfo.mOffset, textLineInfo.mLength);
		m_textArray.push_back(textString);
		break_if(m_textArray.size() < 1);

		break_if(dataBytes + dataLength < ptr);
		readLength = dataLength - remain;

	} while (0);

	mPoint.x = m_mapSignTextData.mBaseX;
	mPoint.y = m_mapSignTextData.mBaseY;
	ZINT32 minX, minY, maxX, maxY; 
	ZUINT8 level = ZNMapLevelFromKind(mBlockKind);
	ZNMapGetLLOfBlockRect(level, mBlockX, mBlockY, minX, minY, maxX, maxY);
	mLLPoint.x = (((mPoint.x + 128) *(ZNMapBlockLongitude[level]/4.0))/255.00  + minX)/3600000.0;
	mLLPoint.y = (((mPoint.y + 128) *(ZNMapBlockLatitude[level]/4.0))/255.00  + minY)/3600000.0;
	return readLength;
}

ZINT32 ZNMapSignTextShape::readDataBytes4(const ZUINT8*dataBytes, ZUINT32 dataLength, wstring& text)
{
	ZNMapTextLineInfo textLineInfo;
	const ZUINT8 *ptr = NULL;
	ZUINT32 remain;
	ZINT32 readLength = -1;
	ZUINT8 lineCount;

	do {
		bIsDigit = FALSE;
		break_if(dataBytes == NULL || dataLength == 0);
		ptr = dataBytes;
		remain = dataLength;

        break_if( remain < 4 );
        m_mapSignTextData.mID = (ZUINT32)ntohl(*(ZUINT32*)ptr);
        ptr += 4;
        remain -= 4;

		break_if(remain < 1);
		m_mapSignTextData.mBaseX = *ptr;
		ptr ++;
		remain --;

		break_if(remain < 1);
		m_mapSignTextData.mBaseY = *ptr;
		ptr ++;
		remain --;

		break_if(remain < 2);
		m_mapSignTextData.mNumber = ntohs(*(ZUINT16*)ptr); 
		ptr += 2;
		remain -= 2;

		break_if(remain < 1);
		m_mapSignTextData.mFlags = *ptr;
		ptr ++;
		remain --;

		break_if(remain < 1);
		lineCount = *ptr;
		ptr ++;
		remain --;

		while (lineCount --)
		{
			break_if(remain < 2);
			textLineInfo.mOffset = ntohs(*(ZUINT16*)ptr);
			ptr += 2;
			remain -= 2;

			break_if(remain < 1);
			textLineInfo.mLength = *ptr;
			ptr ++;
			remain --;

			break_if(text.length() < (ZUINT32)textLineInfo.mOffset + (ZUINT32)textLineInfo.mLength);
			wstring textString = text.substr(textLineInfo.mOffset, textLineInfo.mLength);
			m_textArray.push_back(textString);
			break_if(m_textArray.size() < 1);
		}

		readLength = dataLength - remain;

	} while (0);

	mPoint.x = m_mapSignTextData.mBaseX;
	mPoint.y = m_mapSignTextData.mBaseY;
	ZINT32 minX, minY, maxX, maxY; 
	ZUINT8 level = ZNMapLevelFromKind(mBlockKind);
	ZNMapGetLLOfBlockRect(level, mBlockX, mBlockY, minX, minY, maxX, maxY);
	mLLPoint.x = (((mPoint.x + 128) *(ZNMapBlockLongitude[level]/4.0))/255.00  + minX)/3600000.0;
	mLLPoint.y = (((mPoint.y + 128) *(ZNMapBlockLatitude[level]/4.0))/255.00  + minY)/3600000.0;
	return readLength;
}
