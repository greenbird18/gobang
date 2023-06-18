#ifndef SENDMESSAGE_H
#define SENDMESSAGE_H

#include <QWidget>
#include <QDialog>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>

class SendMessage : public QDialog
{
    Q_OBJECT
public:
    SendMessage(QWidget *parent = nullptr);
    QVBoxLayout *layout;

    void chice();
    void out_text(QString str);
};

#endif // SENDMESSAGE_H
