#ifndef MODBUSCOMMIT_H
#define MODBUSCOMMIT_H
#include <QWidget>
#include <QModbusTcpClient>
#include <QModbusDataUnit>
#include <QModbusReply>


class ModbusCommit:public QWidget
{
    Q_OBJECT

signals:
    void modbus_state_change_signal(QModbusDevice::State state);
    void modbus_error_occurred_signal(QModbusDevice::Error state);
    void modbus_read_over_signal(QModbusDataUnit::RegisterType type,bool isErr,QVector<quint16> readData);

    void modbus_write_res(QModbusDataUnit::RegisterType type,bool isErr);

public:
    explicit ModbusCommit(QWidget *parent = nullptr);
    ~ModbusCommit();
    bool modbus_connect(QString ip,quint16 port);
    void modbus_disconnect();
    bool modbus_read_coils(quint8 slaveId,quint16 readStartAddr,quint16 readNum);
    bool modbus_read_holding(quint8 slaveId,quint16 readStartAddr,quint16 readNum);
    bool modbus_read_input(quint8 slaveId,quint16 readStartAddr,quint16 readNum);
    bool modbus_read_discrete(quint8 slaveId,quint16 readStartAddr,quint16 readNum);

    bool modbus_write_coils(quint8 slaveId,quint16 writeStartAddr,quint16 writeNum,QVector<quint16> writeValue);
    bool modbus_write_holding(quint8 slaveId,quint16 writeStartAddr,quint16 writeNum,QVector<quint16> writeValue);

private slots:
    void state_change_slot(QModbusDevice::State state);//连接状态改变
    void error_occurred_slot(QModbusDevice::Error error);
    void readready_coils_slot();
    void readready_holding_slot();
    void readready_input_slot();
    void readready_discrete_slot();

    void writeready_coil_slot();
    void writeready_holding_slot();
public:



    QModbusTcpClient *modbusClient = nullptr;


    QVector<quint16> readCoilVal;
    QVector<quint16> readHoldingVal;
    QVector<quint16> readInputVal;
    QVector<quint16> readDiscreteVal;

    QString NowWeight;

};

#endif // MODBUSCOMMIT_H
