#include "QBackWindow.h"
#include <QLabel>
#include <QPair>
#include <QVBoxLayout>

QBackWindow::QBackWindow(QWidget* parent)
    : QBaseWindow(parent) {
    ui.setupUi(this);
    this->loadStyleSheet(":/QWlan/Resources/back.qss");
    ui.list_container->setDisabled(true);
}

QBackWindow::~QBackWindow() {
}

void QBackWindow::addClientList(const QList<QPair<QString, QString>>& list) {
    clearList();
    for (auto client : list) {
        pushList(client.first, client.second);
    }
}

void QBackWindow::clearList() {
    ui.list_container->clear();
}

void QBackWindow::pushList(const QString& clientName, const QString& macAddress) {
    QListWidgetItem* item = new QListWidgetItem(ui.list_container);
    item->setSizeHint(QSize(0, 33));
    item->setFlags(Qt::NoItemFlags);
    ui.list_container->addItem(item);

    QVBoxLayout* layout = new QVBoxLayout();
    QWidget* container = new QWidget();
    QLabel* title = new QLabel(QStringLiteral("\u25cf ") + clientName);
    title->setStyleSheet("QLabel { color:#7500A5; font-family: Consolas; font-size: 16px; }");
    QLabel* footer = new QLabel(macAddress);
    footer->setStyleSheet("QLabel { color:#7500A5; font-family: Consolas; font-size: 12px; margin-left: 16px; }");
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(title);
    layout->addWidget(footer);

    container->setLayout(layout);
    ui.list_container->setItemWidget(item, container);
}