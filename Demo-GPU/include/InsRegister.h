#ifndef INSREGISTER_H
#define INSREGISTER_H



#define BOARD_FRMAE_COUNT 1// 图像缓冲区申请的帧数

struct InsCam
{
    int m_width;
    int m_height;
    int m_type;
    int m_exposure;
    int m_frameCount;
    int m_triggerMode;//IKP_RCV_MORE_DATA_IN_TRIGGER_MODE

    int m_stWidth;//IKP_SOFTWARE_TRIGGER_WIDTH
    int m_stPeriod;//IKP_SOFTWARE_TRIGGER_PERIOD
    int m_stCount;//IKP_SOFTWARE_TRIGGER_COUNT
    int m_stDelay;//IKP_SOFTWARE_TRIGGER_DELAY
    int m_stPolarity;// IKP_SFOTWARE_TRIGGER_POLARITY
    int m_stSyncMode;//IKP_SOFTWARE_TRIGGER_SYNC_MODE

    int m_ioTriggerMode;
    int m_ioPolarity;
    int m_ioMinWidth;

    int m_strobeSource;
    int m_strobeMethod;
    int m_strobePolarity;
    int m_strobeDelay;
    int m_strobeWidth;

    int m_etChannel;// IKP_SHAFT_ENCODER1_CHANNEL
    int m_etMultiFactor;//IKP_SHAFT_ENCODER1_MULTIPlY_FACTOR
    int m_etDivisionFactor;
    int m_etMinWidth;//IKP_SHAFT_ENCODER1_MIN_WIDTH
    int m_etValidDirection;// IKP_SHAFT_ENCODER1_VALID_DIRECTION
    int m_etReverseComp;// IKP_SHAFT_ENCODER1_REVERSE_COMPENSATION
    int m_etQFSource;//IKP_SHAFT_ENCODER1_QUAD_FREQUENCY_SOURCE_TYPE
    int m_etReverseCompLimit;// IKP_SHAFT_ENCODER1_REVERSE_COMPENSATION_LIMIT
    int m_etCDCType;//IKP_SHAFT_ENCODER1_CLOCK_DUTY_COMPENSATION_TYPE
    int m_etCDCWidth;// IKP_SHAFT_ENCODER1_CLOCK_DUTY_COMPENSATION_WIDTH

};




#endif // INSREGISTER_H
