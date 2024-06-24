#include "modbuscommit.h"
#include <QDebug>
#include <string>
#include <QVector>

#pragma execution_character_set("utf-8")

ModbusCommit::ModbusCommit(QWidget *parent)
    : QWidget{parent}
{
    modbusClient = new QModbusTcpClient;
    connect(modbusClient,&QModbusTcpClient::stateChanged,this,&ModbusCommit::state_change_slot);
}

ModbusCommit::~ModbusCommit()
{
    if(modbusClient !=nullptr)
    {
        modbusClient->disconnect();
        modbusClient->deleteLater();
        delete modbusClient;
    }
}

bool ModbusCommit::modbus_connect(QString ip, quint16 port)
{
    if(modbusClient->state() == QModbusDevice::ConnectedState)
    {
        return true;
    }
    //配置modbus tcp的连接参数 IP + Port   modbus协议的端口号为502
    modbusClient->setConnectionParameter(QModbusDevice::NetworkAddressParameter,ip);
    modbusClient->setConnectionParameter(QModbusDevice::NetworkPortParameter,port);
    modbusClient->setTimeout(500);
    modbusClient->setNumberOfRetries(3);
    return modbusClient->connectDevice();
}

void ModbusCommit::modbus_disconnect()
{
    modbusClient->disconnectDevice();
}

bool ModbusCommit::modbus_read_coils(quint8 slaveId, quint16 readStartAddr, quint16 readNum)
{
    if(modbusClient->state() != QModbusDevice::ConnectedState)
        return false;
    QModbusDataUnit readDataUnit;
    readDataUnit.setRegisterType(QModbusDataUnit::Coils);
    readDataUnit.setStartAddress(readStartAddr);
    readDataUnit.setValueCount(readNum);

    qDebug()<< "配置ReadUnit完成";
    if (auto *reply = modbusClient->sendReadRequest(readDataUnit, slaveId))     //1是Server_ID
    {
        if (!reply->isFinished())
        {
            QObject::connect(reply, &QModbusReply::finished,this,&ModbusCommit::readready_coils_slot);
            return true;
        }
        else
        {
            delete reply;
            return false;
        }
    }
    else
    {
        return false;
    }

}

bool ModbusCommit::modbus_read_holding(quint8 slaveId, quint16 readStartAddr, quint16 readNum)
{
    if(modbusClient->state() != QModbusDevice::ConnectedState)
        return false;
    QModbusDataUnit readDataUnit;
    readDataUnit.setRegisterType(QModbusDataUnit::HoldingRegisters);
    readDataUnit.setStartAddress(readStartAddr);
    readDataUnit.setValueCount(readNum);

    if (auto *reply = modbusClient->sendReadRequest(readDataUnit, slaveId))     //1是Server_ID
    {
        if (!reply->isFinished())
        {
            QObject::connect(reply, &QModbusReply::finished,this,&ModbusCommit::readready_holding_slot);
            return true;
        }
        else
        {
            delete reply;
            return false;
        }

    }
    else
        return false;

}

bool ModbusCommit::modbus_read_input(quint8 slaveId, quint16 readStartAddr, quint16 readNum)
{
    if(modbusClient->state() != QModbusDevice::ConnectedState)
        return false;
    QModbusDataUnit readDataUnit;
    readDataUnit.setRegisterType(QModbusDataUnit::InputRegisters);
    readDataUnit.setStartAddress(readStartAddr);
    readDataUnit.setValueCount(readNum);

    qDebug()<< "配置ReadUnit完成";
    if (auto *reply = modbusClient->sendReadRequest(readDataUnit, slaveId))     //1是Server_ID
    {
        if (!reply->isFinished())
        {
            QObject::connect(reply, &QModbusReply::finished,this,&ModbusCommit::readready_input_slot);
            return true;
        }
        else
        {
            delete reply;
            return false;
        }
    }
    else
    {
        return false;
    }
}

