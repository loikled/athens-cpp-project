#ifndef PACKET_H
#define PACKET_H
#include <QTypeInfo>
#include <QTextStream>
#include <QTcpSocket>

class Packet
{
public:
    typedef enum{
        NOP                 = 0,
        PUBLIC              = 1,
        CHANGE_NICK         = 2,
        PRIVATE             = 3,
        ACK_CHANGE_NICK     = 4,
        MESSAGE             = 5,
        JOIN                = 6,
        PING                = 7,
        PONG                = 8
        }PacketType_t;

    struct message_header_t {
        quint32 length;
        quint32 type; /* message_type_e */
        quint16 body_checksum;
        quint16 head_checksum;
    } __attribute__((packed)); /* do not use padding for alignment */

    static const quint32 HEADER_LENGTH = sizeof(message_header_t);

protected:
    message_header_t header_;
    char* payload_; //message content

public:
    Packet(PacketType_t type = NOP);
    ~Packet();
    void build();
    friend QDataStream& operator<<(QDataStream& stream, Packet& pack);
    friend QTcpSocket& operator<<(QTcpSocket& sock, Packet& pack);

    friend QDataStream& operator>>(QDataStream& stream, Packet& pack);

    quint32 getLength();
    quint32 getType();

    virtual int payloadSize() = 0;
    virtual const char* getPayload() = 0;

    void getHeader(QDataStream& stream);
    void setHeader(QDataStream& stream);
    void setHeader(message_header_t h);
    bool checkHeader();
    /* source : http://en.wikipedia.org/wiki/Fletcher%27s_checksum */
    inline  quint16 fletcher16(quint8* data, int count) {
        qint16 sum1 = 0;
        qint16 sum2 = 0;
        int index;
        for (index = 0; index < count; ++index) {
            sum1 = (sum1 + data[index]) % 255;
            sum2 = (sum2 + sum1) % 255;
        }
        return (sum2 << 8) | sum1;
    }
};

#endif // PACKET_H
