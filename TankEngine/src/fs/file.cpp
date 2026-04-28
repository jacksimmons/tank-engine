#include <exception>
#include <fstream>
#include "fs/file.hpp"
#include "log.hpp"


namespace Tank::File
{
	bool exists(const std::filesystem::path &fp)
	{
		return std::filesystem::is_regular_file(fp) && std::filesystem::exists(fp);
	}


	ReadResult readLines(const std::filesystem::path &fp, std::string &outStr) noexcept
	{
		if (!File::exists(fp)) return ReadResult::NotFile;

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


	std::optional<std::error_code> tryCopy(const fs::path &srcFile, const fs::path &destFile, fs::copy_options opts) noexcept
	{
		std::error_code ec;

		// Check src exists
		if (!fs::exists(srcFile)) ec = std::make_error_code(std::errc::no_such_file_or_directory);
		// Check src is a regular file
		else if (!fs::is_regular_file(srcFile)) ec = std::make_error_code(std::errc::is_a_directory);

		// No errors so far -> try copying
		if (!ec)
		{
			// Create containing directory
			fs::create_directories(destFile.root_directory(), ec);
			// Copy the file
			fs::copy(srcFile, destFile, opts, ec);
		}

		// Output and return any error codes
		if (ec)
		{
			TE_CORE_ERROR(std::format("\"{}\" (Copying from path {} to {})", ec.message(), srcFile.string(), destFile.string()));
			return ec;
		}

		return {};
	}
}