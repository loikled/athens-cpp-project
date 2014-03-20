#ifndef PACKETJOIN_H
#define PACKETJOIN_H
#include "packet.h"

class PacketJoin : public Packet
{
    QString channel_;
public:
    PacketJoin(const QString& channel);
    const char *getPayload();
    int payloadSize();
};

#endif // PACKETJOIN_H
