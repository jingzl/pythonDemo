#pragma once
#include "DataType.h"
#include "ZNMapShape.h"

class ZNMapSignShape : public ZNMapTextureShape
{
protected:
	ZINT32 skipDataBytes(const ZUINT8*dataPtr, ZUINT32 dataLength, ZUINT8 kind);
public:
	ZNMapSignShape();
	~ZNMapSignShape();
};