#include "ZNMapLayer.h"
#include "QuickFunc.h"

ZNMapLayer::ZNMapLayer(void)
{
	mMapDataRep = NULL;
}

ZNMapLayer::~ZNMapLayer(void)
{
	safe_delete(mMapDataRep);
}
