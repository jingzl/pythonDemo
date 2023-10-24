#pragma once



class CSysPara
{
public:
	CSysPara(void);
	~CSysPara(void);

    // ��ʼ��
	BOOL Init( const char* lpszPath );

    // ϵͳ��־�ļ�
    string m_strLogPath;
    // ϵͳ·��
    string m_strAppPath;

    // ����·��
    string m_strConfigPath;
    // ����·��
    string m_strDataPath;
    // ��ͼ�������·��
    string m_strOutputMapPath;
    // ע���������·��
    string m_strOutputTxtPath;

    // Ĭ�ϵ���ͼ��ʾģʽ
    BYTE m_nDisplayMode;
    // ��ͼȫ����Χ
    double m_dMinX;
    double m_dMinY;
    double m_dMaxX;
    double m_dMaxY;
    // �����ߵȼ�
    ST_SCALECFG_ARY m_MapScaleAry;

    // MapDisplayCfg File
    string m_strMapDisplayCfgFile;
    // DisplayModeCfg File
    string m_strDisplayModeCfgFile;

protected:
    void Release();
    int TransMLevel( const char* level );

};
