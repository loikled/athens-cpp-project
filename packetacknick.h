#ifndef PACKETACKNICK_H
#define PACKETACKNICK_H
#include "packet.h"

#include <QString>

class PacketAckNick : public Packet
{
private:
    QString nick_;
    bool ack_;
public:
    PacketAckNick(const QString& nick);
    friend QDataStream& operator>>(QDataStream& stream, PacketAckNick& pack);
    bool acked();
    void setPayload(char* payload);
    int payloadSize();
    const char* getPayload();

};

#endif // PACKETACKNICK_H
