#ifndef _LABELADJUSTDEFINE_H_
#define _LABELADJUSTDEFINE_H_
#include "globaldefine.h"
#include "GeoRender.h"
#include <string>
#include <map>
#include <vector>
#include <algorithm>
using namespace std;
#include <math.h>



// BAN的一组值，决定显示的疏密程度

// -- 用于背景地图 --
// 禁区域值
#define BAN_VALUE					50//0x64
// 禁区值
#define MAX_BAN_VALUE				100//0x96
// 间距值
#define BAN_OUT_WIDTH               20
#define BAN_OUT_HEIGHT              50
// 决定显示信息的疏密层度，值越大，则越稀疏。!!!
// 用于道路名称
#define BAN_LINENAME_JUDGE_VALUE    50
// 用于注记点和POI点
#define BAN_PNTNAME_JUDGE_VALUE     200

// -- 用于矢量POI --
// 禁区域值
#define POI_BAN_VALUE   			50//0x64
// 禁区值
#define POI_MAX_BAN_VALUE   		100//0x96
// 间距值
#define POI_BAN_OUT_WIDTH           20
#define POI_BAN_OUT_HEIGHT          50
// 决定显示信息的疏密层度，值越大，则越稀疏。!!!
#define POI_BAN_JUDGE_VALUE         500





//绘制文本指向箭头的长度
#define ARROW_LENGTH				0x08

//绘制文本指向箭头的长度
#define ICON_TEXT_SPACE				0x02

//绘制文本指向箭头的宽度 
#define ARROW_WIDTH					0x06

// 方位数
// 一个方位，为方向枚举的第一个方向
#define LABEL_DIRECTS_ENUM_ONE  	0x01
// 两个方位 左右
#define LABEL_DIRECTS_ENUM_LR	    0x02
// 四个方位，覆盖方向枚举的四个方向
#define LABEL_DIRECTS_ENUM_FOUR	    0x04

// 目前采用的方向数（默认）
#define LABEL_DIRECTS_ENUM_COUNT    LABEL_DIRECTS_ENUM_LR

//注记的四个方向枚举：右，上，左，下
#define Right						0x01
#define Top							0x02
#define Left						0x04
#define Bottom						0x08



//#define POISHOW_NUM 30
//#define DRAW_DEBUG_LINE


#define LENGTH(DX, DY)  ( sqrt( (double)(DX*DX)+(DY*DY) ) )
#define PI              3.1415926535897932




//路径拷贝结构，无需内存处理
struct stLinkPart
{
    stLinkPart()
    {
        m_pLinkPtr = NULL;
        m_iLenght = 0;
        m_iPointNum = 0;
        pLayer = NULL;
        m_iTotal = 0;
    }

    ~stLinkPart()
    {
        m_pLinkPtr = NULL;
        m_iLenght = 0;
        m_iPointNum = 0;
        pLayer = NULL;
    }

    stLinkPart(POINT* pList, int iCount, PROAD_LAYER layer)
    {
        if(pList&&iCount>1)
        {
            m_iTotal = 0;
            m_pLinkPtr = new POINT[iCount];
            memcpy(m_pLinkPtr, pList, sizeof(POINT)*iCount);
            m_iPointNum = iCount;
            CalculateLen();
        }
        else
        {
            m_pLinkPtr = NULL;
            m_iPointNum = 0;
        }
        m_iLenght = 0;
        pLayer = layer;
    }

    int CalculateLen()
    {
        int itotal = 0;
        m_iTotal = 0;
        for(int i=0; i<m_iPointNum-1; i++)
        {
            itotal += abs(m_pLinkPtr[i].x-m_pLinkPtr[i+1].x) +
                abs(m_pLinkPtr[i].y-m_pLinkPtr[i+1].y);
        }
        m_iLenght = itotal;
        m_iTotal = itotal;
        return itotal;
    }

    bool operator < (const stLinkPart &m)const 
    {
        return (m_iLenght < m.m_iLenght);
    }

    int m_iTotal;
    POINT* m_pLinkPtr;
    int m_iPointNum;
    int m_iLenght;
    PROAD_LAYER pLayer;
};
typedef map<wstring,vector< stLinkPart > > ROADLINK;
typedef map<wstring,vector< stLinkPart > >::iterator ROADLINKIT; 

