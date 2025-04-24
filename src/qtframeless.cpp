#include "qtframeless.h"
#pragma execution_character_set("utf-8")
#include <qapplication.h>
#include "./custom/widget_background.h"
#include <QThread>

#include <qtoolbutton.h>

#include <QScreen>

#if defined(Q_OS_LINUX) && !defined(Q_OS_ANDROID)
#include "qx11info_x11.h"
#endif
#if defined(Q_OS_LINUX) && !defined(Q_OS_ANDROID)
#include <X11/Xlib.h>

#include <X11/Xutil.h>
#include <QX11Info>

#endif

#ifdef Q_OS_WIN
#include <windows.h>

#endif

void stackRubberUnderMain(QWidget *rubber, QWidget *mainWin) {
#ifdef Q_OS_WIN
    HWND rubberHwnd = (HWND) rubber->winId();
    HWND mainHwnd = (HWND) mainWin->winId();
    SetWindowPos(rubberHwnd, mainHwnd, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
#elif defined(Q_OS_LINUX) && !defined(Q_OS_ANDROID)

    Display *display = QX11Info::display();
    Window rubberWin = rubber->winId();
    Window mainWinId = mainWin->winId();

    Window windows[] = {mainWinId, rubberWin};
    XRestackWindows(display, windows, 2);

    XFlush(display);
#endif
}


#include <qtgllogger/qtgllogger.h>


#include <qtshadowcache/qtshadowcache.h>


//QCoreApplication::instance()
//QCoreApplication::notify();

NAMESPACE_QTFRAMELESS_BEGIN


QtFrameless::QtFrameless(QObject *parent)
    : QObject{parent} {
    connect(&GLOBAL.SIGNAL, &__signal__::themeChanged, this, [=]() { this->loadthemesetting(); });
}

QtFrameless::~QtFrameless() {
    //    DELETE_OBJ_IF_HAD_NEW(shadow);
    //    DELETE_OBJ_IF_HAD_NEW(titlebar_main_layout);
    //    DELETE_OBJ_IF_HAD_NEW(titlebar_main_widget);
    //    DELETE_OBJ_IF_HAD_NEW(central_main_layout);
    //    DELETE_OBJ_IF_HAD_NEW(central_main_widget);
    //    DELETE_OBJ_IF_HAD_NEW(background_main_layout);
    //    DELETE_OBJ_IF_HAD_NEW(background_main_widget);

    DELETE_OBJ_IF_HAD_NEW(rubberBand);
    DELETE_OBJ_IF_HAD_NEW(rubberBand_pop);

    QGL_DEBUG("析构");
}

void QtFrameless::app(QApplication &a) {}

void QtFrameless::_new_widget() {
    //    _window->setWindowFlag(Qt::X11BypassWindowManagerHint,true);
    _window->setWindowFlag(Qt::FramelessWindowHint);
    _window->setAttribute(Qt::WA_TranslucentBackground);
    _window->setAttribute(Qt::WA_StyledBackground);
    _window->setAttribute(Qt::WA_Hover);
    // _window->setMouseTracking(true);

    background_main_widget = new Widget_background(_window);
    background_main_widget->setObjectName(QString("background_main_widget"));


    central_main_widget = new QWidget(_window);
    central_main_widget->setObjectName(QString("central_main_widget"));


    titlebar_main_widget = new QWidget(_window);
    titlebar_main_widget->setObjectName(QString("titlebar_main_widget"));
    titlebar_main_widget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);

    menubar_main_widget = new QWidget(_window);
    menubar_main_widget->setObjectName(QString("menubar_main_widget"));
    menubar_main_widget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);

    bottombar_main_widget = new QWidget(_window);
    bottombar_main_widget->setObjectName(QString("bottombar_main_widget"));
    bottombar_main_widget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);

    background_main_layout = new QGridLayout(background_main_widget);
    background_main_layout->setSpacing(0);

    central_main_layout = new QGridLayout(central_main_widget);
    central_main_layout->setSpacing(0);

    titlebar_main_layout = new QGridLayout(titlebar_main_widget);
    titlebar_main_layout->setContentsMargins(0, 0, 0, 0);
    titlebar_main_layout->setSpacing(0);

    menubar_main_layout = new QGridLayout(menubar_main_widget);
    menubar_main_layout->setContentsMargins(0, 0, 0, 0);
    menubar_main_layout->setSpacing(0);

    bottombar_main_layout = new QGridLayout(bottombar_main_widget);
    bottombar_main_layout->setContentsMargins(0, 0, 0, 0);
    bottombar_main_layout->setSpacing(0);

    background_main_layout->addWidget(central_main_widget, 0, 0, 1, 1);

    central_main_layout->addWidget(titlebar_main_widget, 0, 0, 1, 1);
    central_main_layout->addWidget(menubar_main_widget, 1, 0, 1, 1);
    central_main_layout->addWidget(central_widget, 2, 0, 1, 1);
    central_main_layout->addWidget(bottombar_main_widget, 3, 0, 1, 1);

#ifndef Q_OS_ANDROID
    // shadow = new QGraphicsDropShadowEffect(central_main_widget);
    // central_main_widget->setGraphicsEffect(shadow);
#endif
}

void QtFrameless::create_shadow_image() {
    QSDC::Request request;
    request.parent = nullptr;
    request.window_radius_left_top = round4.at(0);
    request.window_radius_left_bottom = round4.at(2);
    request.window_radius_right_top = round4.at(1);
    request.window_radius_right_bottom = round4.at(3);
    request.blur_radius = shadow_size;
    request.shadow_color = shadow_color;
    request.background_color = colors.at(1);

    const QSDC::Result &result = QSDC::Qtshadowcache::create(request);
    // QGL_DEBUG(result.pixmap, result.cached, result.save2);
    if (!result.cached || !QFile::exists(result.save2)) {
        result.pixmap.save(result.save2);
    }
    background_main_widget->setStyleSheet(QString(R"(
#background_main_widget{
    border-width: %2px;
    border-style: solid;
    border-image: url(%1) %2 %2 %2 %2 stretch stretch;
}
)")
                                              .arg(result.save2, QString::number(result.margin)));
    this->shadow_margin = result.margin;
}

void QtFrameless::init(QMainWindow &w) {
    this->_window = &w;
    this->_window_size = this->_window->size();
    central_widget = w.centralWidget();
    _new_widget();
    w.setCentralWidget(background_main_widget);
#ifndef Q_OS_ANDROID
    w.installEventFilter(this);
#endif
}

void QtFrameless::init(QWidget &w) {}

void QtFrameless::init(QDialog &w) {
    this->_window = &w;
    this->_window_size = this->_window->size();
    central_layout = qobject_cast<QGridLayout *>(w.layout());
    if (central_layout) {
        //        auto lay = qobject_cast<QGridLayout*>(central_layout);
        //        lay->setContentsMargins(0,0,0,0);
        //        lay->setSpacing(0);
        //        central_widget = lay->itemAt(0)->widget();
        //        _new_widget();
        //        lay->removeWidget(central_widget);
        //        lay->addWidget(background_main_widget,0,0,1,1);

        central_widget = new QWidget();
        central_widget->setLayout(central_layout);
        central_layout = new QGridLayout();
        central_layout->setContentsMargins(0, 0, 0, 0);
        central_layout->setSpacing(0);
        _new_widget();
        auto lay = qobject_cast<QGridLayout *>(central_layout);
        lay->addWidget(background_main_widget, 0, 0, 1, 1);
        w.setLayout(central_layout);

    } else {
        central_layout = qobject_cast<QVBoxLayout *>(w.layout());
        if (central_layout) {
            auto lay = qobject_cast<QVBoxLayout *>(central_layout);
            lay->setContentsMargins(0, 0, 0, 0);
            lay->setSpacing(0);
            _new_widget();
            QGL_DEBUG(QGL_FAS(lay->count()));
            for (int var = 0; var < lay->count(); ++var) {
                auto item = lay->itemAt(var);
                central_widget = item->widget();
                if (central_widget) {
                    central_main_layout->addWidget(central_widget, var + 1, 0, 1, 1);
                } else {
                    central_main_layout->addLayout(item->layout(), var + 1, 0, 1, 1);
                }
                lay->removeItem(item);
            }
            lay->addWidget(background_main_widget, 0);
        }
    }

#ifndef Q_OS_ANDROID
    w.installEventFilter(this);
#endif
}

