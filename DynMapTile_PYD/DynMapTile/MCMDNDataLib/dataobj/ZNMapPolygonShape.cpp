#include "ZNMapPolygonShape.h"
#include <WinSock.h>
#include "QuickFunc.h"
#include "CPStl.h"
#include "ZNMapUtility.h"
#pragma comment(lib,"ws2_32")

static inline void
ZNMapPolygonCopyPoint(ZINT8 *dstPtr, const ZINT8 *srcPtr, ZUINT8 count);

ZNMapPolygonShape::ZNMapPolygonShape()
{

}

ZNMapPolygonShape::~ZNMapPolygonShape()
{

}

ZINT32 ZNMapPolygonShape::readDataBytes(const ZUINT8* dataPtr, ZUINT32 dataLength)
{
	return -1;
}

ZINT32 ZNMapPolygonShape::skipDataBytes(const ZUINT8* dataPtr, ZUINT32 dataLength)
{
	return -1;
}

ZBOOL isIndexInSet(CP_VECTOR<ZUINT32> indexset, ZUINT32 index)
{
	ZBOOL result = ZFALSE;
	for (CP_VECTOR<ZUINT32>::iterator it = indexset.begin(); it != indexset.end(); it++ )
	{
		if (*it == index)
		{
			result = ZTRUE;
			break;
		}
	}
	return result;
}

ZINT32
ZNMapPolygonReadPointFromDataBytes(
								   const ZUINT8 *     inBytes,
								   ZUINT32            inLength,
								   ZNInt8Section *outPoint
									)
{
	const ZUINT8 *ptr = NULL;
	ZUINT32 remain;
	ZINT32 readLength = -1;

	ZUINT32 glPointDataLength;
	ZINT8 *glTempPtr = NULL;

	ZUINT8 pointCount;
	ZINT8 *glPointDataPtr = NULL;

	do {
		break_if(inBytes == NULL || inLength == 0);
		break_if(outPoint == NULL);

		ptr = inBytes;
		remain = inLength;

		break_if(remain < 1);
		pointCount = *ptr;
		ptr ++;
		remain --;

		break_if(pointCount < 3);

		// GL_LINE_STRIPE で閉じたポリゴンを描画するときに
		// 開始点が必要になってくるので、( sizeof(ZFLOAT32) * 2 ) だけ大きく
		// メモリを確保して、終点の次に、開始点をコピーしておく

        glPointDataLength = sizeof(ZINT8) * 2 * pointCount + sizeof(ZINT8) * 2;
		glPointDataPtr = new ZINT8[2 * pointCount + 2];
		break_if(glPointDataPtr == NULL);

		glTempPtr = glPointDataPtr;
		ZNMapPolygonCopyPoint(glTempPtr, (const ZINT8*)ptr, pointCount);

		// 終点の後に、開始点をコピー
		*glTempPtr = *glPointDataPtr;
		*(glTempPtr + 1) = *(glPointDataPtr + 1);

		remain -= sizeof(ZUINT8) * 2 * pointCount;
		readLength = inLength - remain;

		outPoint->num = pointCount;
		outPoint->array = glPointDataPtr;
		glPointDataPtr = NULL;
	} while (0);

	safe_delete_array(glPointDataPtr);
	return readLength;
}

//暂时无用，留着以备数据格式有变
ZINT32
ZNMapPolygonReadPenUpFromDataBytes(
								   const ZUINT8       *inBytes,
								   ZUINT32            inLength,
								   ZUINT8             *outCount,
								   ZUINT8             **outDataPtr)
{
	const ZUINT8 *ptr = NULL;
	ZUINT32 remain;
	ZUINT32 penUpDataLength;
	ZINT32 readLength = -1;

	ZUINT8 penUpCount;
	ZUINT8 *penUpDataPtr = NULL;

	do {
		break_if(inBytes == NULL || inLength == 0);
		break_if(outCount == NULL || outDataPtr == NULL);

		ptr = inBytes;
		remain = inLength;

		break_if(remain < 1);
		penUpCount = *ptr;
		ptr ++;
		remain --;

		break_if(penUpCount == 0);

		penUpDataLength = sizeof(ZUINT8) * penUpCount;
		break_if(remain < penUpDataLength);

		penUpDataPtr = new ZUINT8[penUpCount];
		break_if(penUpDataPtr == NULL);

		memcpy(penUpDataPtr, ptr, penUpDataLength);
		ptr += penUpDataLength;
		remain -= penUpDataLength;

		break_if(inBytes + inLength < ptr);
		readLength = inLength - remain;

		*outCount = penUpCount;
		*outDataPtr = penUpDataPtr;
		penUpDataPtr = NULL;

	} while (0);

	safe_delete_array(penUpDataPtr);

	return readLength;
}

