#ifndef PACKET_H
#define PACKET_H

#include "QtWidget.h"
#include <time.h>

using NODE_DATA = int;

class streamManager;

#pragma once

class QtPacketComponent : public QtWidget
{
public:
    position disp;
    size_t iterCount;

    QtPacketComponent(float x, float y);
    ~QtPacketComponent() {}

    void draw() override;
    void animate() override;
};

class packet
{
public:
    packet(NODE_DATA);
    ~packet();
    void render();

    QtPacketComponent *QtComponent;

    NODE_DATA data;

private:
    size_t size;
    int ID;
    time_t startTime, endTime;
};

#endif