bool ModbusCommit::modbus_read_discrete(quint8 slaveId, quint16 readStartAddr, quint16 readNum)
{
    if(modbusClient->state() != QModbusDevice::ConnectedState)
        return false;
    QModbusDataUnit readDataUnit;
    readDataUnit.setRegisterType(QModbusDataUnit::DiscreteInputs);
    readDataUnit.setStartAddress(readStartAddr);
    readDataUnit.setValueCount(readNum);

    qDebug()<< "配置ReadUnit完成";
    if (auto *reply = modbusClient->sendReadRequest(readDataUnit, slaveId))     //1是Server_ID
    {
        if (!reply->isFinished())
        {
            QObject::connect(reply, &QModbusReply::finished,this,&ModbusCommit::readready_input_slot);
            return true;
        }
        else
        {
            delete reply;
            return false;
        }
    }
    else
    {
        return false;
    }
}

bool ModbusCommit::modbus_write_coils(quint8 slaveId, quint16 writeStartAddr, quint16 writeNum, QVector<quint16> writeValue)
{
    if(modbusClient->state() != QModbusDevice::ConnectedState)
        return false;
    QModbusDataUnit writeDataUnit;
    writeDataUnit.setRegisterType(QModbusDataUnit::DiscreteInputs);
    writeDataUnit.setStartAddress(writeStartAddr);
    writeDataUnit.setValueCount(writeNum);
    writeDataUnit.setValues(writeValue);



    if (auto *reply = modbusClient->sendWriteRequest(writeDataUnit,slaveId))
    {
        if (!reply->isFinished())
        {
            connect(reply, &QModbusReply::finished, this,&ModbusCommit::writeready_coil_slot);
            return true;
        }
        else
        {
            if(reply != nullptr)
                reply->deleteLater();
            return false;
        }
    }
    else
    {
        return false;
    }
    return false;
}

bool ModbusCommit::modbus_write_holding(quint8 slaveId, quint16 writeStartAddr, quint16 writeNum, QVector<quint16> writeValue)
{
    if(modbusClient->state() != QModbusDevice::ConnectedState)
        return false;
    QModbusDataUnit writeDataUnit;
    writeDataUnit.setRegisterType(QModbusDataUnit::HoldingRegisters);
    writeDataUnit.setStartAddress(writeStartAddr);
    writeDataUnit.setValueCount(writeNum);
    writeDataUnit.setValues(writeValue);

    if (auto *reply = modbusClient->sendWriteRequest(writeDataUnit,slaveId))
    {
        if (!reply->isFinished())
        {
            connect(reply, &QModbusReply::finished, this,&ModbusCommit::writeready_holding_slot);
            return true;
        }
        else
        {
            if(reply != nullptr)
                reply->deleteLater();
            return false;
        }
    }
    else
    {
        return false;
    }
    return false;
}


void ModbusCommit::state_change_slot(QModbusDevice::State state)
{
    emit modbus_state_change_signal(state);
}

void ModbusCommit::error_occurred_slot(QModbusDevice::Error error)
{
    emit modbus_error_occurred_signal(error);
    qDebug()<<"22";
}

void ModbusCommit::readready_coils_slot()
{
    QModbusReply *reply = qobject_cast<QModbusReply *>(sender());
    if (!reply)
    {
        emit modbus_read_over_signal(QModbusDataUnit::RegisterType::Coils,true,this->readCoilVal);
        return ;
    }
    if (reply->error() == QModbusDevice::NoError)
    {
        const QModbusDataUnit unit = reply->result();
        readCoilVal = unit.values();
        emit modbus_read_over_signal(QModbusDataUnit::RegisterType::Coils,false,this->readCoilVal);
    }
    else
    {
        qDebug()<<"coil read error:"<<reply->error();
        emit modbus_read_over_signal(QModbusDataUnit::RegisterType::Coils,true,this->readCoilVal);
    }
    if(reply != nullptr)
        reply->deleteLater();
}

