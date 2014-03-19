#include "packetnick.h"

PacketNick:: PacketNick(const QString& nick, PacketType_t type)
    : Packet(type),
      nick_(nick)
{
}

const char * PacketNick::getPayload()
{
    return nick_.toUtf8().constData();
}

int PacketNick::payloadSize()
{
    return nick_.size() +1;
}
