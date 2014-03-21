#ifndef PROTOCOLHANDLER_H
#define PROTOCOLHANDLER_H

#include <QObject>
#include <QTcpSocket>
#include <QString>
#include <QTimer>

#include "packet.h"

class ProtocolHandler : public QObject
{
    Q_OBJECT
private:
    QTcpSocket sock_;
    QString host_;
    QString port_;
    QString nickname_;
    QString currentChan_;
    QTimer timer_;
    QByteArray inBuf_;
    bool registered_;

public:
    explicit ProtocolHandler(QObject *parent = 0);
    void setHostAndPort(const QString& host, const QString& Port);
    void connectToServer();
    void disconnectFromServer();
    void sendMessage(const QString& mess);
    QString getTextMsg();

private:
    void sendPacket(Packet& packet);

signals:
    void connected();
    void NewMessage(const QString& mess);

public slots:
    void slotConnected();
    void slotRead();
    void slotError(QAbstractSocket::SocketError e);

};

#endif // PROTOCOLHANDLER_H
