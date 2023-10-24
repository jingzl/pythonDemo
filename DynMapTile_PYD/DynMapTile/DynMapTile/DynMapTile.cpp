// DynMapTile.cpp : ���� DLL Ӧ�ó���ĵ���������
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

	// ��ʼ�� MFC ����ʧ��ʱ��ʾ����
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: ���Ĵ�������Է���������Ҫ
		_tprintf(_T("����: MFC ��ʼ��ʧ��\n"));
		nRetCode = 1;
	}
	else
	{
		// TODO: �ڴ˴�ΪӦ�ó������Ϊ��д���롣
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

    //��unicode����ת��Ϊutf8����
    /*if(NULL==setlocale(LC_ALL,"utf8"))//����unicodeת�������,��ǰΪutf8
    {
    printf("Bad Parameter\n");
    return -1;
    }*/
    int utfLen=wcstombs(NULL,unicodeStr,0);//����ת����ĳ���
    if(utfLen<=0)
    {
        printf("Can not Transfer!!!\n");
        return -1;
    }
    else if(utfLen>=maxUtfStrlen)//�жϿռ��Ƿ��㹻
    {
        printf("Dst Str memory not enough\n");
        return -1;
    }
    wcstombs(utfStr,unicodeStr,utfLen);
    utfStr[utfLen]=0;//��ӽ�����
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
    // ���ƣ��������������ͺ��ĵ��ַ���
    // ��¶��Python�ĺ���
    {"GetMapTile", GetMapTile, METH_VARARGS, "get map tile info"},
    // һ���б�����ı��
    {NULL,NULL,0,NULL}        /* Sentinel */
};

extern "C" DYNMAPTILE_API void initDynMapTile()
{
    PyObject *m, *d;
    m = Py_InitModule("DynMapTile", AllMyMethods); //��ʼ����ģ�飬����¶����
    d = PyModule_GetDict(m);
}

