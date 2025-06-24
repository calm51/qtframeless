#pragma execution_character_set("utf-8")

#ifndef WIDGET_BACKGROUND_H
#define WIDGET_BACKGROUND_H

#include <QObject>
#include <QWidget>

#include "../qtframeless_global.h"

NAMESPACE_QTFRAMELESS_BEGIN

class QTFRAMELESS_EXPORT Widget_background : public QWidget {
    Q_OBJECT
  public:
    explicit Widget_background(QWidget *parent = nullptr);
    ~Widget_background();
    void paintEvent(QPaintEvent *);
  signals:
};

NAMESPACE_QTFRAMELESS_END

#endif // WIDGET_BACKGROUND_H
