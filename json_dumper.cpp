#include "json_dumper.hpp"

#include <fstream>
#include <iomanip>



void JsonDumper::Dump( std::string& name, float windSpeed, float windDirection )
{
	std::time_t time = std::time( nullptr );
	std::ofstream( filename.c_str(), std::ios_base::app )
#pragma warning(suppress : 4996)
		<< "{\n  \"time\": \"" << std::put_time( std::gmtime( &time ), "%F %T" )
		<< "\",\n  \"name\": \"" << name
		<< "\",\n  \"speed\": \"" << windSpeed
		<< "\",\n  \"direction\": \"" << windDirection
		<< "\",\n}\n";
}
