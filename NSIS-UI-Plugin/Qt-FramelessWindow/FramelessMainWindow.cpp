#include "FramelessMainWindow.h"
#include <QMouseEvent>
#include <QApplication>
#include <QDebug>

void FramelessMainWindow::setAllWidgetMouseTracking(QWidget *widget) {
    if (!widget)
        return;
    widget->setMouseTracking(true);
    QObjectList list = widget->children();
    foreach(QObject *obj, list) {
        if (obj->metaObject()->className() == QStringLiteral("QWidget")) {
            QWidget *w = (QWidget *)obj;
            w->setMouseTracking(true);
            setAllWidgetMouseTracking(w);
        }
    }
}

void FramelessMainWindow::loadStyleSheetFile(const QString &sheetName, QWidget *widget) {
    if (widget) {
        QString qss;
        QFile qssFile(sheetName);
        qssFile.open(QFile::ReadOnly);
        if (qssFile.isOpen()) {
            qss = QLatin1String(qssFile.readAll());
            widget->setStyleSheet(qss);
            qssFile.close();
        }
    }
}

FramelessMainWindow::FramelessMainWindow(bool translucentBackground, QWidget *parent) :
    QMainWindow(parent),
    m_bLeftPressed(false),
    m_bEnableResize(false),
    m_Direction(Direction::NONE),
    m_iResizeRegionPadding(4) {
    installEventFilter(this);

    setWindowFlags(Qt::FramelessWindowHint);
    if (translucentBackground) {
        setAttribute(Qt::WA_TranslucentBackground);
    }
}

FramelessMainWindow::~FramelessMainWindow() {
}

void FramelessMainWindow::setTitlebar(QVector<QWidget*> titleBar) {
    m_titlebarWidget = titleBar;
}

void FramelessMainWindow::setResizeable(bool b) {
    m_bEnableResize = b;
}

bool FramelessMainWindow::resizeable() const {
    return m_bEnableResize;
}

bool FramelessMainWindow::eventFilter(QObject *target, QEvent *event) {
    if (event->type() == QEvent::Paint) {
        static bool first = false;
        if (!false) {
            first = true;
            FramelessMainWindow::setAllWidgetMouseTracking((QWidget*)parent());
        }
    }
    return QMainWindow::eventFilter(target, event);
}

void FramelessMainWindow::mouseDoubleClickEvent(QMouseEvent *event) {
    QMainWindow::mouseDoubleClickEvent(event);
}

void FramelessMainWindow::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        if (m_Direction != Direction::NONE) {
            m_bLeftPressed = true;
            this->mouseGrabber();
        } else {
            QWidget *action = QApplication::widgetAt(event->globalPos());
            if (action) {
                bool inTitlebar = false;
                for (auto &item : m_titlebarWidget) {
                    if (action == item) {
                        inTitlebar = true;
                        break;
                    }
                }
                if (inTitlebar) {
                    m_bLeftPressed = true;
                    m_DragPos = event->globalPos() - this->frameGeometry().topLeft();
                }
            }
        }
    }
    return QWidget::mousePressEvent(event);
}

void FramelessMainWindow::mouseMoveEvent(QMouseEvent *event) {
    QPoint globalPoint = event->globalPos();
    if (m_bLeftPressed) {
        if (m_Direction != Direction::NONE) {
            QRect rect = this->rect();
            QPoint tl = mapToGlobal(rect.topLeft());
            QPoint rb = mapToGlobal(rect.bottomRight());

            QRect rMove(tl, rb);

            switch (m_Direction) {
                case Direction::LEFT:
                    if (rb.x() - globalPoint.x() <= this->minimumWidth())
                        rMove.setX(tl.x());
                    else
                        rMove.setX(globalPoint.x());
                    break;
                case Direction::RIGHT:
                    rMove.setWidth(globalPoint.x() - tl.x());
                    break;
                case Direction::UP:
                    if (rb.y() - globalPoint.y() <= this->minimumHeight())
                        rMove.setY(tl.y());
                    else
                        rMove.setY(globalPoint.y());
                    break;
                case Direction::DOWN:
                    rMove.setHeight(globalPoint.y() - tl.y());
                    break;
                case Direction::LEFTTOP:
                    if (rb.x() - globalPoint.x() <= this->minimumWidth())
                        rMove.setX(tl.x());
                    else
                        rMove.setX(globalPoint.x());
                    if (rb.y() - globalPoint.y() <= this->minimumHeight())
                        rMove.setY(tl.y());
                    else
                        rMove.setY(globalPoint.y());
                    break;
                case Direction::RIGHTTOP:
                    rMove.setWidth(globalPoint.x() - tl.x());
                    rMove.setY(globalPoint.y());
                    break;
                case Direction::LEFTBOTTOM:
                    rMove.setX(globalPoint.x());
                    rMove.setHeight(globalPoint.y() - tl.y());
                    break;
                case Direction::RIGHTBOTTOM:
                    rMove.setWidth(globalPoint.x() - tl.x());
                    rMove.setHeight(globalPoint.y() - tl.y());
                    break;
                default:

                    break;
            }
            this->setGeometry(rMove);
        } else {
            this->move(event->globalPos() - m_DragPos);
            event->accept();
        }
    } else {
        region(globalPoint);
    }

    return QWidget::mouseMoveEvent(event);
}


