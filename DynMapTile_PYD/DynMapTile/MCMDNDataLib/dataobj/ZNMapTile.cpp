#include "ZNMapTile.h"
#include <WinSock.h>
#include "ZNMapTextShape.h"
#include "ZNMapPolygonShape.h"
#include "ZNMapLinePolygonShape.h"
#include "ZNMapDataDefine.h"
#include "QuickFunc.h"
#include "ZNMapUtility.h"
#pragma comment(lib,"ws2_32")

// for debug
//#include "globaldefine.h"
//#include <tchar.h>
//LogFunc g_lpLogFunc;
//#define TEST_LOG(x)         if ( mBlockX == 41 && mBlockY == 30 ) {  g_lpLogFunc( _T(x) ); }


ZNMapTile::ZNMapTile()
{
    mMapData = NULL;
}

ZNMapTile::~ZNMapTile()
{
	for (CP_VECTOR<ZNMapLayer*>::iterator it = mLayers.begin(); it != mLayers.end(); it++)
	{
		safe_delete(*it);
	}
    mLayers.clear();
	mPtrShapePart = NULL;
}

CP_VECTOR<ZNMapLayer*>& ZNMapTile::getLayers()
{
	return mLayers;
}

char* ZNMapTile::getBlockKind()
{
	return &mBlockKind;
}

ZUINT32 ZNMapTile::getBlockX()
{
	return mBlockX;
}

ZUINT32 ZNMapTile::getBlockY()
{
	return mBlockY;
}

ZBOOL ZNMapTile::decodeBlockData(char* tileID, ZBYTE *mapData, ZUINT32 dataLength)
{
	ZBOOL bSucceed = 0;
	do 
	{
		mHaveGetLayerIndex = 0;
		mHaveGetShapePart = 0;
		mPtrShapePart = NULL;
		mRemainShapePart = 0;

		mConvertLine = 0;
		break_if(!ZNMapTileGetBlockInfo(tileID, &mBlockKind, &mBlockX, &mBlockY));
		mMapData = mapData;
		break_if(mMapData == NULL);

		mBlockOffset = 0;
		mBlockLength = dataLength;
		bSucceed = this->parseBlockData(mMapData, dataLength);
	} while (0);

	return bSucceed;
}

ZBOOL ZNMapTile::parseBlockData(ZBYTE* mapData, ZUINT32 dataLength)
{
	const ZUINT8 *ptr = NULL;
	ZUINT32 remain, blockBodyRemain;
	ZINT32 actualLength;
	ZBOOL succeed = FALSE;

	do {
		ptr = mapData;
		remain = dataLength;

		break_if(ptr == NULL);
		break_if(remain == 0);

		actualLength = this->parseBlockHeader(ptr, remain);
		break_if(actualLength < 0);
		ptr += actualLength;
		remain -= actualLength;
		break_if(remain < mBlockBodyLength);

		break_if(mBlockBodyLength == 0);
		blockBodyRemain = mBlockBodyLength;
		while (0 < blockBodyRemain)
		{
			actualLength = this->parseBlockBody(ptr, blockBodyRemain);
			break_if(actualLength < 0);
			ptr += actualLength;
			blockBodyRemain -= actualLength;
		}
		break_if(0 < blockBodyRemain);

 		succeed = ZTRUE;

	} while (0);
	return succeed;
}

ZINT32 ZNMapTile::parseBlockHeader(const ZUINT8* dataPtr, ZUINT32 dataLength)
{
	const ZUINT8 *ptr = NULL;
	ZUINT32 remain;
	ZUINT8 signature;
	ZUINT8 blockKind;
	ZUINT32 blockX, blockY;
	ZINT32 readLength = -1;

	do {
		break_if(dataPtr == NULL || dataLength == 0);
		break_if(mBlockLength == 0);

		ptr = dataPtr;
		remain = dataLength;

		break_if(remain < mBlockLength);

		break_if(remain < 1);
		signature = *ptr;
		break_if(signature != 1);
		ptr ++;
		remain --;

		break_if(remain < 1);
		blockKind = *ptr;
		if ((mBlockKind == kZNMapKind_g) && (mBlockKind != blockKind))
		{
			mBlockKind = blockKind;
		}
		else
		{
			break_if(mBlockKind != blockKind);
		}
		ptr ++;
		remain --;

		break_if(remain < 4);
		blockX = ntohl(*(ZUINT32*)ptr);
		break_if(mBlockX != blockX);
		ptr += 4;
		remain -= 4;

		break_if(remain < 4);
		blockY = ntohl(*(ZUINT32*)ptr);
		break_if(mBlockY != blockY);
		ptr += 4;
		remain -= 4;

		break_if(remain < 2);
		mBlockBodyLength = ntohs(*(ZUINT16*)ptr) * 2;
 		ptr += 2;
		remain -= 2;

		break_if(dataPtr + dataLength < ptr);
		readLength = dataLength - remain;

	} while (0);

	return readLength;
}

