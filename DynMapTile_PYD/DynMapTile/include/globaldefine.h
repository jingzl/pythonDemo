#pragma once
#include <vector>
using namespace std;


//////////////////////////////////////////////////////////////////////////
// 日志记录的模式宏
//#define MAPCONFIG_MODE_LOG              // 地图配图时的日志模式
                                        // 配图时的日志模式，数据解析部分日志详细

//#define MAPOUTPUT_MODE_LOG              // 地图输出时的日志模式
                                        // 输出时的日志模式，数据解析部分日志简略，尽可能不影响时间

//#define MAPDISPLAY_LOG                  // 地图显示引擎中的调试信息


//////////////////////////////////////////////////////////////////////////
// log save
typedef void (CALLBACK *LogFunc)( LPCTSTR lpstr );

// 比例尺总等级数
// 参照GOOGLE的WEB地图标准
#define MAP_SCALE_NUM       20

// 地图有效范围
// 采用google的投影
const double MAPRECT_MINX = 60.0;
const double MAPRECT_MINY = 0.0;
const double MAPRECT_MAXX = 156.0;
const double MAPRECT_MAXY = 64.0;

// 绘制Buffer
const int DRAW_BUFFER_WIDTH = 513;
const int DRAW_BUFFER_HEIGHT = 513;


//////////////////////////////////////////////////////////////////////////
// georect
typedef struct _tag_Geo_rect
{
    double dMinX;
    double dMinY;
    double dMaxX;
    double dMaxY;

    _tag_Geo_rect():dMinX(0.0),dMinY(0.0),dMaxX(0.0),dMaxY(0.0){}
    _tag_Geo_rect(double MinX,double MinY,double MaxX,double MaxY):dMinX(MinX),dMinY(MinY),dMaxX(MaxX),dMaxY(MaxY){}
    ~_tag_Geo_rect(){}

    inline BOOL IsValidRect()
    {
        if(dMinX==0&&dMinY==0&&dMaxX==0&&dMaxY==0)
            return FALSE;
        if(dMinX == dMaxX)
            return FALSE;
        if(dMinY == dMaxY)
            return FALSE;
        return TRUE;
    }

    // Is the point in the rectangle ?
    inline BOOL PtInRect( /* [in] */ POINT* pPoint )
    {
        if ( pPoint == NULL )
            return FALSE;
        if (   pPoint->x < dMinX 
            || pPoint->x > dMaxX
            || pPoint->y < dMinY
            || pPoint->y > dMaxY )
            return FALSE;
        return TRUE;
    }

    // Is the point in the rectangle ?
    inline BOOL PtInRect( /* [in] */ double dx, double dy )
    {
        if (   dx < dMinX 
            || dx > dMaxX
            || dy < dMinY
            || dy > dMaxY )
            return FALSE;
        return TRUE;
    }

    inline BOOL IsIncluded( /* [in] */ _tag_Geo_rect* pOtherRect )
    {
        if ( pOtherRect == NULL )
            return FALSE;
        if (   pOtherRect->dMaxX <= dMaxX
            && pOtherRect->dMinX >= dMinX
            && pOtherRect->dMaxY <= dMaxY
            && pOtherRect->dMinY >= dMinY )
            return TRUE;
        return FALSE;
    }

    inline BOOL IsIntersected( /* [in] */ _tag_Geo_rect* pOtherRect )
    {
        if ( pOtherRect == NULL )
            return FALSE;
        if ( pOtherRect->dMaxX   < dMinX
            || pOtherRect->dMinX > dMaxX
            || pOtherRect->dMaxY < dMinY
            || pOtherRect->dMinY > dMaxY )
            return FALSE;
        return TRUE;
    }

    void UniteRect( _tag_Geo_rect* pRect1)
    {
        dMinX = dMinX < pRect1->dMinX ? dMinX : pRect1->dMinX;
        dMaxX = dMaxX < pRect1->dMaxX ? pRect1->dMaxX : dMaxX;
        dMinY = dMinY < pRect1->dMinY ? dMinY : pRect1->dMinY;
        dMaxY = dMaxY < pRect1->dMaxY ? pRect1->dMaxY : dMaxY;
    }

    void InflateRect( long x, long y )
    {
        dMinX -= x;
        dMinY -= y;
        dMaxX += x;
        dMaxY += y;
    }

} GEORECT;