ZINT32
ZNMapPolygonReadSeparatorFromDataBytes(
									   const ZUINT8*      inBytes,
									   ZUINT32            inLength,
									   ZUINT8 *           outCount,
									   ZNMapSeparatorInfo  **outDataPtr)
{
	const ZUINT8 *ptr = NULL;
	ZUINT32 remain;
	ZUINT32 separatorDataLength;
	ZINT32 readLength = -1;

	ZUINT8 separatorCount = 0;
	ZNMapSeparatorInfo *separatorDataPtr = NULL;

	do {
		break_if(inBytes == NULL || inLength == 0);
		break_if(outCount == NULL || outDataPtr == NULL);

		ptr = inBytes;
		remain = inLength;

		break_if(remain < 1);
		separatorCount = *ptr;
		ptr ++;
		remain --;

	//	report_if(separatorCount == 0);

		if (0 < separatorCount)
		{
			separatorDataLength = sizeof(ZNMapSeparatorInfo) * separatorCount;
			break_if(remain < separatorDataLength);

			//separatorDataPtr = (ZNMapSeparatorInfo*)malloc(separatorDataLength);
			separatorDataPtr = new ZNMapSeparatorInfo[separatorCount];
			break_if(separatorDataPtr == NULL);

			memcpy(separatorDataPtr, ptr, separatorDataLength);
			ptr += separatorDataLength;
			remain -= separatorDataLength;
		}

		break_if(inBytes + inLength < ptr);
		readLength = inLength - remain;

		*outCount = separatorCount;
		*outDataPtr = separatorDataPtr;
		separatorDataPtr = NULL;

	} while (0);

	safe_delete_array(separatorDataPtr);

	return readLength;
}

