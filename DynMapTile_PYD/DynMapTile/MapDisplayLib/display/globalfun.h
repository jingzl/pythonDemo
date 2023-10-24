// globalfun.h 
#ifndef _GLOBALFUNC_H_
#define _GLOBALFUNC_H_

 void PolySimplify(long Tol, POINT* pLinkPtr, int iPointNum, int* iRet);

 void PolySimplifyInt2D(float Tol, POINT* pLinkPtr, int iPointNum, int* iRet);

 int RectClipLine(POINT* pIns, int iCount, POINT* pOuts, int* lineList, POINT* minPt, POINT* maxPt);

 int RectClipPolygon(POINT* pIns, int iCount, POINT* pOuts, int* polyList, POINT* minPt, POINT* maxPt);

 long clipGraph(LPRECT rectBound, POINT * pIn, long nPtCount, POINT * pOut, BOOL bLine=TRUE);

#endif

