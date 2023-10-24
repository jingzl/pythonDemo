#include "ZNMapTextShape.h"
/*#include "ZNTextureGroup.h"
#include "ZNMapSignManager.h"
#include "ZNTextureSymbol.h"
#include "ZNMapLayer.h"*/
#include "QuickFunc.h"
#include "ZNMapUtility.h"
#include <winsock.h>

#define kTextListKey		"s"
#define  kTextDrawInfoKey	"i"
#define  kTextDataKey	"d"

#define ZNMAP_INVALID_SID	(0xFFFF)  //removed further.it should be in ZNMapSignManager .WitchNaNa

ZNMapTextShape::ZNMapTextShape(void)
{
}

ZNMapTextShape::~ZNMapTextShape(void)
{
    m_textArray.clear();
    m_textLineInfoArray.clear();
}

ZINT32 ZNMapTextShape::skipDataBytes(const ZUINT8* dataPtr, ZUINT32 dataLength, ZUINT8 kind)
{
	const ZUINT8 *ptr = NULL;
	ZUINT32 remain;
	ZINT32 readLength = -1;
	ZINT32 skipLength;
	ZUINT8 lineCount;

	do {
		break_if(dataPtr == NULL);
		break_if(dataLength == 0);

		ptr = dataPtr;
		remain = dataLength;

		if (kind == kZNMapShapeKind_Text1)
		{
			skipLength = 9;
			break_if(remain < skipLength);
			ptr += skipLength;
			remain -= skipLength;
		}
		else if (kind == kZNMapShapeKind_Text2)
		{
			skipLength = 10;
			break_if(remain < skipLength);
			ptr += skipLength;
			remain -= skipLength;
		}
		else if (kind == kZNMapShapeKind_Text3)
		{
			skipLength = 9;
			break_if(remain < skipLength);
			ptr += skipLength;
			remain -= skipLength;

			break_if(remain < 1);
			lineCount = *(ZUINT8*)ptr;
			ptr ++;
			remain --;

			skipLength = 3 * lineCount;
			break_if(remain < skipLength);
			ptr += skipLength;
			remain -= skipLength;
		}
		else if (kind == kZNMapShapeKind_Text4)
		{
			skipLength = 9;
			break_if(remain < skipLength);
			ptr += skipLength;
			remain -= skipLength;

			break_if(remain < 1);
			lineCount = *(ZUINT8*)ptr;
			ptr ++;
			remain --;

			skipLength = 3 * lineCount;
			break_if(remain < skipLength);
			ptr += skipLength;
			remain -= skipLength;
		}
		else if (kind == kZNMapShapeKind_Text5)
		{
			skipLength = 7;
			break_if(remain < skipLength);
			ptr += skipLength;
			remain -= skipLength;

			break_if(remain < 1);
			lineCount = *(ZUINT8*)ptr;
			ptr ++;
			remain --;

			skipLength = 3 * lineCount;
			break_if(remain < skipLength);
			ptr += skipLength;
			remain -= skipLength;
		}
		else
		{
			break;
		}

		break_if(dataPtr + dataLength < ptr);
		readLength = dataLength - remain;

	} while (0);

	return readLength;
}