extern ZBOOL 
ZNMapPolygonCreateSeparatedPolygon( 
								   ZUINT8                     inPointCount,
								   const ZINT8            *inPointPtr,
								   ZUINT8                     inSeparatorCount,
								   const ZNMapSeparatorInfo    *inSeparatorDataPtr,
								   ZUINT16                    *outPolygonCount,
								   ZNMapPolygonInfo            **outPolygonInfoPtr,
								   ZNInt8Section					*outPolygonPoint)
{
	ZUINT16 newSeparatedPolygonCount = 0;
	ZNMapPolygonInfo *newSeparatedPolygonInfoPtr = NULL;
	ZINT8 *newPolygonPointPtr = NULL;

	CP_VECTOR<ZUINT32> indexSet;
	ZUINT32 targetIndex;

	ZUINT32 polygonPointCount;
	ZUINT32 dataLength;
	ZUINT8 separatorIndex;
	ZUINT8 startIndex, endIndex;
	ZUINT8 polyOffset = 0, polyCount = 0;
	ZUINT32 copyPointCount;
	ZINT8 *dstPointPtr = NULL;
	const ZINT8 *srcPointPtr = NULL;
	ZBOOL succeed = ZFALSE;

	do {
		break_if(inPointCount == 0);
		break_if(inPointPtr == NULL);

		break_if(outPolygonCount == 0);
		break_if(outPolygonInfoPtr == NULL);
		break_if(outPolygonPoint == NULL);

		if (inSeparatorCount == 0)
		{
			dataLength = sizeof(ZINT8) * 2 * inPointCount;
			newPolygonPointPtr = new ZINT8[2 * inPointCount];
			break_if(newPolygonPointPtr == NULL);

			memcpy(newPolygonPointPtr, inPointPtr, dataLength);

			newSeparatedPolygonCount = 1;
			newSeparatedPolygonInfoPtr = new ZNMapPolygonInfo[newSeparatedPolygonCount];
			break_if(newSeparatedPolygonInfoPtr == NULL);

			newSeparatedPolygonInfoPtr[0].mOffset = 0;
			newSeparatedPolygonInfoPtr[0].mCount = inPointCount;

			*outPolygonCount = newSeparatedPolygonCount;
			*outPolygonInfoPtr = newSeparatedPolygonInfoPtr;
			outPolygonPoint->array = newPolygonPointPtr;
			outPolygonPoint->num = inPointCount;

			newSeparatedPolygonInfoPtr = NULL;
			newPolygonPointPtr = NULL;

			succeed = ZTRUE;
			break;
		}

		for (ZUINT32 i = 0; i < inPointCount; i++)
		{
			indexSet.push_back(i);
		}

		polygonPointCount = inPointCount + inSeparatorCount * 2;
		dataLength = sizeof(ZINT8) * 2 * polygonPointCount;
		break_if(dataLength == 0);

		newPolygonPointPtr = new ZINT8[polygonPointCount * 2];
		break_if(newPolygonPointPtr == NULL);

		newSeparatedPolygonCount = inSeparatorCount + 1;
		dataLength = sizeof(ZNMapPolygonInfo) * newSeparatedPolygonCount;
		break_if(dataLength == 0);

		newSeparatedPolygonInfoPtr = new ZNMapPolygonInfo[newSeparatedPolygonCount];
		break_if(newSeparatedPolygonInfoPtr == NULL);

		separatorIndex = 0;
		dstPointPtr = newPolygonPointPtr;

		startIndex = inSeparatorDataPtr[separatorIndex].mStartIndex;
		endIndex = inSeparatorDataPtr[separatorIndex].mEndIndex;
		break_if(startIndex == endIndex);

		if (startIndex < endIndex)
		{
			srcPointPtr = inPointPtr + startIndex * 2;
			copyPointCount = endIndex - startIndex + 1;
			break_if(copyPointCount < 3);
			memcpy(dstPointPtr, srcPointPtr, sizeof(ZINT8) * 2 * copyPointCount);

			dstPointPtr += (copyPointCount * 2);
			polyCount = copyPointCount;

			// Set separated polygon info

			newSeparatedPolygonInfoPtr[separatorIndex].mOffset = polyOffset;
			newSeparatedPolygonInfoPtr[separatorIndex].mCount = polyCount;

			polyOffset += polyCount;

			// Update index set
			removeIndexesInRange(indexSet, startIndex + 1, copyPointCount - 2);
		}
		else if (endIndex < startIndex)
		{
			polyCount = 0;

			// start -> last

			srcPointPtr = inPointPtr + startIndex * 2;
			copyPointCount = inPointCount - startIndex;
			memcpy(dstPointPtr, srcPointPtr, sizeof(ZINT8) * 2 * copyPointCount);

			dstPointPtr += (copyPointCount * 2);
			polyCount += copyPointCount;

			// first -> end

			srcPointPtr = inPointPtr;
			copyPointCount = endIndex + 1;
			memcpy(dstPointPtr, srcPointPtr, sizeof(ZINT8) * 2 * copyPointCount);

			dstPointPtr += (copyPointCount * 2);
			polyCount += copyPointCount;

			// Set separated polygon info

			newSeparatedPolygonInfoPtr[separatorIndex].mOffset = polyOffset;
			newSeparatedPolygonInfoPtr[separatorIndex].mCount = polyCount;

			polyOffset += polyCount;

			// Update index set

			if (startIndex < inPointCount - 1)
			{
				removeIndexesInRange(indexSet, startIndex + 1, inPointCount - startIndex -1);
			}

			if (0 < endIndex)
			{
				removeIndexesInRange(indexSet, 0, endIndex);
			}
		}

		for (separatorIndex = 1; separatorIndex < inSeparatorCount; separatorIndex ++)
		{
			polyCount = 0;

			// Read start index and end index

			startIndex = inSeparatorDataPtr[separatorIndex].mStartIndex;
			endIndex = inSeparatorDataPtr[separatorIndex].mEndIndex;
			break_if(endIndex == startIndex);

			if (startIndex < endIndex)
			{
				for (targetIndex = startIndex; targetIndex <= endIndex; targetIndex ++)
				{
					if ( isIndexInSet(indexSet, targetIndex) )
					{
						srcPointPtr = inPointPtr + targetIndex * 2;
						memcpy(dstPointPtr, srcPointPtr, sizeof(ZINT8) * 2);
						dstPointPtr += 2;
						polyCount ++;
					}
				}

				// Update index set
				removeIndexesInRange(indexSet, startIndex + 1, endIndex - startIndex -1);
			}
			else if (endIndex < startIndex)
			{
				for (targetIndex = startIndex; targetIndex <= inPointCount - 1; targetIndex ++)
				{
					if (isIndexInSet(indexSet, targetIndex))
					{
						srcPointPtr = inPointPtr + targetIndex * 2;
						memcpy(dstPointPtr, srcPointPtr, sizeof(ZINT8) * 2);
						dstPointPtr += 2;
						polyCount ++;
					}
				}

				for (targetIndex = 0; targetIndex <= endIndex; targetIndex ++)
				{
					if (isIndexInSet(indexSet, targetIndex))
					{
						srcPointPtr = inPointPtr + targetIndex * 2;
						memcpy(dstPointPtr, srcPointPtr, sizeof(ZINT8) * 2);
						dstPointPtr += 2;
						polyCount ++;
					}
				}

				// Update index set

				if (startIndex < inPointCount - 1)
				{
					removeIndexesInRange(indexSet, startIndex + 1, inPointCount - startIndex -1);
				}

				if (0 < endIndex)
				{
					removeIndexesInRange(indexSet, 0, endIndex);
				}
			}

			// Set separated polygon info

			newSeparatedPolygonInfoPtr[separatorIndex].mOffset = polyOffset;
			newSeparatedPolygonInfoPtr[separatorIndex].mCount = polyCount;

			polyOffset += polyCount;
		}
		break_if(separatorIndex < inSeparatorCount);

		// Remain polygon
		{
			polyCount = 0;

			for (CP_VECTOR<ZUINT32>::iterator it = indexSet.begin(); it != indexSet.end(); it++)
			{
				targetIndex = *it;
				srcPointPtr = inPointPtr + targetIndex * 2;
				memcpy(dstPointPtr, srcPointPtr, sizeof(ZINT8) * 2);
				dstPointPtr += 2;
				polyCount ++;
			}

			// Set separated polygon info

			newSeparatedPolygonInfoPtr[newSeparatedPolygonCount - 1].mOffset = polyOffset;
			newSeparatedPolygonInfoPtr[newSeparatedPolygonCount - 1].mCount = polyCount;

			polyOffset += polyCount;
		}

		// succeed

		*outPolygonCount = newSeparatedPolygonCount;
		*outPolygonInfoPtr = newSeparatedPolygonInfoPtr;
		outPolygonPoint->array = newPolygonPointPtr;
		outPolygonPoint->num = polygonPointCount;

		newSeparatedPolygonInfoPtr = NULL;
		newPolygonPointPtr = NULL;

		succeed = ZTRUE;

	} while (0);

	if (newSeparatedPolygonInfoPtr != NULL)
	{
		free(newSeparatedPolygonInfoPtr);
		newSeparatedPolygonInfoPtr = NULL;
	}

	if (newPolygonPointPtr != NULL)
	{
		free(newPolygonPointPtr);
		newPolygonPointPtr = NULL;
	}

	return succeed;
}

