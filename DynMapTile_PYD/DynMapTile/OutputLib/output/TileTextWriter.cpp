#include "stdafx.h"
#include "TileTextWriter.h"
#include <fstream>
#include "json.h"


CTileTextWriter::CTileTextWriter()
{
    // 
}

CTileTextWriter::~CTileTextWriter()
{
    // 
}

BOOL CTileTextWriter::Save( LPCTSTR lpszDataFile, GEORECT& rect, PTILETEXT_INFO pTileTextInfo )
{
    if ( lpszDataFile == NULL || _tcslen(lpszDataFile) <= 0 || pTileTextInfo == NULL )
    {
        return FALSE;
    }

    Json::Value root;
    char szID[40] = {0};
    char szKind[10] = {0};
    char szName[100] = {0};
    for ( vector< PTILETEXT_ITEM >::iterator it=pTileTextInfo->m_TileTextItemAry.begin(); 
        it!=pTileTextInfo->m_TileTextItemAry.end(); it++ )
    {
        PTILETEXT_ITEM lpItem = *it;
        if ( lpItem == NULL )
        {
            continue;
        }
        if ( !rect.PtInRect( lpItem->m_ptShape.x, lpItem->m_ptShape.y ) )
        {
            continue;
        }
        Json::Value itemObj;

        // ��Χ
        Json::Value b;
        b.append( Json::Value( lpItem->m_rc.dMinX ) );
        b.append( Json::Value( lpItem->m_rc.dMinY ) );
        b.append( Json::Value( lpItem->m_rc.dMaxX ) );
        b.append( Json::Value( lpItem->m_rc.dMaxY ) );
        itemObj["b"] = b;

        // ID
        ZeroMemory( szID, sizeof(char)*40 );
        UnicodeToUTF8( lpItem->m_szID, szID, 40 );
        itemObj["d"] = Json::Value( szID );

        // ͼ��λ����Ϣ
        Json::Value i;
        i.append( Json::Value( lpItem->m_ico.left ) );
        i.append( Json::Value( lpItem->m_ico.top ) );
        i.append( Json::Value( lpItem->m_ico.right ) );
        i.append( Json::Value( lpItem->m_ico.bottom ) );
        itemObj["i"] = i;

        // ͼ��id
        ZeroMemory( szKind, sizeof(char)*10 );
        UnicodeToUTF8( lpItem->m_szKind, szKind, 10 );
        itemObj["ic"] = Json::Value( szKind );

        // ����
        ZeroMemory( szName, sizeof(char)*100 );
        UnicodeToUTF8( lpItem->m_pText, szName, 100 );
        itemObj["n"] = Json::Value( szName );

        // ��ľ�γ��
        Json::Value p;
        p.append( Json::Value( lpItem->m_ptShape.x ) );
        p.append( Json::Value( lpItem->m_ptShape.y ) );
        itemObj["p"] = p;

        // �Ƿ���ʾ����
        itemObj["tv"] = Json::Value( lpItem->m_nTxtVisable );

        // ����λ����Ϣ
        Json::Value t;
        t.append( Json::Value( lpItem->m_txt.left ) );
        t.append( Json::Value( lpItem->m_txt.top ) );
        t.append( Json::Value( lpItem->m_txt.right ) );
        t.append( Json::Value( lpItem->m_txt.bottom ) );
        itemObj["t"] = t;

        root.append( itemObj );
    }

    if ( root.empty() )
    {
        return TRUE;
    }

    // ���÷Ǹ�ʽ�����ܴ���С�ļ���С
    Json::FastWriter jsonWriter; //StyledWriter��ʽ��
    ofstream fout( lpszDataFile );
    fout << jsonWriter.write( root ) << std::endl;
    fout.close();
    
    root.clear();

    return TRUE;
}



