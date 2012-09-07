#ifdef _WIN32
#include "stdafx.h"
#endif

#include "Exception.h"
#include <stdlib.h>
#include <stdio.h>


Exception::Exception(void)
{
}

Exception::Exception(char * msg)
{
#ifdef _WIN32
	strcpy_s(message, 256, msg);
#else
	strcpy(message, msg);
#endif
}

Exception::~Exception(void)
{
}

void Exception::setMessage(char * msg)
{
#ifdef _WIN32
	strcpy_s(message, 256, msg);
#else
	strcpy(message, msg);
#endif
}

char * Exception::getMessage(void)
{
	return message;
}


OutOfRangeException::OutOfRangeException(char *funName, size_t len, int indx)
//	:Exception(msg)
{
	char str[256];
	sprintf(str,"OutOfRange exception in %s: array size %d, index %d", funName, len, indx);
	setMessage(str);
	length = len;
	index = indx;
}

EndOfMessageException::EndOfMessageException(char *msg, size_t mes_len)
	:Exception(msg)
{
	messageLength = mes_len;
}

DamagedMessageException::DamagedMessageException(char *msg, BYTE *array, size_t len)
	:Exception(msg)
{
	message_array = array;
	length = len;
}

FileNotFoundException::FileNotFoundException(char *msg, char *f)
{
	char str[256];
	sprintf(str,"Could not open file %s: %s", f, msg);
	setMessage(str);
#ifdef _WIN32
	strcpy_s(file,256,f);
#else
	strcpy(file,f);
#endif

}

//FileNotFoundException::FileNotFoundException(char * msg)
//	:Exception(msg)
//{
//}
