// DynMapTile.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "DynMapTile.h"
#include <Python.h>
#include "DynMapTileMgr.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// 初始化 MFC 并在失败时显示错误
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: 更改错误代码以符合您的需要
		_tprintf(_T("错误: MFC 初始化失败\n"));
		nRetCode = 1;
	}
	else
	{
		// TODO: 在此处为应用程序的行为编写代码。
	}

	return nRetCode;
}

//////////////////////////////////////////////////////////////////////////
// 
#if 0
int UnicodeToUTF8( wchar_t *unicodeStr, char * utfStr, int maxUtfStrlen )
{
    //char* locname = setlocale(LC_ALL,".65001");
    //printf("Current locale2 is: %s\n", locname);

    //将unicode编码转换为utf8编码
    /*if(NULL==setlocale(LC_ALL,"utf8"))//设置unicode转换后的码,当前为utf8
    {
    printf("Bad Parameter\n");
    return -1;
    }*/
    int utfLen=wcstombs(NULL,unicodeStr,0);//计算转换后的长度
    if(utfLen<=0)
    {
        printf("Can not Transfer!!!\n");
        return -1;
    }
    else if(utfLen>=maxUtfStrlen)//判断空间是否足够
    {
        printf("Dst Str memory not enough\n");
        return -1;
    }
    wcstombs(utfStr,unicodeStr,utfLen);
    utfStr[utfLen]=0;//添加结束符
    return utfLen;
}
#endif


static PyObject* GetMapTile( PyObject *self, PyObject *args )
{
    char* locname = setlocale(LC_ALL, NULL);
    printf("Current locale is: %s\n", locname);

    locname = setlocale(LC_ALL,"CHS");
    printf("Current locale is: %s\n", locname);

    char* path = NULL;
    int z = 0;
    double minx = 0.0;
    double miny = 0.0;
    double maxx = 0.0;
    double maxy = 0.0;
    if (!PyArg_ParseTuple(args, "sidddd", &path, &z, &minx, &miny, &maxx, &maxy ) )
    {
        printf("PyArg_ParseTuple error. \n");
        return NULL;
    }
    cout << path << endl;
    cout << "z=" << z << ":(" << minx << "," << miny << ";" << maxx << "," << maxy <<")" << endl;

    std::string strRes;
    CDynMapTileMgr dynMapTileMgr;
    if ( dynMapTileMgr.Init( path ) == 0 )
    {
        if ( dynMapTileMgr.GetMapTile( z, minx, miny, maxx, maxy ) == 0 )
        {
            strRes = "0";
        }
        else
        {
            strRes = "-1";
        }
    }
    else
    {
        strRes = "-1";
    }
    return Py_BuildValue("s#", strRes.c_str(), strRes.size() );
}



//////////////////////////////////////////////////////////////////////////
// 
static PyMethodDef AllMyMethods[] = 
{
    // 名称，函数，参数类型和文档字符串
    // 暴露给Python的函数
    {"GetMapTile", GetMapTile, METH_VARARGS, "get map tile info"},
    // 一个列表结束的标记
    {NULL,NULL,0,NULL}        /* Sentinel */
};

extern "C" DYNMAPTILE_API void initDynMapTile()
{
    PyObject *m, *d;
    m = Py_InitModule("DynMapTile", AllMyMethods); //初始化本模块，并暴露函数
    d = PyModule_GetDict(m);
}

