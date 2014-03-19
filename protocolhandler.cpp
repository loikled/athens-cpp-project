#include <QDataStream>
#include "protocolhandler.h"
#include "packetnick.h"
#include "packetjoin.h"

ProtocolHandler::ProtocolHandler(QObject *parent) :
    QObject(parent)
{
    connect(&sock_,SIGNAL(connected()),this,SLOT(slotConnected()));
}

void ProtocolHandler::setHostAndPort(const QString& hostAdd, const QString& port)
{
    host_ = hostAdd;
    port_ = port;
}

void ProtocolHandler::connectToServer()
{
    sock_.connectToHost(host_,port_.toInt());
}

void ProtocolHandler::disconnectFromServer()
{
    sock_.disconnectFromHost();
}

void ProtocolHandler::sendMessage(const QString& mess)
{
    if (mess.contains("/nick"))
    {
        QStringList list = mess.split(" ");
        QString nick = list[1];
        qDebug()<<"nick: "<<nick;
        PacketNick pack(nick);
        QByteArray array;
        QDataStream stream (&array, QIODevice::WriteOnly);
        stream<<pack;
        qDebug()<<"raw Packet: "<<array;
        sock_ << pack;
    }
    else if (mess.contains("/join"))
    {
        QStringList list = mess.split(" ");
        QString channel = list[1];
        qDebug()<<"channel: "<<channel;
        PacketJoin pack(channel);
        QByteArray array;
        QDataStream stream (&array, QIODevice::WriteOnly);
        stream<<pack;
        qDebug()<<"raw Packet: "<<array;
        sock_ << pack;
    }
}

void ProtocolHandler::slotConnected()
{
    emit connected();
}

const QString& ProtocolHandler::getTextMsg()
{
    return QString("Message received");
}