void QtFrameless::load() {
    if (titlebar_MinimumHeight <= 0) {
        titlebar_MinimumHeight = 4;
    }

    //    central_main_widget->setStyleSheet(QString(R"(
    //#central_main_widget{
    //background:%1;
    //border:solid; border-width:0px;
    //border-top-left-radius:%2px; border-top-right-radius:%3px;
    //border-bottom-left-radius:%4px; border-bottom-right-radius:%5px;
    //border:%6px solid %7;
    //})").arg(GLOBAL.qcolor2qss(colors.at(1)))
    //                                       .arg(QString::number(round4.at(0)),QString::number(round4.at(1)),QString::number(round4.at(2)),QString::number(round4.at(3)))
    //                                       .arg(QString::number(border_width),GLOBAL.qcolor2qss(colors.at(2))));
    //    titlebar_main_widget->setStyleSheet(QString(R"(
    //#titlebar_main_widget{
    //background:%1;
    //border:solid; border-width:0px;
    //border-top-left-radius:%2px; border-top-right-radius:%3px;
    //})").arg(GLOBAL.qcolor2qss(colors.at(0)))
    //                                        .arg(QString::number(round4.at(0)),QString::number(round4.at(1))));

    showNormal(true);

    titlebar_main_widget->setMinimumHeight(titlebar_MinimumHeight);

    central_main_layout->setContentsMargins(border_width, border_width, border_width, border_width);

    if (_window->isMaximized()) {
#ifndef Q_OS_ANDROID
        this->create_shadow_image();
#endif
    } else {
#ifndef Q_OS_ANDROID
        this->create_shadow_image();
        if (!rubberBand_delegated) {
            background_main_layout->setContentsMargins(shadow_margin, shadow_margin, shadow_margin, shadow_margin);
        }
#else
        background_main_layout->setContentsMargins(0, 0, 0, 0);
#endif
    }
}

void QtFrameless::loadthemesetting() {
    shadow_size = GLOBAL.currentThemeConf().shadow_size;
    //    resize_rect_out = GLOBAL.theme_config[GLOBAL.currentTheme].resize_rect_out;
    //    resize_rect_in = GLOBAL.theme_config[GLOBAL.currentTheme].resize_rect_in;
    //    resize_rect_corner = GLOBAL.theme_config[GLOBAL.currentTheme].resize_rect_corner;
    shadow_color = GLOBAL.theme_config[GLOBAL.currentTheme].shadow_color;
    round4 = GLOBAL.theme_config[GLOBAL.currentTheme].round4;
    //    allow_showMaximized = GLOBAL.theme_config[GLOBAL.currentTheme].allow_showMaximized;
    //    allow_showMinimized = GLOBAL.theme_config[GLOBAL.currentTheme].allow_showMinimized;
    //    titlebar_MinimumHeight = GLOBAL.theme_config[GLOBAL.currentTheme].titlebar_MinimumHeight;
    border_width = GLOBAL.theme_config[GLOBAL.currentTheme].border_width;
    colors = GLOBAL.theme_config[GLOBAL.currentTheme].Frameless_colors;

    load();

    //#ifdef Q_OS_ANDROID
    //    if (GLOBAL.currentTheme=="Dark"){
    //        QtAndroid::runOnAndroidThread([]() {
    //            QAndroidJniObject activity = QtAndroid::androidActivity();
    //            activity.callMethod<void>("toStatusbarTextWhite", "()V");
    //        });
    //    }else{
    //        QtAndroid::runOnAndroidThread([]() {
    //            QAndroidJniObject activity = QtAndroid::androidActivity();
    //            activity.callMethod<void>("toStatusbarTextBlack", "()V");
    //        });
    //    }
    //#endif
}

void QtFrameless::set_titlebar(QWidget &titlebar) {
    titlebar_widget = &titlebar;
    titlebar_main_layout->addWidget(&titlebar, 0, 0, 1, 1);
#ifndef Q_OS_ANDROID
    foreach (QWidget *wi, titlebar.findChildren<QToolButton *>()) {
        wi->installEventFilter(this);
    }
#endif
}


bool QtFrameless::is_inside_window(const QPoint &pos) {
    //    ____<<pos;
    //    const QPoint &localPoint = this->_window->mapFromGlobal(pos);
    //    ____<<localPoint;
    return this->_window->rect().contains(pos);
}

bool QtFrameless::is_titlebar(const QPoint &pos) {
    int x = pos.x();
    int y = pos.y();
#ifndef Q_OS_ANDROID
    if (!_window->isMaximized()) {
        if (rubberBand_delegated) {
            if (!(rubberBand_direction & Qt::Edge::LeftEdge)) {
                x -= shadow_margin;
            }
            if (!(rubberBand_direction & Qt::Edge::TopEdge) && rubberBand_direction != Qt::Edge::LeftEdge && rubberBand_direction != Qt::Edge::RightEdge
                && rubberBand_direction != Qt::Edge::BottomEdge) {
                y -= shadow_margin;
            }
        } else {
            x -= shadow_margin;
            y -= shadow_margin;
        }
    }
#endif
    if (x < 0 || y < 0) {
        return false;
    }
    if (y > titlebar_main_widget->height() || x > titlebar_main_widget->width()) {
        return false;
    }
    return true;
}

