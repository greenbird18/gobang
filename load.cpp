#include "load.h"
#include "ui_load.h"
#include "game.h"
#include "ui_game.h"
#include "ui_connetwork.h"

#include <QDebug>
#include <QPushButton>
#include <QLabel>
#include <QtWidgets>
#include <QLineEdit>
#include <QMessageBox>
#include <fstream>
#include <iostream>
#include <QFile>
#include <QPainter>
#include <QTextStream>
#include <QFont>
load::load(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::load)
{
     ui->setupUi(this);
     connect(cho,&choice::BackToBegin,[=]{
         close();
         emit BackToBegin();
     });
}
choice::choice(QWidget *parent)
    : QWidget(parent)
{

    creat=new QPushButton("创建房间",this);
    joinin=new QPushButton("加入房间",this);
    back=new QPushButton("返回",this);
    creat->setFont(QFont("华文行楷",28));
    joinin->setFont(QFont("华文行楷",28));
    back->setFont(QFont("华文行楷",28));
    creat->setFixedSize(150,75); joinin->setFixedSize(150,75);
    back->setFixedSize(150,75);
    connect(joinin,&QPushButton::clicked,this,&choice::clie);
    connect(creat,&QPushButton::clicked,this,&choice::serv);
    connect(back,&QPushButton::clicked,[=]{
        close();
        emit BackToBegin();
    });

}
load::~load()
{
    delete ui;
}

void choice::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.drawPixmap(0,0,QPixmap(":/images/p2.jpg"));
}



void choice::serv()
{
    close();
    Ser=new Server(this);
    Ser->lisen_connection();
    connect(Ser,&Server::go_back,this,&choice::SERBACK);
}

void choice::clie()
{
    close();
    Clie=new connetwork();
    Clie->setWindowIcon(QIcon(":/images/TaiChi.jpg"));
    Clie->show();
    connect(Clie,&connetwork::go_back,this,&choice::CONBACK);
}

void choice::CONBACK()
{
    delete Clie;
    show();
}

void choice::SERBACK()
{
    delete Ser;
    show();
}

void load::on_pushButton_clicked()
{
    name=ui->lineEdit->text();
    QString fileName = "NAME.txt";
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out.setCodec("UTF-8");   // 设置文件编码
        out << name << endl;
        file.close();
    }

    if(name.size()>0 && name.size()<=7)
    {
        close();
        //选择创建还是加入房间
        cho->setWindowTitle("五子棋");
        cho->setWindowIcon(QIcon(":/images/TaiChi.jpg"));
        cho->setFixedSize(500,500);
        cho->creat->move(175,50);
        cho->joinin->move(175,200);
        cho->back->move(175,350);
        cho->show();

    }else if(name==""){
        QMessageBox::information(this, "Waring", "没有昵称不让进哦！");
        ui->lineEdit->clear();
    }else{
        QMessageBox::information(this, "Waring", "昵称不能超过七个字喔！");
        ui->lineEdit->clear();
    }
}
