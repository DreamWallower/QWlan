#pragma once

#include "QBackWindow.h"
#include "QFrontWindow.h"
#include "QHotspot.h"
#include <QStackedWidget>

class QWlan : public QStackedWidget {
    Q_OBJECT

public:
    QWlan(QWidget* parent = Q_NULLPTR);
    ~QWlan();

private:
    // 绘制旋转效果
    void paintEvent(QPaintEvent* event);

private slots:
    // 开始旋转窗口
    void onRotateWindow();
    // 窗口旋转结束
    void onRotateFinished();

signals:
    void startHotspot();
    void stopHotspot();

private:
    int m_nextPageIndex;
    bool m_isRoratingWindow;
    QFrontWindow* m_frontWindow;
    QBackWindow* m_backWindow;
    QHotspot m_hotspot;
    QThread* m_thread;
    bool m_isOn;
};