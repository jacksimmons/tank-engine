#include "file.hpp"
#include "log.hpp"


namespace Tank
{
	namespace File
	{
		bool exists(const std::filesystem::path &fp)
		{
			return std::filesystem::exists(fp);
		}


		ReadResult readLines(const std::filesystem::path &fp, std::string &outStr)
		{
			if (!File::exists(fp)) return ReadResult::NoFile;

			try
			{
				std::ifstream in(fp);
				std::string contents;
				std::string line;
				while (std::getline(in, line))
				{
					contents.append(line);
					contents.push_back('\n');
				}
				outStr = contents;
				return ReadResult::Success;
			}
			catch (std::exception const)
			{
				return ReadResult::Error;
			}
		}


		bool writeLines(const std::filesystem::path &fp, const std::string &inStr)
		{
			try
			{
				std::ofstream out(fp);
				out.write(inStr.c_str(), inStr.size());
				return true;
			}
			catch (std::exception const)
			{
				return false;
			}
		}
	
	
		char* readBytes(const std::filesystem::path &fp, int *outSize)
		{
			// ate: Seek to end of stream immediately after open
			std::ifstream stream(fp, std::ios::binary | std::ios::ate);

			if (!stream)
			{
				TE_CORE_ERROR("Failed to open file");
				return nullptr;
			}

			std::streampos end = stream.tellg(); // Start at end of stream
			stream.seekg(0, std::ios::beg); // Go to beginning
			int size = end - stream.tellg();

			if (size == 0)
			{
				return nullptr;
			}

			char *buf = new char[size];
			stream.read(buf, size);
			stream.close();

			*outSize = size;
			return buf;
		}
	}
}