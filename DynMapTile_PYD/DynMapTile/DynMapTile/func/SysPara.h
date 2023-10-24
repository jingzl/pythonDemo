#pragma once



class CSysPara
{
public:
	CSysPara(void);
	~CSysPara(void);

    // 初始化
	BOOL Init( const char* lpszPath );

    // 系统日志文件
    string m_strLogPath;
    // 系统路径
    string m_strAppPath;

    // 配置路径
    string m_strConfigPath;
    // 数据路径
    string m_strDataPath;
    // 地图数据输出路径
    string m_strOutputMapPath;
    // 注记数据输出路径
    string m_strOutputTxtPath;

    // 默认的整图显示模式
    BYTE m_nDisplayMode;
    // 地图全部范围
    double m_dMinX;
    double m_dMinY;
    double m_dMaxX;
    double m_dMaxY;
    // 比例尺等级
    ST_SCALECFG_ARY m_MapScaleAry;

    // MapDisplayCfg File
    string m_strMapDisplayCfgFile;
    // DisplayModeCfg File
    string m_strDisplayModeCfgFile;

protected:
    void Release();
    int TransMLevel( const char* level );

};
