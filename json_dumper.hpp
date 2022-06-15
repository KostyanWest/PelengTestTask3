#pragma once

#include <filesystem>



class JsonDumper
{
public:
	JsonDumper( std::filesystem::path& filename )
		: filename( filename )
	{
	}

	void Dump( std::string& name, float windSpeed, float windDirection );

private:
	std::filesystem::path filename;
};
