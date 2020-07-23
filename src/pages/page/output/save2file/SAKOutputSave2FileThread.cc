﻿/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#include <QFile>
#include <QDateTime>
#include <QEventLoop>
#include <QTextStream>

#include "SAKOutputSave2FileThread.hh"
#include "SAKOutputSave2FileDialog.hh"

SAKOutputSave2FileThread::SAKOutputSave2FileThread(QObject *parent)
    :QThread (parent)
{

}

SAKOutputSave2FileThread::~SAKOutputSave2FileThread()
{
    requestInterruption();
    mThreadWaitCondition.wakeAll();
    wait();
}

void SAKOutputSave2FileThread::writeDataToFile(QByteArray data, SAKOutputSave2FileDialog::ParametersContext parameters)
{
    if(parameters.fileName.isEmpty()){
        return;
    }

    DataInfoStruct dataInfo;
    dataInfo.data = data;
    dataInfo.parameters = parameters;
    mDataListMutex.lock();
    mDataList.append(dataInfo);
    mDataListMutex.unlock();

    mThreadWaitCondition.wakeAll();
}

void SAKOutputSave2FileThread::run()
{
    QEventLoop eventLoop;
    while (true) {
        // wirte data to file
        while (true) {
            DataInfoStruct info = takeDataInfo();
            if (info.data.length()){
                innerWriteDataToFile(info.data, info.parameters);
            }else{
                break;
            }
        }

        // emmmm...
        eventLoop.processEvents();

        // if no interruption requested, the thread will sleep
        if (isInterruptionRequested()){
            break;
        }else{
            mThreadMutex.lock();
            mThreadWaitCondition.wait(&mThreadMutex);
            mThreadMutex.unlock();
        }
    }
}

void SAKOutputSave2FileThread::innerWriteDataToFile(QByteArray data, SAKOutputSave2FileDialog::ParametersContext parameters)
{
    QFile file(parameters.fileName);
    int format = parameters.format;
    QTextStream textStream(&file);

    QString dataString;
    if (file.open(QFile::WriteOnly | QFile::Text | QFile::Append)){
        dataString = bytes2String(data, format);
        QString outString = QString("[%1%2]%2")
                .arg(parameters.saveTimestamp ? QString(QDateTime::currentDateTime().toString("hh:mm:ss ")) : QString(""))
                .arg(parameters.type == SAKOutputSave2FileDialog::ParametersContext::Read ? QString("Rx") : QString("Tx"))
                .arg(dataString);
        textStream << outString;
        file.close();
    }
}

SAKOutputSave2FileThread::DataInfoStruct SAKOutputSave2FileThread::takeDataInfo()
{
    DataInfoStruct info;
    mDataListMutex.lock();
    if (mDataList.length()){
        info = mDataList.takeFirst();
    }
    mDataListMutex.unlock();

    return info;
}

QString SAKOutputSave2FileThread::bytes2String(QByteArray bytes, int format)
{
    QString str;
    switch (format) {
    case SAKOutputSave2FileDialog::ParametersContext::Bin:
        for (int i = 0; i < bytes.length(); i++){
            QString temp = QString::number(int(bytes.at(i)), 2);
            str.append(temp + QString(" "));
        }
        str.trimmed();
        break;
    case SAKOutputSave2FileDialog::ParametersContext::Hex:
        for (int i = 0; i < bytes.length(); i++){
            QString temp = QString("%1").arg(QString::number(int(bytes.at(i)), 16), 2, '0');
            str.append(temp + QString(" "));
        }
        str.trimmed();
        break;
    case SAKOutputSave2FileDialog::ParametersContext::Utf8:
        str = QString::fromUtf8(bytes);
        break;
    default:
        str = tr("Unknow text format!");
        break;
    }

    return str;
}