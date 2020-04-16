#pragma once
#include <QDialog>

class QBaseWindow : public QDialog {
    Q_OBJECT

public:
    QBaseWindow(QWidget* parent = Q_NULLPTR);
    ~QBaseWindow();

signals:
    void rotateWindow();
    void closeWindow();

private slots:
    void onBtnRotate();
    void onBtnExit();

protected:
    void paintEvent(QPaintEvent* event) Q_DECL_OVERRIDE;
    bool eventFilter(QObject* obj, QEvent* evt);
    void loadStyleSheet(const QString& sheetName);
};
