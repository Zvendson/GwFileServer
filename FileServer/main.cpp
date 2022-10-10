#include <stdio.h>
#include "FileClient.h"
#include <fstream>
#include <string>
#include <iostream>


int main(int argc, char const* argv[])
{
	std::cout << "##########################\n";
	std::cout << "# Guild Wars File Server #\n";
	std::cout << "##########################\n\n";
	std::cout << "Usage: Type id and name\nExample:\nFile id: 358542\nFile name: Royal Gift\n\n(Id = 0 will exit)\n\n";

	auto client = FileClient();
	if (client.Connect())
	{
		bool run = true;
		while (run)
		{
			std::string fileName;
			uint32_t file_id;

			std::cout << "File id: ";
			std::cin >> file_id;
			if (file_id == 0)
				break;
			std::cout << "File name: ";
			std::getline(std::cin >> std::ws, fileName);
			
			CreateDirectoryA("Downloads", NULL);
			char buffer[MAX_PATH];
			if (fileName.empty())
			{
				snprintf(buffer, sizeof(buffer), "Downloads\\file_%X.raw", file_id);
			}
			else
			{
				snprintf(buffer, sizeof(buffer), "Downloads\\%s", fileName.c_str());
			}
			fileName = buffer;

			FileRequest file;
			if (client.Download(file_id, &file))
			{
				if (file.Decompress())
				{
					std::ofstream file_out;
					file_out.open(fileName, std::ios::binary | std::ios::out);
					file_out.write((char*)file.m_decompress_buffer, file.m_size_decompressed);
					file_out.close();
					printf("Download saved in '%s'\n", fileName.c_str());
					delete[] file.m_decompress_buffer;
				}
				delete[] file.m_buffer;
			}
		}
		client.Close();
		return 0;
	}
}