#pragma execution_character_set("utf-8")

#include "titlebarlikeandroid.h"

#include "ui_titlebarlikeandroid.h"

#include <qpainter.h>
#include "../qtframeless.h"
#include <QPointer>
//#include "__frameless_include__.h"

#include <qtgllogger/qtgllogger.h>

#ifdef Q_OS_WIN
#include <windows.h>
#endif

NAMESPACE_QTFRAMELESS_BEGIN


namespace TB {


TitlebarLikeAndroid::TitlebarLikeAndroid(QtFrameless *qfl, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TitlebarLikeAndroid) {
    ui->setupUi(this);
    this->qfl = qfl;
    this->_window = parent;

    connect(&GLOBAL.SIGNAL, &__signal__::themeChanged, this, [=]() { this->loadthemesetting(); });
}

TitlebarLikeAndroid::~TitlebarLikeAndroid() {
    QGL_DEBUG("析构");
    delete ui;
}


void TitlebarLikeAndroid::load() {
    //    this->setFixedHeight(qfl.titlebar_MinimumHeight);
    load_round4();
}

void TitlebarLikeAndroid::loadthemesetting() {
    border_top_width = GLOBAL.theme_config[GLOBAL.currentTheme].border_top_width;
    border_bottom_width = GLOBAL.theme_config[GLOBAL.currentTheme].border_bottom_width;
    colors = GLOBAL.theme_config[GLOBAL.currentTheme].TitlebarLikeAndroid_colors;

    load();
}

void TitlebarLikeAndroid::load_round4(bool en) {
    if (en) {
        ui->widget->setStyleSheet(QString(R"(
QWidget#widget{
border:0px solid rgb(0,0,0);
border-top-width:%1px;
border-bottom-width:%2px;
border-top-color:%3;
border-bottom-color:%4;
border-top-left-radius:0px; border-top-right-radius:0px;
}
        )")
                                      .arg(QString::number(border_top_width),
                                           QString::number(border_bottom_width),
                                           GLOBAL.qcolor2qss(colors.at(1)),
                                           GLOBAL.qcolor2qss(colors.at(2)))); // .arg(QString::number(qfl->round4.at(0)),QString::number(qfl->round4.at(1)))
        // ui->widget_4->setContentsMargins(0,border_top_width,0,border_bottom_width);
    } else {
        ui->widget->setStyleSheet(QString(R"(
QWidget#widget{
border:0px solid rgb(0,0,0);
border-top-width:%1px;
border-bottom-width:%2px;
border-top-color:%3;
border-bottom-color:%4;
border-top-left-radius:0px; border-top-right-radius:0px;
}
        )")
                                      .arg(QString::number(border_top_width), QString::number(border_bottom_width), GLOBAL.qcolor2qss(colors.at(1)), GLOBAL.qcolor2qss(colors.at(2))));
    }
}

} // namespace TB

NAMESPACE_QTFRAMELESS_END
