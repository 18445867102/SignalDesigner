#include "mserial.h"

#define cout  qDebug() << __LINE__ << ":"

MSerial::MSerial(QObject *parent) : QThread(parent)
{
    //变量初始化
    isreceivehex = false;
    sendtimes = 0;
    receivetimes = 0;
    //初始化定时器
    autosenddata = QByteArray("");
    autosenddatetimer =new QTimer(this);
    readFinishtimer =new QTimer(this);

    //初始化串口
    serialport = new QSerialPort(this);
    serialport->setPortName(QString("Default"));        //设置初始串口号
    serialport->setBaudRate(QSerialPort::Baud9600);     //设置初始波特率
    serialport->setParity(QSerialPort::NoParity);       //设置没有校验位
    serialport->setDataBits(QSerialPort::Data8);        //设置数据位为8位
    serialport->setStopBits(QSerialPort::OneStop);      //设置停止位为1位

    connect(autosenddatetimer, &QTimer::timeout,        //定时发送定时器,定时发送数据
            [=]()mutable
    {
        if(serialport->isOpen())
        {
            if(autosenddata.isEmpty() == false)
            {
                sendtimes += serialport->write(autosenddata.data());
                emit updatesendtimes(sendtimes);
            }else{
                autosenddatetimer->stop();
            }
        }else {
            autosenddatetimer->stop();
        }
    });

    connect(serialport, &QSerialPort::readyRead,        //当收到字符时候,启动处理接收数据函数
            [=]()mutable        //lambda表达式
            {
                readFinishtimer->start(30);
                readData.append(serialport->readAll());
            });

    connect(readFinishtimer, &QTimer::timeout,
            [=]()mutable
    {
        readFinishtimer->stop();
        receivetimes += readData.length();
        emit updatereceivedtimes(receivetimes);
        if(isreceivehex  == true)        //把串口读出来的数据刷到我的显示textEdit上
        {
            QString temp(readData.toHex().toUpper());
            int len = temp.length()/2;
            for(int i=1;i<len + 1;i++)
            {
                temp.insert(2*i+i - 1, " ");
            }
            emit updatePrintData(temp);     //发送信号,把收到的数据(hex)送到主线程,供给相应的函数进行处理
        }else
        {
            emit updatePrintData(QString(readData));     //发送信号,把收到的数据送到主线程,供给相应的函数进行处理
        }
        readData.clear();
    });
}

void MSerial::openSerialPort(void)
{
    if(serialport->isOpen())
        return;

    serialport->open(QIODevice::ReadWrite);

    emit serialIsOpen(serialport->isOpen());
}

void MSerial::closeSerialPort(void)        //关闭serialport
{
    if(autosenddatetimer->isActive() == true)       //如果自动发送定时器还在活动关闭定时器
    {
        autosenddatetimer->stop();
    }

    if(serialport->isOpen())        //如果串口处于打开状态,关闭串口
    {
        serialport->close();
        emit serialIsOpen(serialport->isOpen());
    }
}

void MSerial::setReceiveMode(bool ishex)       //设置是否以hex模式接收数据
{
    isreceivehex = ishex;
}

void MSerial::sendOnceData(QString oncedate, bool hexsend)       //发送一次数据
{
    if(serialport->isOpen() == true)        //串口打开,才发送数据
    {
        if(hexsend == true)
        {
            bool ok;
            QByteArray ret;
            oncedate = oncedate.simplified();
            QStringList sl = oncedate.split(" ");
            foreach (QString s, sl) {
                if(s.isEmpty() == false)
                {
                    char c = s.toInt(&ok,16)&0xFF;
                    if(ok){
                        ret.append(c);
                    }
                }
            }
           sendtimes += serialport->write(ret.data());        //Hex发送
           emit updatesendtimes(sendtimes);
        }
        else
        {
           // QStringList ls;
           // ls =  oncedate.split('\n');  //适应window换行
           // oncedate = ls.join("\r\n");
            sendtimes += serialport->write(oncedate.toUtf8().data());        //正常发送
            emit updatesendtimes(sendtimes);
        }
    }
}

void MSerial::autoSendDate(QString data, int time, bool hexsend)     //自动循环发送
{
    //autosenddata = data.toUtf8();        //传递需要发送的字符串
    if(hexsend == false)
    {
        QStringList ls;
        ls =  data.split('\n');  //适应window换行
        data = ls.join("\r\n");
        autosenddata =data.toUtf8();
    }
    else
    {
        bool ok;
        QByteArray ret;
        data = data.simplified();
        QStringList sl = data.split(" ");
        foreach (QString s, sl) {
            if(s.isEmpty() == false)
            {
                char c = s.toInt(&ok,16)&0xFF;
                if(ok){
                    ret.append(c);
                }
            }
        }
        autosenddata = ret;
    }
    autosenddatetimer->start(time);     //启动发送定时器
}

void MSerial::stopautoSendDate(void)       //停止自动发送
{
    autosenddata.clear();
    autosenddatetimer->stop();      //只需要停止自动发送定时器即可
}

void MSerial::clearSendReceivedTimes(void)     //清空接收发送次数变量
{
    sendtimes = 0;
    receivetimes = 0;
}

void MSerial::setSerialPortBaudRate(qint32 baud)     //设置serialport端口波特率信号
{
    serialport->setBaudRate(baud);
}

void MSerial::setSerialPortParity(qint32 parity)     //设置serialport端口校验位
{
    switch (parity) {
    case 0:
        serialport->setParity(QSerialPort::NoParity);
        break;
    case 1:
        serialport->setParity(QSerialPort::EvenParity);
        break;
    case 2:
        serialport->setParity(QSerialPort::OddParity);
        break;
    case 3:
        serialport->setParity(QSerialPort::SpaceParity);
        break;
    case 4:
        serialport->setParity(QSerialPort::MarkParity);
        break;
    default:
        break;
    }
}

void MSerial::setSerialPortDateBits(qint32 bits)       //设置串口数据位
{
    switch (bits) {
    case 0:
        serialport->setDataBits(QSerialPort::Data5);
        break;
    case 1:
        serialport->setDataBits(QSerialPort::Data6);
        break;
    case 2:
        serialport->setDataBits(QSerialPort::Data7);
        break;
    case 3:
        serialport->setDataBits(QSerialPort::Data8);
        break;
    default:
        serialport->setDataBits(QSerialPort::UnknownDataBits);
        break;
    }
}

void MSerial::setSerialPortStopBits(qint32 bits)       //设置串口停止位
{
    switch (bits) {
    case 0:
        serialport->setStopBits(QSerialPort::OneStop);
        break;
    case 1:
        serialport->setStopBits(QSerialPort::OneAndHalfStop);
        break;
    case 2:
        serialport->setStopBits(QSerialPort::TwoStop);
        break;
    default:
        serialport->setStopBits(QSerialPort::UnknownStopBits);
        break;
    }
}

