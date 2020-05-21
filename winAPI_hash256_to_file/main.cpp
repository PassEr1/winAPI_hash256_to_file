#include <windows.h>

#include <wchar.h>
#include <iostream>
#include <string>
#include <map>
#include "cmdLinesParser.h"
#include "MD5_calvulator.h"
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
	/*LPCWSTR lpsCommandLine = GetCommandLineW();
	CmdLineParser parser = fnParseComamndLinesArgs(lpsCommandLine, argc);

	wstring sfileNameToHandle(*parser[L"--filePath"].wstringValue);
	wstring sModeToPerform(*parser[L"--mode"].wstringValue);
	*/

	const CHAR brMyMessage[] = {
	'a', 'b', 'c'
	};
	const CHAR brMyMessage2[] = {
	'd', 'e', 'f'
	};

	MD5Calculator md5Calculator;
	md5Calculator.update((PBYTE)brMyMessage, sizeof(brMyMessage));
	md5Calculator.update((PBYTE)brMyMessage2, sizeof(brMyMessage2));
	md5Calculator.digest();

	exit(0);

}