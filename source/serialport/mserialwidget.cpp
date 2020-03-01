#include "mserialwidget.h"
#include "ui_mserialwidget.h"

#define cout  qDebug() << __LINE__ << ":"

MSerialWidget::MSerialWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MSerialWidget)
{
    ui->setupUi(this);
    setWindowTitle(tr("Serial Port"));
    resize(700,360);

    QAction *searchAction = new QAction();
    searchAction->setIcon(QIcon("./icon/ms.ico"));
    ui->lineEdit->addAction(searchAction,QLineEdit::TrailingPosition);

    strlist = {
        "NoError", "DeviceNotFoundError", "PermissionError", "OpenError",
        "ParityError", "FramingError", "BreakConditionError", "WriteError",
        "ReadError","ResourceError", "UnsupportedOperationError","UnknownError",
        "TimeoutError", "NotOpenError"
    };

    mySerialPort = new MSerial;
    f_clearSend = false;
    f_showDate = false;

    connect(ui->checkBox_hexreceive,&QCheckBox::clicked, mySerialPort, &MSerial::setReceiveMode);

    //MyWidget signals to  MSerial slots
    connect(this,&MSerialWidget::signal_openSerialPort, mySerialPort, &MSerial::openSerialPort);                     //建立打开serialport信号链接
    connect(this,&MSerialWidget::signal_closeSerialPort, mySerialPort, &MSerial::closeSerialPort);                   //建立关闭串口信号链接
    connect(this,&MSerialWidget::signal_sendOnceData, mySerialPort, &MSerial::sendOnceData);                         //建立单次发送信号链接
    connect(this,&MSerialWidget::signal_autoSendDate, mySerialPort, &MSerial::autoSendDate);                         //建立自动发送信号链接
    connect(this,&MSerialWidget::signal_stopautoSendDate, mySerialPort, &MSerial::stopautoSendDate);              //建立停止自动发送信号链接

    //MSerial signals to  MyWidget slots
    connect(mySerialPort, &MSerial::updatePrintData, this, &MSerialWidget::slot_updatePrintData);                    //建立接收数据更新到数据显示窗口信号
    connect(mySerialPort, &MSerial::serialIsOpen, this, &MSerialWidget::slot_serialIsOpen);                          //建立端口正式打开关闭信号链接
    connect(mySerialPort, &MSerial::updatereceivedtimes, this, &MSerialWidget::slot_updatereceivedtimes);            //建立接收次数信号链接
    connect(mySerialPort, &MSerial::updatesendtimes, this, &MSerialWidget::slot_updatesendtimes);                    //建立发送次数信号链接
    connect(mySerialPort->serialport, &QSerialPort::errorOccurred,
            [=](QSerialPort::SerialPortError error)mutable
            {
                if(error != QSerialPort::NoError)
                {
                    ui->receiveDataArea->insertHtml(QString("<body><span style=\"color:#ff0000;\">ERROR: %1<br/></body>").arg(strlist[error]));
                    //ui->receiveDataArea->moveCursor(QTextCursor::End);
                    emit signal_closeSerialPort();
                }
            });

    upAvailablePorts();
}

bool MSerialWidget::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    if (eventType == "windows_generic_MSG")
    {
        bool bResult = false;
        MSG* msg = reinterpret_cast<MSG*>(message);

        //PDEV_BROADCAST_HDR lpdb = (PDEV_BROADCAST_HDR)msg->lParam;

        if(msg->message == WM_DEVICECHANGE && msg->wParam >= DBT_DEVICEARRIVAL)
        {
            switch (msg->wParam)
            {
            case DBT_DEVICEARRIVAL:
                //cout<<"DBT_DEVICEARRIVAL";
                upAvailablePorts();
                bResult = true;
                break;
            case DBT_DEVICEREMOVECOMPLETE:
                //cout<<"DBT_DEVICEREMOVECOMPLETE";
                bResult = true;
                break;
            case DBT_DEVNODES_CHANGED:
                //cout<<"DBT_DEVNODES_CHANGED";
                bResult = true;
                break;
            default:
                bResult = false;
                break;
            }
        }
        return bResult;
    } else
    {
        return QWidget::nativeEvent(eventType, message, result);
    }
}

void MSerialWidget::upAvailablePorts()
{
    ui->comboBox_com->clear();
    ui->receiveDataArea->insertHtml(QString("<body>可用串口:<br/></body>"));
    listinfo = QSerialPortInfo::availablePorts();

    if(listinfo.isEmpty())
    {
        ui->receiveDataArea->insertHtml(QString("<body>(default COM2)无可用串口设备<br/></body>"));
        ui->comboBox_com->addItem("COM2");
        return;
    }

    foreach (QSerialPortInfo info, listinfo)
    {
        ui->comboBox_com->addItem(info.portName());
        ui->receiveDataArea->insertHtml(QString("<body>(%1) %2<br/></body>").arg(info.portName()).arg(info.description()));
        //cout<<info.portName()<<info.description();
    }
    //ui->receiveDataArea->moveCursor(QTextCursor::End);
}

void MSerialWidget::sendDate(QByteArray array)
{
    ui->receiveDataArea->append("正在下载程序...");


    emit signal_sendOnceData(QString(array.at(0)), false);

    emit signal_sendOnceData(QString(array.at(1)), false);

    
    ui->receiveDataArea->append("下载完成！");
}

