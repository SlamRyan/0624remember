#ifndef UPDATETHREAD_H
#define UPDATETHREAD_H
#include<QThread>



class UpDateThread : public QThread
{
    Q_OBJECT

//==================================================================
public:
    UpDateThread(QObject *parent = nullptr);

    bool m_run;
    int iStep;

    uint8_t *_mData;
    int64_t mLength;
    int64_t mCnt;
    int64_t mLastlen;
    int mBinType;
    int mBaseAddr;

    QString LightSignStrUpDate;//1225
    int LightSignIntUpDate;

    void run();

    void setData(uint8_t *pdata,int64_t length,int64_t cnt,int64_t lastlen,int binType,int baseAddr); ;//准备升级数据和相关参数 √  //初始化升级过程中所需的数据

    uint16_t usMBCRC16(uint8_t *pucFrame,uint16_t uslen);//16位2字节校验码 √

    int waitingForStatus(uint64_t elapseTime);// 等待并检查目标设备的响应状态

signals:
    void sig_sendState(QString msg,bool flag);

};

#endif // UPDATETHREAD_H