ZINT32 ZNMapTextShape::readDataBytes(const ZUINT8* dataPtr, ZUINT32 dataLength, 
									 ZUINT8 kind, wstring& displayText)
{
	wstring textString;
	ZNMapTextLineInfo textLineInfo;
	ZUINT8 lineIndex = 0;
    ZUINT8 lineCount = 0;
	ZINT32 readLength = -1;
	ZUINT32 remain = 0;
	const ZUINT8 *ptr = NULL;

	do {
		break_if(dataPtr == NULL);
		break_if(dataLength == 0);
		break_if(displayText.size()<=0);
		break_if(m_textArray.size() < 0);

		ptr = dataPtr;
		remain = dataLength;

        m_textData.mKind = kind;
		if ( kind == kZNMapShapeKind_Text1 )
		{
            break_if( remain < 4 );
            m_textData.mID = (ZUINT32)ntohl(*(ZUINT32*)ptr);
            ptr += 4;
            remain -= 4;

			break_if( remain < 1 );
			m_textData.mBaseX = *ptr;
			ptr += 1;
			remain -= 1;

			break_if( remain < 1 );
			m_textData.mBaseY = *ptr;
			ptr += 1;
			remain -= 1;

			break_if(remain < 2);
			textLineInfo.mOffset = ntohs(*(ZUINT16*)ptr);
			ptr += 2;
			remain -= 2;

			break_if(remain < 1);
			textLineInfo.mLength = *ptr;
			ptr++;
			remain--;

			break_if(displayText.length() < (ZUINT32)textLineInfo.mOffset + (ZUINT32)textLineInfo.mLength);

			textString = displayText.substr(textLineInfo.mOffset, textLineInfo.mLength);
			m_textArray.push_back(textString);
		}
		else if ( kind == kZNMapShapeKind_Text2 )
		{
            break_if( remain < 4 );
            m_textData.mID = (ZUINT32)ntohl(*(ZUINT32*)ptr);
            ptr += 4;
            remain -= 4;

			break_if( remain < 1 );
			m_textData.mBaseX = *ptr;
			ptr += 1;
			remain -= 1;

			break_if( remain < 1 );
			m_textData.mBaseY = *ptr;
			ptr += 1;
			remain -= 1;

			break_if(remain < 1);
			m_textData.mFlags = *ptr;
			ptr ++;
			remain --;

			break_if(remain < 2);
			textLineInfo.mOffset = (ZUINT16)ntohs(*(ZUINT16*)ptr);
			ptr += 2;
			remain -= 2;

			break_if(remain < 1);
			textLineInfo.mLength = *(ZUINT8*)ptr;
			ptr ++;
			remain --;

			// Check the text legnth
			break_if(displayText.length() < (ZUINT32)textLineInfo.mOffset + (ZUINT32)textLineInfo.mLength);
			textString = displayText.substr(textLineInfo.mOffset, textLineInfo.mLength);
            // 2
			m_displayFlag = (m_textData.mFlags >> 3) & 0x07;
			m_textArray.push_back(textString);
		}
		else if ( kind == kZNMapShapeKind_Text3 )
		{
            break_if( remain < 4 );
            m_textData.mID = (ZUINT32)ntohl(*(ZUINT32*)ptr);
            ptr += 4;
            remain -= 4;

			break_if( remain < 1 );
			m_textData.mBaseX = *ptr;
			ptr += 1;
			remain -= 1;

			break_if( remain < 1 );
			m_textData.mBaseY = *ptr;
			ptr += 1;
			remain -= 1;

			break_if( remain < 1 );
			m_textData.mFlags = *(ZUINT8*)ptr;
			ptr ++;
			remain --;

			break_if( remain < 1 );
			m_textData.mRotate = *(ZUINT8*)ptr;
			ptr ++;
			remain --;

			break_if(remain < 1);
			m_textData.mFontSize = *(ZUINT8*)ptr;
			ptr ++;
			remain --;

			break_if(remain < 1);
			lineCount = *(ZUINT8*)ptr;
			ptr ++;
			remain --;

			for ( lineIndex = 0; lineIndex < lineCount; lineIndex++ )
			{
				textLineInfo.mOffset = ntohs(*(ZUINT16*)ptr);
				ptr += 2;
				remain -= 2;

				textLineInfo.mLength = *(ZUINT8*)ptr;
				ptr ++;
				remain --;

				// Check the text legnth
				break_if(displayText.length() < (ZUINT32)textLineInfo.mOffset + (ZUINT32)textLineInfo.mLength);

				textString = displayText.substr(textLineInfo.mOffset, textLineInfo.mLength);
                m_textArray.push_back(textString);
			}

			// 3
			m_displayFlag = (m_textData.mFlags >> 3) & 0x07;
		}
		else if ( kind == kZNMapShapeKind_Text4 )
		{
            break_if( remain < 4 );
            m_textData.mID = (ZUINT32)ntohl(*(ZUINT32*)ptr);
            ptr += 4;
            remain -= 4;

			break_if( remain < 1 );
			m_textData.mBaseX = *ptr;
			ptr += 1;
			remain -= 1;

			break_if( remain < 1 );
			m_textData.mBaseY = *ptr;
			ptr += 1;
			remain -= 1;

			break_if(remain < 1);
			m_textData.mFlags = *(ZUINT8*)ptr;
			ptr ++;
			remain --;

			break_if(remain < 1);
			m_textData.mRotate = *(ZUINT8*)ptr;
			ptr ++;
			remain --;

			break_if(remain < 1);
			m_textData.mFontSize = *(ZUINT8*)ptr;
			ptr ++;
			remain --;

			break_if(remain < 1);
			lineCount = *(ZUINT8*)ptr;
			ptr ++;
			remain --;

			for (lineIndex = 0; lineIndex < lineCount; lineIndex ++)
			{
				break_if(remain < 2);
				textLineInfo.mOffset = (ZUINT16)ntohs(*(ZUINT16*)ptr);
				ptr += 2;
				remain -= 2;

				break_if(remain < 1);
				textLineInfo.mLength = *(ZUINT8*)ptr;
				ptr ++;
				remain --;

				// Check the text legnth
				break_if(displayText.length() < (ZUINT32)textLineInfo.mOffset + (ZUINT32)textLineInfo.mLength);

				textString = displayText.substr(textLineInfo.mOffset, textLineInfo.mLength);
                m_textArray.push_back(textString);
			}

			// 4
		}
		else if (kind == kZNMapShapeKind_Text5)
		{
            break_if( remain < 4 );
            m_textData.mID = (ZUINT32)ntohl(*(ZUINT32*)ptr);
            ptr += 4;
            remain -= 4;

			break_if( remain < 1 );
			m_textData.mBaseX = *ptr;
			ptr += 1;
			remain -= 1;

			break_if( remain < 1 );
			m_textData.mBaseY = *ptr;
			ptr += 1;
			remain -= 1;

			break_if(remain < 1);
			m_textData.mFlags = *(ZUINT8*)ptr;
			ptr ++;
			remain --;

			break_if(remain < 1);
			lineCount = *(ZUINT8*)ptr;
			ptr ++;
			remain --;

			for ( lineIndex = 0; lineIndex < lineCount; lineIndex++ )
			{
				break_if(remain < 2);
				textLineInfo.mOffset = (ZUINT16)ntohs(*(ZUINT16*)ptr);
				ptr += 2;
				remain -= 2;

				break_if(remain < 1);
				textLineInfo.mLength = *(ZUINT8*)ptr;
				ptr ++;
				remain --;

				break_if(displayText.length() < (ZUINT32)textLineInfo.mOffset + (ZUINT32)textLineInfo.mLength);

				textString = displayText.substr(textLineInfo.mOffset, textLineInfo.mLength);
                m_textArray.push_back(textString);
			}
			break_if(lineIndex < lineCount);

			// 5
			m_displayFlag = (m_textData.mFlags >> 3) & 0x07;
		}
		else
		{
			break;
		}

		break_if(dataPtr + dataLength < ptr);
		readLength = dataLength - remain;

	} while (0);

	m_pointLatLon.x = m_textData.mBaseX;
	m_pointLatLon.y = m_textData.mBaseY;

	ZINT32 minX, minY, maxX, maxY; 
	ZUINT8 level = ZNMapLevelFromKind(mBlockKind);
	ZNMapGetLLOfBlockRect(level, mBlockX, mBlockY, minX, minY, maxX, maxY);

	m_llPoint.x = (((m_pointLatLon.x + 128) *(ZNMapBlockLongitude[level]/4.0))/255.00  + minX)/3600000.0;
	m_llPoint.y = (((m_pointLatLon.y + 128) *(ZNMapBlockLatitude[level]/4.0))/255.00  + minY)/3600000.0;


	return readLength;
}

