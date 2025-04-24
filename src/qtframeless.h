#ifndef FRAMELESS_H
#define FRAMELESS_H

#include "./qtframeless_global.h"

#include <QDebug>
#include <QLabel>
#include <QMainWindow>
#include <QObject>
#include <QParallelAnimationGroup>
#include <QPropertyAnimation>

#include <qgridlayout.h>
#include <QApplication>
#include <QCursor>
#include <QEvent>
#include <QHoverEvent>
#include <QMouseEvent>
#include <QWindow>

#include <QAbstractNativeEventFilter>

#include "./titlebar/titlebarlikewindows10.h"

#include "./custom/widget_rubberband_pop.h"

NAMESPACE_QTFRAMELESS_BEGIN


#define SET_CURSOR_IF_NOT_SPECIFIED(w, cursorType) \
    if ((w)->cursor() != cursorType) { \
        (w)->setCursor(cursorType); \
    }

#define DELETE_OBJ_IF_HAD_NEW(w) \
    if (w) { \
        delete w; \
        w = nullptr; \
    }

class QTFRAMELESS_EXPORT QtFrameless : public QObject, public QAbstractNativeEventFilter {
    Q_OBJECT
  public:
    explicit QtFrameless(QObject *parent = nullptr);
    ~QtFrameless();

    int shadow_size = 13;
    int resize_rect_out = 6;
    int resize_rect_in = 2;
    int resize_rect_corner = resize_rect_in + 10;
    QColor shadow_color = QColor(140, 140, 140);
    QList<int> round4 = {6, 6, 3, 3};
#ifdef Q_OS_WIN
    QList<bool> rubberBand_enable = {true, true, true}; // 贴边，缩放，双击
#else
    QList<bool> rubberBand_enable = {false, false, false}; // 被系统窗口管理器干扰
#endif
    bool allow_showMaximized = true;
    bool allow_showMinimized = true;
    int titlebar_MinimumHeight = round4.at(0) > round4.at(1) ? round4.at(0) : round4.at(1);
    int border_width = 0;
    bool movable_in_whole_window = false;
    QList<QColor> colors = {
        QColor(241, 241, 241), // titlebar background color
        QColor(255, 255, 255), // central widget background color

        QColor(160, 160, 160) // border color
    };

    void app(QApplication &a);
    void init(QMainWindow &w);
    void init(QWidget &w);
    void init(QDialog &w);

    void load();
    void loadthemesetting();
    void set_titlebar(QWidget &titlebar);
    void set_menubar(QWidget &menubar);
    void set_bottombar(QWidget &bottombar);

    bool eventFilter(QObject *watched, QEvent *event);
    bool nativeEventFilter(const QByteArray &eventType, void *message, long *result);
    bool nativeEvent(const QByteArray &eventType, void *message, long *result);

    void showMaximized(bool only_update_round = false);
    void showNormal(bool only_update_round = false);

    QWidget *titlebar_widget = nullptr;
    QWidget *_window = nullptr;
    QSize _window_size;
    void resizeto();

    //    QSize getsize_central_widget();
    QWidget *central_widget = nullptr;


  signals:
    void windowStateChanged(const Qt::WindowStates &state);

  private:
    void _new_widget();

    int shadow_margin = 9;
    void create_shadow_image();

#if defined(Q_OS_LINUX) && !defined(Q_OS_ANDROID)
    void _x11_move_window(const WId &wid, const QPoint &pos);
#endif

    bool is_inside_window(const QPoint &pos);
    bool is_titlebar(const QPoint &pos);
    QPair<bool, Qt::Edges> is_allow_resize(const QPoint &pos);

    bool m_isPressButton = false;
    bool m_isPress = false;
    QPoint m_pressPos;
    QPoint m_pressGlobalPos;
    QPoint m_pressWindowGlobalPos;
    QRect rectAfterMaximized;

    bool m_isSystemResize = false;


    QLayout *central_layout = nullptr;
    QGridLayout *background_main_layout = nullptr;
    QGridLayout *central_main_layout = nullptr;
    QGridLayout *titlebar_main_layout = nullptr;
    QGridLayout *menubar_main_layout = nullptr;
    QGridLayout *bottombar_main_layout = nullptr;


    QWidget *background_main_widget = nullptr;
    QWidget *central_main_widget = nullptr;
    QWidget *titlebar_main_widget = nullptr;
    QWidget *menubar_main_widget = nullptr;
    QWidget *bottombar_main_widget = nullptr;

    QWidget *rubberBand = nullptr;
    QHBoxLayout *rubberBand_layout = nullptr;
    QLabel *rubberBand_label = nullptr;
    QRect rubberBand_snapRect;
    QRect rubberBand_lastRect;
    qint64 rubberBand_lastRect_recordingtime;
    QPropertyAnimation *rubberBand_animation = nullptr;
    QPropertyAnimation *rubberBand_animation2 = nullptr;
    QParallelAnimationGroup *rubberBand_animationgroup = nullptr;
    bool rubberBand_moved = false;
    bool rubberBand_resized = false;
    QSize rubberBand_begin_size;
    bool rubberBand_delegated = false;
    QRect rubberBand_origin_size;
    QRect rubberBand_origin_location;
    Qt::Edges rubberBand_direction;
    void new_rubberBand();
    void update_rubberBand(QRect screenRect, QRect rect);
    QList<Qt::Edges> _rubberBand_allowed_resize_direction();
    Widget_rubberband_pop *rubberBand_pop = nullptr;
};

NAMESPACE_QTFRAMELESS_END

#endif // FRAMELESS_H
