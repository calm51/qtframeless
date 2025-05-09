#ifndef TITLEBARLIKEANDROID_H
#define TITLEBARLIKEANDROID_H

#include <QWidget>

#include "../qtframeless_global.h"

namespace Ui {
class TitlebarLikeAndroid;
}


NAMESPACE_QTFRAMELESS_BEGIN

class QtFrameless;

namespace TB {

class QTFRAMELESS_EXPORT TitlebarLikeAndroid : public QWidget {
    Q_OBJECT

  public:
    explicit TitlebarLikeAndroid(QtFrameless *qfl, QWidget *parent = nullptr);
    ~TitlebarLikeAndroid();

    QtFrameless *qfl = nullptr;

    void load();
    void loadthemesetting();

    int border_top_width = 1;
    int border_bottom_width = 1;
    QList<QColor> colors = {

        QColor(241, 241, 241), // all buttom background color

        QColor(167, 240, 178), // border top color
        QColor(238, 238, 238)  // border bottom color

    };

    Ui::TitlebarLikeAndroid *ui;

  private:
    QWidget *_window = nullptr;
    void load_round4(bool en = true);
};


} // namespace TB

NAMESPACE_QTFRAMELESS_END
#endif // TITLEBARLIKEANDROID_H
