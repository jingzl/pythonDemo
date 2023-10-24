#pragma once
#include "ZNMapTile.h"
#include "ZNMapDecoder.h"
#include "ZNMapDataDefine.h"


class ZNMapTileLayer{
private:
	CP_VECTOR<ZNMapTile> m_tiles;
	//CP_VECTOR<blockdata> m_blockDataArray;
	//CP_VECTOR<blockdata> m_cachedBlockedDataArray;
	ZNPoint m_location;
	ZNSize m_size;
	ZNPoint m_offset;
	ZUINT8 m_level;
	//回调函数将decode的地图数据结构传出去
	//DYN_CALLBACK_FUNC m_lpMapCallback;//callbackMapTileDidLoad();
	CP_MAP<string, ZNMAPDATAREP*> m_mapDataRepMap;
private:
	ZVOID updateLocation(ZNPoint location, ZNSize size, ZNPoint offset);
public:
	ZVOID setLevel(ZUINT8 level);
	ZVOID setLocation(ZNPoint location, ZNSize size, ZNPoint offset);
	//ZBOOL mapFileDidLoadFunc(CP_VECTOR<blockdata> *m_blockDataArray);
	ZVOID requestLoading(ZVOID);
	ZVOID cancelLoading(ZVOID);
public:
	ZNMapTileLayer();
    ZNMapTileLayer(ZUINT8 level);
	~ZNMapTileLayer();
};