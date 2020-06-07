#pragma once

#include <string>

namespace Util {
	std::string W2S( std::wstring ws )
	{
		return std::string( ws.begin(), ws.end() );
	}
}