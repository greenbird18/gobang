#include "game.h"
#include "ui_game.h"
#include "sendmessage.h"

#include <QPushButton>
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <QMessageBox>
#include <cmath>
#include <QRandomGenerator>
#include <QDateTime>
#include <QFont>
#include <QLabel>
#include <QThread>
#include<QSound>

game::game(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::game)
{
    ui->setupUi(this);
    this->setWindowTitle("五子棋");
    this->setMouseTracking(true);
    ui->label->hide();
    ui->label_2->hide();
    ui->label_3->hide();
    ui->label_4->hide();
    setFixedSize(1600,900);
    connect(ready, &QPushButton::clicked, this, &game::ButtonReady);
    connect(regret, &QPushButton::clicked, this, &game::ButtonRegret);
    connect(peace, &QPushButton::clicked, this, &game::ButtonPeace);
    connect(lose, &QPushButton::clicked, this, &game::ButtonLose);
    connect(back, &QPushButton::clicked, this, &game::ButtonBack);
    back->move(1300,700);
    back->setFont(QFont("华文行楷",28));
    back->setFixedSize(100,50);
    ready->move(1300,100);
    ready->setFont(QFont("华文行楷",28));
    ready->setFixedSize(100,50);
    regret->move(1300,250);
    regret->setFont(QFont("华文行楷",28));
    regret->setFixedSize(100,50);
    peace->move(1300,400);
    peace->setFont(QFont("华文行楷",28));
    peace->setFixedSize(100,50);
    lose->move(1300,550);
    lose->setFont(QFont("华文行楷",28));
    lose->setFixedSize(100,50);
    ready->setEnabled(false);
    regret->setEnabled(false);
    peace->setEnabled(false);
    lose->setEnabled(false);
}

game::~game()
{
    delete ui;
}

void game::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    QBrush brush1(Qt::black); // 创建一个黑色的填充刷
    QBrush brush2(Qt::white);
    QBrush brush3(Qt::red);
    QBrush brush4(Qt::gray);
    QBrush brush5(QColor(255, 228, 205, 50));
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.drawPixmap(0,0,QPixmap(":/images/game_background.jpg"));
    painter.translate(50,0);
    if (con_me) {
        QColor color(255,255,255);
        painter.setPen(QPen(color, 1));
        if(chess_color=="Black")
        {
            painter.setBrush(brush1);
            painter.drawEllipse(60,300,80,80);

        }else if(chess_color=="White"){
            painter.setBrush(brush2);
            painter.drawEllipse(60,300,80,80);
        }
        QPixmap pixmap(":/images/man.jpg");
        QBrush brush(QColor(0, 0, 0, 50));
        painter.setBrush(QBrush(pixmap));
        color.setRgb(0x6d, 0x9e, 0xd9);
        painter.setPen(QPen(color, 4));
        painter.drawEllipse(QRectF(20, 50, 160, 160));
        ui->label->setText(gamer);
        ui->label_2->setText("执子：");
        ui->label->setStyleSheet("QLabel { color: red;}");
        ui->label_2->setFont(QFont("华文行楷",23));
        ui->label->show();
        ui->label_2->show();
        color.setRgb(0,0,0);
        painter.setPen(QPen(color, 0));
    }

    if(con_opponent)
    {
        if(chess_color=="Black")
        {
            painter.setBrush(brush2);
            painter.drawEllipse(60,520,80,80);

        }else if(chess_color=="White"){
            painter.setBrush(brush1);
            painter.drawEllipse(60,520,80,80);
        }
        QPixmap pixmap(":/images/girl.jpg");// 加载图像
        QBrush brush(QColor(0, 0, 30, 0));// 创建 QBrush 对象，用于绘制背景色和阴影
        painter.setBrush(QBrush(pixmap));
        QColor color(0xff, 0x00, 0x22);
        painter.setPen(QPen(color, 4));
        painter.drawEllipse(20, 620, 160, 160);
        ui->label_3->setText("执子：");
        ui->label_4->setText(gamer_opponent);
        ui->label_3->setFont(QFont("华文行楷",23));
        ui->label_3->show();
        ui->label_4->show();
    }
    painter.translate(-50,0);
    painter.setPen(Qt::black);  // 设置画笔颜色
    for(int i=0;i<17;i++)
    {
        painter.drawLine(i*50+300, 50, i*50+300, 850);  // 绘制竖线
    }
    for(int j=0;j<17;j++)
    {
        painter.drawLine(300, j*50+50, 1100,j*50+50);  // 绘制横线
    }

    if(posx>=300&&posx<=1100&&posy>=50&&posy<=850)
    {
        painter.setBrush(brush4);
        painter.drawEllipse(posx-5,posy-5,10,10);
    }

    int j=0;

    for(auto i = gobang.begin();i!=gobang.end();i++) {
        ++j;
        if(j%2) {
            painter.setBrush(brush1); // 应用黑色填充刷
        } else {
            painter.setBrush(brush2); // 应用白色填充刷
        }
        painter.drawEllipse(i->first-20,i->second-20,40,40);
    }
    painter.setBrush(brush5);
    painter.drawRect(280,30,17*50,17*50);
    if(!gobang.empty())
    {
        painter.setBrush(brush3);
        painter.drawRect(gobang.back().first-3,gobang.back().second-3,6,6);
    }

    if(gobang.size()==1){
        if(is_me)
            regret->setEnabled(false);
        else
            regret->setEnabled(true);
    }
    if(gobang.size()<1)
            regret->setEnabled(false);
}