QPair<bool, Qt::Edges> QtFrameless::is_allow_resize(const QPoint &pos) {
    const int &_central_width = central_main_widget->width();
    const int &_central_height = central_main_widget->height();
    bool b = true;
    Qt::Edges n;
    // QGL_DEBUG(QGL_FAS(pos.x()), QGL_FAS(pos.y()), QGL_FAS(_central_width), QGL_FAS(_central_height), QGL_FAS(shadow_margin), QGL_FAS(resize_rect_out));
    if (_window->isMaximized() || m_isPressButton) {
        b = false;
        SET_CURSOR_IF_NOT_SPECIFIED(_window, Qt::ArrowCursor);
    } else if ((shadow_margin - resize_rect_out) < pos.x() && pos.x() < shadow_margin + resize_rect_in && (shadow_margin - resize_rect_out) < pos.y() && pos.y() < shadow_margin + resize_rect_in) {
        n = Qt::TopEdge | Qt::LeftEdge;
        if (rubberBand_delegated && !_rubberBand_allowed_resize_direction().contains(n)) {
            b = false;
            n = Qt::Edges();
            SET_CURSOR_IF_NOT_SPECIFIED(_window, Qt::ArrowCursor);
        } else {
            SET_CURSOR_IF_NOT_SPECIFIED(_window, Qt::SizeFDiagCursor);
        }
    } else if (_central_width + (rubberBand_delegated ? 0 : shadow_margin) - resize_rect_in < pos.x() && pos.x() < _central_width + (rubberBand_delegated ? 0 : shadow_margin) + resize_rect_out
               && (shadow_margin - resize_rect_out) < pos.y() && pos.y() < shadow_margin + resize_rect_in) {
        n = Qt::TopEdge | Qt::RightEdge;
        if (rubberBand_delegated && !_rubberBand_allowed_resize_direction().contains(n)) {
            b = false;
            n = Qt::Edges();
            SET_CURSOR_IF_NOT_SPECIFIED(_window, Qt::ArrowCursor);
        } else {
            SET_CURSOR_IF_NOT_SPECIFIED(_window, Qt::SizeBDiagCursor);
        }
    } else if ((shadow_margin - resize_rect_out) < pos.x() && pos.x() < shadow_margin + resize_rect_in && _central_height + (rubberBand_delegated ? 0 : shadow_margin) - resize_rect_in < pos.y()
               && pos.y() < _central_height + (rubberBand_delegated ? 0 : shadow_margin) + resize_rect_out) {
        n = Qt::BottomEdge | Qt::LeftEdge;
        if (rubberBand_delegated && !_rubberBand_allowed_resize_direction().contains(n)) {
            b = false;
            n = Qt::Edges();
            SET_CURSOR_IF_NOT_SPECIFIED(_window, Qt::ArrowCursor);
        } else {
            SET_CURSOR_IF_NOT_SPECIFIED(_window, Qt::SizeBDiagCursor);
        }
    } else if (_central_width + (rubberBand_delegated ? 0 : shadow_margin) - resize_rect_in < pos.x() && pos.x() < _central_width + (rubberBand_delegated ? 0 : shadow_margin) + resize_rect_out
               && _central_height + (rubberBand_delegated ? 0 : shadow_margin) - resize_rect_in < pos.y() && pos.y() < _central_height + (rubberBand_delegated ? 0 : shadow_margin) + resize_rect_out) {
        n = Qt::BottomEdge | Qt::RightEdge;
        if (rubberBand_delegated && !_rubberBand_allowed_resize_direction().contains(n)) {
            b = false;
            n = Qt::Edges();
            SET_CURSOR_IF_NOT_SPECIFIED(_window, Qt::ArrowCursor);
        } else {
            SET_CURSOR_IF_NOT_SPECIFIED(_window, Qt::SizeFDiagCursor);
        }
    } else if ((shadow_margin - resize_rect_out) < pos.x() && pos.x() < shadow_margin + resize_rect_in && shadow_margin < pos.y() && pos.y() < shadow_margin + _central_height) {
        if (pos.y() - shadow_margin < resize_rect_corner) {
            n = Qt::TopEdge | Qt::LeftEdge;
            if (rubberBand_delegated && !_rubberBand_allowed_resize_direction().contains(n)) {
                b = false;
                n = Qt::Edges();
                SET_CURSOR_IF_NOT_SPECIFIED(_window, Qt::ArrowCursor);
            } else {
                SET_CURSOR_IF_NOT_SPECIFIED(_window, Qt::SizeFDiagCursor);
            }
        } else if (_central_height - (pos.y() - (rubberBand_delegated ? 0 : shadow_margin)) < resize_rect_corner) {
            n = Qt::BottomEdge | Qt::LeftEdge;
            if (rubberBand_delegated && !_rubberBand_allowed_resize_direction().contains(n)) {
                b = false;
                n = Qt::Edges();
                SET_CURSOR_IF_NOT_SPECIFIED(_window, Qt::ArrowCursor);
            } else {
                SET_CURSOR_IF_NOT_SPECIFIED(_window, Qt::SizeBDiagCursor);
            }
        } else {
            n = Qt::LeftEdge;
            if (rubberBand_delegated && !_rubberBand_allowed_resize_direction().contains(n)) {
                b = false;
                n = Qt::Edges();
                SET_CURSOR_IF_NOT_SPECIFIED(_window, Qt::ArrowCursor);
            } else {
                SET_CURSOR_IF_NOT_SPECIFIED(_window, Qt::SizeHorCursor);
            }
        }
    } else if ((shadow_margin - resize_rect_out) < pos.y() && pos.y() < shadow_margin + resize_rect_in && shadow_margin < pos.x() && pos.x() < shadow_margin + _central_width) {
        if (pos.x() - shadow_margin < resize_rect_corner) {
            n = Qt::TopEdge | Qt::LeftEdge;
            if (rubberBand_delegated && !_rubberBand_allowed_resize_direction().contains(n)) {
                b = false;
                n = Qt::Edges();
                SET_CURSOR_IF_NOT_SPECIFIED(_window, Qt::ArrowCursor);
            } else {
                SET_CURSOR_IF_NOT_SPECIFIED(_window, Qt::SizeFDiagCursor);
            }
        } else if (_central_width - (pos.x() - (rubberBand_delegated ? 0 : shadow_margin)) < resize_rect_corner) {
            n = Qt::TopEdge | Qt::RightEdge;
            if (rubberBand_delegated && !_rubberBand_allowed_resize_direction().contains(n)) {
                b = false;
                n = Qt::Edges();
                SET_CURSOR_IF_NOT_SPECIFIED(_window, Qt::ArrowCursor);
            } else {
                SET_CURSOR_IF_NOT_SPECIFIED(_window, Qt::SizeBDiagCursor);
            }
        } else {
            n = Qt::TopEdge;
            if (rubberBand_delegated && !_rubberBand_allowed_resize_direction().contains(n)) {
                b = false;
                n = Qt::Edges();
                SET_CURSOR_IF_NOT_SPECIFIED(_window, Qt::ArrowCursor);
            } else {
                SET_CURSOR_IF_NOT_SPECIFIED(_window, Qt::SizeVerCursor);
            }
        }
    } else if (_central_height + (rubberBand_delegated ? 0 : shadow_margin) - resize_rect_in < pos.y() && pos.y() < _central_height + (rubberBand_delegated ? 0 : shadow_margin) + resize_rect_out
               && shadow_margin < pos.x() && pos.x() < shadow_margin + _central_width) {
        if (pos.x() - shadow_margin < resize_rect_corner) {
            n = Qt::BottomEdge | Qt::LeftEdge;
            if (rubberBand_delegated && !_rubberBand_allowed_resize_direction().contains(n)) {
                b = false;
                n = Qt::Edges();
                SET_CURSOR_IF_NOT_SPECIFIED(_window, Qt::ArrowCursor);
            } else {
                SET_CURSOR_IF_NOT_SPECIFIED(_window, Qt::SizeBDiagCursor);
            }
        } else if (_central_width - (pos.x() - (rubberBand_delegated ? 0 : shadow_margin)) < resize_rect_corner) {
            n = Qt::BottomEdge | Qt::RightEdge;
            if (rubberBand_delegated && !_rubberBand_allowed_resize_direction().contains(n)) {
                b = false;
                n = Qt::Edges();
                SET_CURSOR_IF_NOT_SPECIFIED(_window, Qt::ArrowCursor);
            } else {
                SET_CURSOR_IF_NOT_SPECIFIED(_window, Qt::SizeFDiagCursor);
            }
        } else {
            n = Qt::BottomEdge;
            if (rubberBand_delegated && !_rubberBand_allowed_resize_direction().contains(n)) {
                b = false;
                n = Qt::Edges();
                SET_CURSOR_IF_NOT_SPECIFIED(_window, Qt::ArrowCursor);
            } else {
                SET_CURSOR_IF_NOT_SPECIFIED(_window, Qt::SizeVerCursor);
            }
        }
    } else if (_central_width + (rubberBand_delegated && rubberBand_direction != Qt::Edge::BottomEdge ? 0 : shadow_margin) - resize_rect_in < pos.x()
               && pos.x() < _central_width + (rubberBand_delegated && rubberBand_direction != Qt::Edge::BottomEdge ? 0 : shadow_margin) + resize_rect_out && shadow_margin < pos.y()
               && pos.y() < shadow_margin + _central_height) {
        if (pos.y() - shadow_margin < resize_rect_corner) {
            n = Qt::TopEdge | Qt::RightEdge;
            if (rubberBand_delegated && !_rubberBand_allowed_resize_direction().contains(n)) {
                b = false;
                n = Qt::Edges();
                SET_CURSOR_IF_NOT_SPECIFIED(_window, Qt::ArrowCursor);
            } else {
                SET_CURSOR_IF_NOT_SPECIFIED(_window, Qt::SizeBDiagCursor);
            }
        } else if (_central_height - (pos.y() - (rubberBand_delegated ? 0 : shadow_margin)) < resize_rect_corner) {
            n = Qt::BottomEdge | Qt::RightEdge;
            if (rubberBand_delegated && !_rubberBand_allowed_resize_direction().contains(n)) {
                b = false;
                n = Qt::Edges();
                SET_CURSOR_IF_NOT_SPECIFIED(_window, Qt::ArrowCursor);
            } else {
                SET_CURSOR_IF_NOT_SPECIFIED(_window, Qt::SizeFDiagCursor);
            }
        } else {
            n = Qt::RightEdge;
            if (rubberBand_delegated && !_rubberBand_allowed_resize_direction().contains(n)) {
                b = false;
                n = Qt::Edges();
                SET_CURSOR_IF_NOT_SPECIFIED(_window, Qt::ArrowCursor);
            } else {
                SET_CURSOR_IF_NOT_SPECIFIED(_window, Qt::SizeHorCursor);
            }
        }
    } else {
        b = false;
        SET_CURSOR_IF_NOT_SPECIFIED(_window, Qt::ArrowCursor);
    }
    QPair<bool, Qt::Edges> bn;
    bn.first = b;
    bn.second = n;
    return bn;
}

