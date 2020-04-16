#include "qwifiicon.h"
#include <QPainter>
#include <QStringList>
#include <QStyleOption>
#include <QTimer>

QWifiIcon::QWifiIcon(QWidget* parent)
    : QWidget(parent) {
    // 初始化定时器
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, [=]() {
        static QStringList pendingList = {"pending-0", "pending-1", "pending-2", "pending-3"};
        static int count = 1;
        this->setWhatsThis(pendingList[count]);
        this->style()->unpolish(this);
        this->style()->polish(this);
        this->update();
        count = (count + 1) % 4;
    });
}

void QWifiIcon::paintEvent(QPaintEvent*) {
    // 设置窗口透明后，需要添加如下代码，以实现绘制样式表(qt style sheet)
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void QWifiIcon::setState(const QString& state) {
    if (timer->isActive())
        timer->stop();
    this->setWhatsThis(state);
    this->style()->unpolish(this);
    this->style()->polish(this);
    this->update();
}

void QWifiIcon::ready() {
    this->setState("ready");
}

void QWifiIcon::pendding() {
    if (!timer->isActive())
        timer->start(200);
}

void QWifiIcon::success() {
    this->setState("success");
}

void QWifiIcon::failed() {
    this->setState("failed");
}