ZINT32 ZNMapTile::parseBlockBody(const ZUINT8* dataPtr, ZUINT32 dataLength)
{
	const ZUINT8 *ptr = NULL;
	ZUINT32 remain = 0;
	ZINT32 actualLength = 0;
	ZINT32 readLength = -1;
	ZUINT8 signature = 0;

	do {
		break_if(dataPtr == NULL || dataLength == 0);

		ptr = dataPtr;
		remain = dataLength;

        break_if(remain < 1);
        signature = *(ZUINT8*)ptr;
        ptr ++;
        remain --;

		if (signature == 16)
		{
			actualLength = this->parseTextPart(ptr, remain);
			break_if(actualLength < 0);
			ptr += actualLength;
			remain -= actualLength;
		}
		else if (signature == 1)
		{
			actualLength = this->parseLayerPart(ptr, remain);
			break_if(actualLength < 0);
			ptr += actualLength;
			remain -= actualLength;

			mHaveGetLayerIndex = ZTRUE;

			if(!mHaveGetShapePart)
			{
				this->parseShapePart(mPtrShapePart, mRemainShapePart);
				mHaveGetShapePart = ZTRUE;
			}
		}
		else if (signature == 2)
		{
			if( !mHaveGetLayerIndex || mHaveGetShapePart )
			{
				mPtrShapePart = ptr;
				mRemainShapePart = remain;
				actualLength = this->skipShapePart(ptr, remain);
			}
            else
			{
				actualLength = this->parseShapePart(ptr, remain);
				mHaveGetShapePart = ZTRUE;
			}
			break_if(actualLength < 0);
			ptr += actualLength;
			remain -= actualLength;
		}

        break_if(dataPtr + dataLength < ptr);
		readLength = dataLength - remain;

	} while( 0 );

	return readLength;
}

ZINT32 ZNMapTile::parseTextPart( const ZUINT8* dataPtr, ZUINT32 dataLength )
{
	const ZUINT8 *ptr = NULL;
	ZUINT16 characterLength = 0;
	ZUINT32 requiredLength = 0;
	ZUINT32 remain = 0;
	ZINT32 readLength = -1;
	WCHAR *unicodeText = NULL;

	do {
		break_if( dataPtr == NULL || dataLength == 0 );
		ptr = dataPtr;
		remain = dataLength;

		break_if(remain < 2);
		characterLength = ntohs(*(ZUINT16*)ptr);
		ptr += 2;
		remain -= 2;

		if ( 0 < characterLength )
		{
			requiredLength = characterLength * 2;
			break_if(remain < requiredLength);

			unicodeText = new WCHAR[characterLength+1];
			::ZeroMemory(unicodeText, (characterLength+1)*sizeof(WCHAR));

            // 
            for ( int i=0; i<characterLength; i++ )
            {
                ZUINT16 val = ntohs(*(ZUINT16*)ptr);
                memcpy( unicodeText+i, &val, 2 );
                ptr += 2;
            }
            remain -= requiredLength;

            //memmove_s(unicodeText, requiredLength, ptr, requiredLength);

            //FILE* fp = fopen( "c:\\2.dat", "wb" );
            //fwrite( unicodeText, sizeof(WCHAR), characterLength+1, fp );
            //CUnicodeConverter::Print_UTF16_BOM( fp, TRUE );
            //fwrite( unicodeText, sizeof(WCHAR), characterLength+1, fp );
            //CUnicodeConverter::Print_UTF8Str_By_UTF16Str( fp, (WORD*)unicodeText );
            //fclose(fp);
            //fp = NULL;

            wstring wstr(unicodeText);
			mText = wstr;
            safe_delete_array( unicodeText );
		}

		break_if(dataPtr + dataLength < ptr);
		readLength = dataLength - remain;

	} while (0);

	return readLength;
}

