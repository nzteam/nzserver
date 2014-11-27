#ifndef MUPACKET_H
#define MUPACKET_H

#include <cstring>
#include "application.h"
#include "bytebuffer.h"

namespace nz {

class MuPacket
{

private:
    ByteBuffer m_buffer;
    unsigned char m_type;
    unsigned char m_protocol;
    unsigned char m_subprotocol;
    ByteBuffer m_body;

public:
    MuPacket();
    MuPacket(ByteBuffer buffer);
    void clear();
    bool explain(ByteBuffer buffer);
    void explainC1();
    void explainC2();
    ByteBuffer* getBody();
    void prepare(unsigned char type, unsigned char protocol, unsigned char subprotocol);
    unsigned char* getBuffer();
    int getSize();
    unsigned char getType();
    unsigned char getProtocol();
    unsigned char getSubprotocol();

};

}

#endif // MUPACKET_H
