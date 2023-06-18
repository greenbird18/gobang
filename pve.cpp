#include "pve.h"

#include <vector>
#include <ctime>
#include <QSound>

PvE::PvE(QWidget *parent)
    : QWidget(parent)
{
    ga->p_or_e=0;//人机模式
    ga->flag=1;
    ga->opponent_gaming=1;
    ga->con_me=1;
    ga->con_opponent=1;
    ga->gamer="YOU";
    ga->gamer_opponent="ROBOT";
    ga->connt_yes=1;
    ga->ready->setEnabled(true);
    ga->show();
    connect(ga,&game::robotdown,this,&PvE::RobotPlay);
    connect(ga,&game::BackToBegin,this,&PvE::BackToBegin);

}

void PvE::calculateScore()
{
    //统计玩家或者电脑连成的子
    int personNum = 0;// 玩家连成子的个数
    int botNum = 0;//人机连成子的个数
    int emptyNum = 0;// 各个方向空白位的个数
    int BOARD_GRAD_SIZE=17;
    int tt=6;
    int t=0;
    //清空评分数组
    scoreMapVec.clear();
    for(int i = 0; i < BOARD_GRAD_SIZE+6; i++)
    {
        std::vector<int> lineScores;
        for(int j = 0; j < BOARD_GRAD_SIZE; j++)
            lineScores.push_back(0);
        scoreMapVec.push_back(lineScores);
    }
    //计分(完全遍历)
    for(int row = tt; row < BOARD_GRAD_SIZE+tt; row++)
        for(int col =t; col < BOARD_GRAD_SIZE; col++)
        {
            if(row >tt && col > t && ga->chessborad[row][col] == 0)//从空白点计算
            {
                for(int y = -1; y <= 1; y++)//遍历八个方向
                    for(int x = -1; x <= 1; x++)
                    {
                        // 重置
                        personNum=0;
                        botNum=0;
                        emptyNum=0;

                        //原坐标不算
                        if(!(y == 0 && x == 0))
                        {
                            //每个方向延伸四个棋子
                            //对玩家白子评分（正反两个方向）
                            //正方向
                            for(int i = 1; i <= 4; i++)
                            {
                                if(row + i*y > tt && row + i*y < BOARD_GRAD_SIZE+tt &&
                                        col + i*x > t && col + i*x <BOARD_GRAD_SIZE &&
                                        ga->chessborad[row + i*y][col + i*x] == (ga->chess_color=="Black"?1:-1))// 真人玩家的子
                                {
                                    personNum++;
                                }
                                else if(row + i*y >tt && row + i*y < BOARD_GRAD_SIZE+tt &&
                                        col + i*x > t && col + i*x <BOARD_GRAD_SIZE &&
                                        ga->chessborad[row + i*y][col + i*x]==0)//  空白位
                                {
                                    emptyNum++;
                                    break;
                                }
                                else // 出边界或有人机棋子
                                    break;
                            }

                            //反方向
                            for(int i = 1; i <= 4; i++)
                            {
                                if(row - i*y > tt && row- i*y <BOARD_GRAD_SIZE+tt
                                        && col- i*x > t && col - i*x < BOARD_GRAD_SIZE &&
                                        ga->chessborad[row - i*y][col - i*x]==(ga->chess_color=="Black"?1:-1))
                                {
                                    personNum++;
                                }
                                else if(row - i*y >tt&& row - i*y < BOARD_GRAD_SIZE+tt
                                        && col- i*x > t && col- i*x <BOARD_GRAD_SIZE &&
                                        ga->chessborad[row - i*y][col - i*x]==0)
                                {
                                    emptyNum++;
                                    break;
                                }
                                else // 出边界或有人机棋子
                                    break;
                            }
                            //计分
                            if(personNum == 1)   // 杀一
                                scoreMapVec[row-tt][col-t]+=10;
                            else if(personNum == 2) // 杀二
                            {
                                if(emptyNum == 1)
                                    scoreMapVec[row-tt][col-t] += 30;
                                else if(emptyNum == 2)
                                    scoreMapVec[row-tt][col-t] += 40;
                            }
                            else if(personNum == 3)// 杀四
                            {
                                //量变位不一样，优先级不一样
                                if(emptyNum == 1)
                                    scoreMapVec[row-tt][col-t] += 60;
                                else if(emptyNum == 2)
                                    scoreMapVec[row-tt][col-t]+=110;
                            }
                            else if(personNum == 4) // 杀五
                                scoreMapVec[row-tt][col-t]+=12000;

                            //进行一次清空
                            emptyNum = 0;

                            //对人机白子评分
                            //正方向
                            for(int i=1;i<=4;i++)
                            {
                                if(row + i*y > tt && row + i*y < BOARD_GRAD_SIZE+tt
                                        && col + i*x >t && col + i*x <BOARD_GRAD_SIZE &&
                                        ga->chessborad[row + i*y][col + i*x] == (ga->chess_color=="Black"?-1:1))//真人玩家的子
                                {
                                    botNum++;
                                }
                                else if(row + i*y >tt && row + i*y < BOARD_GRAD_SIZE+tt
                                        && col + i*x >t && col + i*x < BOARD_GRAD_SIZE &&
                                        ga->chessborad[row + i*y][col + i*x]==0)//空白位
                                {
                                    emptyNum++;
                                    break;
                                }
                                else //出边界
                                    break;
                            }

                            // 反方向
                            for(int i = 1; i <= 4; i++)
                            {
                                if(row - i*y > tt && row - i*y < BOARD_GRAD_SIZE+tt
                                        && col- i*x > t && col - i*x < BOARD_GRAD_SIZE &&
                                        ga->chessborad[row - i*y][col - i*x] == (ga->chess_color=="Black"?1:-1))
                                {
                                    botNum++;
                                }
                                else if(row - i*y >tt && row- i*y < BOARD_GRAD_SIZE+tt
                                        && col-i*x>t && col-i*x < BOARD_GRAD_SIZE &&
                                        ga->chessborad[row - i*y][col - i*x]==0)
                                {
                                    emptyNum++;
                                    break;
                                }
                                else // 出边界
                                    break;
                            }

                            //计分
                            if(botNum == 0)                         // 普通
                               scoreMapVec[row-tt][col-t]+=5;
                            else if(botNum == 1)                    // 活二
                                scoreMapVec[row-tt][col-t]+=10;
                            else if(botNum == 2)
                            {
                                if(emptyNum == 1)                   //  死三
                                    scoreMapVec[row-tt][col-t]+=25;
                                else if(emptyNum == 2)              // 活三
                                    scoreMapVec[row-tt][col-t]+=50;
                            }
                            else if(botNum == 3)
                            {
                                if(emptyNum == 1)                    //死四
                                    scoreMapVec[row-tt][col-t] += 55;
                                else if(emptyNum == 2)
                                    scoreMapVec[row-tt][col-t]+=100;     // 活四
                            }
                            else if(botNum >= 4)                  //活五
                                scoreMapVec[row-tt][col-t] += 120000;
                        }
                    }
            }
        }
}

void PvE::RobotPlay()
{
    calculateScore();
    int maxScore = 0;
    int tt=6,t=0;
    std::vector<std::pair<int,int>> maxPoints;

    for(int row = 6; row < 17+6; row++)
    {
        for(int col = 0; col < 17 ; col++)
        {
            //前提是坐标为空
            if(ga->chessborad[row][col] == 0)
            {
                if(scoreMapVec[row-tt][col-t] > maxScore)
                {
                    maxPoints.clear();
                    maxScore = scoreMapVec[row-tt][col-t];
                    maxPoints.push_back(std::make_pair(row,col));
                }
                else if(scoreMapVec[row-tt][col-t] == maxScore)
                    maxPoints.push_back(std::make_pair(row,col));
            }
        }
    }
    srand((unsigned)time(0));
    int index = rand() % maxPoints.size();
    std::pair<int, int> pointPair = maxPoints.at(index);
    ga->fill_gobang(pointPair.first*50,50*pointPair.second);
}

void PvE::BackToBegin()
{
    delete ga;
    emit go_back();

}

