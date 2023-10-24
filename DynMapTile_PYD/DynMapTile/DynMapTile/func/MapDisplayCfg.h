#pragma once




class CMapDisplayCfg 
{
public:
    CMapDisplayCfg();
    ~CMapDisplayCfg();

    // Init 
    BOOL Init( const char* strFilePath );
    const ST_DISPLAYMODE_ARY* get_DisplayMode_Ary() const;
    const COLORREF get_BackColor( BYTE nMapScaleIndex ) const;


protected:
    void Release();
    COLORREF ExtractColor( const char* szchar );

    LPST_DISPLAYMODE ImportDisplayMode( TiXmlElement* pElem );
    LPST_SCALELEVEL ImportScaleLevel( TiXmlElement* pElem );
    LPST_LAYER_CFG ImportLayer( TiXmlElement* pElem );


private:
    ST_DISPLAYMODE_ARY m_DisplayMode_ary;
    std::map<BYTE, COLORREF> m_BackColor_map;


};

