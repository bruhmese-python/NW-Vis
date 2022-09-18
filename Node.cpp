#include "Node.h"
#include "QtWidget.h"
#include "qgraphicsscene.h"
#include "utils.h"
#include "scenemanager.h"

#include "QRandomGenerator"
#include <QGraphicsPixmapItem>

void Node::lazySend() //Send next packet to any (randomized) connected node
{
//    qDebug()<<"Stream size : "<<Sender.stream.size();
    if(not stream.empty())
    {
        int i = QRandomGenerator::global()->bounded(0,static_cast<int>(stream.size()));
        qDebug()<<"Chose "<<i<<"th(st) stream : "<<Sender.stream[i]->tag.c_str();
        Sender.emitPacket(Sender.stream[i]);
    }
}

void Node::updateStreamMap(Node * N, streamManager *s_m)
{
    stream.insert(std::make_pair(N,s_m));
}

void Node::updateProperties()
{
    properties["Name"] = Name;
    properties["Capacity"] = std::to_string(capacity);
    properties["Connected Streams"] = std::to_string(stream.size());
    properties["Packets held"] = std::to_string(storage.size());
}

Node::Node(std::string _name)
{
    Name = _name;
    prevSize = 0;

    auto &context = sceneManager::globalContext;
    auto &scene = sceneManager::globalScene;

    QRect dimensions = context->geometry();
    position _p = utils::randomPosition(dimensions.width(),dimensions.height());

    QtComponent = new QtNodeComponent(_p.x, _p.y);
    QtComponent->Name = &this->Name;
    QtComponent->setContext(context, scene);

    Receiver.connectToStorage(this, capacity);
    Sender.connectToStorage(this, capacity);


    sceneManager::registerActiveElement(this);
}

Node::~Node()
{
}

void Node::putPackets(size_t n, int data)
{
    while(n--)
        Receiver.storage->push_back(data);
}

void Node::trigger()
{
    qDebug()<<"Triggered node";
    if(storage.size() > prevSize)
    {
        prevSize = storage.size();
        QtComponent->blink();
    }
    else if(storage.size() == 0)
        prevSize = 0;

    if(QtComponent->blinked > 0)
        QtComponent->blinked++;

    lazySend();
}

QtNodeComponent::QtNodeComponent(float x, float y): QtWidget(x, y)
{
    blinked = 0;
}

void QtNodeComponent::draw()
{
    position _p = this->getPosition();

    QPixmap picture;
    picture.load(":/res/img/node.png");

    QPointF centPos(utils::centrified(_p,picture.size()));         // to  change center of origin to image center
    QGraphicsPixmapItem* nodePixMap = scene->addPixmap(picture);
    nodePixMap->setPos(centPos);

    QPointF midPos(( -centPos.x() + _p.x)/1.5 + centPos.x() ,( -centPos.y() + _p.y)/2 + centPos.y());
    QGraphicsTextItem* nodeNameText = scene->addText(QString(*(this->Name)->c_str()));
    nodeNameText->setPos(midPos);
    nodePixMap->setAcceptHoverEvents(true);

    this->addGraphicComponent(nodePixMap);
    this->addGraphicComponent(nodeNameText);

//    qDebug() << "Drawing node at "
//              << "(" << _p.x << "," << _p.y << ")\n";

    this->getContext()->setScene(scene);
}
void QtNodeComponent::animate()
{
    if(blinked > 10)
    {
        QPixmap img(":/res/img/node.png");
        ((QGraphicsPixmapItem *)graphicComponents[0])->setPixmap(img);
        blinked = 0;
    }

}

void QtNodeComponent::blink()
{
    QPixmap imgTouch(":/res/img/nodeTouched.png");
    ((QGraphicsPixmapItem *)graphicComponents[0])->setPixmap(imgTouch);
    blinked++;
}

void sender::connectToStorage(Node * _Node, size_t &_capacity)
{
    this->storage=&_Node->storage;
    this->capacity=&_capacity;
}
bool sender::connectToStream(streamManager * _stream)
{
    qDebug()<<"Connecting to Stream";
    //only to validate outgoing connections
    if(not false) // check protocol connections
    //        then connect
    {
        this->stream.push_back(_stream);

        qDebug()<<"Connected";
        return true;
    }
    return false;
}
void receiver::connectToStorage(Node * _Node, size_t &_capacity)
{
    this->storage=&_Node->storage;
    this->capacity=&_capacity;
}
bool receiver::connectToStream(streamManager *_stream)
{
    qDebug()<<"Connecting to Stream";
    //only to accept incoming connections
    if(not false) // check protocol connections
    //        then connect
    {
        this->stream.push_back(_stream);
        qDebug()<<"Connected";
        return true;
    }
    return false;
}
