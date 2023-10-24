#pragma once
#include "ZNMapTextShape.h"
#include "ZNMapLineShape.h"
#include "ZNMapPolygonShape.h" 
#include "ZNMapLinePolygonShape.h"
#include "ZNMapSignTextShape.h"
#include "ZNMapSignImageShape.h"
#include "CPStl.h"



struct _tag_znmapdata_for_display
{
	CP_VECTOR<ZNMapTextShape*> m_textShapes;
	CP_VECTOR<ZNMapLineShape*> m_lineShapes;
	CP_VECTOR<ZNMapLinePolygonShape*> m_linePolygonShapes;
	CP_VECTOR<ZNMapPolygonShape1*> m_polygonShapes;
	CP_VECTOR<ZNMapPolygonShape2*> m_polygonShape2s;
	CP_VECTOR<ZNMapPolygonShape3*> m_polygonShape3s;
	CP_VECTOR<ZNMapPolygonShape4*> m_polygonShape4s;
	CP_VECTOR<ZNMapPolygonShape5*> m_polygonShape5s;
	CP_VECTOR<ZNMapPolygonShape6*> m_polygonShape6s;
	CP_VECTOR<ZNMapSignImageShape*> m_signImageShapes;
	CP_VECTOR<ZNMapSignTextShape*> m_signTextShapes;

    _tag_znmapdata_for_display()
    {
        // 
    }
    ~_tag_znmapdata_for_display()
    {
        for ( CP_VECTOR<ZNMapTextShape*>::iterator it=m_textShapes.begin(); it!=m_textShapes.end(); it++ )
        {
            ZNMapTextShape* lpShape = *it;
            if ( lpShape != NULL )
            {
                delete lpShape;
                lpShape = NULL;
            }
        }
        m_textShapes.clear();

        for ( CP_VECTOR<ZNMapLineShape*>::iterator it=m_lineShapes.begin(); it!=m_lineShapes.end(); it++ )
        {
            ZNMapLineShape* lpShape = *it;
            if ( lpShape != NULL )
            {
                delete lpShape;
                lpShape = NULL;
            }
        }
        m_lineShapes.clear();

        for ( CP_VECTOR<ZNMapLinePolygonShape*>::iterator it=m_linePolygonShapes.begin(); it!=m_linePolygonShapes.end(); it++ )
        {
            ZNMapLinePolygonShape* lpShape = *it;
            if ( lpShape != NULL )
            {
                delete lpShape;
                lpShape = NULL;
            }
        }
        m_linePolygonShapes.clear();

        for ( CP_VECTOR<ZNMapPolygonShape1*>::iterator it=m_polygonShapes.begin(); it!=m_polygonShapes.end(); it++ )
        {
            ZNMapPolygonShape1* lpShape = *it;
            if ( lpShape != NULL )
            {
                delete lpShape;
                lpShape = NULL;
            }
        }
        m_polygonShapes.clear();

        for ( CP_VECTOR<ZNMapPolygonShape2*>::iterator it=m_polygonShape2s.begin(); it!=m_polygonShape2s.end(); it++ )
        {
            ZNMapPolygonShape2* lpShape = *it;
            if ( lpShape != NULL )
            {
                delete lpShape;
                lpShape = NULL;
            }
        }
        m_polygonShape2s.clear();

        for ( CP_VECTOR<ZNMapPolygonShape3*>::iterator it=m_polygonShape3s.begin(); it!=m_polygonShape3s.end(); it++ )
        {
            ZNMapPolygonShape3* lpShape = *it;
            if ( lpShape != NULL )
            {
                delete lpShape;
                lpShape = NULL;
            }
        }
        m_polygonShape3s.clear();

        for ( CP_VECTOR<ZNMapPolygonShape4*>::iterator it=m_polygonShape4s.begin(); it!=m_polygonShape4s.end(); it++ )
        {
            ZNMapPolygonShape4* lpShape = *it;
            if ( lpShape != NULL )
            {
                delete lpShape;
                lpShape = NULL;
            }
        }
        m_polygonShape4s.clear();

        for ( CP_VECTOR<ZNMapPolygonShape5*>::iterator it=m_polygonShape5s.begin(); it!=m_polygonShape5s.end(); it++ )
        {
            ZNMapPolygonShape5* lpShape = *it;
            if ( lpShape != NULL )
            {
                delete lpShape;
                lpShape = NULL;
            }
        }
        m_polygonShape5s.clear();

        for ( CP_VECTOR<ZNMapPolygonShape6*>::iterator it=m_polygonShape6s.begin(); it!=m_polygonShape6s.end(); it++ )
        {
            ZNMapPolygonShape6* lpShape = *it;
            if ( lpShape != NULL )
            {
                delete lpShape;
                lpShape = NULL;
            }
        }
        m_polygonShape6s.clear();

        for ( CP_VECTOR<ZNMapSignImageShape*>::iterator it=m_signImageShapes.begin(); it!=m_signImageShapes.end(); it++ )
        {
            ZNMapSignImageShape* lpShape = *it;
            if ( lpShape != NULL )
            {
                delete lpShape;
                lpShape = NULL;
            }
        }
        m_signImageShapes.clear();

        for ( CP_VECTOR<ZNMapSignTextShape*>::iterator it=m_signTextShapes.begin(); it!=m_signTextShapes.end(); it++ )
        {
            ZNMapSignTextShape* lpShape = *it;
            if ( lpShape != NULL )
            {
                delete lpShape;
                lpShape = NULL;
            }
        }
        m_signTextShapes.clear();
    }

};
typedef _tag_znmapdata_for_display ZNMAPDATAREP, *LPZNMAPDATAREP;
