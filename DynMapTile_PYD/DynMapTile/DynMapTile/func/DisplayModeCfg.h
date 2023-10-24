#pragma once





class CDisplayModeCfg 
{
public:
    CDisplayModeCfg();
    ~CDisplayModeCfg();

    // Init 
    BOOL Init( const char* strFilePath );

    ST_POINTDISPLAYMODE_ARY m_PointDisplayMode_ary;
    ST_LINEDISPLAYMODE_ARY m_LineDisplayMode_ary;
    ST_AREADISPLAYMODE_ARY m_AreaDisplayMode_ary;
    ST_TXTDISPLAYMODE_ARY m_TxtDisplayMode_ary;

protected:
    void Release();
    COLORREF ExtractColor( const char* szchar );

    LPST_POINTDISPLAYMODE ImportPointDisplayMode( TiXmlElement* pElem );
    LPST_LINEDISPLAYMODE ImportLineDisplayMode( TiXmlElement* pElem );
    LPST_AREADISPLAYMODE ImportAreaDisplayMode( TiXmlElement* pElem );
    LPST_TXTDISPLAYMODE ImportTxtDisplayMode( TiXmlElement* pElem );


private:



};

