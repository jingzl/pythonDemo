#pragma once



class CTileTextWriter 
{
public:
    CTileTextWriter();
    ~CTileTextWriter();

    BOOL Save( LPCTSTR lpszDataFile, GEORECT& rect, PTILETEXT_INFO pTileTextInfo );

protected:


private:


};
