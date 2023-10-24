//globalfun.cpp 
#include "stdafx.h"
#include "globalfun.h"
#include <vector>
#include <math.h>
//#include <winioctl.h> // KernelIoControl
typedef long	EDGE;
#define LEFT	0
#define RIGHT	1
#define BOTTOM	2
#define TOP		3
#define N_EDGE	4
#define SMALL_MAX_SIZE  128  //使用小内存分配器的分配内存最大限制

void closeClip(LPRECT rectBound, POINT * pOut, long * cnt, POINT * first[], POINT * s);
void clipPoint(POINT p, EDGE b, LPRECT rectBound, POINT * pOut, long * cnt, POINT * first[], POINT * s);
// void* smalloc(size_t size);
// void sfree(LPVOID p);
//////////////////////////////////////////////////////////////////////////

struct lost  ///多边形裁减状态记录数据结构
{
	int iNextIndex;
	int iPreIndex;
	int rectPPos; ////与矩形的交点的边号（1，2，4，8见线的裁减算法）
	int index;    /////交点在裁减后的节点链表中的位置
};
struct RectNodeList
{
	RectNodeList*	pre;
	int				rectPPos;
	long			lenght;
	long			x;
	long			y;
};


//////////////////////////////////////////////////////////////////////////

// This is an example of an exported variable

inline long inside(POINT p, EDGE b, LPRECT rectBound)
{
	if (b==LEFT && p.x < rectBound->left) 
		return (FALSE);
	if (b==RIGHT && p.x > rectBound->right) 
		return (FALSE);
	if (b==BOTTOM && p.y < rectBound->bottom) 
		return (FALSE);
	if (b==TOP && p.y > rectBound->top) 
		return (FALSE);

	return (TRUE);
}

inline long insidebybottom(POINT p, long lBound)
{
	if (p.y > lBound)
		return (FALSE);
	return (TRUE);
}

inline long cross(POINT p1, POINT p2, EDGE b, LPRECT rectBound)
{
	if (inside(p1, b, rectBound) == inside(p2, b, rectBound))
		return (FALSE);
	else 
		return (TRUE);
}


inline long crossbybottom(POINT p1, POINT p2, long lBound)
{
	if (insidebybottom(p1, lBound) == insidebybottom(p2, lBound))
		return (FALSE);
	else 
		return (TRUE);
}

POINT intersect(POINT p1, POINT p2, EDGE b, LPRECT rectBound)
{
	POINT iPt;
	long  ioffset=0;
	
	switch (b) 
	{
	case LEFT:
		iPt.x = rectBound->left;
		if (p1.x != p2.x)
			ioffset = ((iPt.x - p2.x) * (p1.y - p2.y)) / (p1.x - p2.x);
		iPt.y = p2.y + ioffset;
		break;
	case RIGHT:
		iPt.x = rectBound->right;
		if (p1.x != p2.x)
			ioffset = ((iPt.x - p2.x) * (p1.y - p2.y)) / (p1.x - p2.x);
		iPt.y = p2.y + ioffset;
		break;
	case BOTTOM:
		iPt.y = rectBound->bottom;
		if (p1.y != p2.y) 
			ioffset = ((iPt.y - p2.y) * (p1.x - p2.x)) / (p1.y - p2.y);
		iPt.x = p2.x + ioffset;
		break;
	case TOP:
		iPt.y = rectBound->top;
		if (p1.y != p2.y)
			ioffset = ((iPt.y - p2.y) * (p1.x - p2.x)) / (p1.y - p2.y);
		iPt.x = p2.x + ioffset;
		break;
	}
	return (iPt); 
}

POINT intersectbybottom(POINT p1, POINT p2, long lBound)
{
	POINT iPt;
	long  ioffset=0;
	
	iPt.y = lBound;
	if (p1.y != p2.y) 
		ioffset = ((iPt.y - p2.y) * (p1.x - p2.x)) / (p1.y - p2.y);
	iPt.x = p2.x + ioffset;

	return (iPt); 
}
//线条抽希算法
POINT VecMinInt2D(POINT* p1, POINT* p2)
{
	POINT p;
	p.x = p1->x - p2->x;
	p.y = p1->y - p2->y;
	return p;
}
int DotProdInt2D(POINT* p1, POINT* p2)
{
	return p1->x * p2->x + p1->y * p2->y;
}

