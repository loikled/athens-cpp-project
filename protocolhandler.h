#ifndef PROTOCOLHANDLER_H
#define PROTOCOLHANDLER_H

#include <QObject>
#include <QTcpSocket>
#include <QString>

#include "packet.h"

class ProtocolHandler : public QObject
{
    Q_OBJECT
private:
    QTcpSocket sock_;
    QString host_;
    QString port_;

public:
    explicit ProtocolHandler(QObject *parent = 0);
    void setHostAndPort(const QString& host, const QString& Port);
    void connectToServer();
    void disconnectFromServer();
    void sendMessage(const QString& mess);
    const QString& getTextMsg();

signals:
    void connected();
    void NewMessage(const QString& mess);

public slots:
    void slotConnected();

};

#endif // PROTOCOLHANDLER_H
