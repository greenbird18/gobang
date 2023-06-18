#ifndef LOAD_H
#define LOAD_H
#include "connetwork.h"
#include "server.h"

#include <QWidget>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui {class load;}
QT_END_NAMESPACE

class choice:public QWidget
{
    Q_OBJECT
signals:
   void BackToBegin();
public:
    choice(QWidget *parent = nullptr);
     void paintEvent(QPaintEvent *event);
     Server *Ser=nullptr;
     connetwork *Clie=nullptr;
      QPushButton *creat;
      QPushButton *joinin;
      QPushButton *back;
private slots:
     void serv();
     void clie();
     void SERBACK();
     void CONBACK();
};


class load : public QWidget
{
    Q_OBJECT

public:
    QString name,pass;
    explicit load(QWidget *parent = nullptr);
    ~load();
    QPushButton *load_ready=new QPushButton("进入",this);
    bool LogSuccess = 0;
   choice *cho=new choice;
private slots:
    void on_pushButton_clicked();

private:
    Ui::load *ui;



signals:
    void BackToBegin();
};

#endif // LOAD_H