static inline void
ZNMapPolygonCopyPoint(ZINT8 *dstPtr, const ZINT8 *srcPtr, ZUINT8 count)
{
	ZUINT8 index;
	ZINT8 iValue;
	for (index = 0; index < count; index ++)
	{
        iValue = (ZINT8)(*srcPtr);
		*dstPtr = iValue;
		srcPtr += 1;
		dstPtr ++;
        iValue = (ZINT8)(*srcPtr);
		*dstPtr = iValue;
		srcPtr += 1; 
		dstPtr ++;
	}
}

ZBOOL
ZNMapPolygonCreateSeparatedPolygon8(
									ZUINT8                     inPointCount,
									const ZINT8                *inPointPtr,
									ZUINT8                     inSeparatorCount,
									const ZNMapSeparatorInfo    *inSeparatorDataPtr,
									ZUINT16                    *outPolygonCount,
									ZNMapPolygonInfo            **outPolygonInfoPtr,
									ZNInt8Section				*outPolygonPoint
									)
{
	ZUINT16 newSeparatedPolygonCount = 0;
	ZNMapPolygonInfo *newSeparatedPolygonInfoPtr = NULL;
	ZINT8 *newPolygonPointPtr = NULL;

	CP_VECTOR<ZUINT32> indexSet;
	ZUINT32 targetIndex;

	ZUINT32 polygonPointCount;
	ZUINT32 dataLength;
	ZUINT8 separatorIndex;
	ZUINT8 startIndex, endIndex;
	ZUINT16 polyOffset = 0, polyCount = 0;
	ZUINT32 copyPointCount;
	ZINT8 *dstPointPtr = NULL;
	const ZINT8 *srcPointPtr = NULL;
	ZUINT32 i;

	ZBOOL succeed = ZFALSE;

	do {
		break_if(inPointCount == 0);
		break_if(inPointPtr == NULL);

		break_if(inSeparatorCount == 0);
		break_if(inSeparatorDataPtr == NULL);

		break_if(outPolygonCount == 0);
		break_if(outPolygonInfoPtr == NULL);
		break_if(outPolygonPoint == NULL);

		for (i = 0; i < inPointCount; i++)
		{
			indexSet.push_back(i);
		}

		polygonPointCount = inPointCount + inSeparatorCount * 2;
		dataLength = sizeof(ZINT8) * 2 * polygonPointCount;
		break_if(dataLength == 0);

		newPolygonPointPtr = new ZINT8[polygonPointCount * 2];
		break_if(newPolygonPointPtr == NULL);

		newSeparatedPolygonCount = inSeparatorCount + 1;
		dataLength = sizeof(ZNMapPolygonInfo) * newSeparatedPolygonCount;
		break_if(dataLength == 0);

		newSeparatedPolygonInfoPtr = new ZNMapPolygonInfo[newSeparatedPolygonCount];
		break_if(newSeparatedPolygonInfoPtr == NULL);

		// Read separator info and separate polygon

		separatorIndex = 0;
		dstPointPtr = newPolygonPointPtr;

		startIndex = inSeparatorDataPtr[separatorIndex].mStartIndex;
		endIndex = inSeparatorDataPtr[separatorIndex].mEndIndex;
		break_if(startIndex == endIndex);

		if (startIndex < endIndex)
		{
			srcPointPtr = inPointPtr + startIndex * 2;
			copyPointCount = endIndex - startIndex + 1;
			break_if(copyPointCount < 3);

			ZNMapPolygonCopyPoint(dstPointPtr, srcPointPtr, copyPointCount);

			dstPointPtr += (copyPointCount * 2);
			polyCount = copyPointCount;

			// Set separated polygon info

			newSeparatedPolygonInfoPtr[separatorIndex].mOffset = polyOffset;
			newSeparatedPolygonInfoPtr[separatorIndex].mCount = polyCount;

			polyOffset += polyCount;

			// Update index set
			removeIndexesInRange(indexSet, startIndex + 1, copyPointCount - 2);
		}
		else if (endIndex < startIndex)
		{
			polyCount = 0;

			// start -> last

			srcPointPtr = inPointPtr + startIndex * 2;
			copyPointCount = inPointCount - startIndex;
			ZNMapPolygonCopyPoint(dstPointPtr, srcPointPtr, copyPointCount);

			dstPointPtr += (copyPointCount * 2);
			polyCount += copyPointCount;

			// first -> end

			srcPointPtr = inPointPtr;
			copyPointCount = endIndex + 1;
			ZNMapPolygonCopyPoint(dstPointPtr, srcPointPtr, copyPointCount);

			dstPointPtr += (copyPointCount * 2);
			polyCount += copyPointCount;

			// Set separated polygon info

			newSeparatedPolygonInfoPtr[separatorIndex].mOffset = polyOffset;
			newSeparatedPolygonInfoPtr[separatorIndex].mCount = polyCount;

			polyOffset += polyCount;

			// Update index set

			if (startIndex < inPointCount - 1)
			{
				removeIndexesInRange(indexSet, startIndex + 1, inPointCount - startIndex -1);
			}

			if (0 < endIndex)
			{
				removeIndexesInRange(indexSet, 0, endIndex);
			}
		}

		for (separatorIndex = 1; separatorIndex < inSeparatorCount; separatorIndex ++)
		{
			polyCount = 0;

			// Read start index and end index

			startIndex = inSeparatorDataPtr[separatorIndex].mStartIndex;
			endIndex = inSeparatorDataPtr[separatorIndex].mEndIndex;
			break_if(endIndex == startIndex);

			if (startIndex < endIndex)
			{
				for (targetIndex = startIndex; targetIndex <= endIndex; targetIndex ++)
				{
					if ( isIndexInSet(indexSet, targetIndex))
					{
						srcPointPtr = inPointPtr + targetIndex * 2;
						ZNMapPolygonCopyPoint(dstPointPtr, srcPointPtr, 1);
						dstPointPtr += 2;
						polyCount ++;
					}
				}

				// Update index set
				removeIndexesInRange(indexSet, startIndex + 1, endIndex - startIndex -1);
			}
			else if (endIndex < startIndex)
			{
				for (targetIndex = startIndex; targetIndex <= inPointCount - 1; targetIndex ++)
				{
					if ( isIndexInSet(indexSet, targetIndex) )
					{
						srcPointPtr = inPointPtr + targetIndex * 2;
						ZNMapPolygonCopyPoint(dstPointPtr, srcPointPtr, 1);
						dstPointPtr += 2;
						polyCount ++;
					}
				}

				for (targetIndex = 0; targetIndex <= endIndex; targetIndex ++)
				{
					if ( isIndexInSet(indexSet, targetIndex) )
					{
						srcPointPtr = inPointPtr + targetIndex * 2;
						ZNMapPolygonCopyPoint(dstPointPtr, srcPointPtr, 1);
						dstPointPtr += 2;
						polyCount ++;
					}
				}

				// Update index set

				if (startIndex < inPointCount - 1)
				{
					removeIndexesInRange(indexSet, startIndex + 1, inPointCount - startIndex -1);
				}

				if (0 < endIndex)
				{
					removeIndexesInRange(indexSet, 0, endIndex);
				}
			}

			// Set separated polygon info

			newSeparatedPolygonInfoPtr[separatorIndex].mOffset = polyOffset;
			newSeparatedPolygonInfoPtr[separatorIndex].mCount = polyCount;

			polyOffset += polyCount;
		}
		break_if(separatorIndex < inSeparatorCount);

		// Remain polygon
		{
			polyCount = 0;
			for(CP_VECTOR<ZUINT32>::iterator it = indexSet.begin(); it != indexSet.end(); it++)
			{
				targetIndex = *it;
				srcPointPtr = inPointPtr + targetIndex * 2;
				ZNMapPolygonCopyPoint(dstPointPtr, srcPointPtr, 1);
				dstPointPtr += 2;
				polyCount ++;
			}

			// Set separated polygon info

			newSeparatedPolygonInfoPtr[newSeparatedPolygonCount - 1].mOffset = polyOffset;
			newSeparatedPolygonInfoPtr[newSeparatedPolygonCount - 1].mCount = polyCount;
			polyOffset += polyCount;
		}

		// succeed

		*outPolygonCount = newSeparatedPolygonCount;
		*outPolygonInfoPtr = newSeparatedPolygonInfoPtr;
		if (outPolygonPoint != NULL)
		{
			outPolygonPoint->array = newPolygonPointPtr;
			outPolygonPoint->num = polygonPointCount;
		}

		newSeparatedPolygonInfoPtr = NULL;
		newPolygonPointPtr = NULL;

		succeed = ZTRUE;

	} while (0);

	if (newSeparatedPolygonInfoPtr != NULL)
	{
		free(newSeparatedPolygonInfoPtr);
		newSeparatedPolygonInfoPtr = NULL;
	}

	if (newPolygonPointPtr != NULL)
	{
		free(newPolygonPointPtr);
		newPolygonPointPtr = NULL;
	}

	return succeed;
}

