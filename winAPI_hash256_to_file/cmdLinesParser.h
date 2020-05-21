#pragma once
//README:
//this helper-class should treat as a utility,
//that is, it wasnt written in win32 convention. (-confirm this ageneda) 

#include <map>
#include <string>
#include <vector>
#include <Windows.h>
#include <exception>
#include <iostream>

using namespace std;

//TODO:
//validate argumetns - throw exception if filed to convert


class InvalidFlagException
:public std::exception
 {
 public:
	 InvalidFlagException(std::wstring wrongFlag)
		:_mWrongFlag(wrongFlag)
	 {
	 
	 }

	virtual const char* what()const throw()
	{
		std::wcout << "Error: " << _mWrongFlag << endl;
		return "wrong flag given!";
	}

 private:
	 const std::wstring _mWrongFlag;

};

typedef union multiValue
{
public:
	WCHAR charValue;
	int intValue;
	unsigned int uintValue;
	std::wstring* wstringValue=nullptr;

	multiValue()
	{
	}


	multiValue(const multiValue& other)
	{
		charValue = other.charValue;
		intValue = other.intValue;
		uintValue = other.uintValue;
		wstringValue = new wstring(other.wstringValue->c_str());
	}

	multiValue(multiValue&& other) noexcept
	{
		charValue = other.charValue;
		intValue = other.intValue;
		uintValue = other.uintValue;
		wstringValue = other.wstringValue;
		other.wstringValue = nullptr;

	}


	multiValue& operator=(multiValue&& other) noexcept
	{
		charValue = other.charValue;
		intValue = other.intValue;
		uintValue = other.uintValue;
		wstringValue = other.wstringValue;
		other.wstringValue = nullptr;
		return *this;
	}


	~multiValue()
	{
		if (wstringValue!=nullptr)
		{
			delete wstringValue;
		}
	}

} MultiValue, * pMultiValue;


class CmdLineParser
{
	typedef multiValue (CmdLineParser::* multiValueFactory)(std::wstring);

public:
	enum class optionsTypes { INT_ARGUMENT, UNICODE_ARGUMENT, CHAR_ARGUMENT }; //TODO: add more values + factories

	CmdLineParser() {
		this->m_mapTypeToValeFactory.insert(std::make_pair(optionsTypes::CHAR_ARGUMENT, &CmdLineParser::factoryCharFromWString));
		this->m_mapTypeToValeFactory.insert(std::make_pair(optionsTypes::UNICODE_ARGUMENT, &CmdLineParser::factoryUnicodeFromWString));
		this->m_mapTypeToValeFactory.insert(std::make_pair(optionsTypes::INT_ARGUMENT, &CmdLineParser::factoryIntFromWString));
	}
	CmdLineParser& operator=(CmdLineParser& other)
	{
		this->m_mapArgumentNameToType = other.m_mapArgumentNameToType;
		this->m_mapArgumentNameToValue = other.m_mapArgumentNameToValue;
		this->m_mapTypeToValeFactory = other.m_mapTypeToValeFactory;
		return *this;
	}

	void configOption(std::wstring option, CmdLineParser::optionsTypes type)
	{
		m_mapArgumentNameToType[option] = type;
	}

	// we assume the the input is valid.
	void parse(LPWSTR* args, size_t numOfArgs)
	{
		//serach by arg name.
		for (LPWSTR* token = args+1; token != args + numOfArgs; token+=2)
		{
			//find its type.
			wstring wstringTokenFlag(*token);
			wstring wstringTokenValue(*(token+1));
			
			if (m_mapArgumentNameToType.find(wstringTokenFlag) == m_mapArgumentNameToType.end())
			{
				throw InvalidFlagException(wstringTokenFlag);
			}

			CmdLineParser::optionsTypes excpectedType = m_mapArgumentNameToType[wstringTokenFlag];
			
			//call factory to get multiValue
			multiValue extractedValue  = (this->*(m_mapTypeToValeFactory[excpectedType]))(wstringTokenValue);

			//save corrasponding multiValue of the argument.
			m_mapArgumentNameToValue[wstringTokenFlag] = std::move(extractedValue);
		}
	}

	multiValue& operator[](std::wstring argName)
	{
		return m_mapArgumentNameToValue[argName];
	}

private:
	multiValue factoryCharFromWString(std::wstring arg)
	{	
		WCHAR valueAsChar = arg[0];
		multiValue valueExtracted;
		valueExtracted.charValue = valueAsChar;
		return valueExtracted;
	}

	multiValue factoryUnicodeFromWString(std::wstring arg)
	{
		multiValue valueExtracted;
		std::wstring* copyArgument = new std::wstring(arg);
		valueExtracted.wstringValue = copyArgument;
		return (valueExtracted);
	}

	multiValue factoryIntFromWString(std::wstring arg)
	{
		int valueAsInt = stoi(arg);
		multiValue valueExtracted;
		valueExtracted.intValue = valueAsInt;
		return (valueExtracted);
	}

private:
	map <std::wstring, CmdLineParser::optionsTypes> m_mapArgumentNameToType; // read about multimap
	map <CmdLineParser::optionsTypes, multiValueFactory> m_mapTypeToValeFactory;
	map <std::wstring, multiValue> m_mapArgumentNameToValue;
};
