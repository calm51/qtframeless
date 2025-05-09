#include "titlebarlikewindows10.h"

#include "../custom/toolbutton_righttop_windows10.h"
#include "ui_titlebarlikewindows10.h"

#include <qpainter.h>
#include "../qtframeless.h"
#include <QPointer>
//#include "__frameless_include__.h"

#include <qtgllogger/qtgllogger.h>

#include <qpainterpath.h>

#ifdef Q_OS_WIN
#include <windows.h>
#endif

NAMESPACE_QTFRAMELESS_BEGIN

namespace TB {

//void minimizeWindow(HWND hWnd) {
//    // 发送最小化窗口消息
//    SendMessage(hWnd, WM_SYSCOMMAND, SC_MINIMIZE, 0);
//    // 获取窗口信息
//    WINDOWPLACEMENT wp;
//    wp.length = sizeof(WINDOWPLACEMENT);
//    GetWindowPlacement(hWnd, &wp);
//    // 设置最小化动画效果
//    ANIMATIONINFO ai;
//    ai.cbSize = sizeof(ANIMATIONINFO);
//    ai.iMinAnimate = TRUE;
//    SystemParametersInfo(SPI_SETANIMATION, sizeof(ANIMATIONINFO), &ai, 0);
//    // 更新窗口显示状态
//    SetWindowPlacement(hWnd, &wp);
//}

TitlebarLikeWindows10::TitlebarLikeWindows10(QtFrameless *qfl, const int &buttonFixeWidth, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TitlebarLikeWindows10) {
    ui->setupUi(this);
    this->qfl = qfl;
    this->_window = parent;
    ui->toolButton_4->type = RIGHTTOP_TYPE::__type__::Minimize;
    ui->toolButton_5->type = RIGHTTOP_TYPE::__type__::Maximize;
    ui->toolButton_6->type = RIGHTTOP_TYPE::__type__::Close;

    ui->widget->setObjectName("widget_round4");

    connect(&GLOBAL.SIGNAL, &__signal__::themeChanged, this, [=]() { this->loadthemesetting(); });

    ui->toolButton_4->setFixedWidth(buttonFixeWidth);
    ui->toolButton_5->setFixedWidth(buttonFixeWidth);
    ui->toolButton_6->setFixedWidth(buttonFixeWidth);
    //    ui->toolButton_4->setFixedHeight(qfl->titlebar_MinimumHeight);
    //    ui->toolButton_5->setFixedHeight(qfl->titlebar_MinimumHeight);
    //    ui->toolButton_6->setFixedHeight(qfl->titlebar_MinimumHeight);


    QString s3(R"(
QToolButton{
    background-color:transparent;
    padding:0px 0px;
    margin:0px 0px;
    border:0px solid transparent;
})");
    QString s4(R"(
QToolButton{
    background-color:transparent;
    padding:0px 0px;
    margin:0px 0px;
    border:0px solid transparent;
})");
    ui->widget_4->setStyleSheet(s3);
    ui->toolButton_6->setStyleSheet(s4);

    ui->toolButton_4->installEventFilter(this);
    ui->toolButton_5->installEventFilter(this);
    ui->toolButton_6->installEventFilter(this);
    this->installEventFilter(this);

    draw(ui->toolButton_4);
    draw(ui->toolButton_5);
    draw(ui->toolButton_6);

    connect(ui->toolButton_4, &QToolButton::clicked, this, [=]() {
        if (qfl->allow_showMinimized) {
            this->_window->showMinimized();
        }
        // minimizeWindow(reinterpret_cast<HWND>(this->_window->winId()));
    });
    connect(ui->toolButton_5, &QToolButton::clicked, this, [=]() {
        if (this->_window->isMaximized()) {
            this->qfl->showNormal();
        } else {
            this->qfl->showMaximized();
        }
    });
    connect(ui->toolButton_6, &QToolButton::clicked, this, [=]() { this->_window->close(); });
    //    connect(this,&QWidget::windowTitleChanged,this,[=](const QString &s){
    //        ui->label_title->setText(s);
    //    });
    connect(this, &QWidget::windowIconChanged, this, [=](const QIcon &icon) {
        ui->label->setFixedSize(16, 16);
        ui->label->setPixmap(icon.pixmap(QSize(64, 64)));
    });

    connect(qfl, &QtFrameless::windowStateChanged, this, [=](const Qt::WindowStates &state) { this->load_round4(!state.testFlag(Qt::WindowMaximized)); });
}

TitlebarLikeWindows10::~TitlebarLikeWindows10() {
    QGL_DEBUG("析构");
    delete ui;
}


void TitlebarLikeWindows10::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    //    GLOBAL.ARGS.mainwindow->setMinimumWidth(
    //                ui->label->width()+
    //                ui->label_2->width()+
    //                ui->widget_2->width()+
    //                ui->widget_4->width()+
    //                5*2
    //                );
}

