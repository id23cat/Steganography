#ifdef _WIN32
#include "stdafx.h"
#endif

#include "Exception.h"
#include <stdlib.h>
#include <stdio.h>
#include <sstream>

Exception::Exception(void)
{
}

Exception::Exception(string msg)
{
#ifdef _WIN32
	strcpy_s(message, 256, msg);
#else
	message = msg;
#endif
}

Exception::~Exception(void)
{
}

void Exception::setMessage(string msg)
{
#ifdef _WIN32
	strcpy_s(message, 256, msg);
#else
	message = msg;
#endif
}


OutOfRangeException::OutOfRangeException(string funName, size_t len, int indx)
//	:Exception(msg)
{
//	char str[256];
//	sprintf(str,"OutOfRange exception in %s: array size %d, index %d", funName, len, indx);
//	setMessage(str);
	stringstream tmps;
	tmps <<"OutOfRange exception in " <<funName << ": array size "<<len <<", index " <<indx;
	message = tmps.str();
	length = len;
	index = indx;
}

EndOfMessageException::EndOfMessageException(string msg, size_t mes_len)
	:Exception(msg)
{
	messageLength = mes_len;
}

DamagedMessageException::DamagedMessageException(string msg, BYTE *array, size_t len)
	:Exception(msg)
{
	message_array = array;
	length = len;
}

FileNotFoundException::FileNotFoundException(string msg, string f)
{
	stringstream tmps;
	tmps <<"Could not open file " <<f <<": " <<msg;
	message = tmps.str();
#ifdef _WIN32
	strcpy_s(file,256,f);
#else
	file = f;
#endif

}

//FileNotFoundException::FileNotFoundException(char * msg)
//	:Exception(msg)
//{
//}
