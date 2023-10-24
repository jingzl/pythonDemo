#include "ZNMapSignShape.h"
#include "QuickFunc.h"
#include "ZNMapTypes.h"

ZNMapSignShape::ZNMapSignShape()
{

}

ZNMapSignShape::~ZNMapSignShape()
{

}

ZINT32 ZNMapSignShape::skipDataBytes(const ZUINT8*dataPtr, ZUINT32 dataLength, ZUINT8 kind)
{
	const ZUINT8 *ptr = NULL;
	ZUINT32 remain, skipLength;
	ZINT32 readLength = -1;
	ZUINT8 lineCount;

	do {
		break_if(dataPtr == NULL);
		break_if(dataLength == 0);

		ptr = dataPtr;
		remain = dataLength;

		if (kind == kZNMapShapeKind_Sign1)
		{
            // signData.mID:4
			// signData.mBaseX:1;
			// signData.mBaseY:1;
			// signData.mNumber:2;

			skipLength = 8;
			break_if(remain < skipLength);
			remain -= skipLength;
		}
		else if (kind == kZNMapShapeKind_Sign2)
		{
            // ID: 4
			// signData.mBaseX:1;
			// signData.mBaseY:1;            
			// signData.mNumber:2;
			// signData.mDigit:2;

			skipLength = 10;
			break_if(remain < skipLength);
			remain -= skipLength;
		}
		else if (kind == kZNMapShapeKind_Sign3)
		{
            // ID:4;
			// signData.mBaseX:1;
			// signData.mBaseY:1;
			// signData.mNumber:2;
			// signData.mFlags:1;
			// textLineInfo.mOffset:2;
			// textLineInfo.mLength:1;

			skipLength = 12;
			break_if(remain < skipLength);
			remain -= skipLength;
		}
		else if (kind == kZNMapShapeKind_Sign4)
		{
            // ID:4;
			// signData.mBaseX:1;
			// signData.mBaseY:1;
			// signData.mNumber:2;
			// signData.mFlags:1;

			skipLength = 9;
			break_if(remain < skipLength);
			remain -= skipLength;
			ptr += skipLength;

			break_if(remain < 1);
			lineCount = *ptr;
			ptr ++;
			remain --;

			// textLineInfo.mOffset:2;
			// textLineInfo.mLength:1;

			skipLength = lineCount * 3;
			break_if(remain < skipLength);
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
