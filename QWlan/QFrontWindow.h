#pragma once

#include "QBaseWindow.h"
#include "ui_QFrontWindow.h"

class QFrontWindow : public QBaseWindow {
    Q_OBJECT

public:
    QFrontWindow(QWidget* parent = Q_NULLPTR);
    ~QFrontWindow();
    void setConfig(const QString& ssid, const QString& password);

public slots:
    void setStart();
    void setClose();
    void setWait();
    void enableBtnClick();
    void disableBtnClick();
    void enableEditInput();
    void disableEditInput();
    void onBtnStartOrStop();
    void onTurnNextPage();

signals:
    void setStartOrClose();
    void turnNextPage();

protected:
    void setBtnStartOrStopState(const QString& state);

private:
    Ui::QFrontWindow ui;
};