void game::fill_gobang(int x, int y)
{
    if(x >= 280 && x <= 1120 && y >= 30 && y <= 870)
    {
        x=(x+25)/50;
        y=(y+25)/50;
        x*=50;
        y*=50;
        auto it=std::find(std::begin(gobang), std::end(gobang), std::make_pair(x,y));
        if(it==std::end(gobang))
        {
            regret->setEnabled(true);
            lose->setEnabled(true);
            peace->setEnabled(true);
            chessborad[x/50][y/50] = chess ? 1 : -1;
            QSound::play(":/new/music/01.wav");
            gobang.push_back(std::make_pair(x,y));
            update();
            judge_win_lose(x/50,y/50,gobang.size());
            chess = !chess;
            if(is_me){
                if(p_or_e)
                emit Step(x,y);
                is_me=0;
            }else
            {
                is_me=1;
            }
        }
    }
}

void game::judge_win_lose(int x,int y,int num)//当前棋子坐标以及棋子数
{
    int begin_x=std::max(x-5,0),
        begin_y=std::max(y-5,0),
        end_x  =std::min(x+5,24),
        end_y  =std::min(y+5,19);
    int sum=0;
    for(int i=begin_x;i<=end_x;i++)
    {
        if(chessborad[i][y]!=chessborad[x][y])sum=0;
        else sum+=chessborad[i][y];
        if(sum>=5||sum<=-5)
        {
            WinerLoser(false,sum);//结算页面
            return;
        }
    }

    sum=0;
    for(int i=begin_y;i<=end_y;i++)
    {
        if(chessborad[x][i]!=chessborad[x][y])sum=0;
        else sum+=chessborad[x][i];
        if(sum>=5||sum<=-5)
        {
            WinerLoser(false,sum);
            return;
        }
    }

    sum=0;
    for(int i=x,j=y; i>=begin_x && j>=begin_y; i--,j--)
    {
        if(chessborad[i][j]==chessborad[x][y])sum+=chessborad[x][y];
        else break;

    }
    for(int i=x+1,j=y+1; i<=end_x && j<=end_y; i++,j++)
    {
        if(chessborad[i][j]==chessborad[x][y])sum+=chessborad[x][y];
        else break;
    }
    if(sum>=5||sum<=-5)
    {
        WinerLoser(false,sum);
        return;
    }

    sum=0;
    for(int i=x,j=y; i>=begin_x && j<=end_y; i--,j++)
    {
        if(chessborad[i][j]==chessborad[x][y])sum+=chessborad[x][y];
        else break;
    }
    for(int i=x+1,j=y-1; i<=end_x && j>=begin_y; i++,j--)
    {
        if(chessborad[i][j]==chessborad[x][y])sum+=chessborad[x][y];
        else break;
    }
    if(sum>=5||sum<=-5)
    {
        WinerLoser(false,sum);
        return;
    }
    if(num==340)
    {
        WinerLoser(true,0);
    }
    return;
}

