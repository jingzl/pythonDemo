// TestLib.cpp : ���� DLL Ӧ�ó���ĵ���������
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
    sprintf( g_szMsg, "hello, ���Ǵ�c���з��ص��ַ�����%s", str );
    cout<<g_szMsg<<endl;
    //void* p = g_szMsg;
    //cout<<p<<endl;
    return g_szMsg;
}

// ����Ϊ����python��c��չ���������д������
std::string Recognise_Img(const std::string url)
{
    //���ؽ��
    string str = "��dll�з��ص�����... : " +url;
    return str;
}

// �����ַ���
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

// ��������
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
    // ���ƣ��������������ͺ��ĵ��ַ���
    // ��¶��Python�ĺ���
    {"Recognise", Recognise, METH_VARARGS, "recognise char something"},
    {"RecogniseInt", RecogniseInt, METH_VARARGS, "recognise int something"},
    // һ���б�����ı��
    {NULL,NULL,0,NULL}        /* Sentinel */
};

extern "C" __declspec(dllexport) void initTestLib()
{
    PyObject *m, *d;
    m = Py_InitModule("TestLib", AllMyMethods); //��ʼ����ģ�飬����¶����
    d = PyModule_GetDict(m);
}

