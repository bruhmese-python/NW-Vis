#ifndef QTCONTEXT_H
#define QTCONTEXT_H

#include <QMainWindow>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class QtContext; }
QT_END_NAMESPACE

class QtContext : public QMainWindow
{
    Q_OBJECT

public:
    QtContext(QWidget *parent = nullptr);
    ~QtContext();

private:
    Ui::QtContext *ui;
    QTimer *timer;

private slots:
    void updateFunction();

    void on_rateSlider_sliderMoved(int position);
    void on_playButton_clicked();
};
#endif // QTCONTEXT_H
