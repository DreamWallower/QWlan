#pragma once

#include "QBaseWindow.h"
#include "ui_QBackWindow.h"

class QBackWindow : public QBaseWindow {
    Q_OBJECT

public:
    QBackWindow(QWidget* parent = Q_NULLPTR);
    ~QBackWindow();
    void addClientList(const QList<QPair<QString, QString>>& list);

protected:
    void clearList();
    void pushList(const QString& clientName, const QString& macAddress);

private:
    Ui::QBackWindow ui;
};
