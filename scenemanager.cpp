#include "scenemanager.h"
#include "qgraphicsitem.h"

 std::vector<QtWidget *> sceneManager::_QtWidgetChildren;

 std::vector<Node *> sceneManager::_activeNodes;
 std::vector<streamManager *> sceneManager::_activeStreams;

 std::vector<std::vector<position>> sceneManager::spawnPoints;
 QGraphicsView * sceneManager::globalContext;
 QGraphicsScene * sceneManager::globalScene;

 int sceneManager::animRate = 100;
 bool sceneManager::scenePlay = true;

 void sceneManager::registerWidget(QtWidget* _widget)
 {
       _QtWidgetChildren.push_back(_widget);
 }
 void sceneManager::unRegisterWidget(QtWidget* _widget)
 {
     auto &q = sceneManager::_QtWidgetChildren;
     auto f_it = std::find(q.begin(),q.end(),_widget);
     if(f_it != q.end())
     {
         qDebug()<<"Removing packet from scene";
         for(auto &s : (*f_it)->graphicComponents)
            globalScene->removeItem(s);
         q.erase(f_it);
     }
 }


 void sceneManager::registerActiveElement(Node * _node)
 {
     _activeNodes.push_back(_node);
     registerWidget(_node->QtComponent);
//     qDebug()<<"Registered active Element ("<<_activeNodes.size()<<")";
 }
 void sceneManager::registerActiveElement(streamManager * _node)
 {
     _activeStreams.push_back(_node);
     registerWidget(_node->QtComponent);
//     qDebug()<<"Registered active Element ("<<_activeStreams.size()<<")";
 }


 void sceneManager::unRegisterActiveElement(Node *_node)
 {
     auto &q = sceneManager::_activeNodes;
     auto f_it = std::find(q.begin(),q.end(),_node);
     delete *f_it;
     if(f_it != q.end())
        q.erase(f_it);
     unRegisterWidget(_node->QtComponent);
 }
 void sceneManager::unRegisterActiveElement(streamManager *_node)
 {
     auto &q = sceneManager::_activeStreams;
     auto f_it = std::find(q.begin(),q.end(),_node);
     delete *f_it;
     if(f_it != q.end())
        q.erase(f_it);
     unRegisterWidget(_node->QtComponent);
 }

 void sceneManager::renderAll()
 {
//     qDebug()<<"renderAll\n--------";
     for(QtWidget* child:_QtWidgetChildren)
         child->draw();
     for(Node* node:_activeNodes)                                           //Bring all nodes to front
         for ( QGraphicsItem* each :node->QtComponent->graphicComponents)
             each->setZValue(1);
//     qDebug()<<"rendered widgets";

 }
 void sceneManager::mainLoop()
 {
     if(scenePlay)
     {
         for(streamManager * stream:_activeStreams)
            stream->trigger();
         for(Node * node:_activeNodes)
             node->trigger();
         for(QtWidget* child:_QtWidgetChildren)
             child->animate();
     }
//     globalScene->removeItem(_QtWidgetChildren[0]->graphicComponents[0]); Testing deletion of graphic
 }

 void sceneManager::_init_(QGraphicsView *context, QGraphicsScene *scene)
 {
     sceneManager::globalContext = context;
     sceneManager::globalScene = scene;

     auto xincr = scene->width() / maxNodeCount;
     auto yincr = scene->height() / maxNodeCount;

     spawnPoints.clear();

     for(size_t i=0;i<maxNodeCount;i++)
     {
         std::vector<position> temp;
         for(size_t j=0;j<maxNodeCount;j++)
             temp.push_back(position({static_cast<float>(i*xincr),static_cast<float>(j*yincr)}));
         spawnPoints.push_back(temp);
     }
 }


