#ifndef MSERIALWEIGET_H
#define MSERIALWEIGET_H

#include "mserial.h"

#include <QtWidgets>
#include <windows.h>
#include <stdio.h>
#include <dbt.h>


//#include <QTextBlockFormat>
//#include <QTextCursor>

namespace Ui {
    class MSerialWidget;
}

class MSerialWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MSerialWidget(QWidget *parent = nullptr);
    ~MSerialWidget();
    void upAvailablePorts();
    void sendDate(QByteArray array);

private:
    void closeEvent(QCloseEvent *event);//重写关闭程序事件
    bool nativeEvent(const QByteArray &eventType, void *message, long *result);


    Ui::MSerialWidget *ui;
    MSerial *mySerialPort;
    QList<QSerialPortInfo> listinfo;
    QList<QString> strlist;
    bool f_clearSend;
    bool f_showDate;

signals:
    void signal_openSerialPort(void);              //打开串口信号
    void signal_closeSerialPort(void);             //关闭serialport
    void signal_sendOnceData(QString oncedate, bool hexsend);        //发送一次数据信号
    void signal_autoSendDate(QString data, int time, bool hexsend);  //自动发送数据
    void signal_setReceiveMode(bool);          //设置接收的模式
    void signal_stopautoSendDate(void);        //停止自动发送数据

    void signal_serialIsOpen(bool);

public slots:
    void slot_updatePrintData(QString);      //更新数据显示窗口槽函数
    void slot_serialIsOpen(bool);            //端口打开关闭监视标志处理函数
    void slot_updatereceivedtimes(qint64);   //更新接收次数信号
    void slot_updatesendtimes(qint64 );      //更新发送次数信号

private slots:
    void on_resetCount_clicked();
    void on_autoClearSend_stateChanged(int arg1);
    void on_showDate_stateChanged(int arg1);
    void on_buttonOpenSerial_clicked();
    void on_buttonSend_clicked();
};

#endif // MSERIALWEIGET_H
