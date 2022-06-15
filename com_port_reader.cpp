#include "com_port_reader.hpp"

#include <Windows.h>
#include <stdexcept>
#include <string>



ComPort::ComPort( unsigned char portNumber )
{
	std::wstring portName = std::wstring( L"\\\\.\\COM" ) + std::to_wstring( portNumber );
	nativePortHandle = ::CreateFile( portName.c_str(), GENERIC_READ | GENERIC_WRITE, 0,
		NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
	

	if (nativePortHandle == INVALID_HANDLE_VALUE)
	{
		if (::GetLastError() == ERROR_FILE_NOT_FOUND)
		{
			throw std::runtime_error( "Serial port does not exist." );
		}
		throw std::runtime_error( "Some other error occurred." );
	}
}

void ComPort::Setup()
{
	DCB dcbSerialParams = { 0 };
	dcbSerialParams.DCBlength = sizeof( dcbSerialParams );
	if (!::GetCommState( nativePortHandle, &dcbSerialParams ))
	{
		throw std::runtime_error( "Getting state error." );
	}
	dcbSerialParams.BaudRate = CBR_2400;
	dcbSerialParams.ByteSize = 8;
	dcbSerialParams.StopBits = ONESTOPBIT;
	dcbSerialParams.Parity = NOPARITY;
	if (!::SetCommState( nativePortHandle, &dcbSerialParams ))
	{
		throw std::runtime_error( "Error setting serial port state." );
	}
}

char ComPort::ReadByte()
{
	DWORD size;
	char buffer;

	if (::ReadFile( nativePortHandle, &buffer, 1, &size, NULL ) && size > 0)
	{
		return buffer;
	}
	else
	{
		throw std::runtime_error( "Port read error." );
	}
}

ComPort::~ComPort()
{
	if (nativePortHandle != INVALID_HANDLE_VALUE)
	{
		::CloseHandle( nativePortHandle );
	}
}
