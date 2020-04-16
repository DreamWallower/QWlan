#ifndef QWIFIICON_H
#define QWIFIICON_H

#include <QWidget>

class QWifiIcon : public QWidget {
    Q_OBJECT
public:
    explicit QWifiIcon(QWidget* parent = Q_NULLPTR);

protected:
    void paintEvent(QPaintEvent* event) Q_DECL_OVERRIDE;
    void setState(const QString& state);

public slots:
    void ready();
    void pendding();
    void success();
    void failed();

private:
    QTimer* timer;
};

#endif // QWIFIICON_H
