#include "packetmessage.h"

PacketMessage::PacketMessage(const QString& from,
                             const QString& to,
                             const QString& msg)
    : Packet(MESSAGE),
      res_(0),
      size_(0)
{
    size_ = from.size()+1+to.size()+1+msg.size()+1;
    char * result = new char[size_];
    res_ = result;
    memcpy(result, from.toUtf8().data(), from.size()+1);
    result+=from.size()+1;
    memcpy(result, to.toUtf8().data(), to.size()+1);
    result += to.size() +1;
    memcpy(result, msg.toUtf8().data(), msg.size()+1);
}

PacketMessage::~PacketMessage()
{
    if (res_)
        delete res_;
}

const char* PacketMessage::getPayload()
{
    return res_;
}

int PacketMessage::payloadSize()
{
    return size_;
}

void PacketMessage::setPayload(char* payload)
{
    QString from;
    QString to;
    QString mess;
    quint32 size = header_.length - HEADER_LENGTH;
    quint32 i = 0;
    char current_char = payload[0];
    while (i<size && current_char != 0)
    {
        from.append(current_char);
        current_char = payload[++i];
    }
    current_char = payload[++i];
    while (i<size && current_char != 0)
    {
        to.append(current_char);
        current_char = payload[++i];
    }
    current_char = payload[++i];
    while (i<size && current_char != 0)
    {
        mess.append(current_char);
        current_char = payload[++i];
    }
    from_ = from;
    mess_ = mess;
}

QString PacketMessage::getMessage()
{
    return mess_;
}

QString PacketMessage::from()
{
    return from_;
}
