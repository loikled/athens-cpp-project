#include "packetacknick.h"

PacketAckNick::PacketAckNick(const QString& nick)
    :nick_(nick),
      ack_(false)
{
}

bool PacketAckNick::acked()
{
    return ack_;
}

const char * PacketAckNick::getPayload()
{
    return payload_;
}

int PacketAckNick::payloadSize()
{
    QString ack = ack_ ? "true" :"false";
    return nick_.size() + ack.size()+1;
}

void PacketAckNick::setPayload(char* payload)
{
    const char *cursor = payload;
    QString nick = cursor;
    cursor+=nick.size()+1;
    QString ack = cursor;
    nick_ = nick;
    ack_ = ack == "true"? true : false;
}

QDataStream& operator>>(QDataStream& stream, PacketAckNick& pack)
{
    pack.setHeader(stream);
    stream>>pack.nick_;
    QString ack;
    stream>>ack;
    pack.ack_ = ack == "true"? true : false;
    if (!pack.checkHeader())
        qDebug()<<"Bad header checksum for packet Ack Nick: "<< pack.header_.head_checksum;
    return stream;
}
