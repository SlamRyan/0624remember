#include "updatethread.h"
#include"insglobalvar.h"

#include <QDebug>
#include "QTime"
#include "modbus_udp.h"
#include "Ins_Utility_proc.h"



#define MAX_BLOCK 8
#define MAX_BYTES 4096 //数据块大小 //根据目标最大缓冲区

static const uint8_t aucCRCHi[] = {
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40
};

static const uint8_t aucCRCLo[] = {
    0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7,
    0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E,
    0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09, 0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9,
    0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC,
    0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
    0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32,
    0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D,
    0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A, 0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38,
    0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF,
    0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
    0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1,
    0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4,
    0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F, 0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB,
    0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA,
    0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
    0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0,
    0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97,
    0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C, 0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E,
    0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89,
    0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
    0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83,
    0x41, 0x81, 0x80, 0x40
};




UpDateThread::UpDateThread(QObject* parent) : QThread(parent)
{
    iStep = -1;
}

void UpDateThread::setData(uint8_t* data, int64_t len, int64_t cnt, int64_t lastlen, int binType, int baseAddr)
{
    _mData = new uint8_t[len];
    _mData = data;// memcpy(_mData, data, len);
    mLength = len;
    mCnt = cnt;//总页数  10400/512 = 21
    mLastlen = lastlen; //最后一页长度  //10400 %512  =144
    mBinType = binType;
    mBaseAddr = baseAddr;
}

