﻿/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include "SAKModbusCommonHostSection.hh"
#include "SAKModbusCommonClientSection.hh"
#include "SAKModbusClientControllerTcp.hh"

SAKModbusClientControllerTcp::SAKModbusClientControllerTcp(QWidget *parent)
    :SAKModbusClientController(parent)
{
    mHostSection = new SAKModbusCommonHostSection(this);
    mClientSection = new SAKModbusCommonClientSection(this);
    appendSection(mHostSection);
    appendSection(mClientSection);
}
