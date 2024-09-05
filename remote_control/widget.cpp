#include "widget.h"
#include "ui_widget.h"


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowTitle("remote_control");



    uart_state = s_disconnect;

    mBGdisplaymode = new QButtonGroup(this);
    mBGdisplaymode->addButton(ui->radioButton_displayASCII);
    mBGdisplaymode->addButton(ui->radioButton_displayHEX);
    mBGdisplaymode->setId(ui->radioButton_displayASCII,DISPLAYMODE_ASCII);
    mBGdisplaymode->setId(ui->radioButton_displayHEX,DISPLAYMODE_HEX);



    mBGsendoutmode = new QButtonGroup(this);
    mBGsendoutmode->addButton(ui->radioButton_sendoutASCII);
    mBGsendoutmode->addButton(ui->radioButton_sendoutHEX);
    mBGsendoutmode->setId(ui->radioButton_sendoutASCII,SENDOUTMODE_ASCII);
    mBGsendoutmode->setId(ui->radioButton_sendoutHEX,SENDOUTMODE_HEX);

    mBGwhichhole = new QButtonGroup(this);
    mBGwhichhole->addButton(ui->radioButton_23_1);
    mBGwhichhole->addButton(ui->radioButton_23_2);
    mBGwhichhole->addButton(ui->radioButton_23_3);
    mBGwhichhole->addButton(ui->radioButton_23_4);
    mBGwhichhole->addButton(ui->radioButton_23_5);
    mBGwhichhole->addButton(ui->radioButton_23_6);

    mBGwhichhole->addButton(ui->radioButton_34_1);
    mBGwhichhole->addButton(ui->radioButton_34_2);
    mBGwhichhole->addButton(ui->radioButton_34_3);
    mBGwhichhole->addButton(ui->radioButton_34_4);
    mBGwhichhole->addButton(ui->radioButton_34_5);
    mBGwhichhole->addButton(ui->radioButton_34_6);
    mBGwhichhole->addButton(ui->radioButton_34_7);
    mBGwhichhole->addButton(ui->radioButton_34_8);
    mBGwhichhole->addButton(ui->radioButton_34_9);
    mBGwhichhole->addButton(ui->radioButton_34_10);
    mBGwhichhole->addButton(ui->radioButton_34_11);
    mBGwhichhole->addButton(ui->radioButton_34_12);

    mBGwhichhole->setId(ui->radioButton_23_1,231);
    mBGwhichhole->setId(ui->radioButton_23_2,232);
    mBGwhichhole->setId(ui->radioButton_23_3,233);
    mBGwhichhole->setId(ui->radioButton_23_4,234);
    mBGwhichhole->setId(ui->radioButton_23_5,235);
    mBGwhichhole->setId(ui->radioButton_23_6,236);

    mBGwhichhole->setId(ui->radioButton_34_1,341);
    mBGwhichhole->setId(ui->radioButton_34_2,342);
    mBGwhichhole->setId(ui->radioButton_34_3,343);
    mBGwhichhole->setId(ui->radioButton_34_4,344);
    mBGwhichhole->setId(ui->radioButton_34_5,345);
    mBGwhichhole->setId(ui->radioButton_34_6,346);
    mBGwhichhole->setId(ui->radioButton_34_7,347);
    mBGwhichhole->setId(ui->radioButton_34_8,348);
    mBGwhichhole->setId(ui->radioButton_34_9,349);
    mBGwhichhole->setId(ui->radioButton_34_10,3410);
    mBGwhichhole->setId(ui->radioButton_34_11,3411);
    mBGwhichhole->setId(ui->radioButton_34_12,3412);


    ui->textBrowser_intput->clear();

    ui->comBox_uartDps->setCurrentIndex(0);

    ui->radioButton_displayASCII->setChecked(true);
    ui->radioButton_sendoutASCII->setChecked(true);

    ui->pushButton_uartConnect->setEnabled(true);
    ui->pushButton_uartDisconnect->setEnabled(false);
    ui->buttom_sendout->setEnabled(false);

    ui->checkBox_displayNewline->setChecked(false);
    ui->buttom_sendout->setEnabled(false);




    Init_UartPort();

    connect(ui->comBox_uartPort,SIGNAL(clicked()),this,SLOT(on_comBox_uartPort_clicked()));

}

