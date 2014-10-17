// Filter2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <fstream>
#include <iostream>
#include <string>
#include <cctype>

int _tmain(int argc, _TCHAR* argv[])
{
	const wchar_t * wc_default_out_name = _T("aout.bin");
	std::streambuf * pinold = nullptr;
	std::ifstream infile;
	std::ofstream outfile;
	std::wstring default_out_name;
	bool bOk = true;

	if (argc > 1)
	{
		if (0==wcscmp(argv[1], _T("-?")))
		{
			size_t wclen = wcslen(wc_default_out_name) + 1;
			char * dfltname = new char[wclen];
			size_t szRv = 0;

			(void) memset(dfltname, 0, wclen);
			errno_t err = wcstombs_s(&szRv, dfltname, wclen, wc_default_out_name, _TRUNCATE);

			std::cout << "Filter out non-printable characters from input file" << std::endl;
			std::cout << "Use: filter2 [infile] | [< infile] [> outfile]" << std::endl;
			std::cout << "Non-printable content is written to either- '" << dfltname << "', or <inputname>.bin" << std::endl;
			delete[] dfltname;

			return 0;
		}

		infile.open(argv[1]);

		if (infile.is_open())
		{
			pinold = std::cin.rdbuf(infile.rdbuf());
			default_out_name = std::wstring(argv[1]) + std::wstring(_T(".bin"));
		}
		else
		{
			std::cerr << "ERROR: Unable to open file- " << argv[1];
			bOk = false;
		}
	}
	else
	{
		default_out_name = wc_default_out_name;
	}

	outfile.open(default_out_name);

	if (bOk && std::cin.good() && outfile.good())
	{
		const size_t sizeofBuffer = 256U;
		char * buffer = new char[sizeofBuffer];

		if (nullptr != buffer)
		{
			while (!std::cin.eof())
			{
				std::cin.read(buffer, sizeofBuffer);

				for (int ii = 0; ii < std::cin.gcount(); ++ii)
				{
					if (isgraph(buffer[ii]) || isspace(buffer[ii]))
					{
						std::cout << buffer[ii];
					}
					else
					{
						outfile.write((const char*)&buffer[ii], 1);
					}
				}
			}

			delete[] buffer;
		}
	}

	outfile.close();

	if (nullptr != pinold)
	{
		std::cin.rdbuf(pinold);
		infile.close();
	}
	return 0;
}

