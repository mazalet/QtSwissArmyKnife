﻿/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include "SAKModbusCommonClientSection.hh"
#include "SAKModbusCommonSerialPortSection.hh"
#include "SAKModbusClientControllerSerialPort.hh"

SAKModbusClientControllerSerialPort::SAKModbusClientControllerSerialPort(QWidget *parent)
    :SAKModbusClientController(parent)
{
    mSerialPortSection = new SAKModbusCommonSerialPortSection(this);
    mClientSection = new SAKModbusCommonClientSection(this);
    appendSection(mSerialPortSection);
    appendSection(mClientSection);
}
