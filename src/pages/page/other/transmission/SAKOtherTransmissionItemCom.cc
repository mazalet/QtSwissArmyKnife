﻿/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include <QDebug>
#include <QMetaType>

#ifdef SAK_IMPORT_COM_MODULE
#include <QSerialPortInfo>
Q_DECLARE_METATYPE(QSerialPortInfo)
#endif

#include "SAKGlobal.hh"
#include "SAKDebugPage.hh"
#include "SAKOtherTransmissionItem.hh"
#include "SAKSerialPortTransmissionItemWidget.hh"

#include "ui_SAKSerialPortTransmissionItemWidget.h"

SAKSerialPortTransmissionItemWidget::SAKSerialPortTransmissionItemWidget(SAKDebugPage *_debugPage, QWidget *parent)
    :SAKOtherTransmissionItem (_debugPage, parent)
#ifdef SAK_IMPORT_COM_MODULE
    ,ui (new Ui::SAKSerialPortTransmissionItemWidget)
    ,serialPort (Q_NULLPTR)
#endif
{    
#ifdef SAK_IMPORT_COM_MODULE
    ui->setupUi(this);
    enableCheckBox              = ui->enableCheckBox;
    handleReceiveDataCheckBox   = ui->handleReceiveDataCheckBox;
    comComboBox                 = ui->comComboBox;
    customBaudrateCheckBox      = ui->customBaudrateCheckBox;
    baudRateComboBox            = ui->baudRateComboBox;
    dataBitscomboBox            = ui->dataBitscomboBox;
    stopBitscomboBox            = ui->stopBitscomboBox;
    parityComboBox              = ui->parityComboBox;

    SAKGlobal::initComComboBox(comComboBox);
    SAKGlobal::initBaudRateComboBox(baudRateComboBox);
    SAKGlobal::initDataBitsComboBox(dataBitscomboBox);
    SAKGlobal::initStopBitsComboBox(stopBitscomboBox);
    SAKGlobal::initParityComboBox(parityComboBox);

    handleReceiveDataCheckBox->setChecked(true);
#else
    QHBoxLayout *layout = new QHBoxLayout(this);
    QLabel *label = new QLabel("该版本软件不支持数据通过串口转发", this);
    label->setObjectName(QString("label%1fkldfjsdajflsadfjasdf").arg(__LINE__));
    label->setStyleSheet(QString("QLabel#%1{color:red}").arg(label->objectName()));
    label->setAlignment(Qt::AlignHCenter);
    layout->addWidget(label, Qt::AlignHCenter);
    setLayout(layout);
#endif
}

#ifdef SAK_IMPORT_COM_MODULE
SAKSerialPortTransmissionItemWidget::~SAKSerialPortTransmissionItemWidget()
{
    delete ui;
}
#endif

#ifdef SAK_IMPORT_COM_MODULE
void SAKSerialPortTransmissionItemWidget::write(QByteArray data)
{
    if (serialPort){
        if (serialPort->write(data)){
#ifdef QT_DEBUG
            qDebug() << "send data error:" << serialPort->errorString();
#endif
        }
    }
}
#endif

#ifdef SAK_IMPORT_COM_MODULE
void SAKSerialPortTransmissionItemWidget::on_enableCheckBox_clicked()
{
    // c++11 lambda
    auto closeDev = [&](){
        if (serialPort){
            disconnect(serialPort, &QSerialPort::readyRead, this, &SAKSerialPortTransmissionItemWidget::read);
            serialPort->close();
            serialPort->deleteLater();
            serialPort = Q_NULLPTR;
            this->setUiEnable(true);
        }
    };

    if (enableCheckBox->isChecked()){
        serialPort = new QSerialPort(comComboBox->currentData().value<QSerialPortInfo>());
        serialPort->setBaudRate(baudRateComboBox->currentData().value<qint32>());
        serialPort->setParity(parityComboBox->currentData().value<QSerialPort::Parity>());
        serialPort->setDataBits(dataBitscomboBox->currentData().value<QSerialPort::DataBits>());
        serialPort->setStopBits(stopBitscomboBox->currentData().value<QSerialPort::StopBits>());
        if (serialPort->open(QSerialPort::ReadWrite)){
            this->setUiEnable(false);
            connect(serialPort, &QSerialPort::readyRead, this, &SAKSerialPortTransmissionItemWidget::read, Qt::QueuedConnection);
#ifdef QT_DEBUG
            qInfo() << tr("Open device successfully,")
                    << tr("name:") << serialPort->portName()
                    << tr("baudrate:") << serialPort->baudRate()
                    << tr("data bits:") << serialPort->dataBits()
                    << tr("parity:") << serialPort->parity();
#endif
        }else{
            emit requestOutputMessage(serialPort->errorString(), false);
            enableCheckBox->setChecked(false);
            closeDev();
        }
    }else{
        if (serialPort){
            closeDev();
        }
    }
}
#endif

#ifdef SAK_IMPORT_COM_MODULE
void SAKSerialPortTransmissionItemWidget::on_customBaudrateCheckBox_clicked()
{
    baudRateComboBox->setEditable(customBaudrateCheckBox->isChecked());
}
#endif

#ifdef SAK_IMPORT_COM_MODULE
void SAKSerialPortTransmissionItemWidget::read()
{
    if (serialPort){
        QByteArray data = serialPort->readAll();
        if (!data.isEmpty()){
            if (handleReceiveDataCheckBox->isChecked()){
                emit bytesRead(data);
            }
        }
    }
}
#endif

#ifdef SAK_IMPORT_COM_MODULE
void SAKSerialPortTransmissionItemWidget::setUiEnable(bool enable)
{
    comComboBox->setEnabled(enable);
    customBaudrateCheckBox->setEnabled(enable);
    baudRateComboBox->setEnabled(enable);
    dataBitscomboBox->setEnabled(enable);
    stopBitscomboBox->setEnabled(enable);
    parityComboBox->setEnabled(enable);
}
#endif