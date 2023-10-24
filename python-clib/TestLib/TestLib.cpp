// TestLib.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "TestLib.h"
#include <iostream>
#include <string>
using namespace std;
#include<Python.h>


__declspec(dllexport) int add(int x,int y)
{
    cout<<x<<" "<<y<<endl;
    return x+y;
}

char g_szMsg[100] = {0};
__declspec(dllexport) char* testStr(const char* str)
{
    ZeroMemory( g_szMsg, 100*sizeof(char) );
    sprintf( g_szMsg, "hello, 这是从c库中返回的字符串，%s", str );
    cout<<g_szMsg<<endl;
    //void* p = g_szMsg;
    //cout<<p<<endl;
    return g_szMsg;
}

// 以下为按照python的c扩展库规则所编写的内容
std::string Recognise_Img(const std::string url)
{
    //返回结果
    string str = "从dll中返回的数据... : " +url;
    return str;
}

// 返回字符串
static PyObject* Recognise(PyObject *self, PyObject *args)
{
    const char *url;
    char url2[100] = {0};
    int len;
    std::string sts;
    if (!PyArg_ParseTuple(args, "s#", &url,&len))
    {
        printf("error\n");
        return NULL;
    }
    cout << url << endl;
    memcpy(url2,url,len);
    url2[len] = 0;
    sts = Recognise_Img(url);
    cout << sts << endl;
    memcpy( url2, sts.c_str(), sts.size() );
    return Py_BuildValue("s#", url2, sts.size() );
}

// 返回整型
static PyObject* RecogniseInt(PyObject *self, PyObject *args)
{
    int len;
    if (!PyArg_ParseTuple(args, "i", &len))
    {
        printf("error\n");
        return NULL;
    }
    int sum = len + 100;
    return Py_BuildValue("i", sum );
}

static PyMethodDef AllMyMethods[] = 
{
    // 名称，函数，参数类型和文档字符串
    // 暴露给Python的函数
    {"Recognise", Recognise, METH_VARARGS, "recognise char something"},
    {"RecogniseInt", RecogniseInt, METH_VARARGS, "recognise int something"},
    // 一个列表结束的标记
    {NULL,NULL,0,NULL}        /* Sentinel */
};

extern "C" __declspec(dllexport) void initTestLib()
{
    PyObject *m, *d;
    m = Py_InitModule("TestLib", AllMyMethods); //初始化本模块，并暴露函数
    d = PyModule_GetDict(m);
}