void TitlebarLikeWindows10::draw(ToolButton_righttop_windows10 *button) {
    button->p = QPixmap(button->width(), button->height());

    QPoint pos = button->mapFromGlobal(QCursor::pos());
    const double &width = static_cast<double>(button->width());
    const double &height = static_cast<double>(button->height());

    bool is_not_activate = pos.x() < 0 || pos.y() - qfl->resize_rect_in < 0 || pos.x() > width || pos.y() > height;
    bool is_not_activate__close = pos.x() < 0 || pos.y() - qfl->resize_rect_in < 0 || pos.x() + qfl->resize_rect_in > width || pos.y() > height;

    button->p.fill(Qt::transparent);
    QPainter painter(&button->p);
    QRect border_rect(0, 0, width, height);
    border_rect.setTop(border_top_width);
    border_rect.setBottom(height - border_bottom_width - 1);

    if (button->type == RIGHTTOP_TYPE::__type__::Minimize) {
        if (is_not_activate) {
            painter.setBrush(QBrush(colors.at(0)));
            //            button->p.fill(colors.at(0));
        } else {
            border_rect.setTop(0);
            border_rect.setBottom(height - 1);
            if (button->isDown()) {
                //                button->p.fill(colors.at(2));
                painter.setBrush(QBrush(colors.at(2)));
            } else {
                //                button->p.fill(colors.at(1));
                painter.setBrush(QBrush(colors.at(1)));
            }
        }
    } else if (button->type == RIGHTTOP_TYPE::__type__::Maximize) {
        if (is_not_activate) {
            //            button->p.fill(colors.at(0));
            painter.setBrush(QBrush(colors.at(0)));
        } else {
            border_rect.setTop(0);
            border_rect.setBottom(height - 1);
            if (button->isDown()) {
                //                button->p.fill(colors.at(2));
                painter.setBrush(QBrush(colors.at(2)));
            } else {
                //                button->p.fill(colors.at(1));
                painter.setBrush(QBrush(colors.at(1)));
            }
        }
    } else if (button->type == RIGHTTOP_TYPE::__type__::Close) {
        if (is_not_activate__close) {
            //            button->p.fill(colors.at(0));
            painter.setBrush(QBrush(colors.at(0)));
        } else {
            border_rect.setTop(0);
            border_rect.setBottom(height - 1);
            if (button->isDown()) {
                //                button->p.fill(colors.at(6));
                painter.setBrush(QBrush(colors.at(6)));
            } else {
                //                button->p.fill(colors.at(5));
                painter.setBrush(QBrush(colors.at(5)));
            }
        }
    }
    painter.setPen(Qt::NoPen);
    if (button->_is_most_right && !(this->_window && this->_window->isMaximized())) {
        painter.setRenderHint(QPainter::Antialiasing);
        QPainterPath path(border_rect.topLeft());
        path.setFillRule(Qt::WindingFill);
        //        path.addRoundedRect(border_rect, qfl->round4.at(1),qfl->round4.at(1));
        //        path.addRect(QRect(border_rect.left(), border_rect.top()+border_rect.height()/2, border_rect.width(), border_rect.height()/2));
        //        path.addRect(QRect(border_rect.left(), border_rect.top(), border_rect.width()/2, border_rect.height()/2));
        int radius = qfl->round4.at(1);

        QRectF arc_rect(border_rect.width() - (radius * 2), border_rect.top(), radius * 2, radius * 2);
        path.moveTo(border_rect.topLeft());
        path.lineTo(border_rect.left(), border_rect.height());
        path.lineTo(border_rect.width(), border_rect.height());
        if (radius > 0 && radius <= button->size().height()) {
            path.arcTo(arc_rect, 0, 90);
        } else {
            path.lineTo(border_rect.width(), border_rect.top());
        }
        path.closeSubpath();
        painter.fillPath(path, painter.brush().color());
        //        painter.setBrush(Qt::NoBrush);
        //        painter.setPen(QPen(painter.brush().color()));
        //        painter.drawPath(path);

        painter.setRenderHint(QPainter::Antialiasing, false);
    } else {
        painter.drawRect(border_rect);
    }

    painter.setBrush(Qt::NoBrush);
    QPen pen;
    if (is_not_activate) {
        pen.setColor(colors.at(3));
        pen.setStyle(Qt::SolidLine);
        pen.setWidthF(1);
    } else {
        pen.setColor(colors.at(4));
        pen.setStyle(Qt::SolidLine);
        pen.setWidthF(1);
    }
    if (button->type == RIGHTTOP_TYPE::__type__::Minimize) {
        painter.setPen(pen);
        painter.drawLine(QPointF(width * 4 / 10, height / 2), QPointF(width * 6 / 10, height / 2));
    } else if (button->type == RIGHTTOP_TYPE::__type__::Maximize) {
        painter.setPen(pen);
        if (this->_window && this->_window->isMaximized()) {
            painter.translate(1, -1);
            painter.drawRect(QRectF(QPointF(width * 4 / 10, height * 1.1 / 3), QPointF(width * 5.9 / 10, height * 1.9 / 3)));
            if (is_not_activate) {
                painter.setBrush(QBrush(colors.at(0)));
            } else {
                if (button->isDown()) {
                    painter.setBrush(QBrush(colors.at(2)));
                } else {
                    painter.setBrush(QBrush(colors.at(1)));
                }
            }
            painter.translate(-2, 2);
            painter.drawRect(QRectF(QPointF(width * 4 / 10, height * 1.1 / 3), QPointF(width * 5.9 / 10, height * 1.9 / 3)));
        } else {
            painter.drawLine(QPointF(width * 4 / 10, height * 1 / 3), QPointF(width * 6 / 10, height * 1 / 3));
            painter.drawLine(QPointF(width * 4 / 10, height * 2 / 3), QPointF(width * 6 / 10, height * 2 / 3));
            painter.drawLine(QPointF(width * 4 / 10, height * 1 / 3), QPointF(width * 4 / 10, height * 2 / 3));
            painter.drawLine(QPointF(width * 6 / 10, height * 1 / 3), QPointF(width * 6 / 10, height * 2 / 3));
        }

    } else if (button->type == RIGHTTOP_TYPE::__type__::Close) {
        //        painter.setRenderHint(QPainter::TextAntialiasing);
        if (is_not_activate__close) {
            pen.setColor(colors.at(7));
        } else {
            pen.setColor(colors.at(8));
        }
        pen.setStyle(Qt::SolidLine);
#ifdef Q_OS_WIN
        pen.setWidthF(1);
#else
        pen.setWidthF(1);
#endif
        painter.setPen(pen);
        painter.drawLine(QPointF(width * 4 / 10, height * 1 / 3), QPointF(width * 6 / 10, height * 2 / 3));
        painter.drawLine(QPointF(width * 4 / 10, height * 2 / 3), QPointF(width * 6 / 10, height * 1 / 3));
    }
    button->update();
}

