#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H
#include "Node.h"
#include "QtWidget.h"
#include "streamManager.h"

#define maxNodeCount 25

namespace sceneManager
{
    extern std::vector<QtWidget *> _QtWidgetChildren;

    extern std::vector<Node *> _activeNodes;
    extern std::vector<streamManager *> _activeStreams;

    extern std::vector<std::vector<position>> spawnPoints;
    extern QGraphicsView * globalContext;
    extern QGraphicsScene *globalScene;

    extern int animRate;
    extern bool scenePlay;

    void _init_(QGraphicsView *,QGraphicsScene *);

    void registerWidget(QtWidget*);
    void unRegisterWidget(QtWidget*);

    void registerActiveElement(Node *);
    void registerActiveElement(streamManager *);
    void unRegisterActiveElement(Node *);
    void unRegisterActiveElement(streamManager *);

    void renderAll();
    void mainLoop();

};

#endif // SCENEMANAGER_H