typedef ZFLOAT32 SP_COORD_T;

typedef struct {
	SP_COORD_T x;
	SP_COORD_T y;
} SP_POINT;

typedef struct {
	ZUINT8 from;
	ZUINT8 to;
} SP_SPLIT;

/**
* @brief  インデックスを巡回させる（1順のみ）
* @param  int   index      インデックス
* @param  int   size       バッファサイズ
* @return ポインタを返す
*/
static inline int round_ind(int index, int size) {
	if (index < 0) return index + size;
	if (index >= size) return index - size;
	return index;
}

typedef ZINT8 SP_COORD_T_8;

typedef struct {
	SP_COORD_T_8 x;
	SP_COORD_T_8 y;
} SP_POINT_8;

/**
* @brief  ベクトル p1 -> p2 に対し、p3 が左右いずれかであるかを返す。
* @param  SP_POINT* p1    p1
* @param  SP_POINT* p2    p2
* @param  SP_POINT* p3    p3
* @retval  1 右方向
* @retval -1 左方向
* @retval  0 同一線上
*/
static inline int get_direction_8(const SP_POINT_8* p1, const SP_POINT_8* p2, const SP_POINT_8* p3)
{
	// 有向線分 (p1 -> p3), (p1 -> p2) の外積を求める
	int n = p3->x * (p1->y - p2->y) + p1->x * (p2->y - p3->y) + p2->x * (p3->y - p1->y);
	if      (n > 0) return -1;  // 左
	else if (n < 0) return  1;  // 右
	else            return  0;  // 線上
}

