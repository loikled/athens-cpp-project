#include "packet.h"

Packet::Packet(PacketType_t type)
{
    header_.length = 0;
    header_.type = type;
    header_.body_checksum = 0;
    header_.head_checksum = 0;
}

Packet::~Packet()
{
}

quint32 Packet::getLength()
{
    return header_.length;
}

quint32 Packet::getType()
{
    return header_.type;
}

const char* Packet::getPayload()
{
    return payload_;
}

void Packet::getHeader(QDataStream& stream)
{
    stream<<header_.length;
    stream<<header_.type;
    stream<<header_.body_checksum;
    stream<<header_.head_checksum;
}

QDataStream& operator<<(QDataStream& stream, Packet& pack)
{
    pack.build();
    pack.getHeader(stream);
    stream<<pack.getPayload();
    return stream;
}

QTcpSocket& operator<<(QTcpSocket& sock, Packet& pack)
{
    pack.build();
    sock.write((char*)&pack.header_, Packet::HEADER_LENGTH);
    sock.write(pack.getPayload(),pack.header_.length - Packet::HEADER_LENGTH);
    return sock;
}

void Packet::build()
{
    header_.body_checksum = 0;
    header_.head_checksum = 0;
    //+1 for null char not counted in QString.Size()
    header_.length = Packet::HEADER_LENGTH + payloadSize();
    header_.body_checksum = fletcher16((quint8*)getPayload(),payloadSize());
    header_.head_checksum = fletcher16((quint8*)&header_,12);
}