ZINT32 ZNMapTile::parseLayerPart(const ZUINT8* dataPtr, ZUINT32 dataLength)
{
	ZNMapLayer *layer = NULL;
	const ZUINT8 *ptr = NULL;
	ZUINT32 remain = 0;
	ZUINT8 layerIndex = 0;
    ZUINT8 layerCount = 0;
	ZINT32 readLength = -1;

	do {
		break_if(dataPtr == NULL || dataLength == 0);
		ptr = dataPtr;
		remain = dataLength;

		break_if(remain < 1);
		layerCount = *ptr;
		ptr++;
		remain--;

		for (layerIndex = 0; layerIndex < layerCount; layerIndex ++)
		{
			break_if(remain < 7);

            layer = new ZNMapLayer();
            break_if(layer == NULL);

			layer->mNumber = *ptr;
			ptr++;
			remain--;

			layer->mKind = *ptr;
			ptr++;
			remain--;

			layer->mFlags = *ptr;
			ptr++;
			remain--;

			layer->mShapeOffset = ntohs(*(ZUINT16*)ptr) * 2;
			ptr += 2;
			remain -= 2;

			layer->mShapeCount = ntohs(*(ZUINT16*)ptr);
			ptr += 2;
			remain -= 2;

			mLayers.push_back(layer);
		}
		break_if(mLayers.size() <= 0);
		readLength = dataLength - remain;

	} while (0);

	return readLength;
}