Widget::~Widget()
{
    delete ui;
}



/**
 * @brief Widget::on_pushButton_uartDisconnect_clicked
 * 断开串口连接 - 槽函数
 */

void Widget::on_pushButton_uartDisconnect_clicked()
{
    if(s_connect == uart_state)
    {
        mSerial->close();

        uart_state = s_disconnect;
        ui->pushButton_uartConnect->setEnabled(true);
        ui->pushButton_uartDisconnect->setEnabled(false);
        ui->buttom_sendout->setEnabled(false);
    }
}

/**
 * @brief Widget::on_pushButton_uartConnect_clicked
 * 串口连接 - 槽函数
 */
void Widget::on_pushButton_uartConnect_clicked()
{
    //获取串口参数并设置
    mSerial = new QSerialPort();
    //端口
    QString mPortName = ui->comBox_uartPort->currentText();
    //波特率
    QSerialPort::BaudRate mBaudRate = (QSerialPort::BaudRate)ui->comBox_uartDps->currentText().toInt();
    //数据位8
    QSerialPort::DataBits mDataBits = (QSerialPort::Data8);
    //流控制
    QSerialPort::FlowControl mFlowControl = (QSerialPort::NoFlowControl);
    //奇偶校验N
    QSerialPort::Parity mParity = (QSerialPort::NoParity);;
    //停止位1
    QSerialPort::StopBits mStopBits = (QSerialPort::OneStop);

    qDebug() << mPortName;
    qDebug() << mBaudRate;
    qDebug() << mDataBits;
    qDebug() << mFlowControl;
    qDebug() << mParity;
    qDebug() << mStopBits;

    mSerial->setPortName(mPortName);
    mSerial->setBaudRate(mBaudRate);
    mSerial->setDataBits(mDataBits);
    mSerial->setFlowControl(mFlowControl);
    mSerial->setParity(mParity);
    mSerial->setStopBits(mStopBits);
    if(mSerial->open((QIODevice::ReadWrite)))
    {
        uart_state = s_connect;
        ui->pushButton_uartConnect->setEnabled(false);
        ui->pushButton_uartDisconnect->setEnabled(true);
        ui->buttom_sendout->setEnabled(true);

        //清缓存
        mSerial->clear();
        connect(mSerial,SIGNAL(readyRead()),this,SLOT(slot_uartReadData()));
        QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("串口打开成功"));
    }
    else
    {
        uart_state = s_disconnect;
        QMessageBox::critical(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("串口打开失败"),QMessageBox::Close);

    }

}

/**
 * @brief Widget::Init_UartPort
 * 利用QSerialPortInfo获取端口信息，并更新
 */
void Widget::Init_UartPort()
{

    ui->comBox_uartPort->clear();

    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        ui->comBox_uartPort->addItem(info.portName());
    }

}

/**
 * @brief Widget::on_comBox_uartPort_clicked
 * 点击comBox_uartPort - 槽函数
 * 用以更新端口信息
 */
void Widget::on_comBox_uartPort_clicked()
{
    Init_UartPort();
}


/**
 * @brief Widget::slot_uartReadData
 * 串口接收数据 - 槽函数
 */

void Widget::slot_uartReadData(){

    if(ui->checkBox_displayNewline->isChecked())
    {
        if(ui->textBrowser_intput->document()->toPlainText() != "")
            ui->textBrowser_intput->insertPlainText("\r\n");
    }

        if(DISPLAYMODE_ASCII == mBGdisplaymode->checkedId())
        {
           ui->textBrowser_intput->insertPlainText(QString::fromLocal8Bit(mSerial->readAll()));
        }

        else if(DISPLAYMODE_HEX == mBGdisplaymode->checkedId())
        {

            QString re = "";
            QByteArray bytearray = mSerial->readAll();

            //hex char[] 转 QString
            for(int i = 0; i < bytearray.length(); i++)
            {
                if((unsigned char)bytearray[i] > 255)
                    re.append("Error ");
                re.append("0x" + QString::number((unsigned char)bytearray[i],16) + " ");
            }

            ui->textBrowser_intput->insertPlainText(re);
        }

        ui->textBrowser_intput->moveCursor(QTextCursor::End);


}

