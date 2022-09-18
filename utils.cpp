#include "utils.h"
#include "QRandomGenerator"
#include <iostream>
#include "scenemanager.h"

std::set<position> utils::p_set = {};
int utils::initNodeName = 0;

position utils::randomPosition(int W,int H)
{
//    qDebug()<<"random Postion (W,H) : ("<<W<<":"<<H<<")\n";
    int
            I = QRandomGenerator::global()->bounded(0, maxNodeCount),
            J = QRandomGenerator::global()->bounded(0, maxNodeCount);
    return sceneManager::spawnPoints[I][J];
}

std::string percentFilename(float percent)
{
    int newPercent = static_cast<int>(percent * 10);
    return std::to_string(newPercent);
}

QPointF utils::centrified(position x, QSize s)
{
    return QPointF(x.x - s.width()/2, x.y - s.height()/2);
}
position utils::centrifiedPosition(position x, QSize s)
{
    return position({x.x - s.width()/2, x.y - s.height()/2});
}

double utils::getDistance(position P1, position P2)
{
    return std::sqrt(std::pow(P1.x - P2.x, 2) + std::pow(P1.y - P2.y, 2));
}

position utils::getIncr(position P1, position P2)
{
    position P3= P2-P1;
    P3.x = abs(P3.x);
    P3.y = abs(P3.y);
    return P3 / INCR_UNIT;
}

std::string utils::newNodeName()
{
   return std::to_string(++initNodeName);
}

double utils::getDirection(position P1,position P2)
{
    double angle = atan2(P2.y - P1.y ,P2.x - P2.x);
    qDebug()<<"Returning angle"<<angle;
    return angle;
}