void UpDateThread::run()//升级线程的主要执行函数
{

    uint16_t* p = (uint16_t*)_mData;//8->16  //单字节处理->双字节处理


    LightSignStrUpDate = DEVICE::get_instance()->length;//光源长度 //0321
    qDebug() << "length:::::::::::::::::::::::" << LightSignStrUpDate;
    LightSignIntUpDate = LightSignStrUpDate.toInt();  //0321
    qDebug() << "LightSignIntUpDate::::" << LightSignIntUpDate;


    m_run = false;
    iStep = 0;
    int lastBlockCnt = 0; //最后一个数据块的字节大小
    InsUpgrad insBoot;//升级状态
    int mcnt = (int)ceil(mCnt / (MAX_BLOCK * 1.0));//CRC发送次数
    int lastcnt = mCnt % MAX_BLOCK;//最后一次发送的页数  总发送页数/8
    qDebug() << "lastCnt" << lastcnt;
    if (0 == lastcnt) //如果为0说明8的倍数
    {   //恰巧是MAX_BLOCK的整数倍
        lastcnt = MAX_BLOCK; //最后一次发送的页数是8
        if (mLastlen == 0) // 总字节 % 512
        {
            lastBlockCnt = MAX_BYTES; //lastBlockCnt 最后数据块内的字节数
        }
        else// 总字节% 512 不为0 ，最后一页不是512
        {
            lastBlockCnt = (lastcnt - 1) * 512 + mLastlen;
        }
    }
    else
    {   //最后一块的字节长度
        lastBlockCnt = (lastcnt - 1) * 512 + mLastlen;
    }
    //qDebug()<<"lastBlockCnt:"<<lastBlockCnt<<"   lastcnt:"<<lastcnt;
    //开始升级
    insBoot.inUpdate = 1;
    insBoot.baseAddr = mBaseAddr; //地址
    insBoot.totalPage = mCnt; //总页数
    insBoot.currentPage = 0;
    insBoot.pageSize = MAX_BYTES;
    insBoot.upgradStatus = 0;
    qDebug() << "insBoot.baseAddr" << insBoot.baseAddr << " insBoot.totalPage" << insBoot.totalPage;

    qDebug() << "Run 1";

    InsGlobalVar::insMain->WriteUpgradInfo(insBoot);//将insBoot结构中的信息写入到设备

    emit sig_sendState("Bootloader initial......", false);//升级状态信号
    //0x04000100
    uint16_t* lastPage = new uint16_t[0x204];//正常状态下每次发送crc的一页//8->16

    uint8_t* plastPage = (uint8_t*)lastPage;//

    //uint8_t *lastBlock = new uint8_t[mLastlen+0x004];//最后一次发送crc的一页
    //uint32_t bAddr = 0x00008100;//起始基地址 //1213ADDR

    int currentBlock = 0;//当前发包块索引
    int currentPage = 0;//当前发包页索引

    uint16_t crc = 0;//CRC校验位

    qDebug() << "RUN 2";
    while (!m_run)
    {
        msleep(1);//0321
        if (iStep == 0)//发送clear_cnt
        {
            //Ins_Globalvar::insMain->CIS_Set_ClearCnt(0,1,1000);//8100udp_write

            qDebug() << "iStep == 0";
            uint32_t clearOne = 1;
            uint16_t clearOneHigh = (clearOne >> 16) & 0xFFFF;
            uint16_t clearOneLow = clearOne & 0xFFFF;
            uint16_t bufferOne[2] = { clearOneHigh,clearOneLow };


            if (InsGlobalVar::insMain->UpdateTenLength == 10)//0108
            {
                qDebug() << "--------------10";
                //qDebug() << "InsMain :: LightSignInt::::::" << InsGlobalVar::insMain->UpdateTenLength << " Clear_CNT";
                Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD, 0x4080, bufferOne);//发送Cnt CLEAR
                qDebug() << "LightSingIntUpdate == 10,write";
            }
            else if (InsGlobalVar::insMain->UpdateTenLength == 20)//0108
            {
                qDebug() << "LightSignInt::::::" << InsGlobalVar::insMain->UpdateTenLength << "  Clear_CNT";
                Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD, 0x4080, bufferOne);
                Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(SECONDBOARD, 0x4080, bufferOne);
            }

            else if (InsGlobalVar::insMain->UpdateTenLength == 30)//0108
            {
                qDebug() << "LightSignInt::::::" << InsGlobalVar::insMain->UpdateTenLength << "  Clear_CNT";
                Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD, 0x4080, bufferOne);
                Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(SECONDBOARD, 0x4080, bufferOne);
                Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(THIRDBOARD, 0x4080, bufferOne);
            }
            else if (InsGlobalVar::insMain->UpdateTenLength == 40)
            {
                qDebug() << "--------------50";
                Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD, 0x4080, bufferOne);
                Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(SECONDBOARD, 0x4080, bufferOne);
                Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(THIRDBOARD, 0x4080, bufferOne);
                Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FOURTHBOARD, 0x4080, bufferOne);
            }
            else if (InsGlobalVar::insMain->UpdateTenLength == 50)
            {
                qDebug() << "--------------50";
                Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD, 0x4080, bufferOne);
                Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(SECONDBOARD, 0x4080, bufferOne);
                Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(THIRDBOARD, 0x4080, bufferOne);
                Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FOURTHBOARD, 0x4080, bufferOne);
                Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIFTHBOARD, 0x4080, bufferOne);
            }
            else if (InsGlobalVar::insMain->UpdateTenLength == 60)
            {
                qDebug() << "--------------50";
                Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD, 0x4080, bufferOne);
                Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(SECONDBOARD, 0x4080, bufferOne);
                Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(THIRDBOARD, 0x4080, bufferOne);
                Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FOURTHBOARD, 0x4080, bufferOne);
                Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIFTHBOARD, 0x4080, bufferOne);
                Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(SIXTHBOARD, 0x4080, bufferOne);
            }


            msleep(10);
            uint32_t clearStatus = 999;

            bool flag = false;
            QTime m_time;
            m_time.start();
            do//0108need改
            {

                uint16_t bufferStatus[2];
                Ins_Modbus_udp::get_instance()->ins_modbus_udp_read(FIRSTBOARD, 0x4080, bufferStatus);//flag//0108need改
                clearStatus = (static_cast<uint32_t>((bufferStatus[0]) << 16) | bufferStatus[1]);

                qDebug() << "clearStatus" << clearStatus;



                if (clearStatus == 0)
                {
                    flag = true;
                    break;
                }
                msleep(1);
            } while (m_time.elapsed() < 2000);


            if (flag)
            {
                iStep = 1;
                qDebug() << "iStep" << iStep;
            }
            else
            {
                emit sig_sendState("Clear cnt failed!", true);
                iStep = -1;

                qDebug() << "iStep" << iStep;
            }
            continue;
        }
        if (iStep == 1)
        {
            if (mcnt > 1)//前N次  mcnt 剩余发送数据块的数量
            {
                qDebug() << "iStep111";
                crc = usMBCRC16(_mData + currentBlock * (0x1000), (uint16_t)0x1000);//计算校验位   blockOffset每次以0x1000增加    //4096 数据块计算一次
                qDebug() << "crc" << crc;
                //qDebug()<<"mcnt:"<<mcnt<<"  currentBlock:"<<currentBlock<<"  crc:"<<crc;
                //前MAX_BLOCK-1次
                for (int i = 0; i < MAX_BLOCK - 1; i++)
                {
                    //0321
                    //Update_Modbus_udp::get_instance()->init_ins_modbus_udp("192.168.1.11", 65530);
                    qDebug() << "LightSignInt::::::" << LightSignIntUpDate;
                    Update_Modbus_udp::get_instance()->update_udp_write(FIRSTBOARD, 0, 512 / 2, p + currentPage * (0x200) / 2);
                    currentPage++;
                }
                //第MAX_BLOCK次
                //endBlock = _mData+currentPage*(0x200);
                memcpy(lastPage, _mData + currentPage * (0x200), 0x200 * sizeof(uint8_t));//复制到lastPage

                qDebug() << "crc" << crc;
                plastPage[512] = crc >> 8;
                plastPage[513] = crc;
                qDebug() << "[512]" << lastPage[512];
                qDebug() << "[513]" << lastPage[513];

                //qDebug()<<"[512]"<<(uint16_t)plastPage[512];
                //qDebug()<<"[513]"<<(uint16_t)plastPage[513];

                qDebug() << "LightSignInt::::::" << LightSignIntUpDate;
                Update_Modbus_udp::get_instance()->update_udp_write(FIRSTBOARD, 0, (512 + 2) / 2, lastPage);

                qDebug() << "iStep222";
                currentPage++;  //增加页数
                //qDebug()<<"currentPage:"<<currentPage;
                //发送请求
                insBoot.upgradStatus = 1;
                insBoot.currentPage = currentPage;
                insBoot.pageSize = MAX_BYTES;
                InsGlobalVar::insMain->WriteUpgradInfo(insBoot);//写入状态
                //qDebug()<<"****************WriteInfo:";
                //读取返回状态，超时时间5s

                qDebug() << "before waiting ForStatus";

                msleep(100);//暂停100ms //0110

                int _status = waitingForStatus(10000);
                qDebug() << "_status:" << _status;

                //qDebug()<<"qianN次";

                qDebug() << "iStep333";
                if (_status == 0)//正常
                {
                    qDebug() << "------------------正常---------------";
                    iStep = 2;
                }
                else if (_status == -2)//busy状态
                {
                    qDebug() << "1-1-1-1-1-";
                    emit sig_sendState("Receive busy!", true);
                    iStep = -1;//失败返回
                }
                else if (_status == -1)//err状态
                {
                    qDebug() << "33333333";
                    emit sig_sendState("Receive error!", true);
                    iStep = 4;//resend
                }
            }

            else//最后一次 mcnt <= 1

            {
                //qDebug()<<"a"<<"   lastBlockCnt  "<<lastBlockCnt<<"  currentBlock:"<<currentBlock;
                qDebug() << "lastOne-RUNLASTONE";
                crc = usMBCRC16(_mData + currentBlock * (0x1000), (uint16_t)lastBlockCnt);//计算校验位 //lastBlockCnt 最有一块的字节数

                //qDebug()<<"mcnt:"<<mcnt<<"  currentBlock:"<<currentBlock<<"  crc:"<<crc;


                if (lastcnt == 1)  //最后一次发送页数 是 1页
                {
                    //qDebug()<<"b"<<"  mLastlen "<<mLastlen;
                    //lastBlock = _mData+currentPage*(0x200);
                    memcpy(lastPage, _mData + currentPage * (0x200), mLastlen * sizeof(uint8_t)); //_mData指向缓冲区的指针+当前数据块索引*4096，最后一块数据字节大小
                    qDebug() << "LastOnePage=1";
                    /*for(int i = 0;i<24;i++)
                    {
                        qDebug()<<"i="<<i<<"  lastPage:"<<lastPage[i];
                    }*/


                    //plastPage[mLastlen] = (uint8_t)(crc>>8);
                    //plastPage[mLastlen+1] = (uint8_t)(crc & 0x00ff);

                        //不完整的512

                    int dataSize = mLastlen == 0 ? 512 : mLastlen;  //mLastlen  10040 %512 = 144

                    //memcpy(lastPage, _mData + currentPage * (0x200), dataSize * sizeof(uint8_t)); //注释掉

                    plastPage[dataSize] = crc >> 8;
                    plastPage[dataSize + 1] = crc;

                    Update_Modbus_udp::get_instance()->update_udp_write(FIRSTBOARD, 0, (dataSize + 2) / 2, lastPage);
                    qDebug() << "最后一次write,页数是1";
                    qDebug() << "Last --- LightSignInt::::::" << LightSignIntUpDate;


                    currentPage++;
                    //发送请求
                    insBoot.upgradStatus = 1;
                    insBoot.currentPage = currentPage;
                    insBoot.pageSize = mLastlen;
                    InsGlobalVar::insMain->WriteUpgradInfo(insBoot);
                    //qDebug()<<"b finish";
                    //读取返回状态，超时时间5s
                    int _status = waitingForStatus(5000);
                    if (_status == 0)//正常
                    {
                        qDebug() << "c1";
                        iStep = 5;//完成
                    }
                    else if (_status == -2)//busy状态
                    {
                        qDebug() << "c2";
                        emit sig_sendState("Receive busy!", true);
                        iStep = -1;//失败返回
                    }
                    else if (_status == -1)//err状态
                    {
                        qDebug() << "c3";
                        emit sig_sendState("Receive error!", true);
                        iStep = 4;//resend
                    }
                }

                else //最后一次发送的页数不等于1

                {
                    //qDebug()<<"lastcnt:"<<lastcnt;
                    for (int i = 0; i < lastcnt - 1; i++)
                    {


                        Update_Modbus_udp::get_instance()->update_udp_write(FIRSTBOARD, 0, 512 / 2, p + currentPage * (0x200) / 2);
                        //qDebug()<<"最后一次write,LastOnePage != 1";
                        qDebug() << "Last --- LightSignInt::::::" << LightSignIntUpDate;

                        //Ins_Globalvar::insMain->mDeviceAdapter->WriteMemory((const uint8_t *)(_mData+currentPage*(0x200)),(int64_t)(bAddr+(currentPage%8)*(0x200)),(int64_t)0x200);
                        //udp_write
                        currentPage++;
                    }
                    //qDebug()<<"currentPage:"<<currentPage<<"  mLastlen:"<<mLastlen;
                    //第8次
                    //lastBlock = _mData+currentPage*(0x200);

                    memcpy(lastPage, _mData + currentPage * (0x200), mLastlen * sizeof(uint8_t));

                    qDebug() << "Lastcrc:" << crc << endl;

                    //不完整的512
                    int dataSize = mLastlen == 0 ? 512 : mLastlen;  //mLastlen  10040 %512 = 144
                    qDebug() << "dataSize===" << dataSize;

                    //memcpy(lastPage, _mData + currentPage * (0x200), dataSize * sizeof(uint8_t));//注释掉

                    plastPage[dataSize] = crc >> 8;
                    plastPage[dataSize + 1] = crc;
                    qDebug() << "[512]" << (uint16_t)plastPage[dataSize];
                    qDebug() << "[513]" << (uint16_t)plastPage[dataSize + 1];

                    Update_Modbus_udp::get_instance()->update_udp_write(FIRSTBOARD, 0, 512 / 2, lastPage);
                    qDebug() << "最后一次write,lastPage";
                    qDebug() << "LastLast --- LightSignInt::::::" << LightSignIntUpDate;

                    //udp_write
                    currentPage++;
                    //qDebug()<<"currentPage:"<<currentPage;
                    //发送请求
                    insBoot.upgradStatus = 1;
                    insBoot.currentPage = currentPage;
                    insBoot.pageSize = lastBlockCnt;//(lastcnt-1)*512+mLastlen;
                    //qDebug()<<"insBoot.pageSize:"<<insBoot.pageSize;
                    InsGlobalVar::insMain->WriteUpgradInfo(insBoot);
                    //读取返回状态，超时时间5s
                    int _status = waitingForStatus(5000);
                    //qDebug()<<"_status:"<<_status;
                    if (_status == 0)//正常
                    {
                        iStep = 5;//完成
                    }
                    else if (_status == -2)//busy状态
                    {
                        emit sig_sendState("Receive busy!", true);
                        iStep = -1;//失败返回
                    }
                    else if (_status == -1)//err状态
                    {
                        emit sig_sendState("Receive error!", true);
                        iStep = 4;//resend
                    }
                }
            }
            continue;
        }
        if (iStep == 2)//goon
        {
            currentBlock++;//处理下一个数据块
            InsGlobalVar::percent = (int)(((double)currentPage / (double)mCnt) * 100);//计算百分比
            qDebug() << "Ins_Globalvar::percent:" << InsGlobalVar::percent << "  currentPage:" << currentPage << "  mCnt: " << mCnt;
            mcnt--;  //减去一页
            msleep(1);
            iStep = 1;
            continue;
        }
        if (iStep == 3)//timeout /wait
        {
            QTime m_time;
            m_time.start();
            bool flag = false;
            do
            {
                InsUpgrad insStatus = InsGlobalVar::insMain->ReadUpgradInfo();//111111111
                if (insStatus.upgradStatus == 0 || insStatus.upgradStatus == 3)//idel或者OK
                {
                    flag = true;
                    break;
                }
                msleep(100);
            } while (m_time.elapsed() < 10000);
            if (!flag)
            {
                iStep = -1;
            }
            else
            {
                iStep = 1;
            }
            continue;
        }
        if (iStep == 4)//resend
        {
            currentPage = currentBlock * MAX_BLOCK;
            iStep = 0;
            continue;
        }
        if (iStep == 5)//finish
        {
            //qDebug()<<"finish";
            InsGlobalVar::percent = 100;
            emit sig_sendState("Bootloader finished!", false);
            m_run = true;
            break;
        }
        if (iStep == -1)
        {
            emit sig_sendState("Bootloader failed!", true);
            m_run = true;
            break;
        }

    }
}

