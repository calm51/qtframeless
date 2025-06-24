#pragma execution_character_set("utf-8")

#include "widget_background.h"

#include <qtgllogger/qtgllogger.h>

#include <QPainter>
#include <QStyleOption>

NAMESPACE_QTFRAMELESS_BEGIN

Widget_background::Widget_background(QWidget *parent)
    : QWidget{parent} {}

Widget_background::~Widget_background() {
    QGL_DEBUG("析构");
}


void Widget_background::paintEvent(QPaintEvent *) {
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

NAMESPACE_QTFRAMELESS_END
