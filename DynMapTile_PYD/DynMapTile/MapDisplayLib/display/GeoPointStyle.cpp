#include "StdAfx.h"
#include <math.h>
#include "GeoRender.h"
#include "IMapDisplay.h"


CGeoPointStyle::CGeoPointStyle():
	m_hTrueTypeFont(NULL)
{
	memset(m_szPath, 0, MAX_PATH*sizeof(TCHAR));
	memset(m_szImageFile, 0,  MAX_PATH*sizeof(TCHAR));
	m_Image = NULL;
	m_TransparentColor = RGB(255,0,255);
	TCHAR path[MAX_PATH];
	if(GetBinPath(path) > 0)
	{
		wsprintfW(m_szPath, L"%s\\config\\symbol\\nameimg", path);
	}
}
void CGeoPointStyle::SetImagePath(TCHAR* lpszPath)
{
	wcscpy(m_szPath ,lpszPath); 
}

CGeoPointStyle::~CGeoPointStyle()
{
	if(m_hTrueTypeFont != NULL)
	{
		DeleteObject(m_hTrueTypeFont);
		m_hTrueTypeFont = NULL;
	}
	if(m_Image)
	{
		delete m_Image;
		m_Image = NULL;
	}
}

void CGeoPointStyle::InitMe(st_pointdisplaymode* pStyle)
{
	if ( pStyle != NULL && m_nID != pStyle->m_nID )
	{
		m_nID      = pStyle->m_nID;
		m_drawtype = pStyle->m_drawtype;
		m_nSubCode = pStyle->m_nSubCode;
		m_clr      = pStyle->m_clr;
		m_nSize    = pStyle->m_nSize;
		m_nHeight  = pStyle->m_nHeight;
		m_nWidth   = pStyle->m_nWidth;
		m_nAlfa    = pStyle->m_nAlfa;
		_tcscpy(m_szFontName, pStyle->m_szFontName);
	}
}
CPngImg* CGeoPointStyle::GetImage()
{
	return m_Image;
}

//函数功能: 符号初始化
//输入参数: 无; 
//输出参数: 无
//返回结果: 无
void CGeoPointStyle::InitStyle(CCanvas* pCanvas)
{
	switch(m_drawtype)
	{
	case ePOINT_DT_INIT:
		{
		}
		break;
	case ePOINT_DT_TRUETYPE:
		{
			if(m_hTrueTypeFont != NULL)
			{
				DeleteObject(m_hTrueTypeFont);
				m_hTrueTypeFont = NULL;
			}
			LOGFONT logFont={0};
			logFont.lfCharSet = DEFAULT_CHARSET;
			logFont.lfHeight  = m_nHeight;
			_tcscpy(logFont.lfFaceName, m_szFontName);
			if(m_hTrueTypeFont == NULL)
			{
				m_hTrueTypeFont = ::CreateFontIndirect(&logFont);
			}				
			m_crTextOld = ::SetTextColor(pCanvas->GetDC(), m_clr);
			
		}
		break;
	case ePOINT_DT_ICO:
	case ePOINT_DT_BMP:
		{
            // 目前不支持
            break;
		}
	case ePOINT_DT_PNG:
		{
			TCHAR szFileName[MAX_PATH];
			_stprintf(szFileName,_T("%s\\%04X.png"), m_szPath, m_nSubCode);

			if(_tcscmp(m_szImageFile, szFileName) != 0 || m_Image == NULL)
			{
                if ( m_Image != NULL )
                {
                    delete m_Image;
                    m_Image = NULL;
                }
                m_Image = new CPngImg();
                if ( !m_Image->Load( szFileName ) )
                {
                    //TRACE( _T("\r\nload png failed ! ( 也许这是配置表故意这样设计，以保证不显示任何图标！)\r\n") );
                }
			}
		}
		break;
	}
}

void CGeoPointStyle::UnInitStyle(CCanvas* pCanvas)
{
	switch(m_drawtype)
	{
	case ePOINT_DT_INIT:
		{
		}
		break;
	case ePOINT_DT_TRUETYPE:
		{
			if(m_hTrueTypeFont != NULL)
			{
				DeleteObject(m_hTrueTypeFont);
				m_hTrueTypeFont = NULL;
			}
			::SetTextColor(pCanvas->GetDC(), m_crTextOld);
		}
		break;
	case ePOINT_DT_ICO:
	case ePOINT_DT_BMP:
	case ePOINT_DT_PNG:
		{
            if ( m_Image != NULL )
            {
                delete m_Image;
                m_Image = NULL;
            }
            break;
		}
    default:
        {
            break;
        }
	}
}