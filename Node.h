#ifndef NODE_H
#define NODE_H

#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <memory>
#include <stack>
#include "QtWidget.h"
#include "itemproperties.h"
#include "packet.h"
#include "utils.h"

#pragma once

class streamManager;
class QtNodeComponent : public QtWidget
{
public:
    std::string *Name;
    int blinked;

    QtNodeComponent(float x, float y);
    ~QtNodeComponent() {}

    void draw() override;
    void animate() override;
    void blink();
};

class Node;

struct sender
{
    std::vector<streamManager *> stream; //The main list of streams to keep track of
    std::vector<NODE_DATA> *storage;
    size_t* capacity;

    void emitPacket(streamManager *);
    void connectToStorage(Node *, size_t &);
    bool connectToStream(streamManager *);
};
struct receiver
{
    std::vector<streamManager *> stream;//A list of streams to validate incoming packets with
    std::vector<NODE_DATA> *storage;
    size_t* capacity;

    bool receive(packet*);
    void connectToStorage(Node *, size_t &);
    bool connectToStream(streamManager *);
};

class Node
{
private:
    std::string Name;
    std::map<Node *, streamManager *> stream;

    size_t capacity;

    receiver Receiver;
    sender Sender;

    void lazySend();
    void updateStreamMap(Node *,streamManager*);
    void updateProperties();

    size_t prevSize;
public:
    QtNodeComponent *QtComponent;
    std::vector<NODE_DATA> storage;
    PROPERTIES properties;

    Node(std::string _name = utils::newNodeName());
    ~Node();

    void putPackets(size_t n = 1, int data =  0);
    void trigger();
    friend streamManager;
};

#endif
