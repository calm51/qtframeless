#pragma execution_character_set("utf-8")

#ifndef WIDGET_RUBBERBAND_POP_H
#define WIDGET_RUBBERBAND_POP_H

#include <QWidget>


#include "../qtframeless_global.h"

NAMESPACE_QTFRAMELESS_BEGIN

class Widget_rubberband_pop : public QWidget
{
    Q_OBJECT
public:
    explicit Widget_rubberband_pop(QWidget *parent = nullptr);

    ~Widget_rubberband_pop();

    static QMap<int, QPixmap> caches;

    QPixmap pixmap;
    QTimer timer;

    int step1 = 0;

    void paintEvent(QPaintEvent *event) override;

    void start();
    void stop();

signals:

};

NAMESPACE_QTFRAMELESS_END

#endif // WIDGET_RUBBERBAND_POP_H
