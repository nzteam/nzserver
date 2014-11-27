#ifndef BYTEBUFFER_H
#define BYTEBUFFER_H

#include <string>
#include <list>

namespace nz {

class ByteBuffer
{

private:
    std::list<unsigned char> m_buffer;
    std::list<unsigned char>::iterator m_iterator;

public:
    ByteBuffer();
    ByteBuffer(unsigned char* buffer, int size);
    void insert(unsigned char* buffer, int size);
    void clear();
    void rewind();
    void end();
    int getSize();
    unsigned char* getBuffer();

    // writing
    void writeByte(int byte);
    void writeByte(char byte);
    void writeByte(unsigned char byte);
    void writeShort(int byte);
    void writeUShort(int byte);
    void writeString(std::string bytes, int size);

    // readning
    unsigned char readByte();
    short readShort();
    unsigned short readUShort();

};

}

#endif // BYTEBUFFER_H
