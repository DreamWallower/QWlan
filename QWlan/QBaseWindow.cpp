#include "QBaseWindow.h"
#include <QFile>
#include <QMouseEvent>
#include <QPainter>
#include <QStyleOption>
#include <qApplication>

QBaseWindow::QBaseWindow(QWidget* parent)
    : QDialog(parent) {
    // FramelessWindowHint          属性设置窗口去除边框
    // WindowMinimizeButtonHint     属性设置在窗口最小化时，点击任务栏窗口可以显示出原窗口
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);
    // 设置窗口透明
    this->setAttribute(Qt::WA_TranslucentBackground);
    // 注册事件过滤器,以实现拖动窗口
    qApp->installEventFilter(this);
}

QBaseWindow::~QBaseWindow() {
}

void QBaseWindow::onBtnRotate() {
    emit rotateWindow();
}

void QBaseWindow::onBtnExit() {
    emit closeWindow();
}

void QBaseWindow::paintEvent(QPaintEvent* event) {
    // 设置窗口透明后，需要添加如下代码，以实现绘制样式表(qt style sheet)
    QStyleOption opt;
    opt.init(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
    return QWidget::paintEvent(event);
}

void QBaseWindow::loadStyleSheet(const QString& sheetName) {
    // 读取样式表文件
    QFile file(sheetName);
    file.open(QFile::ReadOnly);
    if (file.isOpen()) {
        QString styleSheet = this->styleSheet();
        styleSheet += QLatin1String(file.readAll());
        this->setStyleSheet(styleSheet);
        file.close();
    }
}

bool QBaseWindow::eventFilter(QObject* obj, QEvent* evt) {
    QMouseEvent* mouse = dynamic_cast<QMouseEvent*>(evt);
    if (obj == this && mouse) {
        if (this->isMaximized()) {
            return true;
        }

        static bool dragFlag = false;
        static QPoint& dragPoint = QPoint(0, 0);
        QEvent::Type&& eventType = mouse->type();
        // 鼠标拖动
        if (dragFlag && eventType == QEvent::MouseMove) {
            parentWidget()->move(mouse->globalPos() - dragPoint); // 移动父窗口
            return true;
        }
        // 左键按下
        else if (mouse->button() == Qt::LeftButton && eventType == QEvent::MouseButtonPress) {
            dragPoint = mouse->pos();
            dragFlag = true;
            return true;
        }
        // 左键松开
        else if (mouse->button() == Qt::LeftButton && eventType == QEvent::MouseButtonRelease) {
            dragFlag = false;
            return true;
        }
    }

    return QWidget::eventFilter(obj, evt);
}