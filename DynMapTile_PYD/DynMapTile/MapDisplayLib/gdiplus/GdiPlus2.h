#pragma once
#include "app_basic.h"

#define YLGDIPLUS_API

#ifdef __cplusplus
extern "C" {
#endif
 YLGDIPLUS_API GDIHandle GetGDI(HDC hDC,int width,int height,LPCSTR lpszPath);
   YLGDIPLUS_API void FlushGDI(GDIHandle handle,int x,int y);
   YLGDIPLUS_API void FlushRectGDI(GDIHandle handle,int x,int y,GDIRect rect);
   YLGDIPLUS_API void ClearGDI(GDIHandle handle,GDIColor color);
   YLGDIPLUS_API void DeleteGDI(GDIHandle handle);
   YLGDIPLUS_API BOOL BitBltGDI(GDIHandle handle,GDIRect dstRect, GDIRect srcRect);

   YLGDIPLUS_API void WriteBuffer(GDIHandle handle,char* filename);

   YLGDIPLUS_API BOOL DrawArc(GDIHandle handle,GDIPen pen, GDIRect rect, float startAngle, float sweepAngle);
   YLGDIPLUS_API BOOL DrawArc1(GDIHandle handle,GDIPen pen, int x, int y, int width, int height, int startAngle, int sweepAngle);
   YLGDIPLUS_API BOOL DrawArc2(GDIHandle handle,GDIPen pen, float x, float y, float width, float height, float startAngle, float sweepAngle);

   YLGDIPLUS_API BOOL DrawBezier(GDIHandle handle,GDIPen pen, GDIPoint pt1, GDIPoint pt2, GDIPoint pt3, GDIPoint pt4);
   YLGDIPLUS_API BOOL DrawBezier1(GDIHandle handle,GDIPen pen, float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);
   YLGDIPLUS_API BOOL DrawBeziers(GDIHandle handle,GDIPen pen, GDIPoint* points,int points_len);

   YLGDIPLUS_API BOOL DrawClosedCurve (GDIHandle handle,GDIPen pen, GDIPoint* points,int points_len); 							
   YLGDIPLUS_API BOOL DrawCurve(GDIHandle handle,GDIPen pen, GDIPoint* points,int points_len);				
   YLGDIPLUS_API BOOL DrawCurve1(GDIHandle handle,GDIPen pen, GDIPoint* points,int points_len, float tension);		
   YLGDIPLUS_API BOOL DrawCurve2(GDIHandle handle,GDIPen pen, GDIPoint* points,int points_len, int offset, int numberOfSegments, float tension);

   YLGDIPLUS_API BOOL DrawEllipse(GDIHandle handle,GDIPen pen, GDIRect rect);
   YLGDIPLUS_API BOOL DrawEllipse1(GDIHandle handle,GDIPen pen, int x, int y, int width, int height);
   YLGDIPLUS_API BOOL DrawEllipse2(GDIHandle handle,GDIPen pen, float x, float y, float width, float height);
	
   YLGDIPLUS_API BOOL DrawLine(GDIHandle handle,GDIPen pen, GDIPoint pt1, GDIPoint pt2);
  YLGDIPLUS_API  BOOL DrawLine1(GDIHandle handle,GDIPen pen, int x1, int y1, int x2, int y2);
   YLGDIPLUS_API BOOL DrawLine2(GDIHandle handle,GDIPen pen, float x1, float y1, float x2, float y2);

   YLGDIPLUS_API BOOL DrawGradientLine(GDIHandle handle,GDIPen pen,GDIColor color, int x1, int y1, int x2, int y2,int gradientType);
   YLGDIPLUS_API BOOL DrawGradientLines(GDIHandle handle,GDIPen pen,GDIColor color,GDIPoint* points,int points_len,int gradientType);

   YLGDIPLUS_API BOOL DrawArrow(GDIHandle handle,GDIPen pen,GDIColor fillcolor,int wd, int nLen, GDIPoint* points,int points_len);
  //added by ganjingjun 2007.8.7
   YLGDIPLUS_API BOOL DrawArrow2(GDIHandle handle,GDIColor pencolor,GDIColor fillcolor, int wdline, int wdarrow, GDIPoint* points,int points_len, int line_stytle);
   YLGDIPLUS_API BOOL DrawArrow3(GDIHandle handle,GDIColor color, int wd,GDIPoint* points,int points_len);
   YLGDIPLUS_API BOOL DrawRoadTipArrow(GDIHandle handle,GDIColor pencolor,GDIColor fillcolor,int roadwd,GDIPoint point1,GDIPoint point2, int flag);
   YLGDIPLUS_API BOOL LineSideArrow(GDIHandle handle, int roadwd, int linewd, GDIColor color, GDIPoint* points, int points_len, int flag);
   YLGDIPLUS_API BOOL DrawRoadsLeftArrow(GDIHandle handle,GDIColor color1,GDIColor color2,int roadwd, int linewd, GDIPoint* points, int points_len);
   YLGDIPLUS_API BOOL DrawRoadsRightArrow(GDIHandle handle,GDIColor color1,GDIColor color2,int roadwd, int linewd, GDIPoint* points, int points_len);
   YLGDIPLUS_API BOOL DrawLinesBorder(GDIHandle handle,GDIColor color, int line_width, int border_width, GDIPoint* points, int points_len, int flag);
   YLGDIPLUS_API BOOL DrawLinesLeftBorder(GDIHandle handle,GDIColor color, int line_width, int border_width, GDIPoint* points, int points_len);
   YLGDIPLUS_API BOOL DrawLinesRightBorder(GDIHandle handle,GDIColor color, int line_width, int border_width, GDIPoint* points, int points_len);
   YLGDIPLUS_API BOOL DrawRailWay(GDIHandle handle, int roadwd, GDIPoint* points, int points_len);

   YLGDIPLUS_API BOOL DrawDashedLine(GDIHandle handle, GDIColor color, GDIPoint point1, GDIPoint point2, int line_wd, int segline_len1, int interval_len1);
   YLGDIPLUS_API BOOL DrawDashedLines(GDIHandle handle, GDIColor color, GDIPoint *points, int points_len, int line_wd, int segline_len1, int interval_len1);
   YLGDIPLUS_API BOOL DrawArrows(GDIHandle handle, GDIColor pencolor, GDIColor fillcolor, int wd, int len1, int len2, GDIPoint* points, int points_len, BOOL bBorder = FALSE);
   YLGDIPLUS_API BOOL DrawSingalArrow(GDIHandle handle, GDIColor pencolor, GDIColor fillcolor, int wd, GDIPoint point1, GDIPoint point2, BOOL bBorder = FALSE);
  //  BOOL DrawLinesLeftSides(GDIHandle handle,GDIPen pen, GDIPoint* points,int points_len);
//  BOOL DrawLinesRightSides(GDIHandle handle,GDIPen pen, GDIPoint* points,int points_len); 
  //end

   YLGDIPLUS_API BOOL DrawPie(GDIHandle handle,GDIPen pen, GDIRect rect, float startAngle, float sweepAngle);
   YLGDIPLUS_API BOOL DrawPie1(GDIHandle handle,GDIPen pen, int x, int y, int width, int height, int startAngle, int sweepAngle);
  YLGDIPLUS_API  BOOL DrawPie2(GDIHandle handle,GDIPen pen, float x, float y, float width, float height, float startAngle, float sweepAngle);
	
   YLGDIPLUS_API BOOL DrawLines(GDIHandle handle,GDIPen pen, GDIPoint* points,int points_len);
   YLGDIPLUS_API BOOL DrawPolygon(GDIHandle handle,GDIPen pen, GDIPoint* points,int points_len);

   YLGDIPLUS_API BOOL DrawRectangle(GDIHandle handle,GDIPen pen, GDIRect rect);
   YLGDIPLUS_API BOOL DrawRectangle1(GDIHandle handle,GDIPen pen, int x, int y, int width, int height);
   YLGDIPLUS_API BOOL DrawRectangle2(GDIHandle handle,GDIPen pen, float x, float y, float width, float height);	
		
   YLGDIPLUS_API BOOL FillClosedCurve(GDIHandle handle,GDIColor color, GDIPoint* points,int len);	
	
   YLGDIPLUS_API BOOL FillEllipse(GDIHandle handle,GDIColor color, GDIRect rect);
   YLGDIPLUS_API BOOL FillEllipse1(GDIHandle handle,GDIColor color, int x, int y, int width, int height);
   YLGDIPLUS_API BOOL FillEllipse2(GDIHandle handle,GDIColor color, float x, float y, float width, float height);
	
   YLGDIPLUS_API BOOL FillPie(GDIHandle handle,GDIColor color, GDIRect rect, float startAngle, float sweepAngle);
   YLGDIPLUS_API BOOL FillPie1(GDIHandle handle,GDIColor color, int x, int y, int width, int height, int startAngle, int sweepAngle);
   YLGDIPLUS_API BOOL FillPie2(GDIHandle handle,GDIColor color, float x, float y, float width, float height, float startAngle, float sweepAngle);
	
   YLGDIPLUS_API BOOL FillPolygon(GDIHandle handle,GDIColor color, GDIPoint* points,int points_len);	

   YLGDIPLUS_API BOOL FillRectangle(GDIHandle handle,GDIColor color, GDIRect rect);
   YLGDIPLUS_API BOOL FillRectangle1(GDIHandle handle,GDIColor color, int x, int y, int width, int height);
   YLGDIPLUS_API BOOL FillRectangle2(GDIHandle handle,GDIColor color, float x, float y, float width, float height);

  // 设置透视变换矩阵 by dywang 2008-04-01
  // pdfMx 为空则不转换
  YLGDIPLUS_API void SetTransform(double* pMx);
  YLGDIPLUS_API void GDIagg_draw2(unsigned char* buf, unsigned w, unsigned h, int stride);
  YLGDIPLUS_API BOOL SetAggTransform(GDIHandle handle, GDIRect rect, GDIPoint* points,double* pMx);

#ifdef __cplusplus
}
#endif
