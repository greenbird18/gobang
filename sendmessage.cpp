#include "sendmessage.h"
#include <QDialogButtonBox>

SendMessage::SendMessage(QWidget *parent)
    : QDialog(parent)
{
    setModal(true);
    setWindowTitle("Notice!");
    resize(400,100);
    layout = new QVBoxLayout(this);
}

void SendMessage::chice()
{
    QDialogButtonBox *button =new QDialogButtonBox(QDialogButtonBox::Yes | QDialogButtonBox::No);
    connect(button,&QDialogButtonBox::accepted,this,&SendMessage::accept);
    connect(button,&QDialogButtonBox::rejected,this,&SendMessage::reject);
    layout->addWidget(button);
}

void SendMessage::out_text(QString str)
{
    QLabel *label = new QLabel(str,this);
    layout->addWidget(label);
    show();
}
