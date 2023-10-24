#include "ZNMapDecoder.h"
#include "QuickFunc.h"


ZNMapDecoder::ZNMapDecoder()
{
}

ZNMapDecoder::~ZNMapDecoder()
{
}

ZBOOL ZNMapDecoder::decodeBlockData(CP_VECTOR<STSingleBlockDataInfo*>* inputBlockDatas,
									CP_VECTOR<ZNMapTile*>* outputTiles)
{
	do 
	{
		break_if(inputBlockDatas == NULL);
		break_if(inputBlockDatas->size() <= 0);
		break_if(outputTiles == NULL);

		for (CP_VECTOR<STSingleBlockDataInfo*>::iterator it = inputBlockDatas->begin();
				it != inputBlockDatas->end(); it++)
		{
			STSingleBlockDataInfo *singleBlockIno =	*it;
            if ( singleBlockIno == NULL )
            {
                continue;
            }
			ZNMapTile *tile = new ZNMapTile();
			if ( tile->decodeBlockData( (char*)singleBlockIno->pTileID, singleBlockIno->pBlcokDataInfo, singleBlockIno->ulBlcokDataLen) )
			{
				outputTiles->push_back(tile);
			}
            else
            {
                delete tile;
                tile = NULL;
            }
		}
	} while (0);
	return  (outputTiles->size() > 0) ? 1 : 0;
}
