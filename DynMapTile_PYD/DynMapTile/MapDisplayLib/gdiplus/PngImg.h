#pragma once
#include <Windows.h>
#include <TCHAR.h>

#ifdef _DEBUG
#pragma comment ( lib, "png_d.lib" )
#else
#pragma comment ( lib, "png.lib" )
#endif

class CPngImg 
{
public:
    CPngImg();
    ~CPngImg();

    BOOL Load( const TCHAR* szFile );

    int GetWidth() const;
    int GetHeight() const;

    BOOL Draw( HDC hDC, int iDestX, int iDestY, int iDestWidth, int iDestHeight, int iSrcX=0, int iSrcY=0, int iSrcWidth = -1, int iSrcHeight = -1 );

private:
    BYTE* m_pBits;
    int m_nWidth;
    int m_nHeight;

};

