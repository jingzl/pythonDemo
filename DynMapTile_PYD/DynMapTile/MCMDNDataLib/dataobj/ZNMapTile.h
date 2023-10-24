#pragma once
#include "ZNMapDataDefine.h"
#include "ZNMapLayer.h"


class ZNMapTile
{
public:
    ZNMapTile();
    ZNMapTile(ZUINT32 x, ZUINT32 y): mBlockX(x), mBlockY(y){};
    ~ZNMapTile();

	ZBOOL decodeBlockData(char* tileID, ZBYTE *mapData, ZUINT32 dataLength);
	CP_VECTOR<ZNMapLayer*>& getLayers();
	char* getBlockKind();
	ZUINT32 getBlockX();
	ZUINT32 getBlockY();

protected:
	ZBOOL parseBlockData(ZBYTE* mapData, ZUINT32 dataLength);
	ZINT32 parseBlockHeader(const ZUINT8* dataPtr, ZUINT32 dataLength);
	ZINT32 parseBlockBody(const ZUINT8* dataPtr, ZUINT32 dataLength);
	ZINT32 parseTextPart(const ZUINT8* dataPtr, ZUINT32 dataLength);
	ZINT32 parseLayerPart(const ZUINT8* dataPtr, ZUINT32 dataLength);
	ZINT32 parseShapePart(const ZUINT8* dataPtr, ZUINT32 dataLength);
	ZINT32 skipShapePart(const ZUINT8* dataPtr, ZUINT32 dataLength);

private:
	char mBlockKind;
	ZUINT32 mBlockX;
	ZUINT32 mBlockY;
	ZUINT32 mBlockOffset;
	ZUINT32 mBlockLength;
	ZUINT32 mBlockBodyLength;
	const ZUINT8 *mPtrShapePart;
	ZUINT32 mRemainShapePart;

	ZBOOL mConvertLine;

	ZBOOL mHaveGetLayerIndex;
	ZBOOL mHaveGetShapePart;

	ZBYTE *mMapData;
	wstring mText;
	CP_VECTOR<ZNMapLayer*> mLayers;


};
