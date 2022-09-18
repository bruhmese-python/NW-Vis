#include "QtWidget.h"
#include "qgraphicsitem.h"
#include <iostream>

QtWidget::QtWidget(float x, float y)
{
    this->setPosition(position({x,y}));
}
QtWidget::~QtWidget()
{
}
void QtWidget::setContext(/*Ui::*/QGraphicsView *_context,QGraphicsScene* _scene)
{
    this->context=_context;
    this->scene=_scene;
//    auto _geometry = this->context->geometry();
//    this->scene = new QGraphicsScene(0, 0,_geometry.width(),_geometry.height());
}
/*Ui::*/QGraphicsView* QtWidget::getContext() { return this->context; }

QGraphicsScene *QtWidget::getParentScene()
{
    return this->scene;
}

void QtWidget::addGraphicComponent(QGraphicsItem* gItem)
{
    this->graphicComponents.push_back(gItem);
}

position &QtWidget::getPosition()
{
    return pos;
}
void QtWidget::setPosition(position _pos)
{
    pos = _pos;
    for (QGraphicsItem* each : this->graphicComponents)
    {
        each->setPos(pos.x,pos.y);
    }
}
