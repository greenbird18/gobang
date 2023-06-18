#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>

#include "pve.h"
#include "load.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void on_pushButton2_clicked();

    void show_mainwindow();
    void show_mainwindow2();
private:
    Ui::MainWindow *ui;
    PvE *pve=nullptr;
    load *l=nullptr;
};
#endif // MAINWINDOW_H
