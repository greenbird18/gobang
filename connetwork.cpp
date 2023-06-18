#include "connetwork.h"
#include "ui_connetwork.h"
#include "game.h"
#include "ui_game.h"
#include "sendmessage.h"

#include <QMessageBox>
#include <QDebug>
#include <QFile>
#include <QTextStream>

connetwork::connetwork(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::connetwork)
{
    ui->setupUi(this);
    this->setWindowTitle("连接");
    this->setFixedSize(500,500);
    ga->connt_yes=1;
    QString fileName = "NAME.txt";
    ga->gamer = ga->readFile(fileName);

    connect(ga,&game::Ready,this,&connetwork::SendReady);
    connect(ga,&game::Regret,this,&connetwork::SendRegret);
    connect(ga,&game::Peace,this,&connetwork::SendPeace);
    connect(ga,&game::Lose,this,&connetwork::SendLose);
    connect(ga,&game::Step,this,&connetwork::SendStep);
    connect(ga,&game::BackToBegin,this,&connetwork::BackToBegin);

    connect(m_s,&QTcpSocket::readyRead,[=](){
           QByteArray data = m_s->readAll();  // 读取接收到的数据
           QString receivedData = QString::fromUtf8(data);  // 将字节数组转换为QString
           if(receivedData == "准备")
           {
               ga->opponent_gaming=1;
           }
           else if(receivedData == "悔棋")
           {
               regret_chess();
           }
           else if(receivedData == "求和")
           {
               peacing();
           }
           else if(receivedData == "认输")
           {
               SendMessage *dlog=new SendMessage;
               dlog->out_text("对方甘拜下风");
               ga->gaming=0;
               ga->opponent_gaming=0;
               ga->ready->setEnabled(true);
               ga->lose->setEnabled(false);
               ga->regret->setEnabled(false);
               ga->peace->setEnabled(false);
           }else if(receivedData == "同意和棋")
           {
               SendMessage *dlog=new SendMessage;
               dlog->out_text("旗鼓相当！");
               ga->gaming=0;
               ga->opponent_gaming=0;
               ga->ready->setEnabled(true);
               ga->lose->setEnabled(false);
               ga->regret->setEnabled(false);
               ga->peace->setEnabled(false);
           }else if(receivedData == "拒绝和棋")
           {
               SendMessage *dlog=new SendMessage;
               dlog->out_text("对方拒绝和棋");
           }else if(receivedData == "同意悔棋")
           {
               if(ga->is_me)
               {
                   pop_gobang();
                   pop_gobang();
                   ga->update();
               }else
               {
                   ga->is_me=1;
                   ga->chess=!ga->chess;
                   pop_gobang();
                   ga->update();
               }
           }else if(receivedData == "拒绝悔棋")
           {
               SendMessage *dlog=new SendMessage;
               dlog->out_text("落子无悔");
           }else if(receivedData == QString("获得先手"))
           {
               ga->is_me=1;
               SendMessage *dlog=new SendMessage;
               dlog->out_text("您获得先手（黑棋）！");
               ga->chess_color="Black";
           }else if(receivedData == QString("获得后手"))
           {
               ga->is_me=0;
               SendMessage *dlog=new SendMessage;
               dlog->out_text("您获得后手（白棋）！");
               ga->chess_color="White";
           }
           else if(receivedData[0]>='0'&&receivedData[0]<='9')
           {
                QStringList dividelist = receivedData.split(","); // 将字符串按逗号分割成字符串列表
                if (dividelist.size() >= 2) {
                    int x = dividelist[0].toInt();  // 将字符串转换为整数
                    int y = dividelist[1].toInt();
                    ga->fill_gobang(x,y);
                }
           }else{
               ga->gamer_opponent=receivedData;
           }
       });

       connect(m_s,&QTcpSocket::disconnected,[=](){
            m_s->close();
            qDebug() << "断开连接";
            ga->con_opponent=0;
            QMessageBox::warning(NULL,"Error","连接断开！");
       });
}

connetwork::~connetwork()
{
    delete ui;
}

void connetwork::client_connect()
{
    m_s->connectToHost(QHostAddress(ip),port);
    if(m_s->waitForConnected()){
        close();
        ga->ready->setEnabled(true);
        ga->peace->setEnabled(false);
        ga->lose->setEnabled(false);
        ga->show();
        //发送连接成功的消息给服务端
        QString str=ga->gamer;
        ga->con_me=1;
        ga->con_opponent=1;
        m_s->write(str.toUtf8());
        m_s->flush();

    }
    else {
        qDebug()<<"链接失败";
        QMessageBox::warning(NULL,"Error","先创房或者IP输错了，笨蛋！！");
    }
}

void connetwork::peacing()
{
    SendMessage *dlog=new SendMessage;
    dlog->chice();
    dlog->out_text("对方请求和棋，是否同意");
    if(dlog->exec()==QDialog::Accepted)
    {
        QString str=QString("同意和棋");
        m_s->write(str.toUtf8());
        m_s->flush();
        SendMessage *dlog=new SendMessage;
        dlog->out_text("旗鼓相当！");
        ga->gaming=0;
        ga->ready->setEnabled(true);
        ga->lose->setEnabled(false);
        ga->regret->setEnabled(false);
        ga->peace->setEnabled(false);
    }else{
        QString str=QString("拒绝和棋");
        m_s->write(str.toUtf8());
        m_s->flush();
    }
}

void connetwork::regret_chess()
{
    SendMessage *dlog=new SendMessage;
    dlog->chice();
    dlog->out_text("对方请求悔棋，是否同意");
    if(dlog->exec()==QDialog::Accepted)
    {
        QString str=QString("同意悔棋");
        m_s->write(str.toUtf8());
        m_s->flush();
        if(ga->is_me)
        {
            ga->is_me=0;
            pop_gobang();
            ga->chess=!ga->chess;
            ga->update();
        }else{
            pop_gobang();
            pop_gobang();
            ga->update();
        }
    }else{
        QString str=QString("拒绝悔棋");
        m_s->write(str.toUtf8());
        m_s->flush();
    }
}

void connetwork::pop_gobang()
{
    ga->chessborad[ga->gobang.back().first/50][ga->gobang.back().second/50] = 0;
    ga->gobang.pop_back();
}

void connetwork::ReadIp()
{
    ip=this->ui->lineEdit->text();
    client_connect();
}

void connetwork::SendReady()
{
    QString str=QString("准备");
    m_s->write(str.toUtf8());
    m_s->flush();
}

void connetwork::SendRegret()
{
    QString str=QString("悔棋");
    m_s->write(str.toUtf8());
    m_s->flush();
}

void connetwork::SendPeace()
{
    QString str=QString("求和");
    m_s->write(str.toUtf8());
    m_s->flush();
}

void connetwork::SendLose()
{
    QString str=QString("认输");
    m_s->write(str.toUtf8());
    m_s->flush();
    SendMessage *dlog=new SendMessage;
    dlog->out_text("大败而归！");
    ga->gaming=0;
    ga->ready->setEnabled(true);
    ga->lose->setEnabled(false);
    ga->regret->setEnabled(false);
    ga->peace->setEnabled(false);
}

void connetwork::SendStep(int x,int y)
{
    QPoint point(x, y);
    QString str = QString("%1,%2").arg(point.x()).arg(point.y());
    m_s->write(str.toUtf8());
    m_s->flush();
}

void connetwork::BackToBegin()
{
    delete ga;
    emit go_back();
}


void connetwork::on_goback_clicked()
{
    emit go_back();
}

void connetwork::on_pushButton_clicked()
{
    ReadIp();
}
