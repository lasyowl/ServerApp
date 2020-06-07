#include "CSV.h"

namespace CSV {
	std::list<std::string> SplitBuffer( const char* sSrc, int nSize, char cDelimiter );
}

namespace CSV {

	const int	CSV_BUFFER_SIZE = 1024;
	const int	CSV_HEADER_SIZE = 64;
	const char	CSV_DELIMITER_COMMA = ',';
	const char	CSV_DELIMITER_EOL = '\n';
	const char	CSV_DELIMITER_NULL = '\0';

	std::list<std::unordered_map<std::string, std::string> > Load( std::string sFilePath )
	{
		std::list<std::unordered_map<std::string, std::string> > lmTable;
		char sBuf[ CSV_BUFFER_SIZE ];

		std::ifstream file( sFilePath );

		file.getline( sBuf, CSV_BUFFER_SIZE );
		std::list<std::string> lsHeader = SplitBuffer( sBuf, CSV_BUFFER_SIZE, CSV_DELIMITER_COMMA );

		while ( true ) {
			file.getline( sBuf, CSV_BUFFER_SIZE );
			if ( sBuf[ 0 ] == CSV_DELIMITER_NULL ) break;
			std::list<std::string> lsCell = SplitBuffer( sBuf, CSV_BUFFER_SIZE, CSV_DELIMITER_COMMA );

			std::unordered_map<std::string, std::string> mRow;
			auto iterHeader = lsHeader.begin();
			auto iterCell = lsCell.begin();
			for ( ; iterHeader != lsHeader.end(); ++iterHeader, ++iterCell ) {
				if ( !iterCell->empty() ) {
					mRow.emplace( std::make_pair( *iterHeader, *iterCell ) );
				}
			}
			lmTable.emplace_back( mRow );
		}

		return lmTable;
	}

	std::list<std::string> SplitBuffer( const char* sSrc, int nSize, char cDelimiter )
	{
		char sBuf[ CSV_HEADER_SIZE ];
		std::list<std::string> lsSplit;

		for ( int srcIdx = 0, bufIdx = 0; srcIdx < nSize; ++srcIdx ) {

			if ( sSrc[ srcIdx ] == cDelimiter ) {
				sBuf[ bufIdx ] = CSV_DELIMITER_NULL;
				bufIdx = 0;
				lsSplit.emplace_back( sBuf );

				continue;
			}

			sBuf[ bufIdx ] = sSrc[ srcIdx ];
			++bufIdx;

			if ( sSrc[ srcIdx ] == CSV_DELIMITER_NULL ) {
				if ( srcIdx != 0 ) lsSplit.emplace_back( sBuf );
				break;
			}
		}

		return lsSplit;
	}

};