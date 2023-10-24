#include "ZNMapLinePolygonShape.h"
#include  "ZNMapLinePolygonShape.h"
#include "ZNMapTypes.h"
#include "QuickFunc.h"
#include "ZNMapPolygonShape.h"
#include <WinSock.h>
#include <math.h>
#include <assert.h>

#pragma comment(lib,"ws2_32")

ZNMapLinePolygonShape::~ZNMapLinePolygonShape(void)
{
}

//这个解析没怎么处理，因为之前iOS的代码里是需要读配置信息看怎么处理数据的
ZINT32 ZNMapLinePolygonShape::readDataBytes(const ZUINT8* dataPtr, ZUINT32 dataLength)
{
    ZNFloatSection *floatPointData = NULL;
    ZNFloatSection *patternPointData = NULL;
    const ZUINT8 *ptr = NULL;
    ZUINT32 remain;
    ZUINT32 pointCount;
    ZINT32 readLength = -1;

    do {
        if (dataPtr == NULL || dataLength == 0)
        {
			printf("error\n");
            break;
        }
        
        ptr = dataPtr;
        remain = dataLength;
        
        if (remain < 1)
        {
            printf("error\n");
            break;
        }
        
        pointCount = (ZUINT32)*ptr;
        ptr ++;
        remain --;
        
        if (pointCount < 2)
        {
            printf("error\n");
            break;
        }
        
        ZUINT32 memLength = sizeof(ZINT8) * pointCount * 2;
        ZINT16 *tempPtr = NULL;
		ZINT16* pointPtr = new ZINT16[pointCount * 2];
        break_if(pointPtr == NULL);
        tempPtr = pointPtr;
    } while (0);
   
    return readLength;
}

static ZVOID
dumpPointS8(const ZINT8 *ptr, ZUINT32 count)
{
    ZUINT32 index;
    int x, y;
     
    for (index = 0; index < count; index ++)
    {
        x = *ptr + 128;
		ptr ++;
        y = *ptr + 128; 
		ptr ++;
    }
}

static void
dumpPointFloat(const ZFLOAT32 *ptr, ZUINT32 count)
{
    ZUINT32 index;
    ZFLOAT32 x, y;
        
    for (index = 0; index < count; index ++)
    {
        x = *ptr; 
		ptr ++;
        y = *ptr;
		ptr ++;
    }
}

extern ZNFloatSection*
floatPointDataFromPoints(const ZINT8 *ptr, ZUINT32 pointCount)
{
    ZUINT32 pointIndex;
    
	ZFLOAT32 *pointPtr = NULL, *pointPos = NULL;
	//dataLength = sizeof(ZFLOAT32) * pointCount * 2;

	pointPtr = new ZFLOAT32[pointCount * 2];
	assert(pointPtr);

	pointPos = pointPtr;

	for (pointIndex = 0; pointIndex < pointCount; pointIndex ++)
	{
		// ( -128 ~ 127 ) >> ( 0.0 ~ 1.25 )
		// ( -128 ~ 127 ) >> ( 0.0 ~ 1.0 )
		*pointPos = (ZFLOAT32)( ( *ptr + 128 ) * kZNMapBlockScaleX100 ) / (ZFLOAT32)kZNMapBlockDataWidth100;
		ptr ++;
		pointPos ++;
		*pointPos = (ZFLOAT32)( ( *ptr + 128 ) * kZNMapBlockScaleY100 ) / (ZFLOAT32)kZNMapBlockDataHeight100;
		ptr ++;
		pointPos ++;
	}
	ZNFloatSection *floatSection = new ZNFloatSection;
	floatSection->array = pointPtr;
	floatSection->num = pointPos - pointPtr;
    return floatSection;
}

