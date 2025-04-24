#include "toolbutton_righttop_windows10.h"

#include <QPainter>
#include <QPen>

#include <qtgllogger/qtgllogger.h>

NAMESPACE_QTFRAMELESS_BEGIN

ToolButton_righttop_windows10::ToolButton_righttop_windows10(QWidget *parent)
    : QToolButton(parent) {}

void ToolButton_righttop_windows10::paintEvent(QPaintEvent *event) {
    QToolButton::paintEvent(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::RenderHint::SmoothPixmapTransform);
    painter.drawPixmap(0, 0, p);
}

ToolButton_righttop_windows10::~ToolButton_righttop_windows10() {
    QGL_DEBUG("析构");
}


NAMESPACE_QTFRAMELESS_END
