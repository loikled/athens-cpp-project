#ifndef PACKETNICK_H
#define PACKETNICK_H
#include "packet.h"

class PacketNick : public Packet
{
private:
   QString nick_;
public:
    PacketNick(const QString& nick);
    const char *getPayload();
    int payloadSize();
};

#endif // PACKETNICK_H
