#include "packetjoin.h"

PacketJoin:: PacketJoin(const QString& channel)
    : Packet(JOIN),
      channel_(channel)
{
}

const char * PacketJoin::getPayload()
{
    //get data aligned on bytes
    return channel_.toUtf8().constData();
}

int PacketJoin::payloadSize()
{
    return channel_.size()+1;
}
