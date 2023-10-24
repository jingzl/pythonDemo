#include "ZNMapTileLayer.h"
ZNMapTileLayer::ZNMapTileLayer()
{

}

ZNMapTileLayer::~ZNMapTileLayer()
{

}

ZVOID ZNMapTileLayer::updateLocation(ZNPoint location, ZNSize size, ZNPoint offset)
{

}

ZVOID ZNMapTileLayer::setLevel(ZUINT8 level)
{
}

ZVOID ZNMapTileLayer::setLocation(ZNPoint location, ZNSize size, ZNPoint offset)
{
}

ZVOID ZNMapTileLayer::requestLoading(ZVOID)
{
}

ZVOID ZNMapTileLayer::cancelLoading(ZVOID)
{

}

/*ZBOOL ZNMapTileLayer::mapFileDidLoadFunc(CP_VECTOR<blockdata> *m_blockDataArray)
{//相差不久都被调的话怎么办,肯定得清空某些数据的。难道在每次下载数据前清空map？
	ZNMapDecoder *decoder = ZNMapDecoder::GetInstance;
	for(CP_VECTOR<blockdata>::iterator it = m_blockDataArray.begin; it != m_blcokDataArray.end; ++i)
	{
		LPZNMAPDATAREP mapDataRep = new ZNMAPDATAREP;
		decoder->decodeBlockData(tileID, ,&mapDataRep);
	}
}*/