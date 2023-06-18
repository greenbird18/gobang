#ifndef PVE_H
#define PVE_H

#include "game.h"

#include <QWidget>

struct poscore{
    int x, y, score;
};

class PvE : public QWidget
{
    Q_OBJECT
public:
    explicit PvE(QWidget *parent = nullptr);
    std::vector<std::vector<int>> scoreMapVec;//存分数
    void calculateScore();
private slots:
    void RobotPlay();//评估函数
    void BackToBegin();
signals:
    void go_back();
private:
    game *ga=new game;
};

#endif // PVE_H