void Widget::on_pushButton_clear_TBinput_clicked()
{
     ui->textBrowser_intput->clear();
}


/**
 * @brief Widget::on_buttom_sendout_clicked
 * 串口数据发送 - 槽函数
 */
void Widget::on_buttom_sendout_clicked()
{

    if(mBGsendoutmode->checkedId() == SENDOUTMODE_ASCII){
        //支持中文需使用toLocal8Bit()
        mSerial->write(ui->textEdit_output->toPlainText().toLocal8Bit().data());
    }
    else if(mBGsendoutmode->checkedId() == SENDOUTMODE_HEX){
        QByteArray bytehex = QByteArray::fromHex(
                    ui->textEdit_output->toPlainText().toLatin1()).data();
         mSerial->write(bytehex);
    }
}

void Widget::on_KEY1_clicked()
{
    if(uart_state == s_connect)
    {
        mSerial->write("1!");
        QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("KEY1按下"),QMessageBox::Ok ,QMessageBox::NoButton);
    }else
    {
        QMessageBox::critical(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("串口未打开"),QMessageBox::Close ,QMessageBox::NoButton);
    }

}


void Widget::on_KEY2_clicked()
{
    if(uart_state == s_connect)
    {
    mSerial->write("2!");
    QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("KEY2按下"),QMessageBox::Ok ,QMessageBox::NoButton);
    }else
    {
        QMessageBox::critical(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("串口未打开"),QMessageBox::Close ,QMessageBox::NoButton);
    }

}


void Widget::on_KEY3_clicked()
{
    if(uart_state == s_connect)
    {
        mSerial->write("3!");
        QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("KEY3按下"),QMessageBox::Ok ,QMessageBox::NoButton);
    }else
    {
        QMessageBox::critical(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("串口未打开"),QMessageBox::Close ,QMessageBox::NoButton);
    }
}


void Widget::on_KEY4_clicked()
{
    if(uart_state == s_connect)
    {
        mSerial->write("4!");
        QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("KEY4按下"),QMessageBox::Ok ,QMessageBox::NoButton);
    }else
    {
        QMessageBox::critical(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("串口未打开"),QMessageBox::Close ,QMessageBox::NoButton);
    }
}


void Widget::on_KEY5_clicked()
{
    if(uart_state == s_connect)
    {
        mSerial->write("5!");
        QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("KEY5按下"),QMessageBox::Ok ,QMessageBox::NoButton);
    }else
    {
        QMessageBox::critical(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("串口未打开"),QMessageBox::Close ,QMessageBox::NoButton);
    }
}


void Widget::on_LEFT_clicked()
{
    if(uart_state == s_connect)
    {
        mSerial->write("6!");
        QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("KEY6按下"),QMessageBox::Ok ,QMessageBox::NoButton);
    }else
    {
        QMessageBox::critical(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("串口未打开"),QMessageBox::Close ,QMessageBox::NoButton);
    }

}


void Widget::on_RIGHT_clicked()
{
    if(uart_state == s_connect)
    {
        mSerial->write("7!");
        QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("右摇杆已按下"),QMessageBox::Ok ,QMessageBox::NoButton);
    }else
    {
        QMessageBox::critical(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("串口未打开"),QMessageBox::Close ,QMessageBox::NoButton);
    }

}


