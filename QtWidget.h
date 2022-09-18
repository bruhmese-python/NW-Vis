#ifndef QTWIDGET_H
#define QTWIDGET_H

#include "qapplication.h"
#pragma once
#include <cmath>
#include<QGraphicsView>

struct position
{
    float x, y;
    position operator/(float secondValue)
    {
        position returnValue;
        returnValue.x = this->x / secondValue;
        returnValue.y = this->y / secondValue;
        return returnValue;
    };
    position operator*(float secondValue)
    {
        position returnValue;
        returnValue.x = this->x * secondValue;
        returnValue.y = this->y * secondValue;
        return returnValue;
    };
    position operator+(position secondValue)
    {
        position returnValue;
        returnValue.x = /*abs*/(this->x + secondValue.x);
        returnValue.y = /*abs*/(this->y + secondValue.y);
        return returnValue;
    };
    position operator-(position secondValue)
    {
        position returnValue;
        returnValue.x = /*abs*/(this->x - secondValue.x);
        returnValue.y = /*abs*/(this->y - secondValue.y);
        return returnValue;
    };
    friend bool operator<(const position& A,const position& B)
    {
        return A.x < B.x and A.y < B.y;
    }
};

class QtWidget
{
private:
    position pos;
    /*Ui::*/QGraphicsView *context;

public:
    QGraphicsScene *scene;
    std::vector<QGraphicsItem *> graphicComponents;
    QtWidget *QtComponent;

    position &getPosition();
    void setPosition(position);

    virtual void animate() = 0;
    virtual void draw() = 0;

    void setContext(/*Ui::*/QGraphicsView *,QGraphicsScene *);
    /*Ui::*/QGraphicsView* getContext();
    QGraphicsScene *getParentScene();
    void addGraphicComponent(QGraphicsItem*);


    QtWidget(float, float);
    ~QtWidget();
};

#endif
