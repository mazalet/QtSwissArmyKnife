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
#include <QWidget>
#include <QHBoxLayout>

#include "SAKUdpClientDevice.hh"
#include "SAKCommonDataStructure.hh"
#include "SAKUdpClientDebugPage.hh"
#include "SAKUdpClientDeviceController.hh"

SAKUdpClientDebugPage::SAKUdpClientDebugPage(int type, QString name, QWidget *parent)
    :SAKDebugPage(type, name, parent)
{
    mUdpDeviceController = new SAKUdpClientDeviceController(this);
    initializingPage();
}

SAKUdpClientDebugPage::~SAKUdpClientDebugPage()
{
    mUdpDeviceController->deleteLater();
}

SAKDebugPageController *SAKUdpClientDebugPage::deviceController()
{
    return mUdpDeviceController;
}

SAKDebugPageDevice *SAKUdpClientDebugPage::createDevice()
{
    SAKUdpClientDevice *ptr = new SAKUdpClientDevice(this);
    mUdpDeviceController->setUdpDevice(ptr);
    return ptr;
}