ZINT32 ZNMapTile::parseShapePart(const ZUINT8* dataPtr, ZUINT32 dataLength)
{
    ZNMapLayer *mapLayer = NULL;
    
    ZNMapTextShape *textShape = NULL;
    ZNMapSignImageShape *signImageShape = NULL;
    ZNMapSignTextShape *signTextShape = NULL;
    ZNMapLineShape *lineShape = NULL;
	ZNMapLinePolygonShape *linePolygonShape = NULL;
    
    BOOL getTextDrawInfo = FALSE;
    BOOL getLineDrawInfo = FALSE;
    BOOL getPolyDrawInfo = FALSE;
    const ZUINT8 *ptr = NULL;
    ZUINT8 shapeKind = 0;

    ZUINT32 shapeBodyLength = 0;
    ZUINT32 shapeBodyOffset = 0;
    ZUINT32 shapeBodyRemain = 0;

    ZUINT16 shapeIndex = 0;
    ZUINT32 remain = 0;
    ZINT32 actualLength = 0;
    ZINT32 readLength = -1;

    ZINT32 layerIndex = 0;

    do {
        break_if(mLayers.size() == 0);
        break_if(dataPtr == NULL);
        break_if(dataLength == 0);
        ptr = dataPtr;
        remain = dataLength;

        break_if( remain < 2 );
        shapeBodyLength = ntohs(*(ZUINT16*)ptr) * 2;
        ptr += 2;
        remain -= 2;

        break_if(remain < shapeBodyLength);
        shapeBodyRemain = shapeBodyLength;

        for ( ZUINT32 i=0; i<mLayers.size(); i++ )
        {
			mapLayer = mLayers[i];

            // ??? 会出现不一致，要调整
            shapeBodyOffset = shapeBodyLength - shapeBodyRemain;
//Modify by libin for PalmIphone <<<<<<<<<<
            if(shapeBodyOffset != mapLayer->mShapeOffset)
            {
            	ptr+=mapLayer->mShapeOffset - shapeBodyOffset;
            	shapeBodyRemain-=mapLayer->mShapeOffset - shapeBodyOffset;
            }
            //break_if(shapeBodyOffset != mapLayer.shapeOffset);
//Modify by libin for PalmIphone >>>>>>>>>>
            getTextDrawInfo = getLineDrawInfo = getPolyDrawInfo = ZTRUE;

            LPZNMAPDATAREP pMapDataRep = new ZNMAPDATAREP();
            for ( shapeIndex = 0; shapeIndex < mapLayer->mShapeCount; shapeIndex++ ) 
            {
                break_if(shapeBodyRemain < 1);
                shapeKind = *ptr;
                ptr++;
                shapeBodyRemain--;

                if ( (shapeKind == kZNMapShapeKind_Text1) ||
                    (shapeKind == kZNMapShapeKind_Text2) ||
                    (shapeKind == kZNMapShapeKind_Text3) ||
                    (shapeKind == kZNMapShapeKind_Text4) ||
                    (shapeKind == kZNMapShapeKind_Text5) )
                {
					textShape = new ZNMapTextShape(mBlockKind, mBlockX, mBlockY);
					break_if(textShape == NULL);

                    //actualLength = textShape->skipDataBytes(ptr, shapeBodyRemain, shapeKind);
					actualLength =  textShape->readDataBytes(ptr, shapeBodyRemain, shapeKind, mText);
					if (actualLength < 0)
                    {
                        delete textShape;
                        textShape = NULL;
                        break;
                    }
					pMapDataRep->m_textShapes.push_back(textShape);
                }
                else if ((shapeKind == kZNMapShapeKind_Polygon1) ||
                         (shapeKind == kZNMapShapeKind_Polygon2) || 
                         (shapeKind == kZNMapShapeKind_Polygon3) || 
                         (shapeKind == kZNMapShapeKind_Polygon4) || 
                         (shapeKind == kZNMapShapeKind_Polygon5) || 
                         (shapeKind == kZNMapShapeKind_Polygon6))
                {
					switch(shapeKind)
                    {
					case kZNMapShapeKind_Polygon1:
                        {
						    ZNMapPolygonShape1 *polygonShape = new ZNMapPolygonShape1(mBlockKind, mBlockX, mBlockY);
						    break_if(polygonShape == NULL);
                            //actualLength = polygonShape->skipDataBytes(ptr, shapeBodyRemain);
                            actualLength = polygonShape->readDataBytes(ptr, shapeBodyRemain);
						    pMapDataRep->m_polygonShapes.push_back(polygonShape);
                            break;
                        }
					case kZNMapShapeKind_Polygon2:
                        {
						    ZNMapPolygonShape2 *polygonShape = new ZNMapPolygonShape2(mBlockKind, mBlockX, mBlockY);
						    break_if(polygonShape == NULL);
						    //actualLength = polygonShape->skipDataBytes(ptr, shapeBodyRemain);
                            actualLength = polygonShape->readDataBytes(ptr, shapeBodyRemain);
						    pMapDataRep->m_polygonShape2s.push_back(polygonShape);
                            break;
					    }
					case kZNMapShapeKind_Polygon3:
                        {
						    ZNMapPolygonShape3 *polygonShape = new ZNMapPolygonShape3(mBlockKind, mBlockX, mBlockY);
						    break_if(polygonShape == NULL);
						    //actualLength = polygonShape->skipDataBytes(ptr, shapeBodyRemain);
                            actualLength = polygonShape->readDataBytes(ptr, shapeBodyRemain);
						    pMapDataRep->m_polygonShape3s.push_back(polygonShape);
                            break;
					    }
					case kZNMapShapeKind_Polygon4:
                        {
						    ZNMapPolygonShape4 *polygonShape = new ZNMapPolygonShape4(mBlockKind, mBlockX, mBlockY);
						    break_if(polygonShape == NULL);
						    //actualLength = polygonShape->skipDataBytes(ptr, shapeBodyRemain);
                            actualLength = polygonShape->readDataBytes(ptr, shapeBodyRemain);
						    pMapDataRep->m_polygonShape4s.push_back(polygonShape);
					        break;
                        }
					case kZNMapShapeKind_Polygon5:
                        {
						    ZNMapPolygonShape5 *polygonShape = new ZNMapPolygonShape5(mBlockKind, mBlockX, mBlockY);
						    break_if(polygonShape == NULL);
						    //actualLength = polygonShape->skipDataBytes(ptr, shapeBodyRemain);
                            actualLength = polygonShape->readDataBytes(ptr, shapeBodyRemain);
						    pMapDataRep->m_polygonShape5s.push_back(polygonShape);
							break;
                        }
					case kZNMapShapeKind_Polygon6:
                        {
						    ZNMapPolygonShape6 *polygonShape = new ZNMapPolygonShape6(mBlockKind, mBlockX, mBlockY);
						    break_if(polygonShape == NULL);
						    //actualLength = polygonShape->skipDataBytes(ptr, shapeBodyRemain);
                            actualLength = polygonShape->readDataBytes(ptr, shapeBodyRemain);
						    pMapDataRep->m_polygonShape6s.push_back(polygonShape);
                            break;
					    }
					}
                    if ( actualLength < 0 )
                    {
                        break;
                    }
                }
                else if ( shapeKind == kZNMapShapeKind_Sign1 )
                {
					signImageShape = new ZNMapSignImageShape(mBlockKind, mBlockX, mBlockY);
					break_if(signImageShape == NULL);

					actualLength = signImageShape->readDataBytes1(ptr, shapeBodyRemain);
					if (actualLength < 0)
                    {
                        delete signImageShape;
                        signImageShape = NULL;
                        break;
                    }
					pMapDataRep->m_signImageShapes.push_back(signImageShape);
                }
                else if ( (shapeKind == kZNMapShapeKind_Sign2) || 
                         (shapeKind == kZNMapShapeKind_Sign3) || 
                         (shapeKind == kZNMapShapeKind_Sign4) )
                {
					signTextShape = new ZNMapSignTextShape(mBlockKind, mBlockX, mBlockY);
                    break_if(signTextShape == NULL);
                    signTextShape->mFlag = shapeKind;

					switch(shapeKind){
						case kZNMapShapeKind_Sign2:
							actualLength = signTextShape->readDataBytes2(ptr, shapeBodyRemain, mText);
							break;
						case kZNMapShapeKind_Sign3:
							actualLength = signTextShape->readDataBytes3(ptr, shapeBodyRemain, mText);
							break;
						case kZNMapShapeKind_Sign4:
							actualLength = signTextShape->readDataBytes4(ptr, shapeBodyRemain, mText);
							break;
						default:
							actualLength = -1;
							break;
					}
					if (actualLength < 0)
                    {
                        delete signTextShape;
                        signTextShape = NULL;
                        break;
                    }
					pMapDataRep->m_signTextShapes.push_back(signTextShape);
                }
                else if (shapeKind == kZNMapShapeKind_LineGraph)
                {
					if (mConvertLine)
					{
						//linePolygonShape = new ZNMapLinePolygonShape(mBlockKind, mBlockX, mBlockY);
						//break_if(linePolygonShape == NULL);
                        actualLength = linePolygonShape->skipDataBytes(ptr, shapeBodyRemain);
						//actualLength = linePolygonShape->readDataBytes(ptr, shapeBodyRemain, mapLayer->mLineDrawInfoPtr);
						//if (actualLength < 0)
                        {
                        //    delete linePolygonShape;
                        //    linePolygonShape = NULL;
                        //    break;
                        }
						//pMapDataRep->m_linePolygonShapes.push_back(linePolygonShape);
					}
					else
					{
						lineShape = new ZNMapLineShape(mBlockKind, mBlockX, mBlockY);
						break_if(lineShape == NULL);
                        //actualLength = lineShape->skipDataBytes(ptr, shapeBodyRemain);
						actualLength = lineShape->readDataBytes(ptr, shapeBodyRemain);
					    if(actualLength < 0)
                        {
                            delete lineShape;
                            lineShape = NULL;
                            break;
                        }
						pMapDataRep->m_lineShapes.push_back(lineShape);
					}
                }
                else
                {
                    break;
                }

                ptr += actualLength;
                shapeBodyRemain -= actualLength;
            }

			ZUINT32 dataCount = 0;
			dataCount = pMapDataRep->m_lineShapes.size() + pMapDataRep->m_polygonShape2s.size() 
				+ pMapDataRep->m_polygonShape3s.size() + pMapDataRep->m_polygonShape4s.size() +
				pMapDataRep->m_polygonShape5s.size() + pMapDataRep->m_polygonShape6s.size() +
				pMapDataRep->m_polygonShapes.size() + pMapDataRep->m_signImageShapes.size() +
				pMapDataRep->m_textShapes.size() + pMapDataRep->m_signTextShapes.size() +
				pMapDataRep->m_linePolygonShapes.size();
			if (dataCount > 0)
            {
				mapLayer->mMapDataRep = pMapDataRep;
			}
            else
            {
				safe_delete(pMapDataRep);
			}
			layerIndex ++;
		}
        break_if(mapLayer != NULL);

        remain -= shapeBodyLength;

        break_if(dataPtr + dataLength < ptr);
        readLength = dataLength - remain;

    } while (0);

	return readLength;
}

ZINT32 ZNMapTile::skipShapePart(const ZUINT8* dataPtr, ZUINT32 dataLength)
{
	const ZUINT8 *ptr = NULL;
	ZUINT32 remain;
	ZINT32 readLength = -1;
	ZUINT16 dataSize;
	do {
		ptr = dataPtr;
		remain = dataLength;

		//Graphic Area Size
		break_if(remain < 2);
		dataSize = ntohs(*(ZUINT16*)ptr) * 2;
		ptr += 2;
		remain -= 2;

		//Graphic Area
		break_if(remain < dataSize);
		remain -= dataSize;
		ptr += dataSize;

		readLength = dataLength - remain;
	} while(0) ;

	return readLength;
}