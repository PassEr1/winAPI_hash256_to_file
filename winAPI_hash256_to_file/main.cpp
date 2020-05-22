#include <windows.h>

#include <wchar.h>
#include <iostream>
#include <string>
#include <map>
#include "cmdLinesParser.h"
#include "MD5_calculator.h"
#include "HashFile.h"

using namespace std;

CmdLineParser fnParseComamndLinesArgs(LPCWSTR lpComamndLine, int numOfArgs)
{	
	CmdLineParser parser;
	parser.configOption(L"--mode", CmdLineParser::optionsTypes::UNICODE_ARGUMENT);
	parser.configOption(L"--filePath", CmdLineParser::optionsTypes::UNICODE_ARGUMENT);
	parser.parse(CommandLineToArgvW(lpComamndLine, &numOfArgs), numOfArgs);
	return parser;
}

int wmain(int argc, wchar_t** argv)
{
	LPCWSTR lpsCommandLine = L"this.exe --mode md5 --filePath C:\\Users\\amitb\\Downloads\\fileToHash1.txt"; //GetCommandLineW()
	CmdLineParser parser = fnParseComamndLinesArgs(lpsCommandLine, 5);

	wstring sfileNameToHandle = (*parser[L"--filePath"].wstringValue);
	wstring sModeToPerform = (*parser[L"--mode"].wstringValue);
	
	if (sModeToPerform == L"md5") {
		HashFile hashForFile(sfileNameToHandle);
		hashForFile.fnPrintHash();
	}



	exit(0);

}