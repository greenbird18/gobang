#ifndef SERVER_H
#define SERVER_H

#include "game.h"

#include <QWidget>
#include <QtNetwork>

class Server : public QWidget
{
    Q_OBJECT
public:
    explicit Server(QWidget *parent = nullptr);
    QTcpServer * m_tcp;
    QTcpSocket * m_s;
    const int port=8888;
    game *ga =new game;
    void lisen_connection();
    void peacing();
    void regret_chess();
    void pop_gobang();
private slots:
    void SendReady();
    void SendRegret();
    void SendPeace();
    void SendLose();
    void SendStep(int,int);
    void Send_Client_fir_sec(QString);
    void BackToBegin();
signals:
    void go_back();
};


#endif // SERVER_H
