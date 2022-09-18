#ifndef UTILS_H
#define UTILS_H

#include<set>
#include "QtWidget.h"

#define INCR_UNIT 20

namespace utils
{
extern std::set<position> p_set;
extern int initNodeName;

position randomPosition(int W,int H);
QPointF centrified(position,QSize);
position centrifiedPosition(position x, QSize s);
double getDistance(position, position);
double getDirection(position ,position);
position getIncr(position ,position);


std::string newNodeName();
};

#endif // UTILS_H
