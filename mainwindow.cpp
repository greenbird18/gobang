#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "load.h"
#include "ui_load.h"
#include "pve.h"
#include<QSound>
#include <QFont>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QSound::play(":/new/music/jsadk.wav");
    QPushButton *btn1=new QPushButton("联机对战",this);
    QPushButton *btn2=new QPushButton("人机对战",this);
    btn1->move(780,600);
    btn2->move(350,600);
    btn1->setFixedSize(150,75);
    btn1->setStyleSheet("font-size: 36px;");
    btn1->setFont(QFont("华文行楷"));
    btn2->setFont(QFont("华文行楷"));
    btn2->setFixedSize(150,75);
    btn2->setStyleSheet("font-size: 36px;");
    connect(btn1,&QPushButton::clicked,this,&MainWindow::on_pushButton_clicked);
    connect(btn2,&QPushButton::clicked,this,&MainWindow::on_pushButton2_clicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    this->hide();
    l = new load;
    l->show();
    connect(l,&load::BackToBegin,this,&MainWindow::show_mainwindow2);
}

void MainWindow::on_pushButton2_clicked()
{
    this->hide();
    pve=new PvE;
    connect(pve,&PvE::go_back,this,&MainWindow::show_mainwindow);

}

void MainWindow::show_mainwindow()
{
    delete pve;
    this->show();
}

void MainWindow::show_mainwindow2()
{
    delete l;
    show();
}
