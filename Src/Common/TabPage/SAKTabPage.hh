﻿/*
 * Copyright (C) 2018-2019 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * If you want to know more about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 * Welcome to bother.
 *
 * I write the comment in English, it's not because that I'm good at English,
 * but for "installing B".
 */
#ifndef SAKTABPAGE_HH
#define SAKTABPAGE_HH

#include <QSize>
#include <QTimer>
#include <QLabel>
#include <QWidget>
#include <QGroupBox>
#include <QComboBox>
#include <QDateTime>
#include <QMetaEnum>
#include <QTextEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QApplication>
#include <QTextBrowser>

#include "SAKReadWriteSetting.hh"
#include "SAKAutoResponseSettingPanel.hh"

class SAKDataFactory;
class SAKHighlighterSettingPanel;

namespace Ui {
class SAKTabPage;
}

/*
 * Hei, Developer! Takeacre of this warning!
 * 由于使用了信号与槽的自动关联，指向ui控件的指针变量的名称必须与ui控件的objectName保持一致。
 * 使用designer改变ui控件变量名称时，objectName属性会自动更新，不要在designer中手动更改ui控件的objectName。
 * objectName与变量名称不一致会导致信号与槽的关联失效。
 */

class SAKTabPage : public QWidget
{
    Q_OBJECT
public:
    SAKTabPage(QWidget *parent = Q_NULLPTR);
    ~SAKTabPage();

    /**
     * @brief   -- 数据显示模式
     */
    enum TextDisplayModel {
        Bin,
        Oct,
        Dec,
        Hex,
        Ascii,
        Local8bit
    };
    Q_ENUM(TextDisplayModel)

    struct OutputParameters {
        bool showDate;
        bool showTime;
        bool showMS;
        bool isReceivedData;
        TextDisplayModel textModel;
    };

    /**
     * @brief readDelayTimes    -- 收到读就绪信号后，延时一段时间
     * @return                  -- 延时时间
     */
    unsigned long readDelayTimes();
    /**
     * @brief readDelayTimes    -- 智行发送函数前，延时一段时间发送
     * @return                  -- 延时时间
     */
    unsigned long writeDelayTimes();
protected:
    /**
     * @brief openOrColoseDevice    -- 打开或者关闭设备
     */
    virtual void openOrColoseDevice(){}
    /**
     * @brief refreshDevice         -- 刷新设备
     */
    virtual void refreshDevice(){}
    /**
     * @brief controllerWidget  -- 安装控制面板
     */
    virtual QWidget *controllerWidget(){return nullptr;}
    /**
     * @brief setUpController -- 安装控制器（控制面板）
     */
    void setUpController();   
    /**
     * @brief bytesRead -- 处理接受到的数据
     * @param data      -- 接受到的数据
     */
    void bytesRead(QByteArray data);
    /**
     * @brief bytesWritten  -- 处理已发送的数据
     * @param data          -- 已发送的数据
     */
    void bytesWritten(QByteArray data);

    /**
     * @brief outputMessage -- 输出信息到ui显示
     * @param msg           -- 需要输出的消息
     * @param isInfo        -- 是否为普通消息
     */
    void outputMessage(QString msg, bool isInfo = true);
protected:
    QTimer cycleTimer;
    SAKDataFactory *dataFactory;
private:
    /// 初始化ui
    void initUI();
    /// 注册用户自定义数据类型，自定义数据类型作为信号参数时，必须先注册
    void registerMetaType();
    /// 获取输出参数
    OutputParameters outputParameters();
    /// 写数据
    void writeBytes(QByteArray data){emit need2writeBytes(data);}
    /// 设置label文本
    void setLabelText(QLabel *label, quint64 text);
    /// 设置输入框的文本输入格式
    void formattingInputText(TextDisplayModel model);
signals:
    /// 该函数并不会真的发送数据，而是发送一个信号，该信号携带需要发送的数据,数据需要经过处理后才能发送
    void sendRawData(QString data, TextDisplayModel textModel);
    /// 将读取到的数据或者已发送的数据发射出去处理
    void dataReadOrwritten(QByteArray data, OutputParameters parameters);
    /// 子类关联该信号来发送数据即可
    void need2writeBytes(QByteArray data);
    /// 子类设备状态（打开/关闭）发生改变时发送该信号
    void deviceStatusChanged(bool opened);

private:
    const char *logCategory = "SAKTabPage";

    ///----------------------------------------------------------------
    QString readSetting(QString &option);
    void writeSetting(QString &option, QString &value);

    /**
     * @brief outputData        -- 更新输出数据
     * @param data              -- 需要输出显示的数据
     */
    void outputData(QString data);
private slots:
    void updateRxImage();
    void updateTxImage();

    void cancleCycle();

    void setCycleTime(QString time);
    void readCycleTime();

    void setOutputMode(QString mode);
    void readOutputMode();

    void setInputMode(QString mode);
    void readInputMode();

    void cycleTimerTimeout();
    void checkedBoxCycleClicked(bool checked);

