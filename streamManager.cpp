#include "streamManager.h"
#include "qgraphicsitem.h"
#include "scenemanager.h"
#include "utils.h"
#include <climits>

#define dir(x) (x>=0)?1:-1

extern ConnProtocol::_tcpClass TcpConnProtocol;

position setIncr(position P1, position P2, int r)
{
    position P3 = P2 - P1;
    return {P3.x * r / 100, P3.y * r / 100};
}
streamManager::~streamManager()
{
}

streamManager::streamManager(Node *A, Node *B,int _rate) : rate(_rate), prevDist(static_cast<float>(INTMAX_MAX))
{
//    qDebug()<<"About to check consent of both nodes";
    if(A->Sender.connectToStream(this) and B->Receiver.connectToStream(this))
    {
        qDebug()<<"Checked consent of both nodes";
        A->updateStreamMap(B,this);
        this->destination = &B->Receiver;

        tag = A->Name + "=>" + B->Name;

        //Graphical portion
        auto &context = sceneManager::globalContext;
        auto &scene = sceneManager::globalScene;

        protocol = &TcpConnProtocol;                //Sometime later : implement protocols

        auto
                &s = A->QtComponent->getPosition(),
                &e = B->QtComponent->getPosition();

        QtComponent = new QtStreamComponent(s.x, s.y,&packetQueue);
        this->QtComponent->setContext(context, scene);

        this->QtComponent->startPos = s;
        this->QtComponent->endPos = e;

//        qDebug()<<"s : ("<<this->QtComponent->startPos.x<<","<<this->QtComponent->startPos.y<<")\n";
//        qDebug()<<"e : ("<<this->QtComponent->endPos.x<<","<<this->QtComponent->endPos.y<<")\n";

        this->QtComponent->displacement = utils::getDistance(s, e);
        this->QtComponent->direction = utils::getDirection(s,e);

        //Get image dimensions of packetComponent and centrify incr rate
        auto pImgSize = QPixmap(":/res/img/packet.png").size();
        this->QtComponent->incr = setIncr(
                    utils::centrifiedPosition(s,pImgSize),
                    utils::centrifiedPosition(e,pImgSize),
                    rate);

        sceneManager::registerActiveElement(this);
    }
    //    qDebug()<<"After if block";
    else
        delete this;
}

void streamManager::pushToStream(packet *p)
{
    auto s = QPixmap(":/res/img/packet.png").size();

    p->QtComponent->setPosition(utils::centrifiedPosition(QtComponent->startPos,s));
    p->render();
    sceneManager::registerWidget(p->QtComponent);
    this->packetQueue.push_back(p);
}
void streamManager::animate()
{
    for(packet* _packet: this->packetQueue)
    {
        _packet->QtComponent->animate();
    }
}

void streamManager::render()
{
    this->QtComponent->draw();
}

void streamManager::trigger()
{
    if(not packetQueue.empty())
    {
//        qDebug()<<"triggering Streamcomponent of stream"<<this->tag.c_str();
        packet *p = packetQueue.front();

//        float P = utils::getDistance(QtComponent->endPos,p->QtComponent->getPosition());

        std::string pQ;
        for(const packet *eachPacket:packetQueue) //for_each .. iterCount++
        {
            eachPacket->QtComponent->iterCount++;
            pQ+=std::to_string(eachPacket->QtComponent->iterCount) + " ";
        }
//        qDebug()<<"pQ(iterCount list) : "<<pQ.c_str();

        //        if(P > prevDist)
        if(p->QtComponent->iterCount / 2 > INCR_UNIT) //
        {
//            qDebug()<<"iterCount of the current packet "<<p->QtComponent->iterCount;
            if(destination->receive(p))
            {
                qDebug()<<"Kill a packet";
                sceneManager::unRegisterWidget(p->QtComponent);

                packetQueue.erase(packetQueue.begin());
                packetQueue.shrink_to_fit();

                delete p;
            }
            else
                qDebug()<<tag.back()<<" rejected packet";
        }
//        prevDist = P;
    }
}

//To mitigate forward declarations
void sender::emitPacket(streamManager *X)
{
    if (not storage->empty())
    {
        qDebug()<<"Ready to emit packets";
        NODE_DATA p = *(storage->end() - 1);
        storage->pop_back();
        packet *_packet = new packet(p);

        auto &qtX = X->QtComponent;
        _packet->QtComponent->disp = utils::getIncr(qtX->startPos,qtX->endPos);

        auto f_it = std::find(stream.begin(),stream.end(),X);
        if (f_it != stream.end())
            (*f_it)->pushToStream(_packet);
        else
            qDebug() << "No Valid stream found\n";
    }
}
bool receiver::receive(packet* x)
{
    if (storage->size() < *capacity)
    {
        qDebug()<<"Pushing data to storage";
        storage->push_back(x->data);
        return true;
    }
    else
        return false;
}

QtStreamComponent::QtStreamComponent(float x, float y,PACKET_QUEUE* _parentPacketQueue) : QtWidget(x, y)
{
//    qDebug()<<"Creating stream component\n";
    parentPacketQueue = _parentPacketQueue;

    //Dummy values
    startPos = position({0,0});
    endPos = position({5,5});
    incr = position({1,1});
    displacement = 0.05;

    sceneManager::registerWidget(this);
}

QtStreamComponent::~QtStreamComponent()
{

}

void QtStreamComponent::draw()
{
    auto &s = this->startPos , &e =this->endPos;

//    qDebug() << "Drawing stream at "
//              << "(" << s.x << "," << s.y << ")"<< ":(" << e.x << "," << e.y << ")\n";


    QBrush brush = Qt::SolidPattern;
    if(startPos.x > endPos.x)
        brush.setColor(QColor(238,24,33));

    QGraphicsLineItem* streamGraphic = scene->addLine(s.x,s.y,e.x,e.y,QPen(brush,1));
    this->addGraphicComponent(streamGraphic);

    QPixmap picture;
    picture.load(":/res/img/direction.png");
    auto picSize=picture.size();

    QGraphicsPixmapItem
            *startArrow = scene->addPixmap(picture),
            *endArrow = scene->addPixmap(picture);

    position newStartPos = utils::centrifiedPosition(startPos + (incr * 5),picSize);
    startArrow->setPos(newStartPos.x, newStartPos.y);
//    startArrow->setRotation(45);

//    position newEndPos = utils::centrifiedPosition(endPos - (incr * 5),picSize);
    position newEndPos = utils::centrifiedPosition(startPos + (incr * (INCR_UNIT * 4)),picSize);
    endArrow->setPos(newEndPos.x, newEndPos.y);
//    endArrow->setRotation(45);

    qDebug()<<"After setting, rotation = "<<startArrow->rotation();

    this->addGraphicComponent(startArrow);
    this->addGraphicComponent(endArrow);

    this->getContext()->setScene(scene);
}

void QtStreamComponent::animate()
{
    for(packet *p : *parentPacketQueue)
    {
        position pos = p->QtComponent->getPosition();
        pos = pos + incr;

//        pos.x = abs(pos.x);
//        pos.y = abs(pos.y);

        p->QtComponent->setPosition(pos);
    }
}
