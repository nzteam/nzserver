#include "mupacket.h"

namespace nz {

MuPacket::MuPacket()
{
    this->clear();
}

MuPacket::MuPacket(ByteBuffer buffer)
{
    this->clear();

    this->explain(buffer);
}

void MuPacket::clear()
{
    this->m_buffer.clear();
    this->m_type = 0;
    this->m_protocol = 0;
    this->m_subprotocol = 0;
    this->m_body.clear();
}

bool MuPacket::explain(ByteBuffer buffer)
{
    this->m_buffer = buffer;
    this->m_type = this->m_buffer.readByte();

    switch(this->m_type)
    {

        case 0xC1:
            this->explainC1();
            break;

        case 0xC2:
            this->explainC2();
            break;

        case 0xC3:
            break;

        case 0xC4:
            break;

        default:
            Application::addLog("[MuPacket] Invalid packet type!");
            return false;
            break;

    }

    return true;
}

void MuPacket::explainC1()
{
    this->m_buffer.rewind();
    this->m_type = this->m_buffer.readByte();
    int size = (int)this->m_buffer.readByte();
    this->m_protocol = this->m_buffer.readByte();
    this->m_subprotocol = this->m_buffer.readByte();

    if(size != this->m_buffer.getSize())
    {
        Application::addLog("[MuPacket] [C1] Invalid packet size! (PACKET: %d | BUFFER: %d)", size, this->m_buffer.getSize());
        this->clear();
        return;
    }

    if((size - 4) > 0)
    {
        unsigned char body[size - 4];
        memcpy(body, (this->m_buffer.getBuffer())+4, (size - 4));

        this->m_body.insert(body, sizeof(body));
        this->m_body.rewind();
    }
    else
    {
        this->m_body.clear();
    }

    this->m_buffer.clear();
}

void MuPacket::explainC2()
{
    this->m_buffer.rewind();
    this->m_type = this->m_buffer.readByte();
    int size = (int)this->m_buffer.readUShort();
    this->m_protocol = this->m_buffer.readByte();
    this->m_subprotocol = this->m_buffer.readByte();

    if(size != this->m_buffer.getSize())
    {
        Application::addLog("[MuPacket] [C2] Invalid packet size! (PACKET: %d | BUFFER: %d)", size, this->m_buffer.getSize());
        this->clear();
        return;
    }

    if((size - 5) > 0)
    {
        unsigned char body[size - 5];
        memcpy(body, (this->m_buffer.getBuffer())+5, (size - 5));

        this->m_body.insert(body, sizeof(body));
        this->m_body.rewind();
    }
    else
    {
        this->m_body.clear();
    }

    this->m_buffer.clear();
}

ByteBuffer* MuPacket::getBody()
{
    return &this->m_body;
}

void MuPacket::prepare(unsigned char type, unsigned char protocol, unsigned char subprotocol)
{
    this->clear();

    this->m_type = type;
    this->m_protocol = protocol;
    this->m_subprotocol = subprotocol;
}

unsigned char* MuPacket::getBuffer()
{
    this->m_buffer.clear();

    this->m_buffer.writeByte(this->m_type);

    if(this->m_type == 0xC1)
    {
        this->m_buffer.writeByte(this->getSize());
    }
    else if(this->m_type == 0xC2)
    {
        this->m_buffer.writeUShort(this->getSize());
    }
    else
    {
        Application::addLog("[MuPacket] Invalid packet type!");
        this->m_buffer.clear();
        return 0;
    }

    this->m_buffer.writeByte(this->m_protocol);
    this->m_buffer.writeByte(this->m_subprotocol);

    this->m_buffer.insert(this->m_body.getBuffer(), this->m_body.getSize());

    return this->m_buffer.getBuffer();
}

int MuPacket::getSize()
{
    if(this->m_type == 0xC1)
    {
        return (4 + this->m_body.getSize());
    }
    else if(this->m_type == 0xC2)
    {
        return (5 + this->m_body.getSize());
    }
    else
    {
        Application::addLog("[MuPacket] Invalid packet type!");
        return 0;
    }
}

unsigned char MuPacket::getType()
{
    return this->m_type;
}

unsigned char MuPacket::getProtocol()
{
    return this->m_protocol;
}

unsigned char MuPacket::getSubprotocol()
{
    return this->m_subprotocol;
}

}