/**
* @brief  凸ポリゴンとポリゴン外頂点の内外判定
* @param  int         numof_point[in]    頂点ポインタ配列要素数
* @param  SP_POINT**  point_of[in]       頂点ポインタ配列要素数
* @param  int         ind_vt[in]         ポリゴン頂点開始インデックス
* @param  int         numof_vt[in]       ポリゴン頂点数
* @return 凸ポリゴンで且つ、その他の頂点がすべて凸ポリゴン領域外の場合に 1 を返す
*/
static int test_polygon_8(int numof_point, const SP_POINT_8** point_of, int ind_vt, int numof_vt)
{
	int i, j;
	for (i = 0; i < numof_vt; i++) {
		int curr = ind_vt + round_ind(i, numof_vt);
		int prev = ind_vt + round_ind(i - 1, numof_vt);
		int next = ind_vt + round_ind(i + 1, numof_vt);
		// printf("%d,%d,%d\n", prev, curr, next);
		if (get_direction_8(point_of[prev], point_of[curr], point_of[next]) < 0.0) {
			/* ベクトルが左向きの場合は凹ポリゴンのため偽を返す */
			// printf("left\n");
			return 0;
		}
	}
	// printf("closed\n");
	for (j = 0; j < numof_point - numof_vt; j++) {
		int ind_pt = round_ind(ind_vt + numof_vt + j, numof_point);
		int outside = 0;
		for (i = 0; i < numof_vt; i++) {
			int curr = ind_vt + round_ind(i, numof_vt);
			int next = ind_vt + round_ind(i + 1, numof_vt);
			if (get_direction_8(point_of[curr], point_of[next], point_of[ind_pt]) < 0.0) {
				/* 評価点が左側の場合は外側にあるため、内外判定を終える */
				outside = 1;
				break;
			}
		}
		// printf("check %d\n", ind_pt);
		if (!outside) {
			// 外側に無ければ偽を返す
			// printf("inside\n");
			return 0;
		}
	}
	// printf("splitable\n");
	return 1;
}

