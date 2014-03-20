#include "packetjoin.h"

PacketJoin:: PacketJoin(const QString& channel)
    : Packet(JOIN),
      channel_(channel)
{
}

const char * PacketJoin::getPayload()
{
    return channel_.toUtf8().constData();
}

int PacketJoin::payloadSize()
{
    return channel_.size()+1;
}