//////////////////////////////////////////////////////////////////////////
// 
// 显示对象配置
//
//////////////////////////////////////////////////////////////////////////
// 空间数据类型
typedef enum enum_geotype
{
    eGeoInit = 0,
    eGeoPoint = 1,
    eGeoLine = 3,
    eGeoArea = 5,

} enumGEOTYPE;

typedef enum enum_featuretype
{
    eFeatInit = 0,
    eFeatBk = 1,
    eFeatRd = 3,
    eFeatTxt = 5,

} enumFEATURETYPE;

typedef struct st_layer_cfg
{
    int m_nID;
    int m_nClone;
    enumGEOTYPE m_geotype;
    enumFEATURETYPE m_featuretype;
    int m_nKind;
    int m_nSubKind;
    int m_nShpDispModeID;
    int m_nTxtDispModeID;

    st_layer_cfg():m_nID(-1),m_nClone(-1),m_geotype(eGeoInit),m_featuretype(eFeatInit),m_nKind(-1),m_nSubKind(-1),m_nShpDispModeID(-1),m_nTxtDispModeID(-1) {}
    ~st_layer_cfg() {}

} ST_LAYER_CFG, *LPST_LAYER_CFG;
typedef vector<LPST_LAYER_CFG> ST_LAYER_CFG_ARY;

typedef struct st_scalelevel 
{
    int m_nScale;
    COLORREF m_clrGround;
    COLORREF m_clrGrid;
    ST_LAYER_CFG_ARY m_layer_ary;

    st_scalelevel():m_nScale(-1),m_clrGround(0),m_clrGrid(0){}
    ~st_scalelevel()
    {
        for ( ST_LAYER_CFG_ARY::iterator it=m_layer_ary.begin(); it!=m_layer_ary.end(); it++ )
        {
            LPST_LAYER_CFG lpLayer = *it;
            if ( lpLayer != NULL )
            {
                delete lpLayer;
                lpLayer = NULL;
            }
        }
        m_layer_ary.clear();
    }

} ST_SCALELEVEL, *LPST_SCALELEVEL;
typedef vector<LPST_SCALELEVEL> ST_SCALELEVEL_ARY;


typedef struct st_displaymode 
{
    int m_nDisplayModeType;
    ST_SCALELEVEL_ARY m_scalelevel_ary;

    st_displaymode():m_nDisplayModeType(0){}
    ~st_displaymode()
    {
        for ( int i=0; i<m_scalelevel_ary.size(); i++ )
        {
            LPST_SCALELEVEL lpscalelevel = m_scalelevel_ary[i];
            if ( lpscalelevel != NULL )
            {
                delete lpscalelevel;
                lpscalelevel = NULL;
            }
        }
        m_scalelevel_ary.clear();
    }

} ST_DISPLAYMODE, *LPST_DISPLAYMODE;
typedef vector<LPST_DISPLAYMODE> ST_DISPLAYMODE_ARY;


//////////////////////////////////////////////////////////////////////////
//
// 显示样式
//
//////////////////////////////////////////////////////////////////////////
// 绘制类型
typedef enum enum_point_drawtype 
{
    ePOINT_DT_INIT = 0,
    ePOINT_DT_TRUETYPE = 1, // TrueType字体
    ePOINT_DT_ICO = 2,      // ICO图标
    ePOINT_DT_BMP = 3,      // BMP图片
    ePOINT_DT_PNG = 4,      // PNG图片

} enumPOINTDRAWTYPE;

typedef enum enum_line_drawtype 
{
    eLINE_DT_INIT = 0,
    eLINE_DT_RAILWAY = 1,   // 铁路
    eLINE_DT_SINGLE = 2,    // 单实线
    eLINE_DT_DOUBLE = 3,    // 双实线

} enumLINEDRAWTYPE;

typedef enum enum_area_drawtype 
{
    eAREA_DT_INIT = 0,
    eAREA_DT_SOLID = 1,     // 实填充
    eAREA_DT_BMP = 2,       // 位图填充

} enumAREADRAWTYPE;

