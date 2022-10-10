#include <stdio.h>
#include "FileClient.h"



int main(int argc, char const* argv[])
{
	auto client = FileClient();
	if (client.Connect())
	{
		FileRequest file;
		if (client.Download(344988, &file))
		{
			printf("My download = 0x%X (%d)\n", file.m_size_compressed, file.m_size_compressed);
			delete[] file.m_buffer;
		}

		client.Close();
		return 0;
	}
}