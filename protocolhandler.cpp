#include <QDataStream>
#include "protocolhandler.h"
#include "packetnick.h"
#include "packetjoin.h"
#include "packetmessage.h"
#include "packetacknick.h"

ProtocolHandler::ProtocolHandler(QObject *parent) :
    QObject(parent),
    host_("127.0.0.1"),
    port_("12345"),
    nickname_("loki"),
    currentChan_("#public"),
    registered_(false)
{
    connect(&sock_,SIGNAL(connected()),this,SLOT(slotConnected()));
    connect(&timer_, SIGNAL(timeout()),this, SLOT(slotRead()));
    timer_.start(100);
    inBuf_.clear();
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
    if (mess.startsWith("/nick"))
    {
        QStringList list = mess.split(" ");
        QString nick = list[1];
        qDebug()<<"nick: "<<nick;
        PacketNick pack(nick);

        //test serialize in stream
        QByteArray array;
        QDataStream stream (&array, QIODevice::WriteOnly);
        stream<<pack;
        qDebug()<<"raw Packet: "<<array;

        sendPacket(pack);
        nickname_ = nick;
    }
    else if (mess.startsWith("/join"))
    {
        QStringList list = mess.split(" ");
        QString channel = list[1];
        qDebug()<<"channel: "<<channel;
        PacketJoin pack(channel);

        //test serialize in stream
        QByteArray array;
        QDataStream stream (&array, QIODevice::WriteOnly);
        stream<<pack;
        qDebug()<<"raw Packet: "<<array;

        sendPacket(pack);
        currentChan_ = channel;
    }
    else if(mess.startsWith("/p"))
    {
        QStringList list = mess.split(" ");
        if (list.size() >= 3)
        {
            QString dest = list[1];
            list.pop_front();
            list.pop_front();
            QString message = list.join(" ");
            qDebug()<<"Private msg from: "<<nickname_<<"to: "<<dest<<" "<<message;
            PacketMessage pack(nickname_,dest,message);
            sendPacket(pack);
        }
    }
    else
    {
        PacketMessage pack(nickname_, currentChan_, mess);
        sendPacket(pack);
    }
}

void ProtocolHandler::sendPacket(Packet& packet)
{
    if (sock_.state() == QTcpSocket::ConnectedState)
    {
        sock_<<packet;
    }
}

void ProtocolHandler::slotConnected()
{
    emit connected();
}

QString ProtocolHandler::getTextMsg()
{
    return QString("Message received");
}

void ProtocolHandler::slotRead()
{
    if (sock_.bytesAvailable() > 0)
    {
       char  header[12];
       sock_.read(header,12);
       Packet::message_header_t* h = reinterpret_cast<Packet::message_header_t*>(header);
       int datalen = h->length-Packet::HEADER_LENGTH;
       char * payload = new char[datalen];
       sock_.read(payload,datalen);

       switch((Packet::PacketType_t)h->type)
       {
       case Packet::ACK_CHANGE_NICK:
           {
                PacketAckNick pack("");
                pack.setHeader(*h);
                pack.setPayload(payload);
                registered_ = pack.acked();
                if (registered_)
                {
                    QString mess = QString("NickName %1 registered on server").arg(nickname_);
                    emit NewMessage(mess);
                }
           }
            break;
       case Packet::MESSAGE:
            {
                PacketMessage pack("","","");
                pack.setHeader(*h);
                pack.setPayload(payload);
                QString mess = QString("%0: %1").arg(pack.from()).arg(pack.getMessage());
                if (mess.size() > 0)
                {
                    emit NewMessage(mess);
                }
            }
        default:
           break;
       }
    }
}
