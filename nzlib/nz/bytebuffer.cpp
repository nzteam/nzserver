#include "bytebuffer.h"

namespace nz {

ByteBuffer::ByteBuffer()
{
    this->clear();
}

ByteBuffer::ByteBuffer(unsigned char* buffer, int size)
{
    this->clear();
    this->insert(buffer, size);
}

void ByteBuffer::insert(unsigned char* buffer, int size)
{
    for(int i = 0; i < size; i++)
    {
        this->m_buffer.push_back(buffer[i]);
    }

    this->rewind();
}

void ByteBuffer::clear()
{
    this->m_buffer.clear();
}

void ByteBuffer::rewind()
{
    this->m_iterator = this->m_buffer.begin();
}

void ByteBuffer::end()
{
    this->m_iterator = this->m_buffer.end();
}

int ByteBuffer::getSize()
{
    return this->m_buffer.size();
}

unsigned char* ByteBuffer::getBuffer()
{
    int size = this->getSize();
    unsigned char* buffer = new unsigned char[size];
    int temp = 0;

    for(auto it = this->m_buffer.begin(); it != this->m_buffer.end(); it++)
    {
        buffer[temp] = (*it);
        temp++;
    }

    return buffer;
}

// writing
void ByteBuffer::writeByte(int byte)
{
    this->m_buffer.push_back((unsigned char)byte);
}

void ByteBuffer::writeByte(char byte)
{
    this->m_buffer.push_back((unsigned char)byte);
}

void ByteBuffer::writeByte(unsigned char byte)
{
    this->m_buffer.push_back((unsigned char)byte);
}

void ByteBuffer::writeShort(int byte)
{
    char temp[2];
    temp[1] = (byte >> 8) & 0xFF;
    temp[0] = (byte & 0xFF);
    this->m_buffer.push_back((unsigned char)temp[0]);
    this->m_buffer.push_back((unsigned char)temp[1]);
}

void ByteBuffer::writeUShort(int byte)
{
    char temp[2];
    temp[0] = (byte >> 8) & 0xFF;
    temp[1] = (byte & 0xFF);
    this->m_buffer.push_back((unsigned char)temp[0]);
    this->m_buffer.push_back((unsigned char)temp[1]);
}

void ByteBuffer::writeString(std::string bytes, int size)
{
    if((int)bytes.size() > size)
    {
        bytes.resize(size);
    }

    int btSize = bytes.size();

    for(int i = 0; i < size; i++)
    {
        if(i < btSize)
        {
            this->m_buffer.push_back(bytes[i]);
        }
        else
        {
            this->m_buffer.push_back(0x00);
        }
    }
}

// readning
unsigned char ByteBuffer::readByte()
{
    unsigned char result = (unsigned char)(*this->m_iterator);
    this->m_iterator++;

    return result;
}

short ByteBuffer::readShort()
{
    unsigned char temp[2];
    temp[0] = (unsigned char)(*this->m_iterator);
    this->m_iterator++;
    temp[1] = (unsigned char)(*this->m_iterator);
    this->m_iterator++;

    short result = ((temp[1] * 256) + temp[0]);

    return result;
}

unsigned short ByteBuffer::readUShort()
{
    unsigned char temp[2];
    temp[0] = (unsigned char)(*this->m_iterator);
    this->m_iterator++;
    temp[1] = (unsigned char)(*this->m_iterator);
    this->m_iterator++;

    unsigned short result = (temp[0] * 256) + temp[1];

    return result;
}


}