void TitlebarLikeWindows10::load() {
    //    QWidget *_border_widget = new QWidget(this);
    //    _border_widget->setObjectName(QString("_border_widget"));
    //    _border_widget->setAttribute(Qt::WA_TransparentForMouseEvents);
    //    qobject_cast<QGridLayout*>(this->layout())->addWidget(_border_widget,0,0,1,1);
    //    _border_widget->setStyleSheet(QString(R"(
    //QWidget#_border_widget{
    //border:0px solid rgb(0,0,0);
    //border-top-width:%1px;
    //border-bottom-width:%2px;
    //border-top-color:%3;
    //border-bottom-color:%4;
    //}
    //)").arg(QString::number(border_top_width),QString::number(border_bottom_width),GLOBAL.qcolor2qss(colors.at(9)),GLOBAL.qcolor2qss(colors.at(10))));
    //    this->setFixedHeight(qfl.titlebar_MinimumHeight);

    load_round4(!this->qfl->_window->isMaximized());

    ui->toolButton_4->setVisible(!hide_button.at(0));
    ui->toolButton_5->setVisible(!hide_button.at(1));
    ui->toolButton_6->setVisible(!hide_button.at(2));
    if (!hide_button.at(2)) {
        ui->toolButton_6->_is_most_right = true;
        ui->toolButton_5->_is_most_right = false;
        ui->toolButton_4->_is_most_right = false;
    } else if (!hide_button.at(1)) {
        ui->toolButton_5->_is_most_right = true;
        ui->toolButton_4->_is_most_right = false;
        ui->toolButton_6->_is_most_right = false;
    } else if (!hide_button.at(0)) {
        ui->toolButton_4->_is_most_right = true;
        ui->toolButton_6->_is_most_right = false;
        ui->toolButton_5->_is_most_right = false;
    }
    //    ui->toolButton_4->update();
    //    ui->toolButton_5->update();
    //    ui->toolButton_6->update();
    draw(ui->toolButton_4);
    draw(ui->toolButton_5);
    draw(ui->toolButton_6);
}

void TitlebarLikeWindows10::loadthemesetting() {
    border_top_width = GLOBAL.theme_config[GLOBAL.currentTheme].border_top_width;
    border_bottom_width = GLOBAL.theme_config[GLOBAL.currentTheme].border_bottom_width;
    //    hide_button = GLOBAL.theme_config[GLOBAL.currentTheme].hide_button;
    colors = GLOBAL.theme_config[GLOBAL.currentTheme].TitlebarLikeWindows10_colors;

    load();
}

