#include "QWlan.h"
#include <QDebug>
#include <QPainter>
#include <QPropertyAnimation>
#include <QThread>

QWlan::QWlan(QWidget* parent)
    : QStackedWidget(parent), m_isRoratingWindow(false), m_nextPageIndex(0) {
    // 隐藏标题栏，设置透明背景
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);
    this->setAttribute(Qt::WA_TranslucentBackground);

    this->setProperty("rotateValue", 0);
    m_frontWindow = new QFrontWindow(this);
    connect(m_frontWindow, SIGNAL(rotateWindow()), this, SLOT(onRotateWindow()));
    connect(m_frontWindow, SIGNAL(closeWindow()), this, SLOT(close()));

    m_backWindow = new QBackWindow(this);
    connect(m_backWindow, SIGNAL(rotateWindow()), this, SLOT(onRotateWindow()));
    connect(m_backWindow, SIGNAL(closeWindow()), this, SLOT(close()));

    this->addWidget(m_frontWindow);
    this->addWidget(m_backWindow);
    this->setStyleSheet("QWlan { padding-bottom:100px; }");
    this->setFixedSize(QSize(m_frontWindow->width(), m_frontWindow->height() + 100));

    // 初始化Qhotspot
    m_isOn = m_hotspot.init();
    if (m_isOn) {
        m_frontWindow->disableEditInput();
        m_frontWindow->setClose();
    }
    m_frontWindow->setConfig(m_hotspot.getSsid(), m_hotspot.getPassword());

    // 点击事件，开启或关闭热点
    connect(m_frontWindow, &QFrontWindow::setStartOrClose, [=]() {
        if (m_isOn) {
            emit stopHotspot();
        } else {
            emit startHotspot();
        }
        m_isOn ^= 1;
    });

    // 点击翻页
    connect(m_frontWindow, &QFrontWindow::turnNextPage, [=]() {
        m_backWindow->addClientList(m_hotspot.getClient());
    });

    // 开启和关闭热点
    connect(this, SIGNAL(startHotspot()), &m_hotspot, SLOT(start()));
    connect(this, SIGNAL(stopHotspot()), &m_hotspot, SLOT(stop()));

    // 开启前
    connect(&m_hotspot, SIGNAL(starting()), m_frontWindow, SLOT(setWait()));
    connect(&m_hotspot, SIGNAL(starting()), m_frontWindow, SLOT(disableBtnClick()));

    // 开启后
    connect(&m_hotspot, SIGNAL(started()), m_frontWindow, SLOT(setClose()));
    connect(&m_hotspot, SIGNAL(started()), m_frontWindow, SLOT(enableBtnClick()));

    // 停止前
    connect(&m_hotspot, SIGNAL(stopping()), m_frontWindow, SLOT(disableBtnClick()));

    // 停止后
    connect(&m_hotspot, SIGNAL(stopped()), m_frontWindow, SLOT(setStart()));
    connect(&m_hotspot, SIGNAL(stopped()), m_frontWindow, SLOT(enableBtnClick()));

    m_thread = new QThread(this);
    m_hotspot.moveToThread(m_thread);
    m_thread->start();
}

QWlan::~QWlan() {
    m_thread->quit();
    m_thread->wait();
    m_thread->deleteLater();
}

void QWlan::onRotateWindow() {
    if (m_isRoratingWindow)
        return;

    m_isRoratingWindow = true;
    m_nextPageIndex = currentIndex() ^ 1;
    QPropertyAnimation* rotateAnimation = new QPropertyAnimation(this, "rotateValue");
    // 设置旋转持续时间;
    rotateAnimation->setDuration(1500);
    // 设置旋转角度变化趋势;
    rotateAnimation->setEasingCurve(QEasingCurve::InOutQuad);
    // 设置旋转角度范围;
    rotateAnimation->setStartValue(0);
    rotateAnimation->setEndValue(180);
    connect(rotateAnimation, SIGNAL(valueChanged(QVariant)), this, SLOT(repaint()));
    connect(rotateAnimation, SIGNAL(finished()), this, SLOT(onRotateFinished()));
    // 隐藏当前窗口，通过不同角度的绘制来达到旋转的效果;
    currentWidget()->hide();
    rotateAnimation->start();
}

void QWlan::onRotateFinished() {
    // 旋转结束
    m_isRoratingWindow = false;
    setCurrentWidget(widget(m_nextPageIndex));
    repaint();
}

void QWlan::paintEvent(QPaintEvent* event) {
    // 绘制旋转效果
    if (m_isRoratingWindow) {
        // 小于90度时
        long long rotateValue = this->property("rotateValue").toLongLong();
        if (rotateValue <= 90) {
            QPixmap rotatePixmap(currentWidget()->size());
            QPainter painter(this);
            QTransform transform;

            currentWidget()->render(&rotatePixmap);
            painter.setRenderHint(QPainter::Antialiasing);
            transform.translate(width() / 2, 0);
            transform.rotate(rotateValue, Qt::YAxis);
            painter.setTransform(transform);
            painter.drawPixmap(-1 * width() / 2, 0, rotatePixmap);
        }
        // 大于90度时
        else {
            QPixmap rotatePixmap(widget(m_nextPageIndex)->size());
            QPainter painter(this);
            QTransform transform;

            widget(m_nextPageIndex)->render(&rotatePixmap);
            painter.setRenderHint(QPainter::Antialiasing);
            transform.translate(width() / 2, 0);
            transform.rotate(rotateValue + 180, Qt::YAxis);
            painter.setTransform(transform);
            painter.drawPixmap(-1 * width() / 2, 0, rotatePixmap);
        }
    } else {
        return __super::paintEvent(event);
    }
}