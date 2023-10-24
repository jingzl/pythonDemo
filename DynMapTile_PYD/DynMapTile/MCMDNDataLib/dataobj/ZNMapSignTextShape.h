#pragma once
#include "DataType.h"
#include "ZNMapSignShape.h"
#include <string>
#include "CPStl.h"

class ZNMapSignTextShape : public ZNMapSignShape
{
public:
    ZNMapSignTextShape();
    ZNMapSignTextShape(char kind, ZUINT32 blockX, ZUINT32 blockY):mBlockKind(kind), mBlockX(blockX), mBlockY(blockY)
    {
        bIsDigit = ZTRUE;
    };
    ~ZNMapSignTextShape();

public:
	ZINT32 readDataBytes2(const ZUINT8*dataBytes, ZUINT32 dataLength, wstring& text);
	ZINT32 readDataBytes3(const ZUINT8*dataBytes, ZUINT32 dataLength, wstring& text);
	ZINT32 readDataBytes4(const ZUINT8*dataBytes, ZUINT32 dataLength, wstring& text);


public:
    ZUINT8 mFlag;
	CP_VECTOR<wstring> m_textArray;
	ZNFloatPoint mLLPoint;
	ZNMapSignDataDigit m_mapSignDigitData; 
	ZNMapSignDataText m_mapSignTextData;
	ZNMapTextLineInfo m_textLineInfo;
	ZBOOL bIsDigit;


protected:
    char mBlockKind;
    ZUINT32 mBlockX;
    ZUINT32 mBlockY;
    ZNPoint mPoint;

};