int NormSquaredInt2D(POINT* p1)
{
	return p1->x * p1->x + p1->y * p1->y;
}
int DistSquaredInt2D(POINT* p1, POINT* p2)
{
	POINT p = VecMinInt2D(p1, p2);
	return NormSquaredInt2D(&p);
}

void SimplifyInt2D(float Tol2, POINT* Orig ,BOOL* Marker, int j, int k)
{
	// Simplify polyline in OrigList between j and k. Marker[] will be set to True
	// for each point that must be included
	int i, MaxI; // Index at maximum value
	double MaxD2;    // Maximum value squared
	double CU, CW, B;
	double DV2;
	POINT P0, P1, PB, U, W;
	// Is there anything to simplify?
	if (k <= j + 1) return;
	
	P0 = Orig[j];
	P1 = Orig[k];
	//TRACE(L"\nP0(%d,%d)  P1(%d,%d)\n", P0.x,P0.y, P1.x,P1.y);
	if(P0.x == P1.x && P0.y == P1.y)
	{
		return;
	}
	U  = VecMinInt2D(&P1, &P0); // Segment vector
	CU = DotProdInt2D(&U, &U); // Segment length squared
	MaxD2 = 0;
	MaxI  = 0;
	
	//TCHAR szInfo[200];
	//wsprintf(szInfo,L"\nU=(%d,%d), CU=%f \n",U.x, U.y,CU);
	//TRACE(szInfo);
	// Loop through points and detect the one furthest away
	for(i = j+1; i<k; i++)
	{
		W  = VecMinInt2D(&Orig[i], &P0);
		CW = DotProdInt2D(&W, &U);
		
		//wsprintf(szInfo,L"\nW=(%d,%d), CW=%f \n", W.x,W.y,CW);
		//TRACE(szInfo);
		// Distance of point Orig[i] from segment
		if (CW <= 0 )
		{
			// Before segment
			DV2 = DistSquaredInt2D(&Orig[i], &P0);
		}
		else 
		{
			if (CW > CU)
			{
				// Past segment
				DV2 = DistSquaredInt2D(&Orig[i], &P1);
				//wsprintf(szInfo,L"\n**DV2=%f\n", DV2);
				//TRACE(szInfo);
			}
			else 
			{
				// Fraction of the segment
				try
				{
					B = CW / CU;
				}
				catch(...) 
				{
					B = 0; // in case CU = 0
				}
				//wsprintf(szInfo,L"\nB=%f, P0(%d,%d),U(%d,%d)\n", B, P0.x, P0.y, U.x, U.y);
				//TRACE(szInfo);
				PB.x = P0.x + B * U.x;
				PB.y = P0.y + B * U.y;
				//wsprintf(szInfo,L"\nPB(%d,%d)\n", PB.x,PB.y);
				//TRACE(szInfo);
				
				DV2 = DistSquaredInt2D(&Orig[i], &PB);
				//wsprintf(szInfo,L"\n--DV2=%f\n", DV2);
				//TRACE(szInfo);
			}
		}
		
		// test with current max distance squared
		if (DV2 > MaxD2 )
		{
			// Orig[i] is a new max vertex
			MaxI  = i;
			MaxD2 = DV2;
		}
	}
	
	// If the furthest point is outside tolerance we must split
	if (MaxD2 > Tol2 )// error is worse than the tolerance
	{
		
		//wsprintf(szInfo,L"\nj=%d,MaxI=%d,K=%d\n",  j, MaxI,k);
		//TRACE(szInfo);
		// split the polyline at the farthest vertex from S
		Marker[MaxI] = TRUE;  // mark Orig[maxi] for the simplified polyline
		
		// recursively simplify the two subpolylines at Orig[maxi]
		SimplifyInt2D(Tol2, Orig, Marker, j, MaxI); // polyline Orig[j] to Orig[maxi]
		SimplifyInt2D(Tol2, Orig, Marker, MaxI, k); // polyline Orig[maxi] to Orig[k]
		
	}
	return;
}

