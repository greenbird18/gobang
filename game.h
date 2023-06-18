#ifndef GAME_H
#define GAME_H
#include <QWidget>
#include <vector>
#include <QPushButton>
#include <QMediaPlayer>
#include <QMediaPlaylist>

namespace Ui {
class game;
}

class game : public QWidget
{
    Q_OBJECT

public:
    explicit game(QWidget *parent = nullptr);
    ~game();
    bool p_or_e = 1;//默认非人机模式
    bool chess = 1;//是否为黑棋
    bool gaming = 0;//准备
    bool opponent_gaming=0;//对手准备
    bool is_me=0;//判定下棋方
    bool connt_yes=0;
    bool con_me;
    bool con_opponent=0;
    int posx=-1,posy=-1;//鼠标实时位置
    bool flag=0;//区分是否为服务端
    QString gamer;
    QString gamer_opponent;
    QString chess_color;

    std::vector<std::pair<int,int> > gobang; //棋子位置顺序
    int chessborad[35][25];

    QPushButton *ready=new QPushButton("准备",this);
    QPushButton *regret=new QPushButton("悔棋",this);
    QPushButton *peace=new QPushButton("求和",this);
    QPushButton *lose=new QPushButton("认输",this);
    QPushButton *back=new QPushButton("返回",this);
    void paintEvent(QPaintEvent *event);
    void fill_gobang(int,int);
    void judge_win_lose(int,int,int);
    void WinerLoser(bool,int);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void begin_game();
    QString readFile(const QString &fileName);

signals:
    void Ready();
    void Regret();
    void Peace();
    void Lose();
    void Step(int,int);
    void Client_fir_sec(QString);
    void robotdown();
    void BackToBegin();

public slots:
    void ButtonReady();
    void ButtonRegret();
    void ButtonPeace();
    void ButtonLose();
    void ButtonBack();
private:
    Ui::game *ui;    

};

#endif // GAME_H
