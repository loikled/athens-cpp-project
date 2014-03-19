#ifndef PACKETMESSAGE_H
#define PACKETMESSAGE_H
#include <QByteArray>
#include "packet.h"

class PacketMessage : public Packet
{
private:
    char* res_;
    int size_;

public:
    PacketMessage(const QString& from,
                  const QString& to,
                  const QString& msg,
                  Packet::PacketType_t type = MESSAGE);
    ~PacketMessage();
    const char *getPayload();
    int payloadSize();
};

#endif // PACKETMESSAGE_H