void game::WinerLoser(bool flag,int sum)//是否平局,棋子分数
{
    if(flag)
    {
        SendMessage *dlog=new SendMessage;
        dlog->out_text(" 平局！");
    }
    else if(sum>=5)
        if(chess_color=="Black")
        {
            SendMessage *dlog=new SendMessage;
            dlog->out_text(gamer+" Winner！\n"+gamer_opponent+" Loser！");
        }
        else
        {
            SendMessage *dlog=new SendMessage;
            dlog->out_text(gamer_opponent+" Winner！\n"+gamer+" Loser！");
        }
    else
        if(chess_color=="White")
        {
            SendMessage *dlog=new SendMessage;
            dlog->out_text(gamer+" Winner！\n"+gamer_opponent+" Loser！");
        }
        else
        {
            SendMessage *dlog=new SendMessage;
            dlog->out_text(gamer_opponent+" Winner！\n"+gamer+" Loser！");
        }
    gaming = 0;
    opponent_gaming=0;
    ready->setEnabled(true);
    regret->setEnabled(false);
    peace->setEnabled(false);
    lose->setEnabled(false);

}


void game::mouseReleaseEvent(QMouseEvent *event)
{
    int x=event->x();
    int y=event->y();
    if(!connt_yes)
        QMessageBox::information(this, "Waring", "请等待对方连接");
    if(gaming && opponent_gaming)//双方准备就绪
    {
        if(is_me)
            fill_gobang(x,y);
        else if(x >= 280 && x <= 1270 && y >= 30 && y <= 870)
            QMessageBox::information(this, "Waring", "请等对方落子");
    }
    if(!p_or_e && !is_me && gaming) emit robotdown();
}

void game::mouseMoveEvent(QMouseEvent *event)
{
    posx=event->x();
    posy=event->y();
    //qDebug()<<posx<<" "<<posy;
    if(posx >= 290 && posx <= 1260 && posy >= 40 && posy <= 860)
    {
        posx=(posx+25)/50;
        posy=(posy+25)/50;
        posx*=50;
        posy*=50;
    }
    update();
}

void game::begin_game()
{
    QRandomGenerator generator(QDateTime::currentMSecsSinceEpoch());
    int rand = generator.bounded(100);
    if(rand & 1)
    {
        is_me = 1;
        if(p_or_e){
            QThread::sleep(1);
            emit Client_fir_sec(QString("获得后手"));
        }
        SendMessage *dlog=new SendMessage;
        dlog->out_text("您获得先手（黑棋）");
        chess_color="Black";
    }else{
        is_me = 0;
        if(p_or_e){
            QThread::sleep(1);
            emit Client_fir_sec(QString("获得先手"));
        }
        SendMessage *dlog=new SendMessage;
        dlog->out_text("您获得后手（白棋）");
        chess_color="White";
        if(!p_or_e&&!is_me)emit robotdown();
    }
}

QString game::readFile(const QString &fileName)
{
    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return "";
    }

    QTextStream in(&file);
    in.setCodec("UTF-8");   // 设置文件编码
    QString text = in.readAll();

    file.close();

    return text;
}

void game::ButtonReady()//准备按钮事件
{
    for(int i=0;i<30;i++)
        for(int j=0;j<25;j++)
            chessborad[i][j]=0;
    gobang.clear();
    update();
    gaming = 1;//游戏开始
    if(!p_or_e){
        flag=1;
        opponent_gaming=1;
        con_me=1;
        con_opponent=1;
    }
    chess=1;
    if(p_or_e) emit Ready();
    ready->setEnabled(false);
    if(opponent_gaming && flag){
        begin_game();//判断先后手
    }
}

void game::ButtonRegret()
{
    if(!p_or_e)
    {
        chessborad[gobang.back().first/50][gobang.back().second/50] = 0;
        gobang.pop_back();
        chessborad[gobang.back().first/50][gobang.back().second/50] = 0;
        gobang.pop_back();
        update();
    }
    else emit Regret();
}

void game::ButtonPeace()
{
    if(!p_or_e)
    {
        QMessageBox::information(NULL,"讲个笑话","有人打人机还求和。");
    }
    else emit Peace();
}

void game::ButtonLose()
{
    if(!p_or_e)
    {
        WinerLoser(0,chess_color=="Black"?-5:5);
    }
    else emit Lose();
}

void game::ButtonBack()
{
    this->close();
    emit BackToBegin();
}