/**
* @brief  凹ポリゴンを凸ポリゴンに分割し、分割線情報を返す
* @param  int        numof_point[in]    点数
* @param  SP_POINT*  points[in]         ポリゴン点列
* @param  int        sizeof_buffer[in]  バッファサイズ
* @param  SP_SPLIT*  result_buffer[out] 分割頂点返却バッファ
* @return 分割頂点数を返す
*/
int split_polygon_8(int numof_point, const SP_POINT_8* points, int sizeof_buffer, SP_SPLIT* result_buffer)
{
	if (numof_point <= 3) {
		return 0;
	}
	int i;

	// 頂点インデックスを作成
	int*       index_of = (int*)malloc(sizeof(int) * numof_point);
	const SP_POINT_8** point_of = (const SP_POINT_8**)malloc(sizeof(SP_POINT_8*) * numof_point);
	if (!index_of || !point_of) {
		if (index_of) free(index_of);
		if (point_of) free(point_of);
		return 0;
	}
	for (i = 0; i < numof_point; i++) {
		index_of[i] = i;
		point_of[i] = &points[i];
	}

	int numof_split = 0;
	int p, from, to;
	for (p = numof_point - 1; p >= 0; p--) {
		int pp = round_ind(p - 1, numof_point);
		int np = round_ind(p + 1, numof_point);
		if (get_direction_8(point_of[pp], point_of[p], point_of[np]) < 0) {
			// 頂点のベクトルが左向きの場合は凹頂点
			// printf("found vertex: %d\n", p);

			from = to = p;
			// 後方の分割線を3頂点から範囲を広げながら探索
			int numof_vt = 3;
			while (p + numof_vt < numof_point &&
				test_polygon_8(numof_point, point_of, p, numof_vt)) {
					// 分割線を仮設定
					to = p + numof_vt - 1;
					// printf("found split: %d to %d\n", index_of[from], index_of[to]);
					numof_vt++;
			}
			if (from == to) {
				// 後方に見つからなかった場合
				// 前方の分割線を3頂点から範囲を広げながら探索
				int numof_vt = 3;
				int polygon_ind = p - numof_vt + 1;
				while (polygon_ind >= 0 &&
					test_polygon_8(numof_point, point_of, polygon_ind, numof_vt)) {
						// 分割線を仮設定
						from = polygon_ind;
						numof_vt++;
						polygon_ind--;
				}
			}
			if (from != to) {
				// 分割線が設定された場合
				if (numof_split < sizeof_buffer) {
					// 分割線情報を出力
					result_buffer[numof_split].from = index_of[from];
					result_buffer[numof_split].to = index_of[to];
				}
				int numof_vt = to - from - 1;
				while (to < numof_point) {
					// 頂点インデックスを詰める
					index_of[to - numof_vt] = index_of[to];
					point_of[to - numof_vt] = point_of[to];
					to++;
				}
				numof_point -= numof_vt;
				numof_split++;
				p = numof_point;
			}
		}
	}
	free(point_of);
	free(index_of);
	return numof_split;
}