static ZNFloatSection*
polygonPointDataFromFloatPoints(ZFLOAT32 lineWidth, const ZFLOAT32* points, ZUINT32 pointCount)
{
	ZNFloatPoint point1, point2, tempPoint;
    ZFLOAT32 x, y;
    ZUINT32 lineIndex, lineCount;
    ZUINT32 polygonPointCount;
    size_t dataLength;
    
    ZFLOAT32 *polygonPtr, *polygonPos;
    ZFLOAT32 theta;
    const ZFLOAT32 *pointPos;
    ZBOOL exchange;
    
    if (pointCount < 2)
    {
        return NULL;
    }
    
    lineCount = pointCount >> 1;
    
    if (lineCount == 1)
    {
        polygonPointCount = 4;
    }
    else
    {
        polygonPointCount = 2 * 5 + (lineCount - 2) * 6;
    }
    
    dataLength = sizeof(ZFLOAT32) * polygonPointCount * 2;
    
    polygonPtr = new ZFLOAT32[polygonPointCount];
    assert(polygonPtr);
    
    pointPos = points;
    polygonPos = polygonPtr;
    
    for (lineIndex = 0; lineIndex < lineCount; lineIndex ++)
    {
        /***
         * x: ( -128 ~ 127 ) >> ( 0.0 ~ 1.25 )
         * y: ( -128 ~ 127 ) >> ( 0.0 ~ 1.0 )
         */
        point1.x = *pointPos; pointPos ++;
        point1.y = *pointPos; pointPos ++;
        
        point2.x = *pointPos; pointPos ++;
        point2.y = *pointPos; pointPos ++;
        
        exchange = point2.x < point1.x;
        if (exchange)
        {
            /*
            
             pt2 +  |
                  \ |   
                   \|
                  --+-
                   pt1
                    
                   pt1
                  --+-
                   /|
                  / |
             pt2 +  |
             
            */
            tempPoint = point1;
            point1 = point2;
            point2 = tempPoint;
        }
        
        if (point2.x == point1.x)
        {
            if (point1.y < point2.y)
            {
                theta = M_PI_2;
            }
			
            else if (point2.y < point1.y)
            {
                theta = - M_PI_2;
            }
            else
            {
                theta = 0.0;
            }
        }
        else
        {
            theta = atanf((point2.y - point1.y) / (point2.x - point1.x));
        }
        
        x = sinf(theta);
        y = cosf(theta);
        
        /***
         * 線幅が 204.0 ... 0.5
         * 線幅が 102.0 ... 0.25
         */
        x *= (lineWidth / (ZFLOAT32)(kZNMapBlockPixel << 1));
        y *= (lineWidth / (ZFLOAT32)(kZNMapBlockPixel << 1));
        
        /*
          |  + pt2
          | /   
          |/
        --+-----
         pt1
        */
        
        /* 
         pt1
         -+------
          |\
          | \
          |  + pt2
          
         x < 0, 0 < y
        */
        
        /* 先頭以外 */
        if (exchange)
        {
            if (0 < lineIndex)
            {
                *polygonPos = point2.x - x; polygonPos ++;
                *polygonPos = point2.y + y; polygonPos ++;
            }
            
            
            *polygonPos = point2.x - x; polygonPos ++;
            *polygonPos = point2.y + y; polygonPos ++;

            
            *polygonPos = point2.x + x; polygonPos ++;
            *polygonPos = point2.y - y; polygonPos ++;
            
            *polygonPos = point1.x - x; polygonPos ++;
            *polygonPos = point1.y + y; polygonPos ++;
            
            *polygonPos = point1.x + x; polygonPos ++;
            *polygonPos = point1.y - y; polygonPos ++;
            
            /* 最後部以外 */
            if ((lineIndex + 1) != lineCount)
            {
                *polygonPos = point1.x + x; polygonPos ++;
                *polygonPos = point1.y - y; polygonPos ++;
            }
        }
        else
        {
            if (0 < lineIndex)
            {
                *polygonPos = point1.x - x; polygonPos ++;
                *polygonPos = point1.y + y; polygonPos ++;
            }
            
            *polygonPos = point1.x - x; polygonPos ++;            
            *polygonPos = point1.y + y; polygonPos ++;

            
            *polygonPos = point1.x + x; polygonPos ++;
            *polygonPos = point1.y - y; polygonPos ++;
            
            *polygonPos = point2.x - x; polygonPos ++;
            *polygonPos = point2.y + y; polygonPos ++;
            
            *polygonPos = point2.x + x; polygonPos ++;
            *polygonPos = point2.y - y; polygonPos ++;
            
            /* 最後部以外 */
            if ((lineIndex + 1) != lineCount)
            {
                *polygonPos = point2.x + x; polygonPos ++;
                *polygonPos = point2.y - y; polygonPos ++;
            }
        }
    }
    
	ZNFloatSection *floatSection = new ZNFloatSection;
	floatSection->array = polygonPtr;
	floatSection->num = polygonPos - polygonPtr;
    return floatSection;
}