void QtFrameless::set_menubar(QWidget &menubar) {
    //    menubar_widget = &menubar;
    menubar_main_layout->addWidget(&menubar, 0, 0, 1, 1);
    //#ifndef Q_OS_ANDROID
    //    foreach (QWidget*wi, menubar.findChildren<QToolButton*>()) {
    //        wi->installEventFilter(this);
    //    }
    //#endif
}

void QtFrameless::set_bottombar(QWidget &bottombar) {
    bottombar_main_layout->addWidget(&bottombar, 0, 0, 1, 1);
}

void QtFrameless::showMaximized(bool only_update_round) {
#ifdef Q_OS_ANDROID
    return;
#endif
    if (!allow_showMaximized) {
        return;
    }

    if (rubberBand_delegated) {
        this->showNormal(true);
        background_main_layout->setContentsMargins(shadow_margin, shadow_margin, shadow_margin, shadow_margin);
        rubberBand_delegated = false;
        rubberBand_lastRect_recordingtime = 0;
        this->_window->setGeometry(rubberBand_origin_size);
    }

    if (!only_update_round) {
        rectAfterMaximized = _window->geometry();
        // if (shadow) {
        //     shadow->setBlurRadius(0);
        //     delete shadow;
        //     shadow = nullptr;
        //     central_main_widget->setGraphicsEffect(nullptr);
        // }
        background_main_layout->setContentsMargins(0, 0, 0, 0);
        this->_window->showMaximized();
    }
    central_main_widget->setStyleSheet(QString(R"(
#central_main_widget{
background:%1;
border:solid; border-width:0px;
border-top-left-radius:0px; border-top-right-radius:0px;
border-bottom-left-radius:0px; border-bottom-right-radius:0px;
border:%6px solid %7;
})")
                                           .arg(GLOBAL.qcolor2qss(colors.at(1)))
                                           .arg(QString::number(border_width), GLOBAL.qcolor2qss(colors.at(2))));
    titlebar_main_widget->setStyleSheet(QString(R"(
#titlebar_main_widget{
background:%1;
border:solid; border-width:0px;
border-top-left-radius:0px; border-top-right-radius:0px;
})")
                                            .arg(GLOBAL.qcolor2qss(colors.at(0))));

    if (!only_update_round) {
        emit this->windowStateChanged(this->_window->windowState());
    }
}

void QtFrameless::showNormal(bool only_update_round) {
#ifndef Q_OS_ANDROID
    if (!only_update_round) {
        this->_window->showNormal();
    }
#endif
    const bool &_isMaximized = this->_window->isMaximized();
    central_main_widget->setStyleSheet(QString(R"(
#central_main_widget{
background:%1;
border:solid; border-width:0px;
border-top-left-radius:%2px; border-top-right-radius:%3px;
border-bottom-left-radius:%4px; border-bottom-right-radius:%5px;
border:%6px solid %7;
})")
                                           .arg(GLOBAL.qcolor2qss(colors.at(1)))
                                           .arg(QString::number(_isMaximized ? 0 : round4.at(0)),
                                                QString::number(_isMaximized ? 0 : round4.at(1)),
                                                QString::number(_isMaximized ? 0 : round4.at(2)),
                                                QString::number(_isMaximized ? 0 : round4.at(3)))
                                           .arg(QString::number(border_width), GLOBAL.qcolor2qss(colors.at(2))));
    titlebar_main_widget->setStyleSheet(QString(R"(
#titlebar_main_widget{
background:%1;
border:solid; border-width:0px;
border-top-left-radius:%2px; border-top-right-radius:%3px;
})")
                                            .arg(GLOBAL.qcolor2qss(colors.at(0)))
                                            .arg(QString::number(_isMaximized ? 0 : round4.at(0)), QString::number(_isMaximized ? 0 : round4.at(1))));
#ifndef Q_OS_ANDROID
    if (!only_update_round) {
        // shadow = new QGraphicsDropShadowEffect(central_main_widget);
        // shadow->setColor(shadow_color);
        // shadow->setOffset(0, 0);
        // shadow->setBlurRadius(shadow_size);
        // central_main_widget->setGraphicsEffect(shadow);
        // background_main_layout->setContentsMargins(shadow_size, shadow_size, shadow_size, shadow_size);
        this->create_shadow_image();
        background_main_layout->setContentsMargins(shadow_margin, shadow_margin, shadow_margin, shadow_margin);
    }
#endif

    if (!only_update_round) {
        emit this->windowStateChanged(this->_window->windowState());
    }
}

void QtFrameless::resizeto() {
#ifndef Q_OS_ANDROID
    _window->resize(_window_size.width() + (shadow_margin * 2),
                    _window_size.height() + (shadow_margin * 2) + titlebar_widget->height()); //
#endif
}


#if defined(Q_OS_LINUX) && !defined(Q_OS_ANDROID)
void QtFrameless::_x11_move_window(const WId &wid, const QPoint &pos) {
    XEvent event;
    memset(&event, 0, sizeof(XEvent));
    Display *display = QX11Info::display();
    event.xclient.type = ClientMessage;
    event.xclient.message_type = XInternAtom(display, "_NET_WM_MOVERESIZE", False);
    event.xclient.display = display;
    event.xclient.window = wid;
    event.xclient.format = 32;
    event.xclient.data.l[0] = pos.x();
    event.xclient.data.l[1] = pos.y();
    event.xclient.data.l[2] = 8;
    event.xclient.data.l[3] = Button1;
    event.xclient.data.l[4] = 1;
    XUngrabPointer(display, CurrentTime);
    XSendEvent(display, QX11Info::appRootWindow(QX11Info::appScreen()), False, SubstructureNotifyMask | SubstructureRedirectMask, &event);
    XFlush(display);
}

#endif

