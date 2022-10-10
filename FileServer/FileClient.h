#pragma once
#pragma comment(lib, "Ws2_32.lib")
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <WinInet.h>
#include <sys/types.h>
#include "Protocol.h"


struct FileRequest
{

	uint32_t m_file_id           = 0;
	uint32_t m_size_decompressed = 0;
	uint32_t m_size_compressed   = 0;
	uint32_t m_crc               = 0;
	uint32_t m_size_downloaded   = 0;
	bool     m_decompressed      = false;
	uint8_t* m_buffer            = nullptr;

	inline bool IsCompleted() { return m_size_downloaded >= m_size_compressed; }
	float GetSizeInPercent() 
	{ 
		if (m_size_compressed == 0)
			return 0.f;

		float size_loaded = static_cast<float>(m_size_downloaded);
		float size_needed = static_cast<float>(m_size_compressed);

		return size_loaded / size_needed;
	}
};


class FileClient
{
public:
	FileClient();
	~FileClient() { if (IsConnected()) Close(); }

	bool Connect();
	bool Close();
	bool Download(uint32_t id, FileRequest* out, uint32_t curr_version = 0);

	inline bool   IsConnected() { return m_socket != 0; }
	inline char*  GetDomain()   { return m_domain; }

private:	
	bool  SendHandshake(CtoFS::GameType type = CtoFS::GameType::GuildWars1);
	bool  SendRequest(uint32_t id, uint32_t curr_version, FStoC::FileDetails* out);
	bool  SendRequestMore(uint32_t offset);
	
	int      Send(void* packet, int size);
	uint16_t Recv(const unsigned int len);

private:
	#define MAX_BUFFER_LEN 0xFFFF

	SOCKET           m_socket        = 0;
	FStoC::Handshake m_handshakeData = { 0 };	
	WSAData          m_wsaData       = { 0 };
	char             m_domain[64];
	char             m_buffer[MAX_BUFFER_LEN];
};