void PolySimplifyInt2D(float Tol, POINT* pLinkPtr, int iPointNum, int* iRet)
{
	int i, N;
	BOOL* Marker;
	float Tol2;
	int iResult = 0;
	if (iPointNum < 2)
	{
		*iRet = iPointNum ;
		return ;
	}
	Tol2 = sqrt(Tol);
	
	// Create a marker array
	N = iPointNum;
	Marker = new BOOL[N];
	// Exclude intermediate for now
	memset(Marker, 0, sizeof(BOOL) * N);
	// Include first and last point
	Marker[0]     = TRUE;
	Marker[N - 1] = TRUE;
	
	//TRACE(L"\nN=%d\n", N);
	// Simplify 
	SimplifyInt2D(Tol2, pLinkPtr, Marker, 0, N - 1);
	
	// Copy to resulting list
	for(i=0; i<N; i++)
	{
		if (Marker[i] == TRUE)
		{
			pLinkPtr[iResult] = pLinkPtr[i];
			iResult++;
		}
	}
	if(Marker != NULL)
	{
		delete []Marker;
		Marker = NULL;
	}
	*iRet = iResult;
	return ;
}

void PolySimplify(long Tol, POINT* pLinkPtr, int iPointNum, int* iRet)
{
	int nIndex = 1;
	for (int i=1; i<iPointNum-2; i++)
	{
		long lDis = abs(pLinkPtr[i].x-pLinkPtr[nIndex-1].x) + abs(pLinkPtr[i].y-pLinkPtr[nIndex-1].y);
		if(lDis>nIndex)
		{
			if(nIndex != i)
			{
				pLinkPtr[nIndex++] = pLinkPtr[i];
			}
			else
			{
				nIndex++;
			}
		}
	}
	if(nIndex != iPointNum-1)
	{
		pLinkPtr[nIndex++] = pLinkPtr[iPointNum-1];
	}
	else
	{
		nIndex++;
	}
	*iRet = nIndex;
}
//线裁剪算法
enum Code{Noknow=0x0,Left=0x1,Right=0x2,Bottom=0x4,Top=0x8};

void swapInt(int *c1, int *c2)
{
	/////裁减时的方向标示量交换
	int tmp;
	tmp = *c1;
	*c1 = *c2;
	*c2 = tmp;
}

int ClipCode(long& x, long& y, POINT* minPt, POINT* maxPt)
{
	//////////检测点在矩形的位置
	int code = Noknow;

	if (x < minPt->x)
	{
		code |= Left;
	}
	else if (x > maxPt->x) 
	{
		code |= Right;
	}

	if (y < minPt->y)
	{
		code |= Bottom;
	}
	else if (y > maxPt->y)
	{
		code |= Top;
	}

	return code;

}

void swapPts(long *x1, long *y1, long *x2, long *y2)
{
	////裁减时的点交换
	long tx,ty;
	tx = *x1;
	ty = *y1;
	*x1 = *x2;
	*y1 = *y2;
	*x2 = tx;
	*y2 = ty;
}

int ClipLine(long& x1, long& y1, long& x2, long& y2, int& flag1, int& flag2, POINT* minPt, POINT* maxPt)
{
	// x1,y1 = one line point; x2,y2 = second line point
	// rect = clipping rectangle
	// return  !0 = 返回两点有效
	///        0 = 返回两点无效,
	// flag1 =0:在内，1，2，4，8：边界
	// flag2 =0:在内，1，2，4，8：边界
	////裁减点包括原本在边界的点和裁减产生的新点，
	/////根据矩形裁减线
	int retV = 0;
	int clip = 0;
	int code1,code2;
	int done = FALSE;
	int swap = FALSE;
	flag2 = 0;
	flag1 = 0;
	double t;
	double m;
	if(x1 != x2)
	{
		///计算斜率
		m = ((double)(y1 - y2)) / ((double)(x1 - x2));
	}
	else
	{
		m = 0.0;
	}
	while(!done)
	{////while
		/////区域检测
		code1 = ClipCode(x1, y1, minPt, maxPt);
		code2 = ClipCode(x2, y2, minPt, maxPt);
		if(!(code1 | code2)) ////两点全在里，包括边界
		{
			done = TRUE;
			retV = 1;/////取前面一点
			if(swap)
			{
				swapPts(&x1, &y1, &x2, &y2);/////将点的顺序换回
				swapInt(&flag1,&flag2);///边状态交换
			}
		}
		else
		{
			if(code1 & code2)/////两点在外且在同一区内
			{
				done = TRUE;
				retV = 0;
			}
			else
			{
				/////p1， p2 可能和矩形又交点情形
				if(!code1)////p1在内
				{
					////为处理方便交换位置
					swapPts(&x1, &y1, &x2, &y2);
					swapInt(&code1, &code2);
					swapInt(&flag1,&flag2);
					swap = !swap;////对交换进行记录，以便换回来
				}
				if(code1 & Left) ////在左边
				{
					t = (minPt->x - x1);
					t *= m;
					y1 += t;
					x1 = minPt->x;
					flag1 = Left;
				}
				else
				{
					if(code1 & Right) ////在右边
					{
						t = (maxPt->x - x1);
						t *= m;
						y1 +=  t;
						x1 = maxPt->x;
						flag1 = Right;
					}
					else
					{
						if(code1 & Bottom) ////在下面
						{
							if(x1 != x2)
							{
								t = (minPt->y - y1);
								t /= m;
								x1 += t;
							}
							y1 = minPt->y;
							flag1 = Bottom;
						}
						else
						{
							if(code1 & Top)////在上面
							{
								if(x1 != x2)
								{
									t = (maxPt->y - y1);
									t /= m;
									x1 +=  t;
								}
								y1 = maxPt->y;
								flag1 = Top;
							}
						}
					}
				}
			}
		}
	}////while
	return retV;
}

