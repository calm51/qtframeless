#ifndef TOOLBUTTON_RIGHTTOP_WINDOWS10_H
#define TOOLBUTTON_RIGHTTOP_WINDOWS10_H

#include <QObject>
#include <QPaintEvent>
#include <QPixmap>
#include <QToolButton>
#include <QWidget>

#include "../qtframeless_global.h"

NAMESPACE_QTFRAMELESS_BEGIN

namespace RIGHTTOP_TYPE {
enum __type__ { Minimize = 1, Maximize = 2, Close = 3 };
};

class QTFRAMELESS_EXPORT ToolButton_righttop_windows10 : public QToolButton {
    Q_OBJECT
  public:
    ToolButton_righttop_windows10(QWidget *parent = nullptr);
    ~ToolButton_righttop_windows10();

    RIGHTTOP_TYPE::__type__ type = RIGHTTOP_TYPE::__type__::Minimize;

    QPixmap p;
    bool _is_most_right = false;

    void paintEvent(QPaintEvent *event);
};

NAMESPACE_QTFRAMELESS_END

#endif // TOOLBUTTON_RIGHTTOP_WINDOWS10_H
