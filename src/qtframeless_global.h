#pragma execution_character_set("utf-8")
#ifndef QTFRAMELESS_GLOBAL_H
#define QTFRAMELESS_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(QTFRAMELESS_LIBRARY)
#define QTFRAMELESS_EXPORT Q_DECL_EXPORT
#else
#define QTFRAMELESS_EXPORT Q_DECL_IMPORT
#endif


// ===================================================


//#include <stdio.h>
//#include <iostream>
//#include <stdlib.h>
//#include <string.h>
//#include <stdlib.h>
//using namespace std;

#include <qdatetime.h>
#include <qdir.h>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qmutex.h>
#include <qthreadpool.h>
#include <quuid.h>
#include <QApplication>
#include <QCoreApplication>
#include <QDebug>
#include <QDialog>
#include <QJsonArray>
#include <QList>
#include <QMainWindow>
#include <QProcess>
#include <QSize>
#include <QTextStream>
#include <QThread>
#include <QThreadPool>
#include <QTime>
#include <QTimer>
#include <QTranslator>
#include <QtWidgets/qfileiconprovider.h>

#include <qlist.h>
#include <QColor>
#include <QMap>
#include <QString>
#include <QtGlobal>


#if defined Q_OS_ANDROID
#include "QtAndroid"
#include <QAndroidJniEnvironment>
#include <QAndroidJniObject>
#include <QtAndroidExtras>
#endif

//Q_DECLARE_METATYPE(std::function<void()>)


#define NAMESPACE_QTFRAMELESS_BEGIN namespace QFL {
#define NAMESPACE_QTFRAMELESS_END }

#if defined(QTFRAMELESS_LIBRARY)
#include "qtgllogger_obj.h"
#endif

NAMESPACE_QTFRAMELESS_BEGIN


#ifndef _FRAMELESS_THEME_ENUM
#define _FRAMELESS_THEME_ENUM
namespace Theme {
extern QTFRAMELESS_EXPORT const QString &Light;
extern QTFRAMELESS_EXPORT const QString &Dark;
extern QTFRAMELESS_EXPORT const QString &White;
extern QTFRAMELESS_EXPORT const QString &Gray;
} // namespace Theme
#endif // _FRAMELESS_THEME_ENUM

namespace OS {
extern QTFRAMELESS_EXPORT const QString &WIN;
extern QTFRAMELESS_EXPORT const QString &ANDROID_;
extern QTFRAMELESS_EXPORT const QString &LINUX;
extern QTFRAMELESS_EXPORT const QString &OSX;
} // namespace OS

class QTFRAMELESS_EXPORT __signal__ : public QObject {
    Q_OBJECT
  public:
    __signal__();
  signals:
    //    void run(std::function<void()> c_fn);
    void themeChanged();
};

class QTFRAMELESS_EXPORT __slots__ : public QObject {
    Q_OBJECT
  public:
    __slots__();
  public slots:
    //    void run(std::function<void()> c_fn);
};

struct QTFRAMELESS_EXPORT __args__{};

struct QTFRAMELESS_EXPORT __themeconf__ {
    int shadow_size = 13;
    //    int resize_rect_out = 6;
    //    int resize_rect_in = 2;
    //    int resize_rect_corner = resize_rect_in+6;
    QColor shadow_color = QColor(140, 140, 140);
    QList<int> round4 = {6, 6, 3, 3};
    //    bool allow_showMaximized = true;
    //    bool allow_showMinimized = true;
    //    int titlebar_MinimumHeight = round4.at(0)>round4.at(1)?round4.at(0):round4.at(1);
    int border_width = 0;
    QList<QColor> Frameless_colors = {
        QColor(241, 241, 241), // titlebar background color
        QColor(255, 255, 255), // central widget background color

        QColor(160, 160, 160) // border color
    };

    // ==============================================

    int border_top_width = 1;
    int border_bottom_width = 1;
    //    QList<bool> hide_button = {false,false,false};
    QList<QColor> TitlebarLikeWindows10_colors = {

        QColor(241, 241, 241), // all button background color

        QColor(228, 228, 228),      // button hover background color
        QColor(212, 212, 212),      // button pressed background color
        QColor(102, 102, 102, 200), // button color
        QColor(0, 0, 0),            // button hover pressed color

        QColor(232, 30, 47),     // close button hover background color
        QColor(236, 118, 128),   // close button pressed background color
        QColor(30, 30, 30, 200), // close button color
        QColor(255, 255, 255),   // close button hover pressed color

        QColor(167, 240, 178), // border top color  167,240,178 170,224,240
        QColor(238, 238, 238)  // border bottom color

    };

    // ==============================================

    QList<QColor> TitlebarLikeAndroid_colors = {

        QColor(241, 241, 241), // all button background color

        QColor(167, 240, 178), // border top color
        QColor(238, 238, 238)  // border bottom color

    };
};

// __declspec(dllexport) __declspec(dllimport)
class QTFRAMELESS_EXPORT __global__ {
  public:
    __global__();

    __args__ ARGS;
    __signal__ SIGNAL;
    __slots__ SLOTS;
    QMap<QString, __themeconf__> theme_config;
    QString currentTheme = "Light";
    void swithTheme(const QString &key);
    inline const __themeconf__ &currentThemeConf() { return theme_config[currentTheme]; }

    void signal_connect_slots();

    QString qcolor2qss(const QColor &c);

#if defined Q_OS_WIN
    QString os = "WIN";
#elif defined Q_OS_ANDROID
    QString os = "ANDROID";
#elif defined Q_OS_LINUX
    QString os = "LINUX";
#elif defined Q_OS_OSX
    QString os = "OSX";
#else
    QString os = "";
#endif
};

extern __global__ &GLOBAL;
QTFRAMELESS_EXPORT __global__ &G();


NAMESPACE_QTFRAMELESS_END


#endif // QTFRAMELESS_GLOBAL_H