int RectClipLine(POINT* pIns, int iCount, POINT* pOuts, int* lineList, POINT* minPt, POINT* maxPt)
{
	int ret = 0;
	int m,flag1,flag2,k;
	long x1,y1,x2,y2;
	/////矩形裁减多边形
	int inc = 0;
	m=0;
	for(k=1; k<iCount; k++)
	{
		x1 = pIns[k-1].x;
		y1 = pIns[k-1].y;
		x2 = pIns[k].x;
		y2 = pIns[k].y;
		m = ClipLine(x1, y1, x2, y2, flag1, flag2, minPt, maxPt);
		if(m)
		{
			////第一点要入数据链表
			pOuts[inc].x = x1;
			pOuts[inc].y = y1;
			inc++;
			if(flag2)
			{///仅裁减点入数据链表
				pOuts[inc].x = x2;
				pOuts[inc].y = y2;
				inc++;
				lineList[ret] = inc;
				ret++;
			}
		}
	}
	//最后一次裁剪是有效的
	if(m && flag2 == 0 && inc>0)
	{
		pOuts[inc].x = x2;
		pOuts[inc].y = y2;
		inc++;
		lineList[ret] = inc;
		ret++;
	}
	return ret;
}
////裁剪结束

//////////////////////////////////////////////////////////////////////////
//裁剪多边形开始
int TrueInside(long& x, long& y, POINT& minPt, POINT& maxPt)
{
	//////////开始裁减时，检测点是否实实在在在矩形内包括不在边上
	int code = 1;

	if (x <= minPt.x) code = 0;
	if (x >= maxPt.x) code = 0;
	if (y <= minPt.y) code = 0;
	if (y >= maxPt.y) code = 0;

	return code;

}
long cal2PDisD(long &x1, long &y1,long &x2, long &y2)
{
	////求两点距离乘方函数
	long dis = 0;
	dis = ((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
	return dis;
}

long cal2PDisD( POINT p1,  POINT p2)
{
	long dis = 0;
	dis = ((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
	return dis;
}

long GetRectVNode(int outFlag, int inFlag, POINT& p1, POINT& p2, RectNodeList* pRectList, POINT RVNode[4], int* iRet)
{
	/////根据状态位确定在裁减多边形上需经过的节点
	/////RVNode的长度为裁减的多边形顶点数，这里为4
	////inFlag 为出标志 outFlag为进标志
	double dis = 0.0;
	int from,to,ret;
	ret = 0;
	from = abs(outFlag);
	to = abs(inFlag);
	if(from != to)
	{
		RectNodeList* t = pRectList;
		while(t->rectPPos != from)
		{
			t = t->pre;
		}
		dis -= t->lenght - sqrt((double)cal2PDisD(p1.x,p1.y, t->x,t->y));
		int nCount = 0;
		while(t->rectPPos != to)
		{
			dis += t->lenght;
			RVNode[nCount].x = t->x;
			RVNode[nCount].y = t->y;
			nCount ++;
			t = t->pre;
		}
		dis += t->lenght - sqrt((double)cal2PDisD(p2.x,p2.y, t->x,t->y));
		*iRet = nCount;
	}
	else
	{
		long x1,y1;
		RectNodeList* t = pRectList;
		while(t->rectPPos != from)
		{
			t = t->pre;
		}
		x1 = t->x;
		y1 = t->y;
		double d1 = sqrt((double)cal2PDisD(p1.x, p1.y, x1,y1));
		double d2 = sqrt((double)cal2PDisD(p2.x, p2.y, x1,y1));
		if(d1 < d2)
		{
			int nCount = 0;
			RVNode[nCount].x = t->x;
			RVNode[nCount].y = t->y;
			nCount ++;

			dis += t->lenght;
			t = t->pre;
			while(t->rectPPos != from)
			{
				dis += t->lenght;
				RVNode[nCount].x = t->x;
				RVNode[nCount].y = t->y;
				nCount ++;
				t = t->pre;
			}
			dis -= d2 - d1;
			*iRet = nCount;
		}
		else
		{
			dis = d1 - d2;
			*iRet = 0;
		}
	}
	return dis;
}

int RectClipPolygon(POINT* pIns, int iCount, POINT* pOuts, int* polyList, POINT minPt, POINT maxPt)
{
	int ret = 0;
	using namespace std;
	vector<int> indexList; 
	lost *list;
	POINT *temp;
	POINT *rList;
	RectNodeList* pRectList, *t1,*t2;
	int jj;


	rList = new POINT[4];
	temp = new POINT[2*iCount+4];

	pRectList = new RectNodeList;
	pRectList->rectPPos = Bottom;
	pRectList->x = minPt.x;
	pRectList->y = minPt.y;
	pRectList->lenght = maxPt.x - minPt.x;
	t1 = pRectList;

	t2 = new RectNodeList;
	t2->rectPPos = Left;
	t2->x = minPt.x;
	t2->y = maxPt.y;
	t2->lenght = maxPt.y - minPt.y;
	t1->pre = t2;
	t1 = t2;

	t2 = new RectNodeList;
	t2->rectPPos = Top;
	t2->x = maxPt.x;
	t2->y = maxPt.y;
	t2->lenght = maxPt.x - minPt.x;
	t1->pre = t2;
	t1 = t2;

	t2 = new RectNodeList;
	t2->rectPPos = Right;
	t2->x = maxPt.x;
	t2->y = minPt.y;
	t2->lenght = maxPt.y - minPt.y;
	t1->pre = t2;
	t1 = t2;
	t1->pre = pRectList;

	list = new lost[2*iCount+4];
	int m,flag1,flag2,k;
	int inc = 0;

	long x1,y1,x2,y2;
	/////矩形裁减多边形
	BOOL b1,b2;
	int losIndex = 0;
	for(k=1; k<iCount; k++)
	{
		x1 = pIns[k-1].x;
		y1 = pIns[k-1].y;
		x2 = pIns[k].x;
		y2 = pIns[k].y;
		b1 = TrueInside(x1, y1, minPt, maxPt);
		b2 = TrueInside(x2, y2, minPt, maxPt);
		if(b1 == TRUE && b2 == TRUE)
		{
			temp[inc].x = x1;
			temp[inc].y = y1;
			inc++;
			continue;
		}
		else
		{
		}
		m = ClipLine(x1, y1, x2, y2, flag1, flag2, &minPt, &maxPt);
		if(m)
		{
			////第一点要入数据链表
			temp[inc].x = x1;
			temp[inc].y = y1;
			if(flag1)
			{
				list[losIndex].rectPPos = -flag1;////进为负
				list[losIndex].index = inc;
				losIndex++;
			}
			inc++;

			if(flag2)
			{///仅裁减点入数据链表
				temp[inc].x = x2;
				temp[inc].y = y2;
				list[losIndex].rectPPos = flag2;////出为正
				list[losIndex].index = inc;
				losIndex++;
				inc++;
			}
		}
	}
	////K
	x1 = pIns[iCount-1].x;
	y1 = pIns[iCount-1].y;
	x2 = pIns[0].x;
	y2 = pIns[0].y;
	b1 = TrueInside(x1, y1, minPt, maxPt);
	b2 = TrueInside(x2, y2, minPt, maxPt);
	if(b1 == TRUE && b2 == TRUE)
	{
		temp[inc].x = x1;
		temp[inc].y = y1;
		inc++;
	}
	else
	{
		m = ClipLine(x1, y1, x2, y2, flag1, flag2, &minPt, &maxPt);
		if(m)
		{
			temp[inc].x = x1;
			temp[inc].y = y1;
			if(flag1)
			{
				list[losIndex].rectPPos = -flag1;////进为负
				list[losIndex].index = inc;
				losIndex++;
			}
			inc++;

			if(flag2)
			{///仅裁减点入数据链表
				temp[inc].x = x2;
				temp[inc].y = y2;
				list[losIndex].rectPPos = flag2;////出为正
				list[losIndex].index = inc;
				losIndex++;
				inc++;
			}
		}
	}
	if(inc == 0)
	{
		t1 = pRectList->pre;
		pRectList->pre = NULL;
		pRectList = t1;
		while(pRectList!=NULL)
		{
			t1 = pRectList;
			t2 = pRectList->pre;
			delete t1;
			pRectList = t2;
		}
		delete []temp;
		delete []list;
		delete []rList;
		return 0;
	}
	if(losIndex == 0)
	{//没裁减
		ret = 0;
		polyList[ret] = inc;
		for(k=0; k<inc; k++)
		{
			pOuts[k].x = temp[k].x;
			pOuts[k].y = temp[k].y;
		}
		ret++;
	}
	else
	{
		//理顺列表，使其标准化
		lost l;
		int iNewIndex = 0;
		for( jj=1; jj<losIndex; jj++)
		{
			l = list[jj-1];
			if(l.rectPPos * list[jj].rectPPos < 0)
			{
				list[iNewIndex] = l;
				iNewIndex++;
			}
		}
		l = list[losIndex-1];
		if(l.rectPPos * list[0].rectPPos < 0)
		{
			list[iNewIndex] = l;
			iNewIndex++;
		}
		losIndex = iNewIndex;

		//建立循环列表
		for( jj=0; jj<losIndex; jj++)
		{
			list[jj].iNextIndex = jj + 1;
			list[jj].iPreIndex = jj  - 1;
		}
		list[0].iPreIndex = losIndex - 1;
		list[losIndex-1].iNextIndex = 0;

		int start;
		start = 0;
		//找到第一个开始正位置（出）
		while(list[start].rectPPos < 0)
		{
			start = list[start].iNextIndex;
		}
		//确定最后一点，断开next列表
		list[list[start].iPreIndex].iNextIndex = -1;
		indexList.push_back(start);
		int j;
		int mc=0;

		int ll;
		for(ll=0; ll<indexList.size(); ll++)
		{
			//start = *v_It;
			start = indexList[ll];
			k = start;
			while(k >= 0)
			{
				if(list[k].rectPPos > 0)
				{ //list[k].rectPPos > 0
					//将目标加入(进为负) //出为正
					int from,to,pre ,next;
					pre = list[k].iPreIndex;
					next = list[k].iNextIndex;
					from = list[pre].index;
					to = list[k].index;
					if(from < to)
					{
						for(j=from; j<to+1; j++)
						{
							pOuts[mc].x = temp[j].x;
							pOuts[mc].y = temp[j].y;
							mc++;
						}
					}
					else
					{
						for(j=from; j<inc; j++)
						{
							pOuts[mc].x = temp[j].x;
							pOuts[mc].y = temp[j].y;
							mc++;
						}
						for(j=0; j<to+1; j++)
						{
							pOuts[mc].x = temp[j].x;
							pOuts[mc].y = temp[j].y;
							mc++;
						}
					}

					////小于0 负为进点（k）,则k-1 为出点
					////将裁减对象加入
					POINT lv1[4];
					double v1,dMin;
					int iRet1;
					//找最近点
					int ii = -1;
					int g = start;
					dMin = (pRectList->lenght + pRectList->pre->lenght) * 2.0;
					while(g >= 0)
					{
						if(list[g].rectPPos < 0)
						{
							v1 = GetRectVNode(list[k].rectPPos, list[g].rectPPos,temp[list[k].index],temp[list[g].index], pRectList, lv1, &iRet1);
							if(v1 < dMin)
							{
								dMin = v1;
								ii = g;
							}
						}
						g = list[g].iNextIndex;
					}
					//后面点闭合
					if(ii < k)
					{
						v1 = GetRectVNode(list[k].rectPPos, list[ii].rectPPos,temp[list[k].index],temp[list[ii].index], pRectList, lv1, &iRet1);
						for(int i1=0; i1<iRet1; i1++)
						{
							pOuts[mc].x = lv1[i1].x;
							pOuts[mc].y = lv1[i1].y;
							mc++;
						}
						polyList[ret] = mc;
						ret++;
					}
					else
					{
						//不为 k+1
						if(ii > k+1)
						{
							list[list[ii].iPreIndex].iNextIndex = list[k].iNextIndex;
							list[list[k].iNextIndex].iPreIndex = list[ii].iPreIndex;
							int h = list[k].iNextIndex;
							while(list[h].iPreIndex < 0)
							{
								h = list[h].iNextIndex;
							}
							if(h >= 0)
							{
								list[list[h].iPreIndex].iNextIndex = -1;
								indexList.push_back(h);
							}
							list[k].iNextIndex = ii;
							list[ii].iPreIndex = k;
						}

						v1 = GetRectVNode(list[k].rectPPos, list[ii].rectPPos,temp[list[k].index],temp[list[ii].index], pRectList, lv1, &iRet1);
						for(int i1=0; i1<iRet1; i1++)
						{
							pOuts[mc].x = lv1[i1].x;
							pOuts[mc].y = lv1[i1].y;
							mc++;
						}
					}
				} //list[k].rectPPos > 0
				if(list[k].iNextIndex == -1)
				{
					if(ret == 0)
					{
						polyList[ret] = mc;
						ret++;
					}
					else
					{
						if(polyList[ret-1] != mc)
						{
							polyList[ret] = mc;
							ret++;
						}
					}
				}
				k = list[k].iNextIndex;
			}//while k
		}//while v_It
		//闭合
		if(mc > 0)
		{
			if(ret == 0)
			{
				polyList[ret] = mc;
				ret++;
			}
			else
			{
				if(polyList[ret-1] != mc)
				{
					polyList[ret] = mc;
					ret++;
				}
			}
		}
	}

	t1 = pRectList->pre;
	pRectList->pre = NULL;
	pRectList = t1;
	while(pRectList!=NULL)
	{
		t1 = pRectList;
		t2 = pRectList->pre;
		delete t1;
		pRectList = t2;
	}
	indexList.clear();
	delete []temp;
	delete []list;
	delete []rList;
	return ret;
}
//裁剪多边形结束

void clipPoint(POINT p, EDGE b, LPRECT rectBound, POINT * pOut, long * cnt, POINT * first[], POINT * s)
{
	POINT iPt;

	/* If no previous point exists for this edge, save this point. */
	if (!first[b])
	{
		first[b] = (POINT*)malloc(sizeof(POINT));
		*first[b] = p;
	}
	else
		if(cross(p, *(s+b), b, rectBound)) 
		{
			iPt = intersect(p, *(s+b), b, rectBound);
			if (b < TOP)
				clipPoint(iPt, b+1, rectBound, pOut, cnt, first, s);
			else 
				*(pOut + (*cnt)++) = iPt;  
		}

		*(s+b) = p;           /* Save 'p' as most recent point for this edge */

		/* For all, if point is 'inside' proceed to next clip edge, if any */
		if(inside(p, b, rectBound))
		{
			if (b < TOP)
				clipPoint(p, b+1, rectBound, pOut, cnt, first, s);
			else 
				*(pOut + (*cnt)++) = p;  
		}
}


void closeClip(LPRECT rectBound, POINT * pOut, long * cnt, POINT * first[], POINT * s)
{
	POINT i;
	EDGE b;	
	for (b = LEFT; b <= TOP; b++) 
	{
		if(first[b]==NULL)
			continue;
		if (cross(s[b], *first[b], b, rectBound)) 
		{
			i = intersect(s[b], *first[b], b, rectBound);
			if (b < TOP)
				clipPoint(i, b+1, rectBound, pOut, cnt, first, s);
			else 
			{
				*(pOut+ (*cnt)++) = i;
			}
		}
	}
}

long clipGraph(LPRECT rectBound, POINT * pIn, long nPtCount, POINT * pOut, BOOL bLine)
{
	POINT  *first[N_EDGE]={NULL, NULL, NULL, NULL};
	POINT		s[N_EDGE];
	long i, cnt = 0;
	if(pIn==NULL||pOut==NULL||nPtCount<1)
		return 0;
	if(rectBound->bottom>rectBound->top)
	{
		long ntop = rectBound->bottom;
		rectBound->bottom = rectBound->top;
		rectBound->top = ntop;
	}

	for (i=0; i<nPtCount; i++) 
		clipPoint(pIn[i], LEFT, rectBound, pOut, &cnt, first, s);
	if(!bLine)  
		closeClip(rectBound, pOut, &cnt, first, s);
	for(i=0; i<N_EDGE; i++)
		if(first[i])
			free(first[i]);
	return cnt;
}