typedef enum enum_txt_drawtype 
{
	//椭圆框:底色:边框:是否轮廓字 如 7 表示字带轮廓,有边框,有底色; 9表示椭圆框
    eTEXT_DT_OUTLINEFONT     = 0x01,		// 表示字带轮廓,无此位域则不带轮廓
    eTEXT_DT_TEXTBACKCOLOR   = 0x02,        //有底色,无此位域则无底色
	eTEXT_DT_DRAWARROWDIR    = 0x04,        //绘制箭头
	eTEXT_DT_STRINGROUND     = 0x08,        //外部有形状包围，如矩形，椭圆框
	eTEXT_DT_ROUNDTYPE       = 0x10,        //椭圆框,无此位域则矩形框,此位有效值依赖StringRound位
} enumTXTDRAWTYPE;

// 显示样式
typedef struct st_pointdisplaymode 
{
    int m_nID;
    enumPOINTDRAWTYPE m_drawtype;
    TCHAR m_szFontName[MAX_PATH];       // 当为TrueType时的字体
    int m_nSubCode;                     // 当为TrueType时的编号
    COLORREF m_clr;
    int m_nSize;
    int m_nHeight;
    int m_nWidth;
    int m_nAlfa;                        // 透明度

    st_pointdisplaymode():m_nID(-1),m_drawtype(ePOINT_DT_INIT),m_nSubCode(-1),m_clr(0),
        m_nSize(-1),m_nHeight(-1),m_nWidth(-1),m_nAlfa(255)
    {
        ZeroMemory( m_szFontName, MAX_PATH*sizeof(TCHAR) );
    }
    ~st_pointdisplaymode(){}

} ST_POINTDISPLAYMODE, *LPST_POINTDISPLAYMODE;
typedef vector<LPST_POINTDISPLAYMODE> ST_POINTDISPLAYMODE_ARY;


typedef struct st_linedisplaymode 
{
    int m_nID;
    enumLINEDRAWTYPE m_drawtype;
    COLORREF m_clrFore;
    int m_nWidth;
    COLORREF m_clrBack;
    int m_nEdgeWidth;
    int m_nAlfa; // 透明度

    st_linedisplaymode():m_nID(-1),m_drawtype(eLINE_DT_INIT),m_clrFore(0),m_nWidth(0),
        m_clrBack(0),m_nEdgeWidth(0),m_nAlfa(255){}
    ~st_linedisplaymode(){}

} ST_LINEDISPLAYMODE, *LPST_LINEDISPLAYMODE;
typedef vector<LPST_LINEDISPLAYMODE> ST_LINEDISPLAYMODE_ARY;

typedef struct st_areadisplaymode
{
    int m_nID;
    enumAREADRAWTYPE m_drawtype;
    COLORREF m_clrFill;
    int m_nEdgeWidth;
    COLORREF m_clrEdge;
    TCHAR m_szFillImgPath[MAX_PATH];
    int m_nAlfa; // 透明度

    st_areadisplaymode():m_nID(-1),m_drawtype(eAREA_DT_INIT),m_clrFill(0),m_nEdgeWidth(0),
        m_clrEdge(0),m_nAlfa(255)
    {
        ZeroMemory( m_szFillImgPath, MAX_PATH*sizeof(TCHAR) );
    }
    ~st_areadisplaymode(){}

} ST_AREADISPLAYMODE, *LPST_AREADISPLAYMODE;
typedef vector<LPST_AREADISPLAYMODE> ST_AREADISPLAYMODE_ARY;

typedef struct st_txtdisplaymode
{
    int m_nID;
    int m_drawtype;
    int m_nSubCode; // 目前保留
    TCHAR m_szFontName[MAX_PATH];
    int m_nFontSize;
    COLORREF m_clrFont;
	COLORREF m_clrBack;
	COLORREF m_clrRoundLine; //外围形状颜色
	int      m_nEdgeWidth; //边宽
    BOOL m_bBold;
    BOOL m_bUnderLine;
    BOOL m_bItalic;
    BOOL m_bShade;
    int m_nAlfa; // 透明度

    st_txtdisplaymode():m_nID(-1),m_drawtype(0),m_nEdgeWidth(2),m_nSubCode(0),m_nFontSize(0),
        m_clrFont(0),m_clrBack(0),m_clrRoundLine(0),m_bBold(FALSE),m_bUnderLine(FALSE),m_bItalic(FALSE),
        m_bShade(FALSE),m_nAlfa(255)
    {
        ZeroMemory( m_szFontName, MAX_PATH*sizeof(TCHAR) );
    }
    ~st_txtdisplaymode(){}

} ST_TXTDISPLAYMODE, *LPST_TXTDISPLAYMODE;
typedef vector<LPST_TXTDISPLAYMODE> ST_TXTDISPLAYMODE_ARY;




