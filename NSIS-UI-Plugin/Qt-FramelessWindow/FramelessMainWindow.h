#pragma once
#include <QtWidgets/QMainWindow>

class FramelessMainWindow : public QMainWindow {
    Q_OBJECT
  public:
    enum class Direction {
        UP = 0,
        DOWN = 1,
        LEFT,
        RIGHT,
        LEFTTOP,
        LEFTBOTTOM,
        RIGHTBOTTOM,
        RIGHTTOP,
        NONE
    };

    static void setAllWidgetMouseTracking(QWidget *widget);
    static void loadStyleSheetFile(const QString &sheetName, QWidget *widget);

    FramelessMainWindow(bool translucentBackground, QWidget *parent = Q_NULLPTR);
    virtual ~FramelessMainWindow();
    virtual void setTitlebar(QVector<QWidget*> titleBar);
    virtual void setResizeable(bool b);
    virtual bool resizeable() const;
  protected:
    bool eventFilter(QObject *target, QEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    virtual void region(const QPoint &cursorGlobalPoint);
  protected:
    bool m_bLeftPressed;
    bool m_bEnableResize;
    Direction m_Direction;
    const int m_iResizeRegionPadding;
    QPoint m_DragPos;
    QVector<QWidget*> m_titlebarWidget;
};