void Widget::on_pushButton_inject_clicked()
{
    if(uart_state == s_connect)
    {
        if(ui->plainTextEdit_volume->toPlainText().toInt() >= 1)    //不能小于1
        {
             switch(mBGwhichhole->checkedId())
             {
                 case 231:
                 mSerial->write("A");
                 mSerial->write(ui->plainTextEdit_volume->toPlainText().toLocal8Bit().data());
                 mSerial->write("!");
                 QMessageBox::information(this, QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("向2×3板1号孔给药"),QMessageBox::Ok ,QMessageBox::NoButton);
                 break;
                 case 232:
                 mSerial->write("B");
                 mSerial->write(ui->plainTextEdit_volume->toPlainText().toLocal8Bit().data());
                 mSerial->write("!");
                 QMessageBox::information(this, QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("向2×3板2号孔给药"),QMessageBox::Ok ,QMessageBox::NoButton);
                  break;
                 case 233:
                 mSerial->write("C");
                 mSerial->write(ui->plainTextEdit_volume->toPlainText().toLocal8Bit().data());
                 mSerial->write("!");
                 QMessageBox::information(this, QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("向2×3板3号孔给药"),QMessageBox::Ok ,QMessageBox::NoButton);
                  break;
                 case 234:
                 mSerial->write("D");
                 mSerial->write(ui->plainTextEdit_volume->toPlainText().toLocal8Bit().data());
                 mSerial->write("!");
                 QMessageBox::information(this, QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("向2×3板4号孔给药"),QMessageBox::Ok ,QMessageBox::NoButton);
                  break;
                 case 235:
                 mSerial->write("E");
                 mSerial->write(ui->plainTextEdit_volume->toPlainText().toLocal8Bit().data());
                 mSerial->write("!");
                 QMessageBox::information(this, QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("向2×3板5号孔给药"),QMessageBox::Ok ,QMessageBox::NoButton);
                  break;
                 case 236:
                 mSerial->write("F");
                 mSerial->write(ui->plainTextEdit_volume->toPlainText().toLocal8Bit().data());
                 mSerial->write("!");
                 QMessageBox::information(this, QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("向2×3板6号孔给药"),QMessageBox::Ok ,QMessageBox::NoButton);
                  break;


                 case 341:
                     mSerial->write("G");
                     mSerial->write(ui->plainTextEdit_volume->toPlainText().toLocal8Bit().data());
                     mSerial->write("!");
                     QMessageBox::information(this, QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("向3×4板1号孔给药"),QMessageBox::Ok ,QMessageBox::NoButton);
                  break;
                 case 342:
                 mSerial->write("H");
                 mSerial->write(ui->plainTextEdit_volume->toPlainText().toLocal8Bit().data());
                 mSerial->write("!");
                 QMessageBox::information(this, QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("向3×4板2号孔给药"),QMessageBox::Ok ,QMessageBox::NoButton);
                  break;
                 case 343:
                 mSerial->write("I");
                 mSerial->write(ui->plainTextEdit_volume->toPlainText().toLocal8Bit().data());
                 mSerial->write("!");
                 QMessageBox::information(this, QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("向3×4板3号孔给药"),QMessageBox::Ok ,QMessageBox::NoButton);
                  break;
                 case 344:
                 mSerial->write("J");
                 mSerial->write(ui->plainTextEdit_volume->toPlainText().toLocal8Bit().data());
                 mSerial->write("!");
                 QMessageBox::information(this, QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("向3×4板4号孔给药"),QMessageBox::Ok ,QMessageBox::NoButton);
                  break;
                 case 345:
                 mSerial->write("K");
                 mSerial->write(ui->plainTextEdit_volume->toPlainText().toLocal8Bit().data());
                 mSerial->write("!");
                 QMessageBox::information(this, QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("向3×4板5号孔给药"),QMessageBox::Ok ,QMessageBox::NoButton);
                  break;
                 case 346:
                 mSerial->write("L");
                 mSerial->write(ui->plainTextEdit_volume->toPlainText().toLocal8Bit().data());
                 mSerial->write("!");
                 QMessageBox::information(this, QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("向3×4板6号孔给药"),QMessageBox::Ok ,QMessageBox::NoButton);
                  break;
                 case 347:
                 mSerial->write("M");
                 mSerial->write(ui->plainTextEdit_volume->toPlainText().toLocal8Bit().data());
                 mSerial->write("!");
                 QMessageBox::information(this, QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("向3×4板7号孔给药"),QMessageBox::Ok ,QMessageBox::NoButton);
                  break;
                 case 348:
                 mSerial->write("N");
                 mSerial->write(ui->plainTextEdit_volume->toPlainText().toLocal8Bit().data());
                 mSerial->write("!");
                 QMessageBox::information(this, QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("向3×4板8号孔给药"),QMessageBox::Ok ,QMessageBox::NoButton);
                  break;
                 case 349:
                 mSerial->write("O");
                 mSerial->write(ui->plainTextEdit_volume->toPlainText().toLocal8Bit().data());
                 mSerial->write("!");
                 QMessageBox::information(this, QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("向3×4板9号孔给药"),QMessageBox::Ok ,QMessageBox::NoButton);
                  break;
                 case 3410:
                 mSerial->write("P");
                 mSerial->write(ui->plainTextEdit_volume->toPlainText().toLocal8Bit().data());
                 mSerial->write("!");
                 QMessageBox::information(this, QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("向3×4板10号孔给药"),QMessageBox::Ok ,QMessageBox::NoButton);
                  break;
                 case 3411:
                 mSerial->write("Q");
                 mSerial->write(ui->plainTextEdit_volume->toPlainText().toLocal8Bit().data());
                 mSerial->write("!");
                 QMessageBox::information(this, QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("向3×4板11号孔给药"),QMessageBox::Ok ,QMessageBox::NoButton);
                  break;
                 case 3412:
                 mSerial->write("R");
                 mSerial->write(ui->plainTextEdit_volume->toPlainText().toLocal8Bit().data());
                 mSerial->write("!");
                 QMessageBox::information(this, QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("向3×4板12号孔给药"),QMessageBox::Ok ,QMessageBox::NoButton);
                  break;

             default:
                 QMessageBox::critical(this,QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("未选择给药孔"),QMessageBox::Close ,QMessageBox::NoButton);
                 break;
             }

         }
         else
         {
             QMessageBox::critical(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("给药体积不能小于10微升"),QMessageBox::Close ,QMessageBox::NoButton);
         }
    }else
    {
        QMessageBox::critical(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("串口未打开"),QMessageBox::Close ,QMessageBox::NoButton);
    }

}


