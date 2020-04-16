#pragma once

#include <QObject>

class QHotspot : public QObject {
    Q_OBJECT

public:
    QHotspot(QObject* parent = Q_NULLPTR);
    ~QHotspot();
    bool init();
    void config(const QString& ssid, const QString& password);
    QString getSsid() const { return m_originSsid; };
    QString getPassword() const { return m_originPassword; };
    QList<QPair<QString, QString>> getClient();

public slots:
    void start();
    void stop();

signals:
    void starting();
    void started();
    void stopping();
    void stopped();
    void failed();

private:
    QString m_originSsid;
    QString m_originPassword;
};