int UpDateThread::waitingForStatus(uint64_t elapseTime)//等待的最长时间 //轮询机制
{
    QTime m_time;
    m_time.start();//跟踪已经使用的时间
    int _status = -1;//读取状态位
    InsUpgrad insStatus;
    do
    {

        qDebug() << "read here1";
        msleep(1);//暂停1ms

        insStatus = InsGlobalVar::insMain->ReadUpgradInfo();
        qDebug() << "ReadUpgradInfo";
        if (insStatus.upgradStatus == 0 || insStatus.upgradStatus == 3)//idle或者OK
        {
            qDebug() << "read here2";
            _status = 0;
            break;
        }
        else if (insStatus.upgradStatus == 2)//busy
        {
            qDebug() << "****************************busy";
            _status = -2;
            msleep(10);
        }
        else if (insStatus.upgradStatus == 4)//err
        {
            qDebug() << "read here2 err";
            _status = -1;
            break;
        }
    } while (m_time.elapsed() < elapseTime);//循环条件为已经过去的时间小于elapseTime
    return _status;
}


uint16_t UpDateThread::usMBCRC16(uint8_t* pucFrame, uint16_t uslen)// 计算数据块的CRC校验码
{
    uint8_t ucCRCHi = 0xFF;
    uint8_t ucCRCLo = 0xFF;
    int iIndex;
    while (uslen--) //遍历pucFrame指向的数据
    {
        iIndex = ucCRCLo ^ *(pucFrame++);//低位异或运算，结果用于查找CRC高位和低位新值，相同为0，不同为1
        ucCRCLo = (uint8_t)(ucCRCHi ^ aucCRCHi[iIndex]);//使用预先计算好的表，所有可能的高位和低位的结果
        ucCRCHi = aucCRCLo[iIndex];
    }
    return (uint16_t)(ucCRCHi << 8 | ucCRCLo);//合并为一个CRC-16校验码
}