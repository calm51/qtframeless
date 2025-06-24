#pragma execution_character_set("utf-8")

#ifndef UTILS_H
#define UTILS_H

#include "./qtframeless.h"
#include "./qtframeless_global.h"
#include "./titlebar/titlebarlikeandroid.h"
#include "./titlebar/titlebarlikewindows10.h"

NAMESPACE_QTFRAMELESS_BEGIN


template<typename T3, typename T4>
QPair<QtFrameless *, T3 *> template_windows10(T4 &w, const qint32 &titlebar_MinimumHeight, const qint32 &button_MinimumWidth = 40, const qint32 &titlebar_FixedHeight = -1) {
    QtFrameless *qfl = new QtFrameless(&w);
    qfl->titlebar_MinimumHeight = titlebar_MinimumHeight; // 24-40 28-44 28-46
    qfl->init(w);
    qfl->loadthemesetting();
    w.qfl = qfl;
#ifndef Q_OS_ANDROID
    TB::TitlebarLikeWindows10 *titlebar = new TB::TitlebarLikeWindows10(qfl, button_MinimumWidth, &w);
    titlebar->loadthemesetting();
    titlebar->setWindowIcon(w.windowIcon());
    qfl->set_titlebar(*titlebar);
#else
    TB::TitlebarLikeAndroid *titlebar = new TB::TitlebarLikeAndroid(qfl, &w);
    titlebar->loadthemesetting();
    titlebar->setFixedHeight(qfl->titlebar_MinimumHeight);
    qfl->set_titlebar(*titlebar);
#endif
    if (titlebar_FixedHeight > -1) {
        titlebar->setFixedHeight(titlebar_FixedHeight);
    }
#ifndef Q_OS_ANDROID
    qfl->resizeto();
#endif
    QPair<QtFrameless *, T3 *> pair;
    pair.first = qfl;
    pair.second = titlebar;
    return pair;
}


#ifndef _FRAMELESS_TEMPLATE_TOPLINE_COLOR
#define _FRAMELESS_TEMPLATE_TOPLINE_COLOR
namespace TopLineColor {
enum Flag { Red = 1, Purple = 2, Green = 3, Blue = 4 };
}
#endif // _FRAMELESS_TEMPLATE_TOPLINE_COLOR

template<typename T3>
void template_topline_color(QPair<QtFrameless *, T3 *> &qfl_tb, const TopLineColor::Flag &tlc) {
    __global__ &fg = G();
    QColor color;
    if (tlc == TopLineColor::Purple) {
        if (fg.is_light()) {
            color = QColor(219, 170, 240);
        } else if (fg.currentTheme == Theme::Gray) {
            color = QColor(82, 54, 95);
        }
    } else if (tlc == TopLineColor::Red) {
        if (fg.currentTheme == Theme::Light) {
            color = QColor(240, 170, 170);
        } else if (fg.currentTheme == Theme::Gray) {
            color = QColor(72, 49, 49);
        }
    }
#ifdef Q_OS_ANDROID
    qfl_tb.second->colors[1] = color;
#else
    qfl_tb.second->colors[9] = color;
#endif

    qfl_tb.second->load();
}


template<typename T1, typename T2>
void template_dialog_tocenter(T1 *dialog, T2 *parent) {
    if (parent) {
        if (parent->window()) {
            parent = parent->window();
        }
        const QRect &parentGeometry = parent->geometry();
        const QRect &childGeometry = dialog->geometry();
        const int &x = parentGeometry.x() + (parentGeometry.width() - childGeometry.width()) / 2;
        const int &y = parentGeometry.y() + (parentGeometry.height() - childGeometry.height()) / 2;
        dialog->move(x, y);
    }
}


NAMESPACE_QTFRAMELESS_END
#endif // UTILS_H
