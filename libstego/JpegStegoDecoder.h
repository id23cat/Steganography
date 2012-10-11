#pragma once
#if !defined(JPEGSTEGODECODER_H)
#define JPEGSTEGODECODER_H
#include "StegoDecoder.h"
#include "JpegStegoLog.h"
//#include "constants.h"
#include "Methods.h"
extern "C" {	
	#include "jpeglib.h"
	#include "jpegtran.h"
	#include "cdjpeg.h"
}

//typedef struct JDPointerStruct
//{
//	//int notStego;		//boolean parameter, 0 if we mast call stego function, else NOT 0
//	void (*CallbackFunction)(j_decompress_ptr cinfo, JBLOCKROW *MCU_data);	//Stego function
//	void *stegoObjPtr;						//pointer to the stego ooject
//}JpegDCallbackData;

class JpegStegoDecoder : public StegoDecoder
	//!OutOfRangeException = false;
{
	JStegoData sData;		//contain data and callback functions
	
private:
	void InitJpegStego(bool decodeMessage);
	//int selectPosition(JCOEF *coef);				//select position in block of coefficients (MCU)
	//int selectSign(JCOEF *coef, int position);		//select sign by current coefficients

public:
	//static void StegoGetMessage(j_decompress_ptr cinfo, JBLOCKROW *MCU_data);	//callback function	
	static void StegoGetMessage(void *cinfo, JBLOCKROW *MCU_data);	//callback function
	static void StegoKochZhaoGet(void *cinfo, JBLOCKROW *MCU_data);	//callback function
public:
	JpegStegoDecoder(void);
	~JpegStegoDecoder(void);

	int Decode(char *infile, char *outfile=NULL, bool getMes=true);
	size_t Test(char *infile, bool wrtLog=false, char *outfile=NULL);
	void LOG(bool log, bool bin=false){blog = log; binary = bin;};
	void Koch(bool k=false){koch = k;};
public:
	bool blog; //write text log
	char *logfile; // file name
	bool binary; //write binary log
	bool koch; //read with Koch-Zhao algorithm
//	bool random;			// read randomly
//	int percent;			//percent capacity
	int work_component;		//(All,Y,Cb,Cr)

	JpegStegoLog *slog;	// logger
};
#endif //JPEGSTEGODECODER_H
