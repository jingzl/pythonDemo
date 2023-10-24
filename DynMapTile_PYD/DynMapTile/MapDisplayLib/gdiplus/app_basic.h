#ifndef APP_BASIC_INCLUDED
#define APP_BASIC_INCLUDED
#include <windows.h>
//gan  线段端点类型
enum LineCapX 
{
	Flat = 0,	
	Square = 1,
	Round = 2,
	//	Triangle = 3
};

enum LineJoinX 
{
	Bevel = 1,
	Miter = 0,
	RoundX = 2
};

typedef RECT    GDIRect;
typedef POINT   GDIPoint;
typedef int     GDIHandle;

typedef struct _GDICOLOR
{	
	unsigned char r;//red分量
	unsigned char g;//green分量
	unsigned char b;//blue分量
	unsigned char a;//透明度
}GDIColor;

#define GDIPEN_STARTLINECAP   1
#define GDIPEN_ENDLINECAP     2
#define GDIPEN_LINEJOIN       4
#define GDIPEN_COLOR          8
#define GDIPEN_WIDTH          16
typedef struct _GDIPEN
{
	DWORD flags;
	LineCapX startCap;
	LineCapX endCap;
	LineJoinX lineJoin;
	GDIColor color;
	int width;
}GDIPen;

#define  ACROSS_CENTER_GRADIENT   1  
#define  ACROSS_GRADIENT          2

#endif