void ModbusCommit::readready_holding_slot()
{
    QModbusReply *reply = qobject_cast<QModbusReply *>(sender());
    if (!reply)
    {
        emit modbus_read_over_signal(QModbusDataUnit::RegisterType::HoldingRegisters,true,this->readHoldingVal);
        return ;
    }
    if (reply->error() == QModbusDevice::NoError)
    {
        const QModbusDataUnit unit = reply->result();
        readHoldingVal = unit.values();
        emit modbus_read_over_signal(QModbusDataUnit::RegisterType::HoldingRegisters,false,this->readHoldingVal);
    }
    else
    {
        qDebug()<<"holding read error:"<<reply->error();
        emit modbus_read_over_signal(QModbusDataUnit::RegisterType::HoldingRegisters,true,this->readHoldingVal);
    }
    if(reply != nullptr)
        reply->deleteLater();
}

void ModbusCommit::readready_input_slot()
{
    QModbusReply *reply = qobject_cast<QModbusReply *>(sender());
    if (!reply)
    {
        emit modbus_read_over_signal(QModbusDataUnit::RegisterType::InputRegisters,true,this->readInputVal);
        return ;
    }
    if (reply->error() == QModbusDevice::NoError)
    {
        const QModbusDataUnit unit = reply->result();
        readInputVal = unit.values();
        emit modbus_read_over_signal(QModbusDataUnit::RegisterType::InputRegisters,false,this->readInputVal);
    }
    else
    {
        qDebug()<<"input read error:"<<reply->error();
        emit modbus_read_over_signal(QModbusDataUnit::RegisterType::InputRegisters,true,this->readInputVal);
    }
    if(reply != nullptr)
        reply->deleteLater();
}

void ModbusCommit::readready_discrete_slot()
{
    QModbusReply *reply = qobject_cast<QModbusReply *>(sender());
    if (!reply)
    {
        emit modbus_read_over_signal(QModbusDataUnit::RegisterType::DiscreteInputs,true,this->readDiscreteVal);
        return ;
    }
    if (reply->error() == QModbusDevice::NoError)
    {
        const QModbusDataUnit unit = reply->result();
        readDiscreteVal = unit.values();
        emit modbus_read_over_signal(QModbusDataUnit::RegisterType::DiscreteInputs,false,this->readDiscreteVal);
    }
    else
    {
        qDebug()<<"discrete read error:"<<reply->error();
        emit modbus_read_over_signal(QModbusDataUnit::RegisterType::DiscreteInputs,true,this->readDiscreteVal);
    }
    if(reply != nullptr)
        reply->deleteLater();
}

void ModbusCommit::writeready_coil_slot()
{
    QModbusReply *reply = qobject_cast<QModbusReply *>(sender());
    if (!reply)
    {
        emit modbus_write_res(QModbusDataUnit::RegisterType::Coils,true);
        return ;
    }
    if(reply->error() == QModbusDevice::NoError)
    {
        emit modbus_write_res(QModbusDataUnit::RegisterType::Coils,false);
    }
    else
    {
        qDebug()<<"write holding error:"<<reply->errorString()<<"    "<<reply->rawResult().exceptionCode();
        emit modbus_write_res(QModbusDataUnit::RegisterType::Coils,true);
    }
    if(reply != nullptr)
        reply->deleteLater();
}

void ModbusCommit::writeready_holding_slot()
{
    QModbusReply *reply = qobject_cast<QModbusReply *>(sender());
    if (!reply)
    {
        emit modbus_write_res(QModbusDataUnit::RegisterType::Coils,true);
        return ;
    }
    if(reply->error() == QModbusDevice::NoError)
    {
        emit modbus_write_res(QModbusDataUnit::RegisterType::HoldingRegisters,false);
    }
    else
    {
        qDebug()<<"write holding error:"<<reply->errorString()<<"    "<<reply->rawResult().exceptionCode();
        emit modbus_write_res(QModbusDataUnit::RegisterType::HoldingRegisters,true);
    }
    if(reply != nullptr)
        reply->deleteLater();
}
