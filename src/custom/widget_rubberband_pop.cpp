#include "widget_rubberband_pop.h"

#include <QPaintEvent>
#include <QPainter>
#include <QStyleOption>

NAMESPACE_QTFRAMELESS_BEGIN

QMap<int, QPixmap> Widget_rubberband_pop::caches;

Widget_rubberband_pop::Widget_rubberband_pop(QWidget *parent)
    : QWidget{parent}{
    setWindowFlags(Qt::FramelessWindowHint |Qt::WindowTransparentForInput|Qt::WindowDoesNotAcceptFocus| Qt::Tool);
    setAttribute(Qt::WA_ShowWithoutActivating);
    setAttribute(Qt::WA_TransparentForMouseEvents);
    setAttribute(Qt::WA_TranslucentBackground);
    //    setAttribute(Qt::WA_AlwaysStackOnTop);
    setFixedSize(92,92);

    pixmap = QPixmap(this->size());
    pixmap.fill(Qt::transparent);

    timer.setInterval(15);
    connect(&timer,&QTimer::timeout,this,[=](){
        step1++;

        if (caches.contains(step1)){
            pixmap = caches.value(step1);

            if (step1==9){
                step1 = 0;
                timer.stop();
                this->setVisible(false);
            }

            update();

            return;
        }

        pixmap = QPixmap(this->size());
        pixmap.fill(Qt::transparent);

        QPainter painter(&pixmap);
        painter.setWindow(0,0,128,128);
        painter.translate(64,64);
        painter.setPen(Qt::NoPen);

        QRadialGradient radialGradient(0,0,64,0,0);

        if (step1==1){
            radialGradient.setColorAt(0,Qt::transparent);
            radialGradient.setColorAt(0.05,QColor(0,128,255,64));
            radialGradient.setColorAt(0.1,QColor(255,255,255,128));
            radialGradient.setColorAt(0.15,QColor(0,128,255,64));
            radialGradient.setColorAt(0.18,Qt::transparent);
        }else if (step1==2){
            radialGradient.setColorAt(0,QColor(255,255,255,128));
            radialGradient.setColorAt(0.1,QColor(0,128,255,64));
            radialGradient.setColorAt(0.15,QColor(255,255,255,144));
            radialGradient.setColorAt(0.20,QColor(0,128,255,64));
            radialGradient.setColorAt(0.23,Qt::transparent);
        }else if (step1==3){
            radialGradient.setColorAt(0,QColor(255,255,255,144));
            radialGradient.setColorAt(0.18,QColor(0,128,255,64));
            radialGradient.setColorAt(0.22,QColor(255,255,255,155));
            radialGradient.setColorAt(0.27,QColor(0,128,255,64));
            radialGradient.setColorAt(0.32,Qt::transparent);
        }else if (step1==4){
            radialGradient.setColorAt(0,QColor(255,255,255,155));
            radialGradient.setColorAt(0.25,QColor(0,128,255,64));
            radialGradient.setColorAt(0.3,QColor(255,255,255,155));
            radialGradient.setColorAt(0.35,QColor(0,128,255,64));
            radialGradient.setColorAt(0.4,Qt::transparent);
        }else if (step1==5){
            radialGradient.setColorAt(0,QColor(255,255,255,166));
            radialGradient.setColorAt(0.3,QColor(0,128,255,64));
            radialGradient.setColorAt(0.35,QColor(0,128,255,64));
            radialGradient.setColorAt(0.4,QColor(255,255,255,155));
            radialGradient.setColorAt(0.45,QColor(0,128,255,64));
            radialGradient.setColorAt(0.5,Qt::transparent);
        }else if (step1==6){
            radialGradient.setColorAt(0,QColor(255,255,255,144));
            radialGradient.setColorAt(0.3,QColor(0,128,255,64));
            radialGradient.setColorAt(0.35,QColor(0,128,255,64));
            radialGradient.setColorAt(0.45,QColor(255,255,255,122));
            radialGradient.setColorAt(0.5,QColor(0,128,255,55));
            radialGradient.setColorAt(0.55,Qt::transparent);
        }else if (step1==7){
            radialGradient.setColorAt(0,QColor(255,255,255,122));
            radialGradient.setColorAt(0.3,QColor(0,128,255,64));
            radialGradient.setColorAt(0.35,QColor(0,128,255,64));
            radialGradient.setColorAt(0.55,QColor(255,255,255,100));
            radialGradient.setColorAt(0.6,QColor(0,128,255,44));
            radialGradient.setColorAt(0.65,Qt::transparent);
        }else if (step1==8){
            radialGradient.setColorAt(0,QColor(255,255,255,111));
            radialGradient.setColorAt(0.3,QColor(0,128,255,64));
            radialGradient.setColorAt(0.4,QColor(0,128,255,64));
            radialGradient.setColorAt(0.6,QColor(255,255,255,90));
            radialGradient.setColorAt(0.65,QColor(0,128,255,33));
            radialGradient.setColorAt(0.7,Qt::transparent);
        }else if (step1==9){
            radialGradient.setColorAt(0,Qt::transparent);
        }

        radialGradient.setColorAt(1.0,Qt::transparent);

        painter.setBrush(QBrush(radialGradient));
        painter.drawRect(-64,-64,2*(64),2*(64));

        painter.end();

        caches.insert(step1, pixmap);

        if (step1==9){
            step1 = 0;
            timer.stop();
            this->setVisible(false);
        }

        update();
    });

}

Widget_rubberband_pop::~Widget_rubberband_pop(){
    stop();
}

void Widget_rubberband_pop::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.drawPixmap(0,0,pixmap);
}

void Widget_rubberband_pop::start(){
    stop();
    timer.start();
    this->setVisible(true);

}

void Widget_rubberband_pop::stop(){
    timer.stop();
    step1 = 0;
    this->setVisible(false);
}



NAMESPACE_QTFRAMELESS_END