typedef struct tagMapRoadLink
{
    tagMapRoadLink()
    {
        RoadLinks = NULL;
        RoadLinks = new ROADLINK;
    }

    ~tagMapRoadLink()
    {
        Clear();
        if(RoadLinks)
        {
            delete RoadLinks;
            RoadLinks = NULL;
        }
    }

    void AddLinkPart(TCHAR* pRoadName, POINT* pLink, int iCount, PROAD_LAYER layer)
    {
        if(pRoadName==NULL || pLink==NULL||iCount<=1)
            return;

        if(!RoadLinks)
            RoadLinks = new ROADLINK;
        wstring  strRN = pRoadName;

        ROADLINKIT  it = RoadLinks->find(strRN);
        if(it!=RoadLinks->end())
        {
            vector< stLinkPart >::iterator vctit;
            for(vctit=((*it).second).begin(); vctit!=((*it).second).end(); ++vctit)
            {
                if( ((vctit->m_pLinkPtr[0].x == pLink[0].x) && 
                    (vctit->m_pLinkPtr[0].y == pLink[0].y) && 
                    (vctit->m_pLinkPtr[vctit->m_iPointNum-1].x == pLink[iCount-1].x) &&
                    (vctit->m_pLinkPtr[vctit->m_iPointNum-1].y == pLink[iCount-1].y))
                    ||((vctit->m_pLinkPtr[0].x == pLink[iCount-1].x) && 
                    (vctit->m_pLinkPtr[0].y == pLink[iCount-1].y) && 
                    (vctit->m_pLinkPtr[vctit->m_iPointNum-1].x == pLink[0].x) &&
                    (vctit->m_pLinkPtr[vctit->m_iPointNum-1].y == pLink[0].y)))
                {
                    return;
                }
            }
            stLinkPart stpk = stLinkPart(pLink, iCount, layer);			
            if(stpk.m_pLinkPtr != NULL)
            {
                (*it).second.push_back(stpk);
            }
        }
        else
        {
            vector< stLinkPart > tmpvct;
            stLinkPart stpk = stLinkPart(pLink, iCount, layer);
            tmpvct.push_back(stpk);
            if(stpk.m_pLinkPtr != NULL)
            {
                (*RoadLinks)[strRN] = tmpvct;
            }
        }
    }

    void Clear()
    {		
        if(!RoadLinks)
            return;
        ROADLINKIT mapit;
        vector< stLinkPart >::iterator vctit;
        for(mapit=(*RoadLinks).begin(); mapit!=(*RoadLinks).end(); ++mapit)
        {
            vector< stLinkPart >* pvct = &((*mapit).second);
            for(vctit=pvct->begin(); vctit!=pvct->end(); ++vctit)
            {
                POINT *pPnts = vctit->m_pLinkPtr;
                if(pPnts)
                {
                    if(pPnts[0].x>600 || pPnts[0].x<-10)
                    {
                        continue;
                    }
                    else
                    {
                        delete[] pPnts;
                        pPnts = vctit->m_pLinkPtr = NULL;
                    }
                }

            }
            if(pvct !=NULL)
            {
                pvct->clear();
            }
        }
        RoadLinks->clear();
    }

    ROADLINK* RoadLinks;
}MapRoadLink, *PMapRoadLink;

//线注记临时数据结构
struct stLabelLine
{
    //可以检查是否满足连续注记
    BOOL bContinuePre; //是否和前面的注记连续

    BOOL bFollowLine;

    //检索到点坐标
    unsigned short iIndex;//线段索引

    //可以得知注记字数
    unsigned short iCanLabelNum;//连续注记的字数

    //可以计算起始注记位置
    float fStartlen;//注记起始位置
    float fEndLen; ////注记终止位置
    double fDx; //x增长率
    double fDy;  //y增长率
    double dAng; // 角度

    //可以计算总长
    float fToPreDis; //包括本线在内的前面所有线长总合
    float flength; //线段长度
};
//
struct LineStatInfo 
{
    float				fHalfRate;	//居中度
    unsigned short		iStartIndex; //数组起始位置
    unsigned short		iEndIndex; //数组终止位置
    unsigned short		iLabelNums; //数组终止位置
    double              dLength;
    int					iPos;
    double				dAng;
    BOOL				bFollowLine;

    LineStatInfo()
    {
        bFollowLine = TRUE;
        iPos =0;
        iStartIndex = 0;
        iEndIndex = 0;
        iLabelNums = 0;
        dLength = 0;
    }
};

//POI注记避让运算时的临时数据结构
struct stLabelPOI
{
    //字符长
    SIZE				m_Size;