    void resetSendDataCount();
    void resetReceiveDataCount();
    // ------------------------------------------------------------------------
    // 设备设置
protected:    
    QPushButton *refreshPushButton              = nullptr;  /// 刷新按钮
    QPushButton *switchPushButton               = nullptr;  /// 打开关闭设备按钮
    QGroupBox   *deviceSettingGroupBox          = nullptr;  /// 控制面板
private slots:
    void on_refreshPushButton_clicked(){refreshDevice();}
    void on_switchPushButton_clicked(){openOrColoseDevice();}

    // 消息输出组
protected:
    QTextBrowser *messageTextBrowser            = nullptr;  /// 消息输出框

     // 输入设置组
protected:
    TextDisplayModel inputModel;
    bool cyclEnable;
    quint32 cycleTime = 0;

    QComboBox   *inputModelComboBox             = nullptr;  /// 输入模式预选框
    QCheckBox   *cycleEnableCheckBox            = nullptr;  /// 循环使能复选框
    QLineEdit   *cycleTimeLineEdit              = nullptr;  /// 循环周期输入框
    QPushButton *readinFilePushButton           = nullptr;  /// 读入文件按钮
    QPushButton *clearInputPushButton           = nullptr;  /// 清空输入框按钮
    QPushButton *sendPushButton                 = nullptr;  /// 发送数据按钮
    QTextEdit   *inputTextEdit                  = nullptr;  /// 数据输入框        
private slots:
    void on_inputModelComboBox_currentTextChanged(const QString &text);
    void on_cycleEnableCheckBox_clicked();
    void on_cycleTimeLineEdit_textChanged(const QString &text);
    void on_readinFilePushButton_clicked();
    void on_clearInputPushButton_clicked();
    void on_sendPushButton_clicked();
    void on_inputTextEdit_textChanged();

    //数据输出组管理
protected:
    TextDisplayModel outputTextModel = SAKTabPage::Hex;   

    QLabel      *rxLabel                        = nullptr;  /// 接受指示灯
    QLabel      *txLabel                        = nullptr;  /// 发送指示灯
    QComboBox   *outputModelComboBox            = nullptr;  /// 输出模式复选框
    QCheckBox   *showDateCheckBox               = nullptr;  /// 显示日期使能复选框
    QCheckBox   *autoWrapCheckBox               = nullptr;  /// 自动换行使能复选框
    QCheckBox   *showTimeCheckBox               = nullptr;  /// 显示时间使能复选框
    QCheckBox   *showMsCheckBox                 = nullptr;  /// 显示毫秒使能复选框
    QCheckBox   *showRxDataCheckBox             = nullptr;  /// 显示接收数据使能复选框
    QCheckBox   *showTxDataCheckBox             = nullptr;  /// 显示发送数据使能复选框
    QPushButton *clearOutputPushButton          = nullptr;  /// 清空输出按钮
    QPushButton *saveOutputPushButton           = nullptr;  /// 保存输出按钮
    QTextBrowser *outputTextBroswer             = nullptr;  /// 用于输出显示收发的数据
private slots:
    void on_outputModelComboBox_currentTextChanged(const QString &text);
    void on_autoWrapCheckBox_clicked();
    void on_clearOutputPushButton_clicked();
    void on_saveOutputPushButton_clicked();


    // 数据管理组
protected:
    quint64     receiveFrames                   = 0;        /// 接受帧数
    quint64     receiveBytes                    = 0;        /// 接受字节数
    quint64     sendFrames                      = 0;        /// 发送帧数
    quint64     sendBytes                       = 0;        /// 发送字节数

    QLabel      *rxFramesLabel                  = nullptr;  /// 用于显示接受帧数
    QLabel      *txFramesLabel                  = nullptr;  /// 用于显示发送帧数
    QLabel      *rxBytesLabel                   = nullptr;  /// 用于显示接收字节数
    QLabel      *txBytesLabel                   = nullptr;  /// 用于显示发送字节数
    QPushButton *resetRxCountPushButton         = nullptr;  /// 重置接收计数
    QPushButton *resetTxCountPushButton         = nullptr;  /// 重置发送计数
private slots:
    void on_resetRxCountPushButton_clicked();
    void on_resetTxCountPushButton_clicked();


    // 其他设置组
protected:
    QPushButton *autoResponseSettingPushButton  = nullptr;  /// 自动回复设置面板调出按钮
    QPushButton *highlightSettingPushButton     = nullptr;  /// 高亮设置面板调出按钮
    QPushButton *readWriteSettingPushButton     = nullptr;  /// 读写设置面包调出按钮
private slots:
    void on_autoResponseSettingPushButton_clicked();        /// 创建一个高亮设置窗口并显示，该窗口关闭后将被销毁
    void on_highlightSettingPushButton_clicked();           /// 创建一个高亮设置窗口并显示，该窗口关闭后将被销毁
    void on_readWriteSettingPushButton();                   /// 创建一个读写参数设置窗口并显示，该窗口关闭后将被销毁

private:
    /**
     * @brief ui    -- 界面文件
     */
    Ui::SAKTabPage *ui = nullptr;

    /**
     * @brief initUiPointer -- 初始化指向ui控件的数据成员（指针）
     */
    void initUiPointer();       
};

Q_DECLARE_METATYPE(SAKTabPage::TextDisplayModel);
Q_DECLARE_METATYPE(SAKTabPage::OutputParameters);

#endif  // SAKTabPage_H