static  ZNFloatSection*
polygonPointJointedDataFromPoints(ZFLOAT32 lineWidth, const ZINT8 *points, ZUINT32 pointCount)
{
    ZNFloatPoint point1, point2, tempPoint, lastPoint;
    ZFLOAT32 x, y;
    ZUINT32 lineIndex, lineCount;
    ZUINT32 jointIndex, jointCount, jointOffset;
    ZUINT32 pointIndex;
    ZUINT32 polygonPointCount;
    size_t dataLength;
    ZFLOAT32 *polygonPtr, *polygonPos, *jointPos;
    ZFLOAT32 *jointPrevPtr, *jointNextPtr, *jointLastPtr;
    ZFLOAT32 theta;
    const ZINT8 *pointPos;
    ZBOOL exchange;
    
    if (pointCount < 2)
    {
        int wwwwwww;
        wwwwwww = 0;
        return NULL;
    }
    
    lineCount = pointCount - 1;
    
    if (lineCount == 1)
    {
        polygonPointCount = 4;
        
        jointCount = 0;
        jointOffset = 0;
    }
    else /* ( 2 <= lineCount ) */
    {
        polygonPointCount = (2 * 5) + ((lineCount - 2) * 6);
        
        jointCount = lineCount - 1;
        jointOffset = (polygonPointCount + 2) * 2;
        
        if (jointCount == 1)
        {
            polygonPointCount += (2 + 4);
        }
        else /* ( 2 <= jointCount ) */
        {
            polygonPointCount += (2 + (2 * 5) + ((jointCount - 2) * 6));
        }
    }
    
    dataLength = sizeof(ZFLOAT32) * polygonPointCount * 2;
    
    polygonPtr = new ZFLOAT32[polygonPointCount * 2];
    assert(polygonPtr);
    
    pointPos = points;
    polygonPos = polygonPtr;
    jointPos = polygonPtr + jointOffset;
    jointIndex = 0;
    
    /***
     * x: ( -128 ~ 127 ) >> ( 0.0 ~ 1.25 )
     * y: ( -128 ~ 127 ) >> ( 0.0 ~ 1.0 )
     */
    lastPoint.x = (ZFLOAT32)( (*pointPos + 128) * kZNMapBlockScaleX100 ) / (ZFLOAT32)kZNMapBlockDataWidth100; pointPos ++;
    lastPoint.y = (ZFLOAT32)( (*pointPos + 128) * kZNMapBlockScaleY100 ) / (ZFLOAT32)kZNMapBlockDataHeight100; pointPos ++;
    
    for (pointIndex = 1, lineIndex = 0; pointIndex < pointCount; pointIndex ++, lineIndex ++)
    {
        /***
         * x: ( -128 ~ 127 ) >> ( 0.0 ~ 1.25 )
         * y: ( -128 ~ 127 ) >> ( 0.0 ~ 1.0 )
         */
        point1 = lastPoint;
        
        point2.x = (ZFLOAT32)( (*pointPos + 128) * kZNMapBlockScaleX100 ) / (ZFLOAT32)kZNMapBlockDataWidth100; pointPos ++;
        point2.y = (ZFLOAT32)( (*pointPos + 128) * kZNMapBlockScaleY100 ) / (ZFLOAT32)kZNMapBlockDataHeight100; pointPos ++;
        
        lastPoint = point2;
        
        /*
         
         theta が (PI / 2) ~ (PI) ~ (3 PI / 2) の間にある場合は
         point1 と point2 を交換して
         ( PI / 2 ) ~ ( 0 ) ~ ( - PI / 2 ) の間になるようにする
          
         pt2 +  |
              \ |   
               \|
              --+-
               pt1
                
               pt1
              --+-
               /|
              / |
         pt2 +  |
         
        */
        exchange = point2.x < point1.x;
        if (exchange)
        {
            tempPoint = point1;
            point1 = point2;
            point2 = tempPoint;
        }
        
        if (point2.x == point1.x)
        {
            if (point1.y < point2.y)
            {
                theta = M_PI_2;
            }
            else if (point2.y < point1.y)
            {
                theta = - M_PI_2;
            }
            else
            {
                theta = 0.0;
            }
        }
        else
        {
            theta = atanf((point2.y - point1.y) / (point2.x - point1.x));
        }
        
        x = sinf(theta);
        y = cosf(theta);
        
        /***
         * 線幅が 204.0 ... 0.5
         * 線幅が 102.0 ... 0.25
         */
        x *= (lineWidth / (ZFLOAT32)(kZNMapBlockPixel << 1));
        y *= (lineWidth / (ZFLOAT32)(kZNMapBlockPixel << 1));
        
        /*
          |  + pt2
          | /   
          |/
        --+-----
         pt1
        */
        
        /* 
         pt1
         -+------
          |\
          | \
          |  + pt2
          
         x < 0, 0 < y
        */
        
        /* 先頭以外 */
        if (0 < lineIndex)
        {
            *polygonPos = point1.x - x; polygonPos ++;
            *polygonPos = point1.y + y; polygonPos ++;

            
        }
        
        *polygonPos = point1.x - x; polygonPos ++;
        *polygonPos = point1.y + y; polygonPos ++;
        
        *polygonPos = point1.x + x; polygonPos ++;
        *polygonPos = point1.y - y; polygonPos ++;
        
        *polygonPos = point2.x - x; polygonPos ++;
        *polygonPos = point2.y + y; polygonPos ++;
        
        *polygonPos = point2.x + x; polygonPos ++;
        *polygonPos = point2.y - y; polygonPos ++;
        
        /*
         
         反転している場合 (exchange == YES の場合)
         point1 が後部、point2 が前部
         
         素のままの場合 (exchange == NO の場合)
         point1 が前部、point2 が後部
         
         */
        jointPrevPtr = jointLastPtr;
        if (exchange)
        {
            jointNextPtr = (polygonPos - 4);
            jointLastPtr = (polygonPos - 8);
        }
        else
        {
            jointNextPtr = (polygonPos - 8);
            jointLastPtr = (polygonPos - 4);
        }
        
        /* 最後部以外 */
        if ((lineIndex + 1) != lineCount)
        {
            *polygonPos = point2.x + x; polygonPos ++;
            *polygonPos = point2.y - y; polygonPos ++;
        }
        
        /* ジョイント部分のポリゴン生成 */
        if (0 < lineIndex)
        {
            if (0 < jointIndex)
            {
                *jointPos = *(jointPrevPtr); jointPos ++;
                *jointPos = *(jointPrevPtr + 1); jointPos ++;
            }
            
            *jointPos = *(jointPrevPtr); jointPos ++;
            *jointPos = *(jointPrevPtr + 1); jointPos ++;
            
            *jointPos = *(jointNextPtr); jointPos ++;
            *jointPos = *(jointNextPtr + 1); jointPos ++;
            
            *jointPos = *(jointNextPtr + 2); jointPos ++;
            *jointPos = *(jointNextPtr + 3); jointPos ++;
            
            *jointPos = *(jointPrevPtr + 2); jointPos ++;
            *jointPos = *(jointPrevPtr + 3); jointPos ++;
            
            if ((jointIndex + 1) != jointCount)
            {
                *jointPos = *(jointPrevPtr + 2); jointPos ++;
                *jointPos = *(jointPrevPtr + 3); jointPos ++;
            }
            
            jointIndex ++;
        }
    }
    
    if (0 < jointCount)
    {
        *polygonPos = *(polygonPos - 2); polygonPos ++;
        *polygonPos = *(polygonPos - 2); polygonPos ++;
        *polygonPos = *(polygonPos + 2); polygonPos ++;
        *polygonPos = *(polygonPos + 2); // polygonPos ++;
    }
    
	ZNFloatSection *floatSection = new ZNFloatSection;
	floatSection->num = polygonPointCount * 2;
	floatSection->array = polygonPtr;
    return floatSection;
}

