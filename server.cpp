#include "server.h"
#include "sendmessage.h"

#include <QMessageBox>
#include <QFile>
#include <QTextStream>

Server::Server(QWidget *parent) : QWidget(parent)
{
    m_s=new QTcpSocket(this);
    m_tcp=new QTcpServer(this);
    ga->con_me=1;
    ga->flag=1;
    QString fileName = "NAME.txt";
    ga->gamer = ga->readFile(fileName);

    ga->show();
    connect(ga,&game::Ready,this,&Server::SendReady);
    connect(ga,&game::Regret,this,&Server::SendRegret);
    connect(ga,&game::Peace,this,&Server::SendPeace);
    connect(ga,&game::Lose,this,&Server::SendLose);
    connect(ga,&game::Step,this,&Server::SendStep);
    connect(ga,&game::Client_fir_sec,this,&Server::Send_Client_fir_sec);
    connect(ga,&game::BackToBegin,this,&Server::BackToBegin);

    connect(m_tcp,&QTcpServer::newConnection,[=](){
       m_s=m_tcp->nextPendingConnection();
       connect(m_s,&QTcpSocket::readyRead,[=](){
           QByteArray data = m_s->readAll();  // 读取接收到的数据
           QString receivedData = QString::fromUtf8(data);  // 将字节数组转换为QString
           if(receivedData == "准备")
           {
               ga->opponent_gaming=1;
               if(ga->gaming)ga->begin_game();

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
           }else if(receivedData == "拒绝和棋"){
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
               dlog->out_text("落子无悔！");
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
               QString str=ga->gamer;
               m_s->write(str.toUtf8());
               m_s->flush();
               ga->connt_yes=1;
               ga->ready->setEnabled(true);
               ga->peace->setEnabled(false);
               ga->con_opponent=1;
               ga->update();
           }
       });

       connect(m_s,&QTcpSocket::disconnected,[=](){
           m_s->close();
           ga->con_opponent=0;
           QMessageBox::warning(NULL,"Error","连接断开！");
       });
    });
}

void Server::lisen_connection()
{
    m_tcp->listen(QHostAddress::Any,port);
}

void Server::peacing()
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

void Server::regret_chess()
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

void Server::pop_gobang()
{
    ga->chessborad[ga->gobang.back().first/50][ga->gobang.back().second/50] = 0;
    ga->gobang.pop_back();
}

void Server::SendReady()
{
    QString str=QString("准备");
    m_s->write(str.toUtf8());
    m_s->flush();
}

void Server::SendRegret()
{
    QString str=QString("悔棋");
    m_s->write(str.toUtf8());
    m_s->flush();
}

void Server::SendPeace()
{
    QString str=QString("求和");
    m_s->write(str.toUtf8());
    m_s->flush();
}

void Server::SendLose()
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

void Server::SendStep(int x,int y)
{
    QPoint point(x, y);
    QString str = QString("%1,%2").arg(point.x()).arg(point.y());
    m_s->write(str.toUtf8());
    m_s->flush();
}

void Server::Send_Client_fir_sec(QString str)
{
    qDebug()<<str;
    m_s->write(str.toUtf8());
    m_s->flush();
}

void Server::BackToBegin()
{
    delete ga;
    emit go_back();
}