bool QtFrameless::eventFilter(QObject *watched, QEvent *event) {
    // QGL_DEBUG(watched, event);
#ifdef Q_OS_ANDROID
    return QObject::eventFilter(watched, event);
#endif

    //    if (!QList<QEvent::Type>({
    //                             QEvent::MouseButtonPress, QEvent::MouseMove ,QEvent::MouseButtonRelease,
    //                             QEvent::MouseButtonDblClick,
    //                             QEvent::HoverMove,QEvent::Move,QEvent::CursorChange,
    //                             QEvent::UpdateRequest,QEvent::Paint,
    //                             QEvent::LayoutRequest,QEvent::PolishRequest
    //}).contains(event->type())){
    //        ____<<watched<<event->type();
    //    }
    QWidget *_watched_widget = qobject_cast<QWidget *>(watched);
    const bool &_is_titlebar_child = _watched_widget && titlebar_widget && titlebar_widget->isAncestorOf(_watched_widget);
    if (watched == this->_window || _is_titlebar_child) {
        if (event->type() == QEvent::MouseButtonPress) {
            auto e = static_cast<QMouseEvent *>(event);
            if (_is_titlebar_child) {
                const QPoint &_titlebar_child_globalpos = _watched_widget->mapToGlobal(e->pos());
                const QPoint &_titlebar_child_topos = _window->mapFromGlobal(_titlebar_child_globalpos);
                if (e->button() == Qt::LeftButton) {
                    QPair<bool, Qt::Edges> _bn = is_allow_resize(_titlebar_child_topos);
                    if (_bn.first) {
                        e->ignore();
                        return true;
                    } else {
                        m_isPressButton = true;
                        return QObject::eventFilter(watched, event);
                    }
                } else {
                    return QObject::eventFilter(watched, event);
                }
            } else {
                if (e->button() == Qt::LeftButton) {
                    QPair<bool, Qt::Edges> _bn = is_allow_resize(e->pos());
                    if ((is_titlebar(e->pos()) || (movable_in_whole_window && watched == this->_window && is_inside_window(e->pos()))) && !_bn.first) {
                        //#ifdef Q_OS_LINUX
                        //                if (w->isMaximized()){
                        //                    shadow->setBlurRadius(shadow_size);
                        //                    background_main_layout->setContentsMargins(shadow_size,shadow_size,shadow_size,shadow_size);
                        //                }
                        //                w->windowHandle()->startSystemMove();
                        //                e->accept();
                        //                return true;
                        //#else
                        m_isPress = true;
                        m_pressPos = e->pos();
                        m_pressGlobalPos = e->globalPos();
                        m_pressWindowGlobalPos = _window->pos();
                        this->_window->setCursor(Qt::ArrowCursor);
                    } else if (_bn.first) {
                        m_isSystemResize = true;
                        _window->windowHandle()->startSystemResize(_bn.second);
                        e->accept();
                        return true;
                    }
                }
            }

            //            ____<< e << "鼠标按下";
        } else if (event->type() == QEvent::MouseMove) {
            if (_is_titlebar_child) {
                m_isPressButton = true;
                return QObject::eventFilter(watched, event);
            }
            auto e = static_cast<QMouseEvent *>(event);
            if (m_isPress) {
                if (_window->isMaximized()) {
                    QSize s = _window->size();
                    showNormal();
                    //                    shadow->setBlurRadius(shadow_size);
                    //                    w->showNormal();
                    //                    background_main_layout->setContentsMargins(shadow_size,shadow_size,shadow_size,shadow_size);

                    //#ifdef Q_OS_LINUX
                    //   qApp->processEvents();
                    //   QThread::msleep(200);
                    //#endif
                    //                    ____<<rectAfterMaximized<<w->geometry();
                    //                    w->setGeometry(-shadow_size+m_pressGlobalPos.x()-((double)m_pressGlobalPos.x() / s.width()*(rectAfterMaximized.width()-(shadow_size*2))),
                    //                                   m_pressGlobalPos.y()-((double)m_pressGlobalPos.y() / titlebar_main_widget->height()*titlebar_main_widget->height())-shadow_size,
                    //                                   rectAfterMaximized.width(),rectAfterMaximized.height());
                    int x = 0;
                    if (m_pressGlobalPos.x() < rectAfterMaximized.width() / 2) {
                        x = -shadow_margin;
                    } else if (s.width() - m_pressGlobalPos.x() < rectAfterMaximized.width() / 2) {
                        x = (s.width() - rectAfterMaximized.width()) + shadow_margin;
                    } else {
                        x = m_pressGlobalPos.x() - (rectAfterMaximized.width() / 2);
                    }
                    _window->setGeometry(x,
                                         m_pressGlobalPos.y() - ((double) m_pressGlobalPos.y() / titlebar_main_widget->height() * titlebar_main_widget->height()) - shadow_margin,
                                         rectAfterMaximized.width(),
                                         rectAfterMaximized.height());
                    m_pressPos = e->pos();
                    m_pressGlobalPos = e->globalPos();
                    m_pressWindowGlobalPos = _window->pos();

                    if (rubberBand) {
                        rubberBand_moved = false;
                        rubberBand_resized = false;
                        rubberBand->setVisible(false);
                        rubberBand_lastRect_recordingtime = 0;
                    }


                } else if (rubberBand_delegated) {
                    this->showNormal(true);
                    auto _margins = background_main_layout->contentsMargins();
                    background_main_layout->setContentsMargins(shadow_margin, shadow_margin, shadow_margin, shadow_margin);

                    rubberBand_delegated = false;
                    rubberBand_lastRect_recordingtime = 0;

                    rectAfterMaximized = rubberBand_origin_size;

                    QSize s = _window->size();
                    int x = 0;
                    if (qAbs(m_pressGlobalPos.x() - _window->x()) < rectAfterMaximized.width() / 2) {
                        x = _margins.left() == 0 ? (_window->x() - shadow_margin) : _window->x();
                    } else if (s.width() - qAbs(m_pressGlobalPos.x() - _window->x()) < rectAfterMaximized.width() / 2) {
                        x = _window->x() + (s.width() - rectAfterMaximized.width()) + (_margins.left() == 0 ? 0 : shadow_margin);
                    } else {
                        x = m_pressGlobalPos.x() - (rectAfterMaximized.width() / 2);
                    }
                    _window->setGeometry(x,
                                         _window->y() + m_pressGlobalPos.y() - ((double) m_pressGlobalPos.y() / titlebar_main_widget->height() * titlebar_main_widget->height())
                                             - (_margins.top() == 0 ? shadow_margin : 0),
                                         rectAfterMaximized.width(),
                                         rectAfterMaximized.height());

                    m_pressPos = e->pos();
                    m_pressGlobalPos = e->globalPos();
                    m_pressWindowGlobalPos = _window->pos();

                    rubberBand_moved = false;
                    rubberBand_resized = false;
                    rubberBand->setVisible(false);
                    rubberBand_lastRect_recordingtime = 0;

                } else {
#if defined(Q_OS_LINUX) && !defined(Q_OS_ANDROID)
                    _x11_move_window(_window->winId(), m_pressGlobalPos);
                    m_isPress = false;
#else
                    QPoint pos = m_pressWindowGlobalPos;
                    pos += e->globalPos() - m_pressGlobalPos;
                    _window->move(pos);
#endif
                }
            }
            //            ____ << e<< "鼠标移动";
        } else if (event->type() == QEvent::MouseButtonRelease) {
            auto e = static_cast<QMouseEvent *>(event);
            //            qDebug()<< e << "鼠标松开"<<_is_titlebar_child<<m_isPressButton;

            if (rubberBand_moved || rubberBand_resized) {
                if (rubberBand && rubberBand->isVisible()) {
                    rubberBand_animationgroup->stop();
                    if (rubberBand_direction == Qt::Edge::TopEdge) {
                        this->_window->move(rubberBand_origin_location.x(), rubberBand_origin_location.y());
                        this->showMaximized();
                    } else {
                        // rubberBand_origin_size = this->_window->geometry();
                        rubberBand_origin_size = rubberBand_origin_location;
                        QRect rect = rubberBand_animation->endValue().toRect();
                        if (rubberBand_direction == Qt::Edge::BottomEdge) {
                            rect.setLeft(rect.left() - shadow_margin);
                            rect.setRight(rect.right() + shadow_margin);
                            rubberBand_origin_size.setSize(rubberBand_begin_size);
                        }
                        // this->showMaximized(true);
                        if (rubberBand_direction == Qt::Edge::LeftEdge) {
                            rect.setRight(rect.right() + shadow_margin);
                            background_main_layout->setContentsMargins(0, 0, shadow_margin, 0);
                        } else if (rubberBand_direction == Qt::Edge::RightEdge) {
                            rect.setLeft(rect.left() - shadow_margin);
                            background_main_layout->setContentsMargins(shadow_margin, 0, 0, 0);
                        } else if (rubberBand_direction == Qt::Edge::BottomEdge) {
                            background_main_layout->setContentsMargins(shadow_margin, 0, shadow_margin, 0);
                        } else if (rubberBand_direction == (Qt::Edge::TopEdge | Qt::Edge::LeftEdge)) {
                            rect.setRight(rect.right() + shadow_margin);
                            rect.setBottom(rect.bottom() + shadow_margin);
                            background_main_layout->setContentsMargins(0, 0, shadow_margin, shadow_margin);
                        } else if (rubberBand_direction == (Qt::Edge::TopEdge | Qt::Edge::RightEdge)) {
                            rect.setLeft(rect.left() - shadow_margin);
                            rect.setBottom(rect.bottom() + shadow_margin);
                            background_main_layout->setContentsMargins(shadow_margin, 0, 0, shadow_margin);
                        } else if (rubberBand_direction == (Qt::Edge::BottomEdge | Qt::Edge::LeftEdge)) {
                            rect.setRight(rect.right() + shadow_margin);
                            rect.setTop(rect.top() - shadow_margin);
                            background_main_layout->setContentsMargins(0, shadow_margin, shadow_margin, 0);
                        } else if (rubberBand_direction == (Qt::Edge::BottomEdge | Qt::Edge::RightEdge)) {
                            rect.setLeft(rect.left() - shadow_margin);
                            rect.setTop(rect.top() - shadow_margin);
                            background_main_layout->setContentsMargins(shadow_margin, shadow_margin, 0, 0);
                        }

                        rubberBand_delegated = true;

                        this->_window->setGeometry(rect);
                    }
                }
            }
            if (rubberBand) {
                rubberBand_moved = false;
                rubberBand_resized = false;
                rubberBand->setVisible(false);
                rubberBand_lastRect_recordingtime = 0;
            }

            m_isSystemResize = false;

            if (_is_titlebar_child || m_isPressButton) {
                m_isPressButton = false;
                const QPoint &_titlebar_child_globalpos = _watched_widget->mapToGlobal(e->pos());
                const QPoint &_titlebar_child_topos = _window->mapFromGlobal(_titlebar_child_globalpos);
                if (e->button() == Qt::LeftButton) {
                    QPair<bool, Qt::Edges> _bn = is_allow_resize(_titlebar_child_topos);
                    if (_bn.first) {
                        e->ignore();
                        return true;
                    } else {
                        return QObject::eventFilter(watched, event);
                    }
                } else {
                    return QObject::eventFilter(watched, event);
                }
            }
            if (m_isPress) {
                m_isPress = false;
                this->_window->setCursor(Qt::ArrowCursor);
            }
            if (e->button() == Qt::RightButton) {
            }
            //            ____<< e << "鼠标松开";
        } else if (event->type() == QEvent::MouseButtonDblClick) {
            if (_is_titlebar_child) {
                return QObject::eventFilter(watched, event);
            }
            auto e = static_cast<QMouseEvent *>(event);
            if (e->button() == Qt::LeftButton) {
                QPair<bool, Qt::Edges> _bn = is_allow_resize(e->pos());
                if (_bn.first && _bn.second != Qt::TopEdge && _bn.second != Qt::BottomEdge) {
                } else if (_bn.first && (_bn.second == Qt::TopEdge || _bn.second == Qt::BottomEdge)) { // 单方向最大化

                    if (!rubberBand_enable[2]) {
                        return QObject::eventFilter(watched, event);
                    }

                    if (!rubberBand) {
                        new_rubberBand();
                    }

                    if (rubberBand_delegated) {
                        return QObject::eventFilter(watched, event);
                    }

                    if (!rubberBand_resized) {
                        rubberBand_begin_size = this->_window->size();
                        rubberBand_origin_location = this->_window->geometry();
                    }
                    rubberBand_resized = true;

                    QRect screenRect = this->_window->screen()->availableGeometry();
                    QRect rect = this->_window->frameGeometry();
                    rect.setLeft(rect.left() + shadow_margin);
                    rect.setRight(rect.right() - shadow_margin);
                    rect.setTop(rect.top() + shadow_margin);
                    rect.setBottom(rect.bottom() - shadow_margin);

                    if (true) {
                        rubberBand_direction = Qt::Edge::BottomEdge;
                        rubberBand_snapRect = screenRect;
                        rubberBand_snapRect.setLeft(rect.left());
                        rubberBand_snapRect.setRight(rect.right());
                        update_rubberBand(screenRect, rect);
                    }


                } else {
                    if (this->is_titlebar(e->pos())) {
                        if (_window->isMaximized()) {
                            showNormal();
                            //                        w->showNormal();
                            //                        shadow->setBlurRadius(shadow_size);
                            //                        background_main_layout->setContentsMargins(shadow_size,shadow_size,shadow_size,shadow_size);
                        } else {
                            //                        rectAfterMaximized = w->geometry();
                            //                        shadow->setBlurRadius(0);
                            //                        background_main_layout->setContentsMargins(0,0,0,0);
                            //                        w->showMaximized();
                            showMaximized();
                            is_allow_resize(e->pos());
                        }
                    }
                }
            }
            //            ____<< e << "鼠标双击";
        } else if (event->type() == QEvent::HoverMove) {
            auto e = static_cast<QHoverEvent *>(event);
            //            ____ << e<< "鼠标悬停";
            if (is_allow_resize(e->pos()).first) {
                //                ____<<"允许缩放11111111111111";
            } else {
                //                ____<<"不允许缩放2222222222222222";
            }
        } else if (event->type() == QEvent::Leave) {
            auto e = static_cast<QEvent *>(event);
            //            ____ << e<< "鼠标离开";
        } else if (event->type() == QEvent::ActivationChange) {
            auto e = static_cast<QEvent *>(event);
            //            ____ << e <<"ActivationChange";
        } else if (event->type() == QEvent::WindowDeactivate) {
            auto e = static_cast<QEvent *>(event);
            //            ____ << e<< "WindowDeactivate";
        } else if (event->type() == QEvent::WindowActivate) {
            auto e = static_cast<QEvent *>(event);
            //            ____ << e<< "WindowActivate";
        } else if (event->type() == QEvent::WindowStateChange) {
            QWindowStateChangeEvent *stateChangeEvent = static_cast<QWindowStateChangeEvent *>(event);
            //            ____<<"WindowStateChange"<<stateChangeEvent<<_window->isMaximized();
            if (_window->isMaximized()) {
                const auto &geometry = _window->geometry();
                if (geometry.x() != 0 && geometry.y() != 0) {
                    rectAfterMaximized = _window->geometry();
                }
                // if (shadow) {
                //     shadow->setBlurRadius(0);
                // }
                background_main_layout->setContentsMargins(0, 0, 0, 0);
                showMaximized(true);
            }
        } else if (event->type() == QEvent::Resize) {
            auto e = static_cast<QResizeEvent *>(event);

            if (!rubberBand_enable[1] && !rubberBand_enable[0]) {
                return QObject::eventFilter(watched, event);
            }

            if (!rubberBand) {
                new_rubberBand();
            }

            if (!m_isPress && !m_isPressButton && !m_isSystemResize) {
                return QObject::eventFilter(watched, event);
            }

            if (rubberBand_delegated) {
                return QObject::eventFilter(watched, event);
            }

            if (!rubberBand_resized) {
                rubberBand_begin_size = e->oldSize();
                rubberBand_origin_location = this->_window->geometry();
            }

            rubberBand_resized = true;

            if (!rubberBand_enable[1]) {
                return QObject::eventFilter(watched, event);
            }


            QRect screenRect = this->_window->screen()->availableGeometry();
            QRect rect = this->_window->frameGeometry();
            rect.setLeft(rect.left() + shadow_margin);
            rect.setRight(rect.right() - shadow_margin);
            rect.setTop(rect.top() + shadow_margin);
            rect.setBottom(rect.bottom() - shadow_margin);
            QPoint globalPos = QCursor::pos();


            if (screenRect.bottom() - globalPos.y() < 2 || globalPos.y() < 2) {
                rubberBand_direction = Qt::Edge::BottomEdge;
                rubberBand_snapRect = screenRect;
                rubberBand_snapRect.setLeft(rect.left());
                rubberBand_snapRect.setRight(rect.right());
                update_rubberBand(screenRect, rect);
            } else {
                rubberBand_animationgroup->stop();
                rubberBand->hide();
                rubberBand_direction = Qt::Edges();
            }

        } else if (event->type() == QEvent::Move) {
            auto e = static_cast<QMoveEvent *>(event);

            if (!rubberBand_enable[0]) {
                return QObject::eventFilter(watched, event);
            }

            if (!rubberBand) {
                new_rubberBand();
            }

            if (!m_isPress && !m_isPressButton && !m_isSystemResize) {
                return QObject::eventFilter(watched, event);
            }

            if (rubberBand_delegated) {
                return QObject::eventFilter(watched, event);
            }

            if (rubberBand_resized) {
                return QObject::eventFilter(watched, event);
            }

            if (!rubberBand_moved) {
                rubberBand_lastRect_recordingtime = 0;
                rubberBand_origin_location = this->_window->geometry();
            }

            rubberBand_moved = true;

            QRect screenRect = this->_window->screen()->availableGeometry();
            QRect rect = this->_window->frameGeometry();
            rect.setLeft(rect.left() + shadow_margin);
            rect.setRight(rect.right() - shadow_margin);
            rect.setTop(rect.top() + shadow_margin);
            rect.setBottom(rect.bottom() - shadow_margin);
            QPoint globalPos = QCursor::pos();


            int _accepted_corner_width = 30;
            if ((globalPos.y() < 2 && globalPos.x() < _accepted_corner_width) || (globalPos.y() < _accepted_corner_width && globalPos.x() < 2)) { // 左上
                if (!rubberBand->isVisible() || (rubberBand_direction & Qt::Edge::BottomEdge) == 0) {
                    if (rubberBand_direction != (Qt::Edge::LeftEdge | Qt::Edge::TopEdge)) {
                        rubberBand_direction = Qt::Edge::LeftEdge | Qt::Edge::TopEdge;
                        rubberBand_snapRect = screenRect;
                        rubberBand_snapRect.setTop(0);
                        rubberBand_snapRect.setLeft(0);
                        rubberBand_snapRect.setRight(screenRect.width() / 2);
                        rubberBand_snapRect.setBottom(screenRect.height() / 2);
                        rubberBand_lastRect_recordingtime = 0;
                        update_rubberBand(screenRect, rect);
                    }
                }
            } else if (screenRect.bottom() - globalPos.y() < _accepted_corner_width && globalPos.x() < 2) { // 左下
                if (!rubberBand->isVisible() || (rubberBand_direction & Qt::Edge::BottomEdge) == 0) {
                    if (rubberBand_direction != (Qt::Edge::LeftEdge | Qt::Edge::BottomEdge)) {
                        rubberBand_direction = Qt::Edge::LeftEdge | Qt::Edge::BottomEdge;
                        rubberBand_snapRect = screenRect;
                        rubberBand_snapRect.setTop(screenRect.height() / 2);
                        rubberBand_snapRect.setLeft(0);
                        rubberBand_snapRect.setRight(screenRect.width() / 2);
                        rubberBand_lastRect_recordingtime = 0;
                        update_rubberBand(screenRect, rect);
                    }
                }
            } else if ((globalPos.y() < 2 && screenRect.right() - globalPos.x() < _accepted_corner_width)
                       || (globalPos.y() < _accepted_corner_width && screenRect.right() - globalPos.x() < 2)) { // 右上
                if (!rubberBand->isVisible() || (rubberBand_direction & Qt::Edge::BottomEdge) == 0) {
                    if (rubberBand_direction != (Qt::Edge::RightEdge | Qt::Edge::TopEdge)) {
                        rubberBand_direction = Qt::Edge::RightEdge | Qt::Edge::TopEdge;
                        rubberBand_snapRect = screenRect;
                        rubberBand_snapRect.setTop(0);
                        rubberBand_snapRect.setLeft(screenRect.width() / 2);
                        rubberBand_snapRect.setBottom(screenRect.height() / 2);
                        rubberBand_lastRect_recordingtime = 0;
                        update_rubberBand(screenRect, rect);
                    }
                }
            } else if (screenRect.bottom() - globalPos.y() < _accepted_corner_width && screenRect.right() - globalPos.x() < 2) { // 右下
                if (!rubberBand->isVisible() || (rubberBand_direction & Qt::Edge::BottomEdge) == 0) {
                    if (rubberBand_direction != (Qt::Edge::RightEdge | Qt::Edge::BottomEdge)) {
                        rubberBand_direction = Qt::Edge::RightEdge | Qt::Edge::BottomEdge;
                        rubberBand_snapRect = screenRect;
                        rubberBand_snapRect.setTop(screenRect.height() / 2);
                        rubberBand_snapRect.setLeft(screenRect.width() / 2);
                        rubberBand_lastRect_recordingtime = 0;
                        update_rubberBand(screenRect, rect);
                    }
                }
            } else if (allow_showMaximized && globalPos.y() < 2) { // 上
                if (!rubberBand->isVisible() || (rubberBand_direction & Qt::Edge::BottomEdge) == 0) {
                    if (rubberBand_direction != (Qt::Edge::TopEdge)) {
                        rubberBand_direction = Qt::Edge::TopEdge;
                        rubberBand_snapRect = screenRect;
                        rubberBand_lastRect_recordingtime = 0;
                        update_rubberBand(screenRect, rect);
                    }
                }
            } else if (globalPos.x() < 2) { // 左
                if (!rubberBand->isVisible() || (rubberBand_direction & Qt::Edge::BottomEdge) != Qt::Edge::LeftEdge) {
                    if (rubberBand_direction != (Qt::Edge::LeftEdge)) {
                        rubberBand_direction = Qt::Edge::LeftEdge;
                        rubberBand_snapRect = screenRect;
                        rubberBand_snapRect.setTop(0);
                        rubberBand_snapRect.setLeft(0);
                        rubberBand_snapRect.setRight(screenRect.width() / 2);
                        rubberBand_lastRect_recordingtime = 0;
                        update_rubberBand(screenRect, rect);
                    }
                }
            } else if (screenRect.right() - globalPos.x() < 2) { // 右
                if (!rubberBand->isVisible() || (rubberBand_direction & Qt::Edge::BottomEdge) != Qt::Edge::RightEdge) {
                    if (rubberBand_direction != Qt::Edge::RightEdge) {
                        rubberBand_direction = Qt::Edge::RightEdge;
                        rubberBand_snapRect = screenRect;
                        rubberBand_snapRect.setTop(0);
                        rubberBand_snapRect.setLeft(screenRect.width() / 2);
                        rubberBand_lastRect_recordingtime = 0;
                        update_rubberBand(screenRect, rect);
                    }
                }
            } else {
                rubberBand_animationgroup->stop();
                rubberBand->hide();
                rubberBand_direction = Qt::Edges();
            }
        }
    }

    return QObject::eventFilter(watched, event);
}