extern ZBOOL
ZNMapPolygonSplitPolygon(
						  ZUINT8                     inPointCount,
						  const ZINT8                *inPointPtr,
						  ZUINT8                     *outSeparatorCount,
						  ZNMapSeparatorInfo          **outSeparatorInfoPtr)
{
	static SP_SPLIT tempBuffer[256];
	ZINT32 splitCount;
	ZUINT32 dataLength;
	ZNMapSeparatorInfo *dataPtr = NULL;
	ZBOOL succeed = ZFALSE;

	do {
		break_if(outSeparatorCount == NULL || outSeparatorInfoPtr == NULL);

		splitCount = split_polygon_8(inPointCount, (const SP_POINT_8*)inPointPtr, 256, tempBuffer);
		if (splitCount <= 0)
		{
			break;
		}

		if (255 < splitCount)
		{
			//report_msg("splitCount:%d\n", splitCount);
			break;
		}

		dataLength = sizeof(ZNMapSeparatorInfo) * splitCount;
	//	dataPtr = (ZNMapSeparatorInfo*)malloc(dataLength);
		dataPtr = new ZNMapSeparatorInfo[splitCount];
		break_if(dataPtr == NULL);

		memcpy(dataPtr, tempBuffer, dataLength);

		*outSeparatorCount = splitCount;
		*outSeparatorInfoPtr = dataPtr;
		dataPtr = NULL;

		succeed = ZTRUE;

	} while (0);

	safe_delete_array(dataPtr);

	return succeed;
}

////edited by Copy 20110309 for convert point to 3D
extern ZVOID 
ConvertPointTo3D(ZFLOAT32 *InThePointListPtr ,ZFLOAT32 *OutThePointFor3D, ZUINT8 DR_PointCount) //傳入 2D座標 3D座標 點個數
{

	int PointFor3DIndexCount=0;

	for(int i=0;i<DR_PointCount*2;i++){
		if(i==0){         //第一個x座標
			OutThePointFor3D[PointFor3DIndexCount]=InThePointListPtr[i];
			PointFor3DIndexCount++;

		}
		else {
			if(i%2==0){    //2,4,6,8,10... x座標
				OutThePointFor3D[PointFor3DIndexCount]=InThePointListPtr[i];
				PointFor3DIndexCount++;

			}
			if(i%2!=0){         //1,3,5,7,9...  y座標
				OutThePointFor3D[PointFor3DIndexCount]=InThePointListPtr[i];

				//test
				//NSLog(@"y point:%f",InThePointListPtr[i]);

				PointFor3DIndexCount++;

				//Z座標給值
				//OutThePointFor3D[PointFor3DIndexCount]=InThePointListPtr[i]; //z座標給入y的值
				OutThePointFor3D[PointFor3DIndexCount]= 0.0; 
				PointFor3DIndexCount++;

			}
		}
	}
}

ZVOID convertStandBlockXY2LL(const ZNInt8Section* srcPoint, ZNFloatSection *dstPoint,
                                    char kind, ZUINT32 blockX, ZUINT32 blockY)
{
    do 
    {
        break_if(!srcPoint);
        break_if(!dstPoint);
        ZINT32 minX, minY, maxX, maxY; 
        ZUINT8 level = ZNMapLevelFromKind(kind);
        ZNMapGetLLOfBlockRect(level, blockX, blockY, minX, minY, maxX, maxY);
        ZFLOAT32 *pointPtr, *dstPtr;
        pointPtr = dstPtr  = new ZFLOAT32[srcPoint->num*2];
        const ZINT8 *srcPtr = srcPoint->array;
        for (ZUINT16 index = 0; index < srcPoint->num; index ++)
        {
            *dstPtr = (((*srcPtr + 128) *(ZNMapBlockLongitude[level]/4.0))/255.00  + minX)/3600000.0;
            srcPtr ++;
            dstPtr ++;
            *dstPtr = (((*srcPtr + 128) * (ZNMapBlockLatitude[level] /4.0))/255.00 + minY) / 3600000.0;
            srcPtr ++; 
            dstPtr ++;
        }
        safe_delete_array(dstPoint->array);
        dstPoint->array = pointPtr;
        dstPoint->num = srcPoint->num;

    } while (0);
}