//////////////////////////////////////////////////////////////////////////
// 
// MapDisplay Engine Object
//
//////////////////////////////////////////////////////////////////////////


typedef struct _tag_geo_point
{
    double x;
    double y;
    _tag_geo_point():x(0.0),y(0.0){}
    _tag_geo_point(double _x, double _y):x(_x), y(_y) {}
    ~_tag_geo_point(){}
} GEOPOINT;

//==============================================显示数据类型=====================================

typedef struct _tag_road_feature
{	
    GEORECT	   m_rcFeatureRect; //道路对象的外接矩形
    TCHAR*     m_pSzName;       //名称字符串指针
    WORD	   m_wptCount;      //道路对象的点序列个数	
    GEOPOINT*  m_pShape;        //道路对象点序列	
    _tag_road_feature()
	{
		m_pSzName = NULL;
		m_pShape = NULL;
	}
    ~_tag_road_feature() 
    {
        if(m_pSzName != NULL) 
        {
            delete[] m_pSzName;
            m_pSzName = NULL;
        } 
        if(m_pShape!= NULL) 
        {
            delete[] m_pShape;
            m_pShape = NULL;
        } 
    }
} ROAD_FEATURE, *PROAD_FEATURE;

typedef struct _tag_background_feature
{
    GEORECT	    m_rcFeatureRect;  //背景特征的外接矩形		
    WORD	    m_wptCount;       //背景对象的点序列个数
    GEOPOINT*	m_pShape;         //点或者点序列
    _tag_background_feature()
	{ 
		m_pShape = NULL;
	}

    ~_tag_background_feature() 
    {
        if(m_pShape!= NULL) 
        {
            delete[] m_pShape;
            m_pShape = NULL;
        } 
    }
} BACK_FEATURE, *PBACK_FEATURE;

typedef struct _tag_Annotate_feature
{		
    TCHAR*	    m_pText;         //字符串
    GEOPOINT	m_ptShape;       //注记位置

    // 针对MCMDN数据显示添加的属性
    int         m_nKind;        // POI的FeatCode
    int         m_nSubKind;     // POI的SortCode


    _tag_Annotate_feature()
	{
		m_pText = NULL;
        m_nKind = -1;
        m_nSubKind = -1;
	}
    ~_tag_Annotate_feature() 
    {
        if(m_pText!= NULL) 
        {
            delete[] m_pText;
            m_pText = NULL;
        } 
    }
} ANNO_FEATURE, *PANNO_FEATURE;

typedef ANNO_FEATURE POI_FEATURE, *PPOI_FEATURE;

typedef ANNO_FEATURE SPECIAL_FEATURE, *PSPECIAL_FEATURE; // 特殊点数据，处理时单独处理

typedef struct _tag_GridRect_feature // 格网数据，计算WEBGIS横轴墨卡托投影下的标准格网
{
    GEORECT	m_rcFeatureRect;
    WORD	    m_wptCount;
    GEOPOINT*	m_pShape;
    int m_nTileX; // 行
    int m_nTileY; // 列

    _tag_GridRect_feature():m_wptCount(0),m_pShape(NULL),m_nTileX(0), m_nTileY(0)
    {
        m_pShape = NULL;
    }
    ~_tag_GridRect_feature()
    {
        if(m_pShape!= NULL) 
        {
            delete[] m_pShape;
            m_pShape = NULL;
        }
    }
} GRIDRECT_FEATURE, *PGRIDRECT_FEATURE;


typedef struct _tag_road_layer 
{
    int layerID;
    vector< PROAD_FEATURE > features;
    ST_LINEDISPLAYMODE      lineMode;
    ST_TXTDISPLAYMODE       textMode;

    _tag_road_layer():layerID(-1){}
    ~_tag_road_layer()
    {
        vector<PROAD_FEATURE>::iterator it = features.begin();
        for ( ; it!=features.end(); it++ )
        {
            PROAD_FEATURE lpfeature = *it;
            if ( lpfeature != NULL )
            {
                delete lpfeature;
                lpfeature = NULL;
            }
        }
        features.clear();
    }
} ROAD_LAYER, *PROAD_LAYER;