void MSerialWidget::slot_updatePrintData(QString data)
{
    if(!data.isEmpty())
    {
        QTime time = QTime::currentTime();  //获取系统时间
        QDate date = QDate::currentDate();  //获取系统日期
        data.replace('\n', "<br/>");        //把字符里面的回车变成html中的换行

        if(f_showDate)
        {
            ui->receiveDataArea->insertHtml(QString("</p><span style=\" font-size:10pt; color:#23719c;\">接收 %1 %2 <br/>""</span></p></body>")
                                         .arg(date.toString("yyyy/MM/dd")).arg(time.toString("HH:mm:ss")));
        }
        ui->receiveDataArea->insertHtml(QString("<body>%1<br/></body>").arg(data));
        ui->receiveDataArea->moveCursor(QTextCursor::End);
   }
}

void MSerialWidget::slot_serialIsOpen(bool figure)
{
    if(figure == true)
    {
        ui->buttonOpenSerial->setText(QString("关闭串口"));
        ui->receiveDataArea->insertHtml(QString("</p><span style=\" font-size:10pt; color:#00ff00;\">串口已连接！<br/></p></body>"));
    } else {
        ui->buttonOpenSerial->setText(QString("打开串口"));
        ui->receiveDataArea->insertHtml(QString("</p><span style=\" font-size:10pt; color:#ff0000;\">串口已断开！<br/></p></body>"));
    }

    emit signal_serialIsOpen(figure);
  //  ui->receiveDataArea->moveCursor(QTextCursor::End);
}

void MSerialWidget::slot_updatereceivedtimes(qint64 count)
{
    ui->receiveCount->setText(QString::number(count));
}
void MSerialWidget::slot_updatesendtimes(qint64 count)
{
    ui->sendCount->setText(QString::number(count));
}

void MSerialWidget::closeEvent(QCloseEvent *event)
{
//    if(mySerialPort->serialport->isOpen() == true)
//    {
//        emit signal_closeSerialPort();
//    }
//    while(mySerialPort->serialport->isOpen());
//    myserialthread->quit();     //给myserial线程发出退出指令
//    while(false == myserialthread->wait());    //等待线程退出
//        delete mySerialPort;        //释放申请的内存
    QWidget::closeEvent(event);     //退出事件继续向下传递
}

void MSerialWidget::on_resetCount_clicked()
{
    mySerialPort->clearSendReceivedTimes();
    ui->receiveCount->setText(QString::number(0));
    ui->sendCount->setText(QString::number(0));
}

void MSerialWidget::on_autoClearSend_stateChanged(int arg1)
{
    if(arg1 == Qt::Checked)
        f_clearSend = true;
    else
        f_clearSend = false;
}

void MSerialWidget::on_showDate_stateChanged(int arg1)
{
    if(arg1 == Qt::Checked)
        f_showDate = true;
    else
        f_showDate = false;
}

void MSerialWidget::on_buttonOpenSerial_clicked()
{
    if(mySerialPort->serialport->isOpen() == true)
    {
        emit signal_closeSerialPort();     //发送关闭串口信号
    }else
    {            
        mySerialPort->serialport->setPortName(ui->comboBox_com->currentText()); //发送设置com口信号
        mySerialPort->setSerialPortBaudRate(ui->comboBox_baud->currentText().toInt());//设置端口波特率
        mySerialPort->setSerialPortParity(ui->comboBox_parity->currentIndex()); //设置serialport端口校验位
        mySerialPort->setSerialPortDateBits(ui->comboBox_databit->currentIndex()); //设置serialport端口数据位  
        mySerialPort->setSerialPortStopBits(ui->comboBox_stopbit->currentIndex());//设置serialport停止位    
        emit signal_openSerialPort();       //发送打开串口信号
    }
}

void MSerialWidget::on_buttonSend_clicked()
{
    if(mySerialPort->serialport->isOpen() == true)
    {
        QTime time = QTime::currentTime();        //获取当地时间
        QDate date = QDate::currentDate();        //获取当地日期
        QString data = ui->sendDataArea->toPlainText();     //获取文本框的数据
        data.replace('\n', "<br/>");
        if(mySerialPort->serialport->error() == QSerialPort::NoError)
        {
            if(data.isEmpty() == false)
            {
               //html.replace("</body>", QString("<ul style=\"-qt-list-indent: 0;\"><li style=\" font-size:12pt; color:#760e10;\" style=\"margin-top:2px; margin-bottom:2px;\"><span style=\" font-size:10pt;\">%1</span></li></ul>").arg(data));
               //ui->textEditShow->setHtml(html);
                if(f_showDate)
                {
                    ui->receiveDataArea->insertHtml(QString("</p><span style=\" font-size:10pt; color:#21c824;\">发送 %1 %2 <br/></p></body>")
                                                 .arg(date.toString("yyyy/MM/dd")).arg(time.toString("HH:mm:ss")));
                }
                ui->receiveDataArea->insertHtml(QString("<body>%1<br/></body>").arg(data));
             //   ui->receiveDataArea->moveCursor(QTextCursor::End);
                emit signal_sendOnceData(data, ui->checkBox_hexsend->isChecked());     //发射单次发送信号
            }
            if(f_clearSend)
                ui->sendDataArea->clear();
        }
    } else {
        ui->receiveDataArea->insertHtml(QString("</p><span style=\" font-size:10pt; color:#ff0000;\">串口未连接！<br/></p></body>"));
     //   ui->receiveDataArea->moveCursor(QTextCursor::End);
    }
}

MSerialWidget::~MSerialWidget()
{
    delete ui;
}