bool QtFrameless::nativeEventFilter(const QByteArray &eventType, void *message, long *result) {
    // QGL_DEBUG(eventType, message, result);

    //    if (eventType == "windows_generic_MSG" || eventType == "windows_dispatcher_MSG"){
    //        MSG * pMsg = reinterpret_cast<MSG *>(message);

    //        if (pMsg->message == WM_NCMOUSEMOVE)
    //        {
    //            qDebug() <<QString("获取到系统鼠标移动，可以做像qq一样的忙碌检测");
    //        }
    //    }

    // if (eventType == "xcb_generic_event_t") {
    //     xcb_generic_event_t *event = static_cast<xcb_generic_event_t *>(message);
    //     uint8_t responseType = event->response_type & ~0x80;

    //     // QGL_DEBUG("消息类型", responseType);

    //     // if (responseType == XCB_GE_GENERIC) {
    //     //     xcb_ge_generic_event_t *ge = reinterpret_cast<xcb_ge_generic_event_t *>(event);
    //     //     QGL_DEBUG(QGL_FAS(ge->event_type), QGL_FAS(ge->extension), QGL_FAS(ge->response_type), QGL_FAS(ge->sequence), QGL_FAS(ge->full_sequence), QGL_FAS(ge->length), QGL_FAS(ge->pad0[0]));
    //     //     // Generic Event: extension= 132  event_type= 6
    //     //     if (ge->extension == 132 && ge->event_type == 1) {
    //     //         this->background_main_layout->setContentsMargins(0, 0, 0, 0);
    //     //     }
    //     // }
    // }
    return false;
}

