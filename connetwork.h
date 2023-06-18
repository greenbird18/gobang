#ifndef CONNETWORK_H
#define CONNETWORK_H

#include <QWidget>
#include <QPushButton>
#include <QtNetwork>
#include "game.h"

namespace Ui {
class connetwork;
}

class connetwork : public QWidget
{
    Q_OBJECT

public:
    explicit connetwork(QWidget *parent = nullptr);
    ~connetwork();
    bool ok=0;
    QTcpSocket * m_s=new QTcpSocket;
    game *ga=new game;
    const int port = 8888;

    void client_connect();
    void peacing();
    void regret_chess();
    void pop_gobang();
    void ReadIp();
private slots:    
    void SendReady();
    void SendRegret();
    void SendPeace();
    void SendLose();
    void SendStep(int,int);
    void BackToBegin();

    void on_goback_clicked();

    void on_pushButton_clicked();

signals:
    void go_back();
private:
    Ui::connetwork *ui;
    QString ip="";

};

#endif // CONNECT_H
