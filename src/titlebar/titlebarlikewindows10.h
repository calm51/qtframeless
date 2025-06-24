#pragma execution_character_set("utf-8")

#ifndef TITLEBARLIKEWINDOWS10_H
#define TITLEBARLIKEWINDOWS10_H

#include <qtoolbutton.h>
#include <QResizeEvent>
#include <QWidget>

#include "../custom/toolbutton_righttop_windows10.h"

#include "../qtframeless_global.h"

#include <QGridLayout>


namespace Ui {
class TitlebarLikeWindows10;
}

NAMESPACE_QTFRAMELESS_BEGIN

class QtFrameless;

namespace TB {

class QTFRAMELESS_EXPORT TitlebarLikeWindows10 : public QWidget {
    Q_OBJECT

  public:
    explicit TitlebarLikeWindows10(QtFrameless *qfl, const int &buttonFixeWidth = 40, QWidget *parent = nullptr);
    ~TitlebarLikeWindows10();

    void resizeEvent(QResizeEvent *event);
    bool eventFilter(QObject *watched, QEvent *event);

    QtFrameless *qfl = nullptr;
    void load();
    void loadthemesetting();

    int border_top_width = 1;
    int border_bottom_width = 1;
    QList<bool> hide_button = {false, false, false};
    QList<QColor> colors = {

        QColor(241, 241, 241), // all buttom background color

        QColor(228, 228, 228),      // buttom hover background color
        QColor(212, 212, 212),      // buttom pressed background color
        QColor(102, 102, 102, 200), // buttom color
        QColor(0, 0, 0),            // buttom hover pressed color

        QColor(232, 30, 47),     // close buttom hover background color
        QColor(236, 118, 128),   // close buttom pressed background color
        QColor(30, 30, 30, 200), // close buttom color
        QColor(255, 255, 255),   // close buttom hover pressed color

        QColor(167, 240, 178), // border top color
        QColor(238, 238, 238)  // border bottom color

    };

    Ui::TitlebarLikeWindows10 *ui;

    QHBoxLayout *w3layout();
    QHBoxLayout *w6layout();
    void set_title(const QString &s);
    void set_title_enable(const bool &b);
    void set_title_fontsize(const int &pt);


  private:
    QWidget *_window = nullptr;
    void draw(ToolButton_righttop_windows10 *button);
    void load_round4(bool en = true);
};

} // namespace TB

NAMESPACE_QTFRAMELESS_END
#endif // TITLEBARLIKEWINDOWS10_H