//struct MSG {
//    void* hwnd;
//    UINT message;
//    WPARAM wParam;
//    LPARAM lParam;
//    DWORD time;
//    POINT pt;
//    DWORD lPrivate;
//};
bool QtFrameless::nativeEvent(const QByteArray &eventType, void *message, long *result) {
    // QGL_DEBUG(eventType, message, result);
#ifdef Q_OS_WIN
    MSG *msg = reinterpret_cast<MSG *>(message);
    if (msg->message == 134 && shadow_margin > 0) {
        if (qobject_cast<QDialog *>(this->_window)) {
            if (msg->wParam) {
                create_shadow_image();
            } else {
                QColor _shadow_color = this->shadow_color;
                this->shadow_color.setAlpha(static_cast<double>(this->shadow_color.alpha()) * 0.6);
                create_shadow_image();
                this->shadow_color = _shadow_color;
            }
        }
    }
#elif defined Q_OS_LINUX
    // if (eventType == "xcb_generic_event_t") {
    //     xcb_generic_event_t *event = static_cast<xcb_generic_event_t *>(message);
    //     uint8_t responseType = event->response_type & ~0x80;

    //     QGL_DEBUG("消息类型", responseType);

    //     if (responseType == XCB_GE_GENERIC) {
    //         // auto *msg = reinterpret_cast<xcb_client_message_event_t *>(event);

    //         // Display *dpy = QX11Info::display();
    //         // Atom netWmState = XInternAtom(dpy, "_NET_WM_STATE", False);
    //         // Atom maxHorz = XInternAtom(dpy, "_NET_WM_STATE_MAXIMIZED_HORZ", False);
    //         // Atom maxVert = XInternAtom(dpy, "_NET_WM_STATE_MAXIMIZED_VERT", False);

    //         // auto *cfg = reinterpret_cast<xcb_configure_notify_event_t *>(event);
    //         // qDebug() << "窗口位置:" << cfg->x << cfg->y << "尺寸:" << cfg->width << cfg->height;

    //         xcb_ge_generic_event_t *ge = reinterpret_cast<xcb_ge_generic_event_t *>(event);
    //         QGL_DEBUG(QGL_FAS(ge->event_type), QGL_FAS(ge->extension), QGL_FAS(ge->response_type), QGL_FAS(ge->sequence), QGL_FAS(ge->full_sequence), QGL_FAS(ge->length), QGL_FAS(ge->pad0[0]));
    //         // qDebug() << "Generic Event: extension=" << ge->extension << " event_type=" << ge->event_type;
    //         // Generic Event: extension= 132  event_type= 6
    //     }
    // }

#endif
    return false;
}


