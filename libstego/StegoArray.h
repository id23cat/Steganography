#pragma once
#include "BitArray.h"
#undef GetMessage

#define BEG_FLAG	"qwer"
#define END_FLAG	"zxcv"
#define BEG_LEN		4
#define END_LEN		4
#define LEN_LEN		4
#define CRC_LEN		4


// Message Structure:
//      4            4          length      4          4
// [beginFlag][message length][message][CRC32 code][endFlag]
//
// Totally: 16+length

class StegoArray:public BitArray
{
	const char *beginFlag;
	const char *endFlag;
	
	size_t messageLength;
public:
		//class StegoArrayIterator:public BitArrayIterator
		//{
		//public:
		//	StegoArrayIterator();
		//	StegoArrayIterator(size_t byte, size_t bit, StegoArray *par) throw(...);
		//	
		//	StegoArrayIterator &operator[](size_t index)  throw(...);	//index by bits in BitArray
		//	StegoArrayIterator &operator=(BYTE bit);					//set bit to current position in BitArray object
		//	StegoArrayIterator &operator=(StegoArrayIterator &it);		//copy constructor
		//	StegoArrayIterator &operator++() throw(Exception, int);
		//};
public:
	StegoArray(void);
	StegoArray(size_t len);
	~StegoArray(void);
	void SetMessage(BYTE *mes, size_t len) throw(Exception);
	BYTE* GetMessage(size_t& len);
	void SetBit(size_t byte, size_t bit, BYTE b)  throw(Exception,
														OutOfRangeException,
														DamagedMessageException,
														EndOfMessageException);
	void Initialize() throw(Exception);
	/*StegoArrayIterator& Begin();
	StegoArrayIterator& End();*/

#ifdef _DEBUG
	void DEBUG_Print_Array();
	void DEBUG_Print_Bits();
#endif
};
