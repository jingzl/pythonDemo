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
{//���ö������Ļ���ô��,�϶������ĳЩ���ݵġ��ѵ���ÿ����������ǰ���map��
	ZNMapDecoder *decoder = ZNMapDecoder::GetInstance;
	for(CP_VECTOR<blockdata>::iterator it = m_blockDataArray.begin; it != m_blcokDataArray.end; ++i)
	{
		LPZNMAPDATAREP mapDataRep = new ZNMAPDATAREP;
		decoder->decodeBlockData(tileID, ,&mapDataRep);
	}
}*/