void TitlebarLikeWindows10::load_round4(bool en) {
    if (en) {
        ui->widget->setStyleSheet(QString(R"(
QWidget#widget_round4{
border:0px solid rgb(0,0,0);
border-top-width:%1px;
border-bottom-width:%2px;
border-top-color:%3;
border-bottom-color:%4;
border-top-left-radius:%5px; border-top-right-radius:%6px;
}
        )")
                                      .arg(QString::number(border_top_width), QString::number(border_bottom_width), GLOBAL.qcolor2qss(colors.at(9)), GLOBAL.qcolor2qss(colors.at(10)))
                                      .arg(QString::number(qfl->round4.at(0)), QString::number(qfl->round4.at(1))));
        // ui->widget_4->setContentsMargins(0,border_top_width,0,border_bottom_width);
        QGridLayout *l1 = qobject_cast<QGridLayout *>(ui->widget_2->layout());
        if (qfl->round4.at(0) / 2 > 6) {
            auto l1_cm = l1->contentsMargins();
            l1_cm.setLeft(qfl->round4.at(0) / 2);
            l1->setContentsMargins(l1_cm);
        } else {
            auto l1_cm = l1->contentsMargins();
            l1_cm.setLeft(6);
            l1->setContentsMargins(l1_cm);
        }
    } else {
        ui->widget->setStyleSheet(QString(R"(
QWidget#widget_round4{
border:0px solid rgb(0,0,0);
border-top-width:%1px;
border-bottom-width:%2px;
border-top-color:%3;
border-bottom-color:%4;
border-top-left-radius:0px; border-top-right-radius:0px;
}
        )")
                                      .arg(QString::number(border_top_width), QString::number(border_bottom_width), GLOBAL.qcolor2qss(colors.at(9)), GLOBAL.qcolor2qss(colors.at(10))));
        QGridLayout *l1 = qobject_cast<QGridLayout *>(ui->widget_2->layout());
        auto l1_cm = l1->contentsMargins();
        l1_cm.setLeft(6);
        l1->setContentsMargins(l1_cm);
    }
}

QHBoxLayout *TitlebarLikeWindows10::w3layout() {
    return qobject_cast<QHBoxLayout *>(ui->widget_3->layout());
}
QHBoxLayout *TitlebarLikeWindows10::w6layout() {
    return qobject_cast<QHBoxLayout *>(ui->widget_6->layout());
}
void TitlebarLikeWindows10::set_title(const QString &s) {
    ui->label_title->setText(s);
}
void TitlebarLikeWindows10::set_title_enable(const bool &b) {
    ui->label_title->setEnabled(b);
}

void TitlebarLikeWindows10::set_title_fontsize(const int &pt) {
    if (pt == 0) {
        ui->label_title->setStyleSheet("");
    } else {
        ui->label_title->setStyleSheet(QString("#label_title{font-size:%1pt;}").arg(pt));
    }
}

bool TitlebarLikeWindows10::eventFilter(QObject *watched, QEvent *event) {
    if (watched == this) {
        if (event->type() == QEvent::Resize) {
            draw(ui->toolButton_5);
            draw(ui->toolButton_6);
        } else if (event->type() == QEvent::Hide || event->type() == QEvent::WindowDeactivate || event->type() == QEvent::Show || event->type() == QEvent::WindowActivate) {
            draw(ui->toolButton_4);
            draw(ui->toolButton_5);
            draw(ui->toolButton_6);
        }
    } else if (watched == ui->toolButton_4 || watched == ui->toolButton_5 || watched == ui->toolButton_6) {
        if (event->type() == QEvent::HoverLeave) {
            auto button = qobject_cast<ToolButton_righttop_windows10 *>(watched);
            draw(button);
            QPointer<TitlebarLikeWindows10> _p(this);
            QTimer::singleShot(60, [=]() {
                if (!_p) {
                    return;
                }
                draw(button);
            });
        } else if (event->type() == QEvent::HoverMove || event->type() == QEvent::HoverEnter) {
            auto button = qobject_cast<ToolButton_righttop_windows10 *>(watched);
            QPointer<TitlebarLikeWindows10> _p(this);
            QTimer::singleShot(0, [=]() {
                if (!_p) {
                    return;
                }
                draw(button);
            });
        } else if (event->type() == QEvent::MouseButtonPress || event->type() == QEvent::MouseButtonRelease) {
            auto button = qobject_cast<ToolButton_righttop_windows10 *>(watched);
            QPointer<TitlebarLikeWindows10> _p(this);
            QTimer::singleShot(0, [=]() {
                if (!_p) {
                    return;
                }
                draw(button);
            });
        }
    }
    return QObject::eventFilter(watched, event);
}

} // namespace TB


NAMESPACE_QTFRAMELESS_END