static ZBOOL
twoPolygonPointJointedDataFromPoints(ZFLOAT32 lineWidth1, ZFLOAT32 lineWidth2, const ZINT8 *points, ZUINT32 pointCount, ZNFloatSection **outData1, ZNFloatSection **outData2)
{
    ZNFloatPoint point1, point2, tempPoint, lastPoint;
    ZUINT32 lineIndex, lineCount;
    ZUINT32 jointIndex, jointCount, jointOffset;
    ZUINT32 pointIndex;
    ZUINT32 polygonPointCount;
    size_t dataLength;
    
    ZFLOAT32 x1, y1;
    ZFLOAT32 *polygonPtr1, *polygonPos1, *jointPos1;
    ZFLOAT32 *jointPrevPtr1, *jointNextPtr1, *jointLastPtr1;
    
    ZFLOAT32 x2, y2;
    ZFLOAT32 *polygonPtr2, *polygonPos2, *jointPos2;
    ZFLOAT32 *jointPrevPtr2, *jointNextPtr2, *jointLastPtr2;
    
    ZFLOAT32 theta, x, y;
    const ZINT8 *pointPos;
    ZBOOL exchange;
    
    if ((outData1 == NULL) || (outData2 == NULL))
    {
        return ZFALSE;
    }
    
    if (pointCount < 2)
    {
        return ZFALSE;
    }
    
    lineCount = pointCount - 1;
    
    if (lineCount == 1)
    {
        polygonPointCount = 4;
        
        jointCount = 0;
        jointOffset = 0;
    }
    else /* ( 2 <= lineCount ) */
    {
        polygonPointCount = (2 * 5) + ((lineCount - 2) * 6);
        
        jointCount = lineCount - 1;
        jointOffset = (polygonPointCount + 2) * 2;
        
        if (jointCount == 1)
        {
            polygonPointCount += (2 + 4);
        }
        else /* ( 2 <= jointCount ) */
        {
            polygonPointCount += (2 + (2 * 5) + ((jointCount - 2) * 6));
        }
    }
    
    dataLength = sizeof(ZFLOAT32) * polygonPointCount * 2;
    
    polygonPtr1 = new ZFLOAT32[polygonPointCount * 2];
    assert(polygonPtr1);
    
    polygonPtr2 = new ZFLOAT32[polygonPointCount * 2];
    assert(polygonPtr2);
    
    pointPos = points;
    polygonPos1 = polygonPtr1;
    polygonPos2 = polygonPtr2;
    jointPos1 = polygonPtr1 + jointOffset;
    jointPos2 = polygonPtr2 + jointOffset;
    jointIndex = 0;
    
    /***
     * x: ( -128 ~ 127 ) >> ( 0.0 ~ 1.25 )
     * y: ( -128 ~ 127 ) >> ( 0.0 ~ 1.0 )
     */
    lastPoint.x = (ZFLOAT32)( (*pointPos + 128) * kZNMapBlockScaleX100 ) / (ZFLOAT32)kZNMapBlockDataWidth100; pointPos ++;
    lastPoint.y = (ZFLOAT32)( (*pointPos + 128) * kZNMapBlockScaleY100 ) / (ZFLOAT32)kZNMapBlockDataHeight100; pointPos ++;
    
    for (pointIndex = 1, lineIndex = 0; pointIndex < pointCount; pointIndex ++, lineIndex ++)
    {
        /***
         * x: ( -128 ~ 127 ) >> ( 0.0 ~ 1.25 )
         * y: ( -128 ~ 127 ) >> ( 0.0 ~ 1.0 )
         */
        point1 = lastPoint;
        
        point2.x = (ZFLOAT32)( (*pointPos + 128) * kZNMapBlockScaleX100 ) / (ZFLOAT32)kZNMapBlockDataWidth100; pointPos ++;
        point2.y = (ZFLOAT32)( (*pointPos + 128) * kZNMapBlockScaleY100 ) / (ZFLOAT32)kZNMapBlockDataHeight100; pointPos ++;
        
        lastPoint = point2;
        
        /*
         
         theta が (PI / 2) ~ (PI) ~ (3 PI / 2) の間にある場合は
         point1 と point2 を交換して
         ( PI / 2 ) ~ ( 0 ) ~ ( - PI / 2 ) の間になるようにする
          
         pt2 +  |
              \ |   
               \|
              --+-
               pt1
                
               pt1
              --+-
               /|
              / |
         pt2 +  |
         
        */
        exchange = point2.x < point1.x;
        if (exchange)
        {
            tempPoint = point1;
            point1 = point2;
            point2 = tempPoint;
        }
        
            
        
        theta = atanf((point2.y - point1.y) / (point2.x - point1.x));
        x = sinf(theta);
        y = cosf(theta);
        
        /***
         * 線幅が 204.0 ... 0.5
         * 線幅が 102.0 ... 0.25
         */
        x1 = x * (lineWidth1 / (ZFLOAT32)(kZNMapBlockPixel << 1));    //(kZNMapBlockPixel << 1  ==   kZNMapBlockPixel x 2)
        y1 = y * (lineWidth1 / (ZFLOAT32)(kZNMapBlockPixel << 1));
        x2 = x * (lineWidth2 / (ZFLOAT32)(kZNMapBlockPixel << 1));
        y2 = y * (lineWidth2 / (ZFLOAT32)(kZNMapBlockPixel << 1));
        
        
        /*
          |  + pt2
          | /   
          |/
        --+-----
         pt1
        */
        
        /* 
         pt1
         -+------
          |\
          | \
          |  + pt2
          
         x < 0, 0 < y
        */
        
        /* 先頭以外 */
        if (0 < lineIndex)
        {
            /* 1 */
            *polygonPos1 = point1.x - x1;
			  polygonPos1 ++;
            *polygonPos1 = point1.y + y1;
			  polygonPos1 ++;
            
            /* 2 */
            *polygonPos2 = point1.x - x2; 
			  polygonPos2 ++;
            *polygonPos2 = point1.y + y2; 
			  polygonPos2 ++;
        }
        
        /* 1 */
        *polygonPos1 = point1.x - x1;
		  polygonPos1 ++;
        *polygonPos1 = point1.y + y1;
		  polygonPos1 ++;
        
        *polygonPos1 = point1.x + x1; polygonPos1 ++;
        *polygonPos1 = point1.y - y1; polygonPos1 ++;
        
        *polygonPos1 = point2.x - x1; polygonPos1 ++;
        *polygonPos1 = point2.y + y1; polygonPos1 ++;
        
        *polygonPos1 = point2.x + x1; polygonPos1 ++;
        *polygonPos1 = point2.y - y1; polygonPos1 ++;
        
        /* 2 */
        *polygonPos2 = point1.x - x2; polygonPos2 ++;
        *polygonPos2 = point1.y + y2; polygonPos2 ++;
        
        *polygonPos2 = point1.x + x2; polygonPos2 ++;
        *polygonPos2 = point1.y - y2; polygonPos2 ++;
        
        *polygonPos2 = point2.x - x2; polygonPos2 ++;
        *polygonPos2 = point2.y + y2; polygonPos2 ++;
        
        *polygonPos2 = point2.x + x2; polygonPos2 ++;
        *polygonPos2 = point2.y - y2; polygonPos2 ++;
        
        /*
         
         反転している場合 (exchange == YES の場合)
         point1 が後部、point2 が前部
         
         素のままの場合 (exchange == NO の場合)
         point1 が前部、point2 が後部
         
         */
        jointPrevPtr1 = jointLastPtr1;
        jointPrevPtr2 = jointLastPtr2;
        
        if (exchange)
        {
            jointNextPtr1 = (polygonPos1 - 4);
            jointLastPtr1 = (polygonPos1 - 8);
            jointNextPtr2 = (polygonPos2 - 4);
            jointLastPtr2 = (polygonPos2 - 8);
        }
        else
        {
            jointNextPtr1 = (polygonPos1 - 8);
            jointLastPtr1 = (polygonPos1 - 4);
            jointNextPtr2 = (polygonPos2 - 8);
            jointLastPtr2 = (polygonPos2 - 4);
        }
        
        /* 最後部以外 */
        if ((lineIndex + 1) != lineCount)
        {
            /* 1 */
            *polygonPos1 = point2.x + x1; polygonPos1 ++;
            *polygonPos1 = point2.y - y1; polygonPos1 ++;
            
            /* 2 */
            *polygonPos2 = point2.x + x2; polygonPos2 ++;
            *polygonPos2 = point2.y - y2; polygonPos2 ++;
        }
        
        /* ジョイント部分のポリゴン生成 */
        if (0 < lineIndex)
        {
            if (0 < jointIndex)
            {
                /* 1 */
                *jointPos1 = *(jointPrevPtr1); jointPos1 ++;
                *jointPos1 = *(jointPrevPtr1 + 1); jointPos1 ++;
                
                /* 2 */
                *jointPos2 = *(jointPrevPtr2); jointPos2 ++;
                *jointPos2 = *(jointPrevPtr2 + 1); jointPos2 ++;
            }
            
            /* 1 */
            *jointPos1 = *(jointPrevPtr1); jointPos1 ++;
            *jointPos1 = *(jointPrevPtr1 + 1); jointPos1 ++;
            
            *jointPos1 = *(jointNextPtr1); jointPos1 ++;
            *jointPos1 = *(jointNextPtr1 + 1); jointPos1 ++;
            
            *jointPos1 = *(jointNextPtr1 + 2); jointPos1 ++;
            *jointPos1 = *(jointNextPtr1 + 3); jointPos1 ++;
            
            *jointPos1 = *(jointPrevPtr1 + 2); jointPos1 ++;
            *jointPos1 = *(jointPrevPtr1 + 3); jointPos1 ++;
            
            /* 2 */
            *jointPos2 = *(jointPrevPtr2); jointPos2 ++;
            *jointPos2 = *(jointPrevPtr2 + 1); jointPos2 ++;
            
            *jointPos2 = *(jointNextPtr2); jointPos2 ++;
            *jointPos2 = *(jointNextPtr2 + 1); jointPos2 ++;
            
            *jointPos2 = *(jointNextPtr2 + 2); jointPos2 ++;
            *jointPos2 = *(jointNextPtr2 + 3); jointPos2 ++;
            
            *jointPos2 = *(jointPrevPtr2 + 2); jointPos2 ++;
            *jointPos2 = *(jointPrevPtr2 + 3); jointPos2 ++;
            
            if ((jointIndex + 1) != jointCount)
            {
                /* 1 */
                *jointPos1 = *(jointPrevPtr1 + 2); jointPos1 ++;
                *jointPos1 = *(jointPrevPtr1 + 3); jointPos1 ++;
                
                /* 2 */
                *jointPos2 = *(jointPrevPtr2 + 2); jointPos2 ++;
                *jointPos2 = *(jointPrevPtr2 + 3); jointPos2 ++;
            }
            
            jointIndex ++;
        }
    }
   
    if (0 < jointCount)
    {
        /* 1 */
        *polygonPos1 = *(polygonPos1 - 2); polygonPos1 ++;
        *polygonPos1 = *(polygonPos1 - 2); polygonPos1 ++;
        *polygonPos1 = *(polygonPos1 + 2); polygonPos1 ++;
        *polygonPos1 = *(polygonPos1 + 2); // polygonPos1 ++;
        
        /* 2 */
        *polygonPos2 = *(polygonPos2 - 2); polygonPos2 ++;
        *polygonPos2 = *(polygonPos2 - 2); polygonPos2 ++;
        *polygonPos2 = *(polygonPos2 + 2); polygonPos2 ++;
        *polygonPos2 = *(polygonPos2 + 2); // polygonPos2 ++;
    }
 
  //  *outData1 = polygonPtr1;
    //*outData2 = polygonPtr2;
	ZNFloatSection *newPolygonPtr1 = new ZNFloatSection;
	newPolygonPtr1->array = polygonPtr1;
	newPolygonPtr1->num = polygonPos1 - polygonPtr1;
	ZNFloatSection *newPolygonPtr2 = new ZNFloatSection;
	newPolygonPtr2->array = polygonPtr2;
	newPolygonPtr2->num = polygonPos2 - polygonPtr2;
	*outData1= newPolygonPtr1;
	*outData2 = newPolygonPtr2;
    return ZTRUE;
}