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
    QString nick;
    QString ack;
    quint32 size = header_.length - HEADER_LENGTH;
    quint32 i = 0;
    char current_char = payload[0];
    while (i<size && current_char != 0)
    {
        nick.append(current_char);
        current_char = payload[++i];
    }
    current_char = payload[++i];
    while (i<size && current_char != 0)
    {

        ack.append(current_char);
        current_char = payload[++i];
    }
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