typedef struct _tag_back_layer
{
    int layerID;
    vector< PBACK_FEATURE > features;
    enumGEOTYPE             type;
    ST_LINEDISPLAYMODE      lineMode;
    ST_AREADISPLAYMODE      areaMode;

    _tag_back_layer():layerID(-1){}
    ~_tag_back_layer()
    {
        vector<PBACK_FEATURE>::iterator it = features.begin();
        for ( ; it!=features.end(); it++ )
        {
            PBACK_FEATURE lpfeature = *it;
            if ( lpfeature != NULL )
            {
                delete lpfeature;
                lpfeature = NULL;
            }
        }
        features.clear();
    }

}BACK_LAYER, *PBACK_LAYER;

typedef struct _tag_anno_layer
{
    int layerID;
    vector< PANNO_FEATURE > features;
    ST_POINTDISPLAYMODE     pointMode;
    ST_TXTDISPLAYMODE       textMode;

    _tag_anno_layer():layerID(-1){}
    ~_tag_anno_layer()
    {
        vector<PANNO_FEATURE>::iterator it = features.begin();
        for ( ; it!=features.end(); it++ )
        {
            PANNO_FEATURE lpfeature = *it;
            if ( lpfeature != NULL )
            {
                delete lpfeature;
                lpfeature = NULL;
            }
        }
        features.clear();
    }

} ANNO_LAYER, *PANNO_LAYER;

typedef struct _tag_poi_layer
{
    int layerID;
    vector< PPOI_FEATURE > features;
    ST_POINTDISPLAYMODE     pointMode;
    ST_TXTDISPLAYMODE       textMode;

    _tag_poi_layer():layerID(-1){}
    ~_tag_poi_layer()
    {
        vector<PPOI_FEATURE>::iterator it = features.begin();
        for ( ; it!=features.end(); it++ )
        {
            PPOI_FEATURE lpfeature = *it;
            if ( lpfeature != NULL )
            {
                delete lpfeature;
                lpfeature = NULL;
            }
        }
        features.clear();
    }

} POI_LAYER, *PPOI_LAYER;

typedef struct _tag_special_layer
{
    int layerID;
    vector< PSPECIAL_FEATURE > features;
    ST_POINTDISPLAYMODE     pointMode;
    ST_TXTDISPLAYMODE       textMode;

    _tag_special_layer():layerID(-1){}
    ~_tag_special_layer()
    {
        vector<PSPECIAL_FEATURE>::iterator it = features.begin();
        for ( ; it!=features.end(); it++ )
        {
            PSPECIAL_FEATURE lpfeature = *it;
            if ( lpfeature != NULL )
            {
                delete lpfeature;
                lpfeature = NULL;
            }
        }
        features.clear();
    }

} SPECIAL_LAYER, *PSPECIAL_LAYER;

typedef struct _tag_gridrect_layer
{
    vector<PGRIDRECT_FEATURE> features;
    ST_LINEDISPLAYMODE      lineMode;
    int m_nZoom;
    int m_nLevel; // mcmdn level

    _tag_gridrect_layer():m_nZoom(0),m_nLevel(0){}
    ~_tag_gridrect_layer()
    {
        vector<PGRIDRECT_FEATURE>::iterator it = features.begin();
        for ( ; it!=features.end(); it++ )
        {
            PGRIDRECT_FEATURE lpfeature = *it;
            if ( lpfeature != NULL )
            {
                delete lpfeature;
                lpfeature = NULL;
            }
        }
        features.clear(); 
    }
} GRIDRECT_LAYER, *PGRIDRECT_LAYER;


