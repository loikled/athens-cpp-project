#ifndef PACKETMESSAGE_H
#define PACKETMESSAGE_H
#include <QByteArray>
#include "packet.h"

class PacketMessage : public Packet
{
private:
    char* res_;
    int size_;
    QString mess_;
    QString from_;

public:
    PacketMessage(const QString& from,
                  const QString& to,
                  const QString& msg);
    ~PacketMessage();
    const char *getPayload();
    int payloadSize();
    void setPayload(char* payload);
    QString getMessage();
    QString from();
};

#endif // PACKETMESSAGE_H
