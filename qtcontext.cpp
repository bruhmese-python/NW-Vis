#include "qtcontext.h"
#include "ui_qtcontext.h"

#include "Node.h"
#include "scenemanager.h"
#include "streamManager.h"
#include <QDebug>

QtContext::QtContext(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::QtContext)
{
    ui->setupUi(this);

    auto _geometry = ui->Canvas->geometry();
    QGraphicsScene *scene = new QGraphicsScene(0, 0,/*_geometry.width()*/640,/*_geometry.height()*/480);

    sceneManager::_init_(ui->Canvas,scene);

    //Only edit this code for visualization
    Node* l = new Node();
    Node* m = new Node();
    Node* n = new Node();
    Node* o = new Node();
    Node* p = new Node();
    Node* q = new Node();
    streamManager* s = new streamManager(l,m);
    streamManager* t = new streamManager(m,n);
    streamManager* u = new streamManager(n,l);
    streamManager* v = new streamManager(n,o);
    streamManager* w = new streamManager(m,p);
    streamManager* x = new streamManager(p,q);
    streamManager* y = new streamManager(q,o);
    l->putPackets(10);
    n->putPackets(2);
    p->putPackets(5);

    sceneManager::renderAll();

    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(updateFunction()));
    timer->start(100);
}

QtContext::~QtContext()
{
    delete ui;
}

void QtContext::updateFunction()
{
    sceneManager::mainLoop();
    this->timer->setInterval(sceneManager::animRate);
//    qDebug()<<"Update..";
//    qDebug()<<"sceneManager::_QtWidgetChildren : "<<sceneManager::_QtWidgetChildren.size();
}

void QtContext::on_rateSlider_sliderMoved(int position)
{
    auto r = ui->rateSlider;
    int newRate =  r->maximum() - r->value();
    sceneManager::animRate = newRate;
}


void QtContext::on_playButton_clicked()
{
    auto _this = ui->playButton;
    if(_this->text() == "▐▐")
    {
        _this->setText("⏵");
        sceneManager::scenePlay = false;
    }
    else
    {
        _this->setText("▐▐");
        sceneManager::scenePlay = true;
    }
}

