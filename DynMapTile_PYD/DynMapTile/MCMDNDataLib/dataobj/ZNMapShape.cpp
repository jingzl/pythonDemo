#include "ZNMapShape.h"
#include "CPStl.h "
#include "QuickFunc.h"

ZNMapShape::ZNMapShape(void)
{
}

ZNMapShape::~ZNMapShape(void)
{
}

ZNFloatSection* patternPointsForKind(ZUINT8 kind, const ZFLOAT32* points, ZUINT32 pointCount)
{
	ZNFloatSection *data = NULL;
	ZFLOAT32 pattern[6];
	ZUINT32 patternCount = 0;
	switch(kind){
		case 1:{
			pattern[0] = 5.0 / 204.0;
			pattern[1] = 2.0 / 204.0;
			patternCount = 2;
			}
			break;
		case 2:{
			 pattern[0] = pattern[1] = 2.0 / 204.0;
			 patternCount = 2;
			}
			break;
		case 3:{
			pattern[0] = pattern [1] = 5.0 / 204.0;
			pattern[2] = pattern [3] = 5.0 / 204.0;
			patternCount = 4;
			}
			break;
		case 4:{
			pattern[0] = 9.0 / 204.0;
			pattern[0] = pattern [1] = 5.0 / 204.0;
			patternCount = 4;
			}
			break;
	}
	if (0 < patternCount)
	{
		data = patternPoints(points, pointCount, pattern, patternCount);
	}
	return data;
}

ZBOOL ZNMapTextureShape::decode()
{
	return 0;
};

ZNMapTextureShape::ZNMapTextureShape()
{

};

ZNMapTextureShape::~ZNMapTextureShape()
{

};

#define FLOAT_EQUAL(_A_, _B_)	( !(((_A_) > (_B_)) || ((_A_) < (_B_))) )

ZNFloatSection* patternPoints(const ZFLOAT32 *points, ZUINT32 pointCount, const ZFLOAT32 *pattern, ZUINT32 patternCount)
{
	CP_VECTOR<ZFLOAT32>* data = new CP_VECTOR<ZFLOAT32>;
	ZFLOAT32 *result = NULL;
	ZUINT32 pointIndex = 0;
	ZUINT32 patternIndex = 0;
	ZUINT32 totalPointCount = 1;

	ZFLOAT32 cpx, cpy, npx, npy, tpx, tpy;
	
	ZFLOAT32 distPointEnd;
	ZFLOAT32 distPatternEnd;

	ZFLOAT32 newPoints[2];

	//check parameters;
	if (points == NULL || pointCount == 0) 
		return NULL;

	if (pattern == NULL || patternCount == 0)
		return NULL;

	if ((patternCount % 2) != 0)
	{
		printf("pattern count is not an even number.\n");
		return NULL;
	}

	npx = points[2 * pointIndex];
	npy = points[2 * pointIndex + 1];
	distPointEnd = 0.0;

	patternIndex = 0;
	distPatternEnd = pattern[patternIndex];

	newPoints[0] = npx;
	newPoints[1] = npy;
	
	data->push_back(newPoints[0]);
	data->push_back(newPoints[1]);
	
	while (1)
	{
		if (distPointEnd == 0.0)
		{
			pointIndex++;
			if (pointCount <= pointIndex)
				break;
			cpx = npx;
			cpy = npy;
			npx = points[2 * pointIndex];
			npy = points[2 * pointIndex + 1];
			if (FLOAT_EQUAL(points[2 * (pointIndex - 1)], npx) &&
				FLOAT_EQUAL(points[2 * (pointIndex - 1) + 1], npy))
			{
				if (totalPointCount % 2)
				{
					data->push_back(newPoints[0]);
					data->push_back(newPoints[1]);
					totalPointCount++;
				}
				do 
				{
					pointIndex += 2;
					if (pointCount <= pointIndex)
						break;
					cpx = points[2 * (pointIndex - 1)];
					cpy = points[2 * (pointIndex - 1) + 1];
					npx = points[2 * pointIndex];
					npy = points[2 * pointIndex  + 1];
				} while ((cpx == npx) && (cpy == npy));
				data->push_back(newPoints[0]);
				data->push_back(newPoints[1]);
				totalPointCount++;
			}
			distPointEnd = sqrtf((npx - cpx) * (npx - cpx) + (npy - cpy) * (npy - cpy));
		}

		if (distPatternEnd == 0.0)
		{
			patternIndex++;
			if (patternCount <= patternIndex)
				patternIndex = 0;
			distPatternEnd = pattern[patternIndex];
		}

		if (distPointEnd == distPatternEnd)
		{
			newPoints[0] = npx;
			newPoints[1] = npy;
			
			data->push_back(newPoints[0]);
			data->push_back(newPoints[1]);

			distPointEnd = distPatternEnd = 0.0;
			totalPointCount++;
		}else if (distPointEnd < distPatternEnd)
		{
			newPoints[0] = npx;
			newPoints[1] = npy;
			
			data->push_back(newPoints[0]);
			data->push_back(newPoints[1]);
			data->push_back(newPoints[0]);
			data->push_back(newPoints[1]);

			totalPointCount += 2;
			distPatternEnd -= distPointEnd;
			distPointEnd = 0.0;
		}else if(distPatternEnd < distPointEnd){
			if (cpx == npx){
				tpx = cpx;
			}
			else{
				tpx = cpx + (npx - cpx) * distPatternEnd / distPointEnd;
			}

			if (cpy == npy){
				tpy = cpy;
			}else{
				tpy = cpy + (npy - cpy) * distPatternEnd / distPointEnd;
			}

			newPoints[0] = tpx;
			newPoints[1] = tpy;

			data->push_back(newPoints[0]);
			data->push_back(newPoints[1]);

			totalPointCount++;
			distPointEnd -= distPatternEnd;

			distPatternEnd = 0.0;

			cpx = tpx;
			cpy = tpy;
		}
	}

	result = new ZFLOAT32[data->size()];
	ZFLOAT32 *newResult = result;
	for (CP_VECTOR<ZFLOAT32>::iterator it = data->begin(); it != data->end(); ++it)
	{
		memmove(newResult, &(*it), sizeof(ZFLOAT32));
		newResult += sizeof(ZFLOAT32);
	}

	ZNFloatSection *floatSection = new ZNFloatSection;
	floatSection->array = result;
	floatSection->num = data->size();
	safe_delete(data);
	return floatSection;
}