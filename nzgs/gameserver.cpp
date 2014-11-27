#include "gameserver.h"

void GameServer::before()
{
    this->m_gameServerVersion = nz::Configuration::getString("PROTOCOL", "version", "0.00.00");
}

void GameServer::after()
{
    //
}

void GameServer::onClientConnect(int id)
{
    nz::MuPacket packet;
    packet.prepare(0xC1, 0xF1, 0x00);
    packet.getBody()->writeByte(1);
    packet.getBody()->writeUShort(id);
    packet.getBody()->writeByte(this->m_gameServerVersion[0]);
    packet.getBody()->writeByte(this->m_gameServerVersion[2]);
    packet.getBody()->writeByte(this->m_gameServerVersion[3]);
    packet.getBody()->writeByte(this->m_gameServerVersion[5]);
    packet.getBody()->writeByte(this->m_gameServerVersion[6]);
    this->writeToClient(id, packet.getBuffer(), packet.getSize());
}

void GameServer::onClientDisconnect(int id)
{
    //
}

void GameServer::onClientWrite(int id, unsigned char* buffer, int size)
{
    nz::MuPacket packet;

    if(packet.explain(nz::ByteBuffer(buffer, size)))
    {
        if(packet.getProtocol() == 0xC3 || packet.getProtocol() == 0xC4)
        {
            nz::Application::addLog("[TCPServer] Packet is packed!");
        }
        else
        {
            nz::Application::addLog("[TCPServer] Client try use unknown method! (ID: %d | PROTOCOL: %02X | SUBPROTOCOL: %02X)", id, packet.getProtocol(), packet.getSubprotocol());
        }
    }
    else
    {
            nz::Application::addLog("[TCPServer] Client send invalid packet! (ID: %d | SIZE: %d)", id, size);
            this->closeClient(id);
    }
}
