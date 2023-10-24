#pragma once
#include "ZNMapShape.h"
#include "ZNMapTypes.h"
#include <string>
#include "cpstl.h"
using namespace std;


class ZNMapTextShape : public ZNMapTextureShape
{
public:
    ZNMapTextShape();
    ZNMapTextShape(char kind, ZUINT32 blockX, ZUINT32 blockY):mBlockKind(kind), mBlockX(blockX), mBlockY(blockY)
    {};
    ~ZNMapTextShape();

public:
	ZINT32 skipDataBytes(const ZUINT8* dataPtr, ZUINT32 dataLength, ZUINT8 kind);
	ZINT32 readDataBytes(const ZUINT8* dataPtr, ZUINT32 dataLength, ZUINT8 kind, 
											wstring& displayText);

public:
    CP_VECTOR<wstring> m_textArray;
    CP_VECTOR<ZNMapTextLineInfo> m_textLineInfoArray;
    ZNFloatPoint m_llPoint;
    ZNMapTextData m_textData;
    ZUINT8   m_displayFlag;

private:
	char mBlockKind;
	ZUINT32 mBlockX;
	ZUINT32 mBlockY;
	ZNPoint m_pointLatLon;

};