typedef struct _tag_map_display_data
{
    COLORREF clrback; // 背景色
    GRIDRECT_LAYER gridrectLayer; // 当前比例尺等级下的格网

    // 栅格背景数据
    vector< PROAD_LAYER >  roadLayers;
    vector< PBACK_LAYER >  backLayers;
    vector< PANNO_LAYER >  annoLayers;
    vector< PPOI_LAYER >   poiLayers;
    vector< PSPECIAL_LAYER >  specialLayers;

    _tag_map_display_data() {}
    ~_tag_map_display_data()
    {
        release();
    }

    void release()
    {
        vector<PROAD_LAYER>::iterator it_road;
        for ( it_road=roadLayers.begin(); it_road!=roadLayers.end(); it_road++ )
        {
            PROAD_LAYER lpLayer = *it_road;
            if ( lpLayer != NULL )
            {
                delete lpLayer;
                lpLayer = NULL;
            }
        }
        roadLayers.clear();

        vector<PBACK_LAYER>::iterator it_back;
        for ( it_back=backLayers.begin(); it_back!=backLayers.end(); it_back++ )
        {
            PBACK_LAYER lpLayer = *it_back;
            if ( lpLayer != NULL )
            {
                delete lpLayer;
                lpLayer = NULL;
            }
        }
        backLayers.clear();

        vector<PANNO_LAYER>::iterator it_anno;
        for ( it_anno=annoLayers.begin(); it_anno!=annoLayers.end(); it_anno++ )
        {
            PANNO_LAYER lpLayer = *it_anno;
            if ( lpLayer != NULL )
            {
                delete lpLayer;
                lpLayer = NULL;
            }
        }
        annoLayers.clear();

        vector<PPOI_LAYER>::iterator it_poi;
        for ( it_poi=poiLayers.begin(); it_poi!=poiLayers.end(); it_poi++ )
        {
            PPOI_LAYER lpLayer = *it_poi;
            if ( lpLayer != NULL )
            {
                delete lpLayer;
                lpLayer = NULL;
            }
        }
        poiLayers.clear();

        vector<PSPECIAL_LAYER>::iterator it_special;
        for ( it_special=specialLayers.begin(); it_special!=specialLayers.end(); it_special++ )
        {
            PSPECIAL_LAYER lpLayer = *it_special;
            if ( lpLayer != NULL )
            {
                delete lpLayer;
                lpLayer = NULL;
            }
        }
        specialLayers.clear();
    }


} MAP_DISP_DATA, *PMAP_DISP_DATA;


typedef struct st_scalecfg 
{
    int m_nScale;
    int m_nMLevel;

    st_scalecfg():m_nScale(-1),m_nMLevel(-1) {}
    ~st_scalecfg() {}

} ST_SCALECFG, *LPST_SCALECFG;
typedef vector<LPST_SCALECFG> ST_SCALECFG_ARY;

// 提取底图上显示的注记及POI数据
// 回避之后的有效数据
// 原始数据中用于显示的注记及索引数据均采用此对象
typedef struct st_tiletext_item
{
    TCHAR  m_szID[20];      // ID
    TCHAR* m_pText;         // 名称
    TCHAR m_szKind[5];      // 类型
    int m_nTxtVisable;      // 文字是否显示
    GEOPOINT m_ptShape;     // 标记点的位置
    GEORECT m_rc;           // 整体外边框经纬度
    RECT m_ico;             // 图标的位置信息（相对信息，像素坐标）
    // left - 以热点范围左下角为原点的横向偏移
    // top - 以热点范围左下角为原点的纵向偏移
    // right - 图标所占区域的宽度
    // bottom - 图标所占区域的高度
    RECT m_txt;             // 文字的位置信息
    // 同上

    st_tiletext_item()
    {
        ZeroMemory( m_szID, 20*sizeof(TCHAR) );
        m_pText = NULL;
        ZeroMemory( m_szKind, 5*sizeof(TCHAR) );
        m_nTxtVisable = 0;
        SetRectEmpty( &m_ico );
        SetRectEmpty( &m_txt );
    }
    ~st_tiletext_item()
    {
        if ( m_pText != NULL )
        {
            delete [] m_pText;
            m_pText = NULL;
        }
    }

} ST_TILETEXT_ITEM, *PTILETEXT_ITEM;

typedef struct st_tiletext_info
{
    int m_nScale;
    int m_nTileX;
    int m_nTileY;
    vector< PTILETEXT_ITEM > m_TileTextItemAry;

    st_tiletext_info()
    {
        release();
    }

    ~st_tiletext_info()
    {
        release();
    }

    void release()
    {
        m_nScale = 0;
        m_nTileX = 0;
        m_nTileY = 0;
        release( m_TileTextItemAry );
    }

    void release( vector<PTILETEXT_ITEM>& itemAry )
    {
        vector<PTILETEXT_ITEM>::iterator it_a = itemAry.begin();
        for ( ; it_a!=itemAry.end(); it_a++ )
        {
            PTILETEXT_ITEM lpItem = *it_a;
            if ( lpItem != NULL )
            {
                delete lpItem;
                lpItem = NULL;
            }
        }
        itemAry.clear();
    }


} ST_TILETEXT_INFO, *PTILETEXT_INFO;


//////////////////////////////////////////////////////////////////////////