void QtFrameless::new_rubberBand() {
    rubberBand = new QWidget(nullptr);
    rubberBand_layout = new QHBoxLayout(rubberBand);
    rubberBand_layout->setContentsMargins(0, 0, 0, 0);
    rubberBand_label = new QLabel(rubberBand);
    rubberBand_layout->addWidget(rubberBand_label);
    rubberBand->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowTransparentForInput | Qt::WindowDoesNotAcceptFocus | Qt::Tool);
    rubberBand->setAttribute(Qt::WA_ShowWithoutActivating);
    rubberBand->setAttribute(Qt::WA_TransparentForMouseEvents);
    rubberBand->setAttribute(Qt::WA_TranslucentBackground);
    rubberBand_label->setStyleSheet(R"(
        background-color: rgba(0, 120, 215, 40);
        border: 2px solid rgba(0, 120, 215, 160);
    )");

    rubberBand_animation = new QPropertyAnimation(rubberBand, "geometry");
    rubberBand_animation->setDuration(100);
    rubberBand_animation->setEasingCurve(QEasingCurve::OutQuad);

    rubberBand_animation2 = new QPropertyAnimation(rubberBand, "windowOpacity");
    rubberBand_animation2->setDuration(100);
    rubberBand_animation2->setEasingCurve(QEasingCurve::OutQuad);

    rubberBand_animationgroup = new QParallelAnimationGroup(rubberBand);
    rubberBand_animationgroup->addAnimation(rubberBand_animation);
    rubberBand_animationgroup->addAnimation(rubberBand_animation2);

    this->connect(rubberBand_animationgroup, &QPropertyAnimation::stateChanged, this, [=](QAbstractAnimation::State newState, QAbstractAnimation::State oldState) {
        stackRubberUnderMain(rubberBand, this->_window);
        stackRubberUnderMain(this->_window, rubberBand_pop);
        if (newState == QAbstractAnimation::Stopped) {
            rubberBand_lastRect = rubberBand->frameGeometry();
            rubberBand_lastRect_recordingtime = QDateTime::currentSecsSinceEpoch();
        }
    });

    rubberBand->show();
    rubberBand->setVisible(false);

    rubberBand_pop = new Widget_rubberband_pop(nullptr);
    rubberBand_pop->show();
    //    rubberBand_pop->setVisible(false);
}

void QtFrameless::update_rubberBand(QRect screenRect, QRect rect) {
    if (!rubberBand->isVisible()) {
        rubberBand->setGeometry(rect);
        rubberBand->setWindowOpacity(0);
    }
    if (rubberBand_animationgroup->state() == QAbstractAnimation::State::Running) {
        rubberBand_animationgroup->stop();
        rubberBand_animation->setStartValue(rubberBand->frameGeometry());
        rubberBand_animation2->setStartValue(rubberBand->windowOpacity());
    } else if (QDateTime::currentSecsSinceEpoch() - rubberBand_lastRect_recordingtime <= 3) {
        rubberBand_animation->setStartValue(rubberBand_lastRect);
        rubberBand_animation2->setStartValue(static_cast<double>(0));
        rubberBand->setWindowOpacity(0);
    } else {
        rubberBand_animation->setStartValue(rect);
        rubberBand_animation2->setStartValue(static_cast<double>(0));
        rubberBand->setWindowOpacity(0);
    }

    rubberBand_animation->setEndValue(rubberBand_snapRect);
    rubberBand_animation2->setEndValue(static_cast<double>(1));

    bool _showonce_pop = !rubberBand->isVisible();

    // rubberBand->stackUnder(this);
    stackRubberUnderMain(rubberBand, this->_window);
    rubberBand->show();
    stackRubberUnderMain(rubberBand, this->_window);

    if (_showonce_pop) {
        rubberBand_pop->stop();
        QPoint _cur_pos = QCursor::pos();
        _cur_pos.setX(_cur_pos.x() - rubberBand_pop->width() / 2);
        _cur_pos.setY(_cur_pos.y() - rubberBand_pop->height() / 2);
        rubberBand_pop->move(_cur_pos);

        stackRubberUnderMain(this->_window, rubberBand_pop);
        rubberBand_pop->show();
        stackRubberUnderMain(this->_window, rubberBand_pop);
        rubberBand_pop->start();
    }

    rubberBand_animationgroup->start();
}

QList<Qt::Edges> QtFrameless::_rubberBand_allowed_resize_direction() {
    QList<Qt::Edges> l;
    if (rubberBand_delegated) {
        if (rubberBand_direction == Qt::Edge::LeftEdge) {
            l.append(Qt::Edge::RightEdge);
        } else if (rubberBand_direction == Qt::Edge::RightEdge) {
            l.append(Qt::Edge::LeftEdge);
        } else if (rubberBand_direction == (Qt::Edge::LeftEdge | Qt::Edge::TopEdge)) {
            l.append(Qt::Edge::RightEdge);
            l.append(Qt::Edge::BottomEdge);
            l.append(Qt::Edge::RightEdge | Qt::Edge::BottomEdge);
        } else if (rubberBand_direction == (Qt::Edge::LeftEdge | Qt::Edge::BottomEdge)) {
            l.append(Qt::Edge::RightEdge);
            l.append(Qt::Edge::TopEdge);
            l.append(Qt::Edge::RightEdge | Qt::Edge::TopEdge);
        } else if (rubberBand_direction == (Qt::Edge::RightEdge | Qt::Edge::TopEdge)) {
            l.append(Qt::Edge::LeftEdge);
            l.append(Qt::Edge::BottomEdge);
            l.append(Qt::Edge::LeftEdge | Qt::Edge::BottomEdge);
        } else if (rubberBand_direction == (Qt::Edge::RightEdge | Qt::Edge::BottomEdge)) {
            l.append(Qt::Edge::LeftEdge);
            l.append(Qt::Edge::TopEdge);
            l.append(Qt::Edge::LeftEdge | Qt::Edge::TopEdge);
        } else if (rubberBand_direction == Qt::Edge::BottomEdge) {
            l.append(Qt::Edge::LeftEdge);
            l.append(Qt::Edge::RightEdge);
        }
    }
    return l;
}


NAMESPACE_QTFRAMELESS_END
