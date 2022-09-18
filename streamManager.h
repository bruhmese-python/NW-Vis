#ifndef STREAMMANAGER_H
#define STREAMMANAGER_H

#pragma once

#include "QtWidget.h"
#include "packet.h"
#include <math.h>
#include <queue>
#include "Node.h"
#include "ConnProtocol.h"

using PACKET_QUEUE = std::vector<packet *>;

class QtStreamComponent : public QtWidget
{
public:
    position startPos, endPos;
    position incr;
    float displacement;
    float direction;
    PACKET_QUEUE* parentPacketQueue;

    QtStreamComponent(float x, float y,PACKET_QUEUE*);
    ~QtStreamComponent();

    void draw() override;
    void animate() override;
};
class streamManager
{
private:
    ConnProtocol::ConnProtocol *protocol;
    PACKET_QUEUE packetQueue;
    receiver *destination;

    int rate;
    float prevDist;
public:
    QtStreamComponent *QtComponent;

    streamManager(Node *, Node *,int _rate = 1);
    ~streamManager();

    void pushToStream(packet *);
    void animate();
    void render();
    void trigger();

    std::string tag;
};

#endif