    //字符检索位置
    unsigned short		m_Index;

    //注记的点
    POINT   m_LabelPnt;

    //标记方位
    unsigned char	m_LabelDirect;

    stLabelPOI()
    {
        m_Index = 0;
        memset(&m_LabelDirect, 0, sizeof(m_LabelDirect));
    };
};

//点注记，数据接口
class IPointNameListLabel
{
public:
    //的到列表大小
    virtual int Size()=0;

    //得到一项的坐标
    virtual GEOPOINT* GetItemPoint(int index)=0;

    //得到一项的注记值
    virtual TCHAR* GetItemText(int index)=0;

    //得到点风格
    virtual CGeoPointStyle* GetPointStyle(int index=0)=0;

    //得到注记风格
    virtual CGeoTextStyle* GetTextStyle(int index=0)=0;

    //是否显示
    virtual BOOL GetDispState(int index)=0;
};

#define DRAW_POINT_SYMBOL 1
#define DRAW_TEXT		  2
#define DRAW_BOTH		  3



//首尾相连的两线的节点描述
struct MergeNodeInfo 
{
    short iPreLineIndex;
    short iPreLinePntIndex;
    short iNextLineIndex;
    short iNextLinePntIndex;
};

struct LineScript
{
    LineScript()
    {
        LinkPos = -1;
        totalCost = 100000;
        cost = 100000;
        stindex = -1;
        edindex = -1;
        path = -1;

    };
    ~LineScript()
    {

    };
    int LinkPos;
    int cost;
    int totalCost;
    int stindex;
    int edindex;
    int path;
};
#define CMP_INT 10000


#define  OFF_CX 6
#define  OFF_CY 4



//////////////////////////////////////////////////////////////////////////
// API

//列表排序比较函数
inline int compare(const void * v1, const void * v2) 
{
    if( ((LineStatInfo *)v1)->fHalfRate < ((LineStatInfo *)v2)->fHalfRate)
    {
        return -1;
    }
    else
    {
        if( ((LineStatInfo *)v1)->fHalfRate > ((LineStatInfo *)v2)->fHalfRate)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
}

inline int compareEx(const void * v1, const void * v2) 
{
    if( ((LineStatInfo *)v1)->iLabelNums > ((LineStatInfo *)v2)->iLabelNums)
    {
        return -1;
    }
    else
    {
        if( ((LineStatInfo *)v1)->iLabelNums < ((LineStatInfo *)v2)->iLabelNums)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
}

inline void vect(POINT* p1, POINT* p2, double *x, double *y, double *length)
{
    double dx, dy, l;
    dx  = p2->x - p1->x;
    dy  = p2->y - p1->y;
    l   = LENGTH ( dx, dy );
    if (l < 1.0) {
        dx = dy = 0;
    }
    *x  = dx/l;
    *y  = dy/l;
    *length = l;
}

inline bool CanMerge(POINT *p1, POINT *p2, double* Fuzzy)
{
    double d = (p1->x-p2->x)*(p1->x-p2->x) + (p1->y-p2->y)*(p1->y-p2->y);
    if(d <= *Fuzzy)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

inline int Vect_line_prune (stLinkPart* Points, int length)
{
    int i, j =-1;
    int len =  Points->m_iPointNum > length ? length : Points->m_iPointNum;
    if ( len > 0 ) 
    {
        j = 1;
        for (i = 1; i < len; i++) 
        {
            if(Points->m_pLinkPtr[i].x != Points->m_pLinkPtr[j-1].x || Points->m_pLinkPtr[i].y != Points->m_pLinkPtr[j-1].y)
            {
                Points->m_pLinkPtr[j] = Points->m_pLinkPtr[i] ;
                j++;
            }
        }
    }
    return j;
}

inline BOOL IsAlphabet(TCHAR* szName)
{
    if (szName== NULL)
    {
        return FALSE;
    }
    // 根据四维数据中的道路名情况
    TCHAR* pstr = wcstok(szName, L"ＧＳＸＹGSXYgsxy");
    if(pstr != NULL)
    {
        if(wcslen(pstr) < wcslen(szName))
        {
            return TRUE;
        }
    }		
    return FALSE;
}

inline BOOL ToAlphabet(TCHAR* srcName, TCHAR* desName)
{
    int i=0;
    for (i=0; i<wcslen(srcName); i++)
    {
        desName[i] = srcName[i]-65248;
    }
    desName[i] = '\0';
    return TRUE;
}
























#endif
