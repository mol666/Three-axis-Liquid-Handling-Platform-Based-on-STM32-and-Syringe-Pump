#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QMessageBox>
#include <QButtonGroup>
#include <QString>
#include <QTimer>
#include <QDebug>

#define DISPLAYMODE_ASCII 0
#define DISPLAYMODE_HEX 1

#define SENDOUTMODE_ASCII 0
#define SENDOUTMODE_HEX 1



QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:


    void Init_UartPort();


    Ui::Widget *ui;

    QSerialPort *mSerial;   //串口
    QButtonGroup* mBGdisplaymode;   //接收数据的显示模式
    QButtonGroup* mBGsendoutmode;   //发送数据的模式
    QButtonGroup* mBGwhichhole;     //选择单孔

    enum state { s_connect, s_disconnect} uart_state;


private slots:

    void on_pushButton_uartDisconnect_clicked();

    void on_pushButton_uartConnect_clicked();

    void slot_uartReadData();

    void on_buttom_sendout_clicked();

    void on_pushButton_clear_TBinput_clicked();

    void on_comBox_uartPort_clicked();

    void on_KEY1_clicked();

    void on_KEY2_clicked();

    void on_KEY3_clicked();

    void on_KEY4_clicked();

    void on_KEY5_clicked();

    void on_LEFT_clicked();

    void on_RIGHT_clicked();

    void on_pushButton_inject_clicked();

    void on_pushButton_absorb_clicked();
};
#endif // WIDGET_H
