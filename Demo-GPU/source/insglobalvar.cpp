#include "insglobalvar.h"


#pragma execution_character_set("utf-8")  //解决中文乱码

MainWindow *InsGlobalVar::insMain = NULL;
bool InsGlobalVar::pixelView_update_flag = FALSE;
int InsGlobalVar::m_Width = 4100;
int InsGlobalVar::m_Height = 40000;//16384;//40000//0907
//InsCam InsGlobalVar::m_InsCam;
bool InsGlobalVar::cam_update = false;
int InsGlobalVar::nFrameCount = 0;
uint32_t InsGlobalVar::frameID = 0;
bool InsGlobalVar::m_saveImage = false;
int InsGlobalVar::lines = 1;//采集卡帧

//===================1222在线升级=================
InsUpgrad InsGlobalVar::m_insUpgrad;
long InsGlobalVar::lastLen = 0;
long InsGlobalVar::fileLen = 0;
long InsGlobalVar::sendCnt = 0;
int InsGlobalVar::percent = 0;

InsGlobalVar::InsGlobalVar()
{

}
