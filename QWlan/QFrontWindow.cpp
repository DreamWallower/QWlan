#include "QFrontWindow.h"
#include <QMouseEvent>
#include <QStyle>
#include <QThread>

QFrontWindow::QFrontWindow(QWidget* parent)
    : QBaseWindow(parent) {
    ui.setupUi(this);
    this->loadStyleSheet(":/QWlan/Resources/front.qss");
    disableEditInput();
}

QFrontWindow::~QFrontWindow() {
}

void QFrontWindow::setConfig(const QString& ssid, const QString& password) {
    // 设置wifi名称和密码
    ui.lineEdit_name->setText(ssid);
    ui.lineEdit_password->setText(password);
}

void QFrontWindow::setStart() {
    // 设置QWifiIcon为      "准备"
    // 设置按钮为            "开启"
    ui.wifi_icon->ready();
    this->setBtnStartOrStopState("start");
}

void QFrontWindow::setClose() {
    // 设置QWifiIcon为      "成功"
    // 设置按钮为            "关闭"
    ui.wifi_icon->success();
    this->setBtnStartOrStopState("close");
}

void QFrontWindow::setWait() {
    // 设置QWifiIcon为      "开启中"
    ui.wifi_icon->pendding();
}

void QFrontWindow::enableBtnClick() {
    // 启用按钮
    ui.button_start_or_stop->setEnabled(true);
}

void QFrontWindow::disableBtnClick() {
    // 禁用按钮
    ui.button_start_or_stop->setDisabled(true);
}

void QFrontWindow::enableEditInput() {
    // 启用编辑框
    ui.lineEdit_name->setEnabled(true);
    ui.lineEdit_password->setEnabled(true);
}

void QFrontWindow::disableEditInput() {
    // 禁用编辑框
    ui.lineEdit_name->setDisabled(true);
    ui.lineEdit_password->setDisabled(true);
}

void QFrontWindow::setBtnStartOrStopState(const QString& state) {
    // 设置按钮的whitsthis属性，以此改变样式
    ui.button_start_or_stop->setWhatsThis(state);
    ui.button_start_or_stop->style()->unpolish(ui.button_start_or_stop);
    ui.button_start_or_stop->style()->polish(ui.button_start_or_stop);
    ui.button_start_or_stop->update();
}

void QFrontWindow::onBtnStartOrStop() {
    // 转发按钮的clicked()信号
    emit setStartOrClose();
}

void QFrontWindow::onTurnNextPage() {
    // 转发按钮的clicked()信号
    emit turnNextPage();
}