void FramelessMainWindow::region(const QPoint &cursorGlobalPoint) {
    if (!m_bEnableResize)
        return;
    QRect rect = this->rect();
    QPoint tl = mapToGlobal(rect.topLeft());
    QPoint rb = mapToGlobal(rect.bottomRight());
    int x = cursorGlobalPoint.x();
    int y = cursorGlobalPoint.y();

    if (tl.x() + m_iResizeRegionPadding >= x && tl.x() <= x && tl.y() + m_iResizeRegionPadding >= y && tl.y() <= y) {
        // ×óÉÏ½Ç
        m_Direction = Direction::LEFTTOP;
        this->setCursor(QCursor(Qt::SizeFDiagCursor));
    } else if (x >= rb.x() - m_iResizeRegionPadding && x <= rb.x() && y >= rb.y() - m_iResizeRegionPadding && y <= rb.y()) {
        // ÓÒÏÂ½Ç
        m_Direction = Direction::RIGHTBOTTOM;
        this->setCursor(QCursor(Qt::SizeFDiagCursor));
    } else if (x <= tl.x() + m_iResizeRegionPadding && x >= tl.x() && y >= rb.y() - m_iResizeRegionPadding && y <= rb.y()) {
        //×óÏÂ½Ç
        m_Direction = Direction::LEFTBOTTOM;
        this->setCursor(QCursor(Qt::SizeBDiagCursor));
    } else if (x <= rb.x() && x >= rb.x() - m_iResizeRegionPadding && y >= tl.y() && y <= tl.y() + m_iResizeRegionPadding) {
        // ÓÒÉÏ½Ç
        m_Direction = Direction::RIGHTTOP;
        this->setCursor(QCursor(Qt::SizeBDiagCursor));
    } else if (x <= tl.x() + m_iResizeRegionPadding && x >= tl.x()) {
        // ×ó±ß
        m_Direction = Direction::LEFT;
        this->setCursor(QCursor(Qt::SizeHorCursor));
    } else if (x <= rb.x() && x >= rb.x() - m_iResizeRegionPadding) {
        // ÓÒ±ß
        m_Direction = Direction::RIGHT;
        this->setCursor(QCursor(Qt::SizeHorCursor));
    } else if (y >= tl.y() && y <= tl.y() + m_iResizeRegionPadding) {
        // ÉÏ±ß
        m_Direction = Direction::UP;
        this->setCursor(QCursor(Qt::SizeVerCursor));
    } else if (y <= rb.y() && y >= rb.y() - m_iResizeRegionPadding) {
        // ÏÂ±ß
        m_Direction = Direction::DOWN;
        this->setCursor(QCursor(Qt::SizeVerCursor));
    } else {
        // Ä¬ÈÏ
        m_Direction = Direction::NONE;
        this->setCursor(QCursor(Qt::ArrowCursor));
    }
}


void FramelessMainWindow::mouseReleaseEvent(QMouseEvent *event) {
    m_bLeftPressed = false;
    if (m_Direction != Direction::NONE) {
        m_Direction = Direction::NONE;
        this->releaseMouse();
        this->setCursor(QCursor(Qt::ArrowCursor));
    }
    return QWidget::mouseReleaseEvent(event);
}

void FramelessMainWindow::resizeEvent(QResizeEvent *event) {
    return QWidget::resizeEvent(event);
}