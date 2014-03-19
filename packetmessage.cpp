#include "packetmessage.h"

PacketMessage::PacketMessage(const QString& from,
                             const QString& to,
                             const QString& msg,
                             Packet::PacketType_t type)
    : Packet(type),
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
