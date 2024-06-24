#ifndef INSGLOBALVAR_H
#define INSGLOBALVAR_H
#include "mainwindow.h"
#include "InsRegister.h"
#include "baseAddr.h"
#include "ins_camera_c.h"
class InsGlobalVar
{
public:
    InsGlobalVar();

    static MainWindow * insMain ;
    static bool pixelView_update_flag;
    static int m_Width;
    static int m_Height;
    //static InsCam m_InsCam;
    static bool cam_update;//第一次刷新参数
    static int nFrameCount;
    static uint32_t frameID;
    static bool m_saveImage;
    static int lines;

    //=================1222=======================
    static InsUpgrad m_insUpgrad;//升级相关信息  储存升级过程中的状态
    static long lastLen;
    static long fileLen;
    static long sendCnt;
    static int percent;//升级进度
};

#endif // INSGLOBALVAR_H