void Widget::on_pushButton_absorb_clicked()
{
    if(uart_state == s_connect)
    {
       if(ui->plainTextEdit_volume->toPlainText().toInt() >= 1)    //不能小于1
       {
             switch(mBGwhichhole->checkedId())
             {
                 case 231:
                 mSerial->write("a");
                 mSerial->write(ui->plainTextEdit_volume->toPlainText().toLocal8Bit().data());
                 mSerial->write("!");
                 QMessageBox::information(this, QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("从2×3板1号孔采样"),QMessageBox::Ok ,QMessageBox::NoButton);
                 break;
                 case 232:
                 mSerial->write("b");
                 mSerial->write(ui->plainTextEdit_volume->toPlainText().toLocal8Bit().data());
                 mSerial->write("!");
                 QMessageBox::information(this, QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("从2×3板2号孔采样"),QMessageBox::Ok ,QMessageBox::NoButton);
                  break;
                 case 233:
                 mSerial->write("c");
                 mSerial->write(ui->plainTextEdit_volume->toPlainText().toLocal8Bit().data());
                 mSerial->write("!");
                 QMessageBox::information(this, QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("从2×3板3号孔采样"),QMessageBox::Ok ,QMessageBox::NoButton);
                  break;
                 case 234:
                 mSerial->write("d");
                 mSerial->write(ui->plainTextEdit_volume->toPlainText().toLocal8Bit().data());
                 mSerial->write("!");
                 QMessageBox::information(this, QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("从2×3板4号孔采样"),QMessageBox::Ok ,QMessageBox::NoButton);
                  break;
                 case 235:
                 mSerial->write("e");
                 mSerial->write(ui->plainTextEdit_volume->toPlainText().toLocal8Bit().data());
                 mSerial->write("!");
                 QMessageBox::information(this, QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("从2×3板5号孔采样"),QMessageBox::Ok ,QMessageBox::NoButton);
                  break;
                 case 236:
                 mSerial->write("f");
                 mSerial->write(ui->plainTextEdit_volume->toPlainText().toLocal8Bit().data());
                 mSerial->write("!");
                 QMessageBox::information(this, QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("从2×3板6号孔采样"),QMessageBox::Ok ,QMessageBox::NoButton);
                  break;

                 case 341:
                     mSerial->write("g");
                     mSerial->write(ui->plainTextEdit_volume->toPlainText().toLocal8Bit().data());
                     mSerial->write("!");
                     QMessageBox::information(this, QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("从3×4板1号孔采样"),QMessageBox::Ok ,QMessageBox::NoButton);
                  break;
                 case 342:
                 mSerial->write("h");
                 mSerial->write(ui->plainTextEdit_volume->toPlainText().toLocal8Bit().data());
                 mSerial->write("!");
                 QMessageBox::information(this, QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("从3×4板2号孔采样"),QMessageBox::Ok ,QMessageBox::NoButton);
                  break;
                 case 343:
                 mSerial->write("i");
                 mSerial->write(ui->plainTextEdit_volume->toPlainText().toLocal8Bit().data());
                 mSerial->write("!");
                 QMessageBox::information(this, QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("从3×4板3号孔采样"),QMessageBox::Ok ,QMessageBox::NoButton);
                  break;
                 case 344:
                 mSerial->write("j");
                 mSerial->write(ui->plainTextEdit_volume->toPlainText().toLocal8Bit().data());
                 mSerial->write("!");
                 QMessageBox::information(this, QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("从3×4板4号孔采样"),QMessageBox::Ok ,QMessageBox::NoButton);
                  break;
                 case 345:
                 mSerial->write("k");
                 mSerial->write(ui->plainTextEdit_volume->toPlainText().toLocal8Bit().data());
                 mSerial->write("!");
                 QMessageBox::information(this, QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("从3×4板5号孔采样"),QMessageBox::Ok ,QMessageBox::NoButton);
                  break;
                 case 346:
                 mSerial->write("l");
                 mSerial->write(ui->plainTextEdit_volume->toPlainText().toLocal8Bit().data());
                 mSerial->write("!");
                 QMessageBox::information(this, QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("从3×4板6号孔采样"),QMessageBox::Ok ,QMessageBox::NoButton);
                  break;
                 case 347:
                 mSerial->write("m");
                 mSerial->write(ui->plainTextEdit_volume->toPlainText().toLocal8Bit().data());
                 mSerial->write("!");
                 QMessageBox::information(this, QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("从3×4板7号孔采样"),QMessageBox::Ok ,QMessageBox::NoButton);
                  break;
                 case 348:
                 mSerial->write("n");
                 mSerial->write(ui->plainTextEdit_volume->toPlainText().toLocal8Bit().data());
                 mSerial->write("!");
                 QMessageBox::information(this, QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("从3×4板8号孔采样"),QMessageBox::Ok ,QMessageBox::NoButton);
                  break;
                 case 349:
                 mSerial->write("o");
                 mSerial->write(ui->plainTextEdit_volume->toPlainText().toLocal8Bit().data());
                 mSerial->write("!");
                 QMessageBox::information(this, QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("从3×4板9号孔采样"),QMessageBox::Ok ,QMessageBox::NoButton);
                  break;
                 case 3410:
                 mSerial->write("p");
                 mSerial->write(ui->plainTextEdit_volume->toPlainText().toLocal8Bit().data());
                 mSerial->write("!");
                 QMessageBox::information(this, QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("从3×4板10号孔采样"),QMessageBox::Ok ,QMessageBox::NoButton);
                  break;
                 case 3411:
                 mSerial->write("q");
                 mSerial->write(ui->plainTextEdit_volume->toPlainText().toLocal8Bit().data());
                 mSerial->write("!");
                 QMessageBox::information(this, QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("从3×4板11号孔采样"),QMessageBox::Ok ,QMessageBox::NoButton);
                  break;
                 case 3412:
                 mSerial->write("r");
                 mSerial->write(ui->plainTextEdit_volume->toPlainText().toLocal8Bit().data());
                 mSerial->write("!");
                 QMessageBox::information(this, QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("从3×4板12号孔采样"),QMessageBox::Ok ,QMessageBox::NoButton);
                  break;

             default:
                 QMessageBox::critical(this,QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("未选择给药孔"),QMessageBox::Close ,QMessageBox::NoButton);
                 break;
             }
           }else
           {
               QMessageBox::critical(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("采样体积不能小于10微升"),QMessageBox::Close ,QMessageBox::NoButton);
           }
    }else
    {
        QMessageBox::critical(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("串口未打开"),QMessageBox::Close ,QMessageBox::NoButton);
    }
}

