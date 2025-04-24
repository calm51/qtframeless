#pragma execution_character_set("utf-8")

#include "./qtframeless_global.h"


#include <qdir.h>
#include <QDir>
#include <QFile>
#include <QIODevice>

NAMESPACE_QTFRAMELESS_BEGIN


const QString &OS::WIN = "WIN";
const QString &OS::ANDROID_ = "ANDROID";
const QString &OS::LINUX = "LINUX";
const QString &OS::OSX = "OSX";


__global__::__global__() {
    this->theme_config.insert("Light", *(new __themeconf__()));

    this->theme_config.insert("Dark", *(new __themeconf__()));
    this->theme_config["Dark"].border_top_width = 1;
    this->theme_config["Dark"].border_bottom_width = 0;
    this->theme_config["Dark"].shadow_color = QColor(10, 10, 10); // QColor(32,32,32);
    this->theme_config["Dark"].Frameless_colors.replace(0, QColor(36, 47, 61));
    this->theme_config["Dark"].Frameless_colors.replace(1, QColor(23, 33, 43));
    this->theme_config["Dark"].Frameless_colors.replace(2, QColor(16, 25, 33));
    this->theme_config["Dark"].TitlebarLikeWindows10_colors.replace(0, QColor(36, 47, 61));
    this->theme_config["Dark"].TitlebarLikeWindows10_colors.replace(1, QColor(44, 56, 71));
    this->theme_config["Dark"].TitlebarLikeWindows10_colors.replace(2, QColor(49, 60, 76));
    this->theme_config["Dark"].TitlebarLikeWindows10_colors.replace(3, QColor(137, 157, 175));
    this->theme_config["Dark"].TitlebarLikeWindows10_colors.replace(4, QColor(224, 224, 224));
    //    this->theme_config["Dark"].TitlebarLikeWindows10_colors.replace(5,QColor(233,37,57));
    this->theme_config["Dark"].TitlebarLikeWindows10_colors.replace(6, QColor(147, 29, 39));
    this->theme_config["Dark"].TitlebarLikeWindows10_colors.replace(7, QColor(137, 157, 175));
    //    this->theme_config["Dark"].TitlebarLikeWindows10_colors.replace(8,QColor(36,47,61));
    this->theme_config["Dark"].TitlebarLikeWindows10_colors.replace(9, QColor(46, 72, 54));
    this->theme_config["Dark"].TitlebarLikeWindows10_colors.replace(10, QColor(16, 25, 33));
    this->theme_config["Dark"].TitlebarLikeAndroid_colors.replace(0, QColor(36, 47, 61));
    this->theme_config["Dark"].TitlebarLikeAndroid_colors.replace(1, QColor(46, 72, 54));
    this->theme_config["Dark"].TitlebarLikeAndroid_colors.replace(2, QColor(16, 25, 33));


    this->theme_config.insert("White", *(new __themeconf__()));
    this->theme_config["White"].border_top_width = 1;
    this->theme_config["White"].border_bottom_width = 0;
    this->theme_config["White"].Frameless_colors.replace(0, QColor(255, 255, 255));
    this->theme_config["White"].Frameless_colors.replace(1, QColor(240, 240, 240));
    this->theme_config["White"].Frameless_colors.replace(2, QColor(170, 170, 170));
    this->theme_config["White"].TitlebarLikeWindows10_colors.replace(0, QColor(255, 255, 255));
    this->theme_config["White"].TitlebarLikeWindows10_colors.replace(1, QColor(232, 232, 232));
    this->theme_config["White"].TitlebarLikeWindows10_colors.replace(2, QColor(216, 216, 216));
    this->theme_config["White"].TitlebarLikeWindows10_colors.replace(3, QColor(80, 80, 80));
    //    this->theme_config["White"].TitlebarLikeWindows10_colors.replace(4,QColor(0,0,0));
    //    this->theme_config["White"].TitlebarLikeWindows10_colors.replace(5,QColor(233,37,57));
    this->theme_config["White"].TitlebarLikeWindows10_colors.replace(6, QColor(242, 124, 134));
    this->theme_config["White"].TitlebarLikeWindows10_colors.replace(7, QColor(60, 60, 60));
    //    this->theme_config["White"].TitlebarLikeWindows10_colors.replace(8,QColor(36,47,61));
    //    this->theme_config["White"].TitlebarLikeWindows10_colors.replace(9,QColor(64,64,64));
    //    this->theme_config["White"].TitlebarLikeWindows10_colors.replace(10,QColor(16,25,33));
    this->theme_config["White"].TitlebarLikeAndroid_colors.replace(0, QColor(255, 255, 255));
    //    this->theme_config["White"].TitlebarLikeAndroid_colors.replace(1,QColor(64,64,64));
    //    this->theme_config["White"].TitlebarLikeAndroid_colors.replace(2,QColor(16,25,33));


    this->theme_config.insert("Gray", *(new __themeconf__()));
    this->theme_config["Gray"].border_top_width = 1;
    this->theme_config["Gray"].border_bottom_width = 0;
    this->theme_config["Gray"].shadow_color = QColor(10, 10, 10); // QColor(32,32,32);
    this->theme_config["Gray"].Frameless_colors.replace(0, QColor(44, 44, 44));
    this->theme_config["Gray"].Frameless_colors.replace(1, QColor(62, 62, 62));
    this->theme_config["Gray"].Frameless_colors.replace(2, QColor(34, 34, 34));
    this->theme_config["Gray"].TitlebarLikeWindows10_colors.replace(0, QColor(44, 44, 44));
    this->theme_config["Gray"].TitlebarLikeWindows10_colors.replace(1, QColor(62, 62, 62));
    this->theme_config["Gray"].TitlebarLikeWindows10_colors.replace(2, QColor(72, 72, 72));
    this->theme_config["Gray"].TitlebarLikeWindows10_colors.replace(3, QColor(190, 190, 190));
    this->theme_config["Gray"].TitlebarLikeWindows10_colors.replace(4, QColor(220, 220, 220));
    //    this->theme_config["Gray"].TitlebarLikeWindows10_colors.replace(5,QColor(233,37,57));
    this->theme_config["Gray"].TitlebarLikeWindows10_colors.replace(6, QColor(147, 29, 39));
    this->theme_config["Gray"].TitlebarLikeWindows10_colors.replace(7, QColor(200, 200, 200));
    //    this->theme_config["Gray"].TitlebarLikeWindows10_colors.replace(8,QColor(36,47,61));
    this->theme_config["Gray"].TitlebarLikeWindows10_colors.replace(9, QColor(64, 64, 64));
    this->theme_config["Gray"].TitlebarLikeWindows10_colors.replace(10, QColor(16, 25, 33));
    this->theme_config["Gray"].TitlebarLikeAndroid_colors.replace(0, QColor(44, 44, 44));
    this->theme_config["Gray"].TitlebarLikeAndroid_colors.replace(1, QColor(64, 64, 64));
    this->theme_config["Gray"].TitlebarLikeAndroid_colors.replace(2, QColor(16, 25, 33));
}

void __global__::swithTheme(const QString &key) {
    if (this->theme_config.contains(key)) {
        currentTheme = key;
        emit this->SIGNAL.themeChanged();
    }
}

__signal__::__signal__()
    : QObject() {}
__slots__::__slots__()
    : QObject() {}
//void __slots__::run(std::function<void()> c_fn){
//    c_fn();
//}


void __global__::signal_connect_slots() {
    //    GLOBAL.SIGNAL.connect(&GLOBAL.SIGNAL,&__signal__::run, &GLOBAL.SLOTS,&__slots__::run);
}

QString __global__::qcolor2qss(const QColor &c) {
    int r, g, b, a;
    c.getRgb(&r, &g, &b, &a);
    return QString(R"(rgba(%1,%2,%3,%4))").arg(r).arg(g).arg(b).arg(a);
}

__global__ &GLOBAL = *new __global__();
__global__ &G() {
    return GLOBAL;
}

NAMESPACE_QTFRAMELESS_END


const QString &QFL::Theme::Light = "Light";
const QString &QFL::Theme::Dark = "Dark";
const QString &QFL::Theme::White = "White";
const QString &QFL::Theme::Gray = "Gray";
