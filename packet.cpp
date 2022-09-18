#include "packet.h"
#include "qgraphicsitem.h"
#include "scenemanager.h"
#include <iostream>
#include <memory>
#include "utils.h"

packet::packet(NODE_DATA d)
{
    auto &context = sceneManager::globalContext;
    auto &scene = sceneManager::globalScene;

    size = 0;
    ID = 0;
    data = d;
    startTime = 0;
    endTime = 0;

    QRect dimensions = context->geometry();
    position _p = utils::randomPosition(dimensions.width(),dimensions.height());
    QtComponent = new QtPacketComponent(_p.x, _p.y);

    this->QtComponent->setContext(context, scene);
}

packet::~packet()
{
    auto &gC = this->QtComponent->graphicComponents;
    for(auto each: gC)
        delete each;
    gC.clear();
}

void packet::render()
{
    this->QtComponent->draw();
}

QtPacketComponent::QtPacketComponent(float x, float y): QtWidget(x, y)
{
//    qDebug()<<"Creating packet component\n";
    disp = position({1,1});
    iterCount = 0;
    sceneManager::registerWidget(this);
}

void QtPacketComponent::draw()
{
    QPixmap picture;
    position _p = this->getPosition();
    picture.load(":/res/img/packet.png");
    QGraphicsPixmapItem* nodePixMap = scene->addPixmap(picture);
    nodePixMap->setPos(utils::centrified(_p,picture.size()));
//    QGraphicsTextItem* info = scene->addText("(" + QString::number(_p.x) + "," + QString::number(_p.y) + ")");
//    info->setPos(utils::centrified(_p,picture.size()));
//    this->addGraphicComponent(info);

//    qDebug() << "Drawing packet at "
//              << "(" << _p.x << "," << _p.y << ")\n";
    this->addGraphicComponent(nodePixMap);
    this->getContext()->setScene(scene);

}
void QtPacketComponent::animate()
{
    position _p = this->getPosition();
//    ((QGraphicsTextItem *)graphicComponents[1])->setPlainText("(" + QString::number(_p.x) + "," + QString::number(_p.y) + ")");
}
