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

void Packet::setHeader(QDataStream& stream)
{
    quint32 length;
    stream>>length;
    quint32 type;
    stream>>type;

    quint16 bd_chk;
    stream>>bd_chk;

    quint16 hd_chk;
    stream>>hd_chk;
    header_.length = length;
    header_.type = type;
    header_.body_checksum = bd_chk;
    header_.head_checksum = hd_chk;
}

 void Packet::setHeader(message_header_t h)
 {
    header_ = h;
 }

 bool Packet::checkHeader()
 {
     bool ok = false;
     quint16 headerChecksum = header_.head_checksum;
     header_.head_checksum = 0;
     quint16 hchk = fletcher16((quint8*)&header_,HEADER_LENGTH);
     ok = hchk == headerChecksum;
     header_.head_checksum = headerChecksum;
     return ok;
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

QDataStream& operator>>(QDataStream& stream, Packet& pack)
{
    pack.getHeader(stream);
    return stream;
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

QString Packet::getArg(char *ptr)
{
    QString res;
    char c = 1;
    while(c != 0)
    {
        c = *ptr++;
        res.append(c);
    }
    return res;
}
