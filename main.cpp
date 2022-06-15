#include "com_port_reader.hpp"
#include "json_dumper.hpp"

#include <iostream>
#include <charconv>



static std::string ReceiveMessage( ComPort& port )
{
	// look for message start character
	while (port.ReadByte() != '$');

	std::string result {};
	char ch = port.ReadByte();
	for (int i = 0;
		i < 20 && (ch >= '0' && ch <= '9' || ch == '.' || ch == ',');
		i++)
	{
		result += ch;
		ch = port.ReadByte();
	}

	// check last message character
	if (ch != '\r' && result.size() < 3)
	{
		throw std::runtime_error( std::string("Invalid data: ") + result );
	}

	return result;
}

static void ExtractDataFromMessage( const std::string& message, float& windSpeed, float& windDirection )
{
	size_t commaIndex = message.find_first_of( ',' );
	if (commaIndex != std::string::npos && commaIndex == message.find_last_of( ',' ))
	{
		const char* ptr_begin = message.c_str();
		const char* ptr_comma = ptr_begin + commaIndex;
		const char* ptr_end = ptr_begin + message.size();

		if (std::from_chars( ptr_begin, ptr_comma, windSpeed, std::chars_format::fixed ).ptr != ptr_comma)
		{
			throw std::runtime_error( std::string( "Invalid speed value: " ).append( ptr_begin, ptr_comma ) );
		}
		if (std::from_chars( ptr_comma + 1, ptr_end, windDirection, std::chars_format::fixed ).ptr != ptr_end)
		{
			throw std::runtime_error( std::string( "Invalid direction value: " ).append( ptr_comma + 1, ptr_end ) );
		}
	}
}

int main()
{
	try
	{
		std::filesystem::path dumpPath = L"dump.json";
		JsonDumper dumper( dumpPath );
		std::string deviceName = "WMT700";

		ComPort port( 1 );
		port.Setup();
		while (true)
		{
			try
			{
				std::string msg = ReceiveMessage( port );
				float windSpeed = 0;
				float windDirection = 0;
				ExtractDataFromMessage( msg, windSpeed, windDirection );
				dumper.Dump( deviceName, windSpeed, windDirection );
			}
			catch (std::exception ex)
			{
				std::cerr << ex.what() << std::endl;
			}
		}
	}
	catch (std::exception ex)
	{
		std::cerr << ex.what() << std::endl;
	}
	return 0;
}
