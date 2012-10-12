#ifdef _WIN32
#include "StdAfx.h"
#endif
#include "JpegStegoEncoder.h"
#include <string>
#include <iostream>
using namespace std;

#define MCU_DATA(blkn, idx) MCU_data[blkn][0][idx]

static int ie;

JpegStegoEncoder::JpegStegoEncoder(void)
{
	blog = false;
	binary = false;
	koch = false;
	percent = 100;
	perc = P100;
	random = false;
	D = 0;
	Pi=1;
	quality = 0;
	slog1 = NULL;
	slog2 = NULL;
	work_component=ALL;
	logfile = NULL;
	bit=0;
///********************************************************/
//	//cData.CallbackFunction = &(this->StegoHideLength);
//	cData.CallbackFunction = &(this->StegoHideMessage);
//	cData.isStego = 0;
//	cData.stegoObjPtr = this;
//
//	/*dData.CallbackFunction = NULL;
//	dData.notStego = 1;
//	dData.stegoObjPtr = this;*/
///********************************************************/
}

JpegStegoEncoder::~JpegStegoEncoder(void)
{
}

void JpegStegoEncoder::InitPercent()
{
	Pi=1;
	if(random)
	{
		switch(percent)
		{case 100:
		case 90:
			perc = P100;
			break;
		case 75:
			perc = P75;
			break;
		case 50:
			perc = P50;
			break;
		case 25:
			perc = P25;
			break;
		default:
			perc = P100;
		}
	}
}

void JpegStegoEncoder::InitJpegStego(bool encMes)
{	
	InitPercent();
	srand(1);
	paste_message = encMes;
	if(paste_message)
	{							
		sData.CallbackFunction = &StegoHideMessage;
		if(koch)
			sData.CallbackFunction = &StegoKochZhaoHide;
		sData.isStego = 1;
	}else if(!paste_message)													//
	{
		sData.CallbackFunction = &StegoTestContainer;
		if(koch)
			sData.CallbackFunction = &StegoKochZhaoTest;
		sData.isStego = 1;
		sData.CallbackFunction2 = &StegoTestContainer2;
		sData.isStegoAnalysis = 1;
		
	}else
		sData.isStego = 0;

	sData.stegoObjPtr = this;

	/*dData.CallbackFunction = NULL;
	dData.notStego = 1;
	dData.stegoObjPtr = this;*/

}

//int JpegStegoEncoder::selectPosition(JCOEF *coef)
//{
//	for(size_t i=0; i<ALLOW; i++)
//	{
//		switch(coef[allowable_position[i]])
//			case 0:
//			case 1:
//			case -1:{
//				return allowable_position[i];
//			}
//	}
//	return -1;
//}

//int JpegStegoEncoder::selectSign(JCOEF *coef, int position)
//{
//	if(coef[position]==1)
//		return 1;
//	else if(coef[position]==-1)
//		return -1;
//	else if(coef[position]==0)
//	{
//		int tmp = coef[position-7];
//		tmp += coef[position+7];
//		tmp += coef[position-8];
//		tmp += coef[position+8];
//		if(tmp<0)
//			return -1;
//		else
//			return 1;
//	}
//	return 1;
//}

//bool JpegStegoEncoder::readBMP(char *fname)
//{
//	BMPimage bmpImg;
//	bmpImg.ReadRGB(fname);
//	rgb = bmpImg.GetRGBptr(height,width);
//	return true;
//}


//void JpegStegoEncoder::StegoHideMessage(j_compress_ptr cinfo, JBLOCKROW *MCU_data)
//{
//	//JpegECallbackData *pJCD = static_cast<JpegECallbackData*>(cinfo->stegoEncoderData/*.stegoObjPtr*/);
//	//JpegStegoEncoder *pJSE = static_cast<JpegStegoEncoder*>(pJCD->stegoObjPtr);
//	//StegoCData *pJCD = static_cast<StegoCData*>(cinfo->stegoEncoderData/*.stegoObjPtr*/);
//	JpegStegoEncoder *pJSE = static_cast<JpegStegoEncoder*>(cinfo->stegoEncoderData.stegoObjPtr);
//	if(!pJSE->paste_message || !pJSE->mesArray.IsArraySet())
//		return;
//	try
//	{
//		int DCT_pos;
//		jpeg_component_info *compptr;
//
//		for (int blkn = 0; blkn < cinfo->blocks_in_MCU; blkn++)
//		{
//			compptr = cinfo->cur_comp_info[cinfo->MCU_membership[blkn]];
//			if(pJSE->paste_message)
//			{
//				DCT_pos = pJSE->selectPosition(MCU_data[blkn][0]);
//				if(DCT_pos != -1)
//					MCU_data[blkn][0][DCT_pos] = pJSE->mit *				//set bit with sign
//												pJSE->selectSign(MCU_data[blkn][0],DCT_pos);	//
//			}
//			pJSE->mit++;
//		}
//
//	}catch(OutOfRangeException oorExc)
//	{
//		//cerr<<oorExc.getMessage();
//		pJSE->cData.isStego = 0;
//	}
//	catch(Exception exc)
//	{
//		cerr << exc.getMessage();
//		//pOCD->isStego = 0;
//	}
//}
//
//
//
//void JpegStegoEncoder::StegoTestContainer(j_compress_ptr cinfo, JBLOCKROW *MCU_data)
//{
//	/*JpegECallbackData *pJCD = static_cast<JpegECallbackData*>(cinfo->stegoEncoderData.stegoObjPtr);
//	JpegStegoEncoder *pJSE = static_cast<JpegStegoEncoder*>(pJCD->stegoObjPtr);*/
//	JpegStegoEncoder *pJSE = static_cast<JpegStegoEncoder*>(cinfo->stegoEncoderData.stegoObjPtr);
//	
//	try
//	{
//		int DCT_pos;
//		jpeg_component_info *compptr;
//
//		for (int blkn = 0; blkn < cinfo->blocks_in_MCU; blkn++)
//		{
//			compptr = cinfo->cur_comp_info[cinfo->MCU_membership[blkn]];
//			DCT_pos = pJSE->selectPosition(MCU_data[blkn][0]);
//			if(DCT_pos != -1)
//				pJSE->capacityBit++;			
//		}
//	}catch(Exception exc)
//	{
//		pJSE->cData.isStego = 0;
//	}
//}

void JpegStegoEncoder::StegoHideMessage(void *cinfo, JBLOCKROW *MCU_data)
{
	//JpegECallbackData *pJCD = static_cast<JpegECallbackData*>(cinfo->stegoEncoderData/*.stegoObjPtr*/);
	//JpegStegoEncoder *pJSE = static_cast<JpegStegoEncoder*>(pJCD->stegoObjPtr);
	//StegoCData *pJCD = static_cast<StegoCData*>(cinfo->stegoEncoderData/*.stegoObjPtr*/);
	j_compress_ptr *ccinfop = static_cast<j_compress_ptr*>(cinfo);
	j_compress_ptr ccinfo = *ccinfop;
	JpegStegoEncoder *pJSE = static_cast<JpegStegoEncoder*>(ccinfo->stego.stegoObjPtr);
	
	
	/*if(!pJSE->paste_message || !pJSE->mesArray.IsArraySet())
		return;*/
	try
	{

		BYTE bit;
		int DCT_pos;
		jpeg_component_info *compptr;

		for (int blkn = 0; blkn < ccinfo->blocks_in_MCU; blkn++)
		{
			
			compptr = ccinfo->cur_comp_info[ccinfo->MCU_membership[blkn]];
			if(pJSE->work_component == ALL || pJSE->work_component == compptr->component_id)	//check current component
			{
				if(pJSE->paste_message && pJSE->mesArray.IsArraySet())
				{
					if(pJSE->paste_message)
					{
						DCT_pos = /*pJSE->*/selectPosition(MCU_data[blkn][0]);
						if(DCT_pos != -1)
						{
							bit = pJSE->mit;
							//cerr << (int)bit;
							MCU_data[blkn][0][DCT_pos] = bit *				//set bit with sign
							/*pJSE->*/selectSign(MCU_data[blkn][0],DCT_pos);	//
							pJSE->mit++;
						}
					}
				}
				if(pJSE->blog) 
				{
					pJSE->slog1->writeData(compptr->component_id);
					for(int i=0; i<8; i++)
					{
						for(int j=0; j<8; j++)
						{						
							pJSE->slog1->writeCoef(MCU_data[blkn][0][i*8+j]);
						}
						pJSE->slog1->endStr();
					}				
					pJSE->slog1->endBlock();
				}			
			}
		}

	}catch(OutOfRangeException oorExc)
	{
		//cerr<<oorExc.getMessage();
		ccinfo->stego.isStego=0;
		//pJSE->sData.isStego = 0;
	}
	catch(Exception exc)
	{
		cerr << exc.getMessage();
		//pOCD->isStego = 0;
	}
}

void JpegStegoEncoder::StegoKochZhaoHide(void *cinfo, JBLOCKROW *MCU_data)
{
	//JpegECallbackData *pJCD = static_cast<JpegECallbackData*>(cinfo->stegoEncoderData/*.stegoObjPtr*/);
	//JpegStegoEncoder *pJSE = static_cast<JpegStegoEncoder*>(pJCD->stegoObjPtr);
	//StegoCData *pJCD = static_cast<StegoCData*>(cinfo->stegoEncoderData/*.stegoObjPtr*/);
	j_compress_ptr *ccinfop = static_cast<j_compress_ptr*>(cinfo);
	j_compress_ptr ccinfo = *ccinfop;
	JpegStegoEncoder *pJSE = static_cast<JpegStegoEncoder*>(ccinfo->stego.stegoObjPtr);


	/*if(!pJSE->paste_message || !pJSE->mesArray.IsArraySet())
		return;*/
	try
	{

		BYTE bit;
		LKoch DCT_pos;
		jpeg_component_info *compptr;
		int D = pJSE->D;
		int d = D>>1;
		int d3=D-d*2;
		int ii=0;

		for (int blkn = 0; blkn < ccinfo->blocks_in_MCU; blkn++)
		{
			compptr = ccinfo->cur_comp_info[ccinfo->MCU_membership[blkn]];
			if(pJSE->blog) 
			{
				pJSE->slog1->writeData(compptr->component_id);
				for(int i=0; i<8; i++)
				{
					for(int j=0; j<8; j++)
					{						
						pJSE->slog1->writeCoef(MCU_data[blkn][0][i*8+j]);
					}
					pJSE->slog1->endStr();
				}				
				pJSE->slog1->endBlock();
			}

			
			if(pJSE->work_component == ALL || pJSE->work_component == compptr->component_id) //check current component
			{
				if(pJSE->paste_message && pJSE->mesArray.IsArraySet())
				{
					if(pJSE->paste_message && pJSE->perc>=pJSE->Pi)
					{				
						bit = pJSE->mit;
//#ifdef DEBUG
//						if(pJSE->mit.GetCurIdx()==13)
//							cerr << 13 <<endl;
//#endif
						for(int i=0;i<KOCH_MAX; i++)
						{
							ii = (((double) rand() / 
								 (double) RAND_MAX) * (KOCH_MAX-1) + 0);

							DCT_pos = /*pJSE->*/KochZhaoPosition(ii);
#ifdef DEBUG
							cout <<ie++ <<": ii=" <<ii
									<<", bit=" <<(int)bit;
#endif
							
							if(bit==1)
							{

								if(min(MCU_DATA(blkn, DCT_pos.l1),MCU_DATA(blkn, DCT_pos.l2))+D
									<= MCU_DATA(blkn, DCT_pos.l3))	// check_write
								{
									// invalid block
//									 l1=L, l2=H, l3=M
									MCU_DATA(blkn, DCT_pos.l1)=MCU_DATA(blkn, DCT_pos.l3)-d;
									MCU_DATA(blkn, DCT_pos.l2)=MCU_DATA(blkn, DCT_pos.l3)+d;

//									// alternative invalidation
//									if(min(MCU_DATA(blkn, DCT_pos.l1), MCU_DATA(blkn, DCT_pos.l2)) ==
//											MCU_DATA(blkn, DCT_pos.l1))
//										MCU_DATA(blkn, DCT_pos.l2) = MCU_DATA(blkn, DCT_pos.l3) + d;
//									else if(min(MCU_DATA(blkn, DCT_pos.l1), MCU_DATA(blkn, DCT_pos.l2)) ==
//											MCU_DATA(blkn, DCT_pos.l2))
//										MCU_DATA(blkn, DCT_pos.l1) = MCU_DATA(blkn, DCT_pos.l3) + d;

//									MCU_data[blkn][0][DCT_pos.l1]=255;
//									MCU_data[blkn][0][DCT_pos.l2]=255;
//									MCU_data[blkn][0][DCT_pos.l3]=255;
																		
									//continue;
#ifdef DEBUG
									cout
										<<", l1=" <<MCU_DATA(blkn, DCT_pos.l1)
										<<", l2=" <<MCU_DATA(blkn, DCT_pos.l2)
										<<", l3=" <<MCU_DATA(blkn, DCT_pos.l3)
										<<" -- break" <<endl;
#endif
									break;
								}
								
								// write bit
								MCU_DATA(blkn, DCT_pos.l3)-=d-d3;
								MCU_DATA(blkn, DCT_pos.l1)+=d/*+1*/;
								MCU_DATA(blkn, DCT_pos.l2)+=d/*+1*/;
								///*********************/
								//MCU_data[blkn][0][0]=5*(pJSE->mit.byteIndex*8+pJSE->mit.bitIndex);								
								///*********************/
								pJSE->mit++;
#ifdef DEBUG
							cout
								<<", l1=" <<MCU_DATA(blkn, DCT_pos.l1)
								<<", l2=" <<MCU_DATA(blkn, DCT_pos.l2)
								<<", l3=" <<MCU_DATA(blkn, DCT_pos.l3)
								<<" -- embedded" <<endl;
#endif
								break;
							}else
							{
								if(max(MCU_DATA(blkn, DCT_pos.l1),MCU_DATA(blkn, DCT_pos.l2)) >=
									MCU_DATA(blkn, DCT_pos.l3)+D/*-1*/)
								{
									// invalid block
									// l1=L, l2=H, l3=M
									MCU_DATA(blkn, DCT_pos.l1)=MCU_DATA(blkn, DCT_pos.l3)-d;
									MCU_DATA(blkn, DCT_pos.l2)=MCU_DATA(blkn, DCT_pos.l3)+d;

//									// alternative invalidation
//									if (max(MCU_DATA(blkn, DCT_pos.l1),MCU_DATA(blkn, DCT_pos.l2))==
//											MCU_DATA(blkn, DCT_pos.l1))
//										MCU_DATA(blkn, DCT_pos.l2) = MCU_DATA(blkn, DCT_pos.l3) - d;
//									else if (max(MCU_DATA(blkn, DCT_pos.l1),MCU_DATA(blkn, DCT_pos.l2))==
//											MCU_DATA(blkn, DCT_pos.l2))
//										MCU_DATA(blkn, DCT_pos.l1) = MCU_DATA(blkn, DCT_pos.l3) - d;

//									MCU_data[blkn][0][DCT_pos.l1]=-255;
//									MCU_data[blkn][0][DCT_pos.l1]=-255;
//									MCU_data[blkn][0][DCT_pos.l1]=-255;

									//continue;
#ifdef DEBUG
									cout
										<<", l1=" <<MCU_DATA(blkn, DCT_pos.l1)
										<<", l2=" <<MCU_DATA(blkn, DCT_pos.l2)
										<<", l3=" <<MCU_DATA(blkn, DCT_pos.l3)
										<<" -- break" <<endl;
#endif
									break;
								}

								// write bit
								MCU_DATA(blkn, DCT_pos.l3)+=d+d3;
								MCU_DATA(blkn, DCT_pos.l1)-=d;
								MCU_DATA(blkn, DCT_pos.l2)-=d;
								///*********************/
								//MCU_data[blkn][0][0]=5*(pJSE->mit.byteIndex*8+pJSE->mit.bitIndex);								
								///*********************/
								pJSE->mit++;
#ifdef DEBUG
								cout
									<<", l1=" <<MCU_DATA(blkn, DCT_pos.l1)
									<<", l2=" <<MCU_DATA(blkn, DCT_pos.l2)
									<<", l3=" <<MCU_DATA(blkn, DCT_pos.l3)
									<<" -- embedded" <<endl;
#endif
								break;
							}						
							
							
						}
						//pJSE->mit++;
						//if(DCT_pos != -1)
						//{
						//	bit = pJSE->mit;
						//	//cerr << (int)bit;
						//	MCU_data[blkn][0][DCT_pos] = bit *				//set bit with sign
						//		/*pJSE->*/selectSign(MCU_data[blkn][0],DCT_pos);	//
						//	pJSE->mit++;
						//}
					}
					
					pJSE->Pi++;
						if(pJSE->Pi>4)
							pJSE->Pi=1;
				}
			}			
		}

	}catch(OutOfRangeException oorExc)
	{
		//ccinfo->stego.isStego=0;
		pJSE->paste_message=false;
		
	}
	catch(Exception exc)
	{
		cerr << exc.getMessage();
		//pOCD->isStego = 0;
	}
}

void JpegStegoEncoder::StegoKochZhaoTest(void *cinfo, JBLOCKROW *MCU_data)
{
	//JpegECallbackData *pJCD = static_cast<JpegECallbackData*>(cinfo->stegoEncoderData/*.stegoObjPtr*/);
	//JpegStegoEncoder *pJSE = static_cast<JpegStegoEncoder*>(pJCD->stegoObjPtr);
	//StegoCData *pJCD = static_cast<StegoCData*>(cinfo->stegoEncoderData/*.stegoObjPtr*/);
	j_compress_ptr *ccinfop = static_cast<j_compress_ptr*>(cinfo);
	j_compress_ptr ccinfo = *ccinfop;
	JpegStegoEncoder *pJSE = static_cast<JpegStegoEncoder*>(ccinfo->stego.stegoObjPtr);


	/*if(!pJSE->paste_message || !pJSE->mesArray.IsArraySet())
	return;*/
	try
	{

		BYTE bit;
		LKoch DCT_pos;
		jpeg_component_info *compptr;
		int D = pJSE->D;
		int ii=0;

		for (int blkn = 0; blkn < ccinfo->blocks_in_MCU; blkn++)
		{
			compptr = ccinfo->cur_comp_info[ccinfo->MCU_membership[blkn]];
			if(pJSE->work_component == ALL || pJSE->work_component == compptr->component_id)
			{
				if(pJSE->perc>=pJSE->Pi)
				{				
					bit = pJSE->bit;
					for(int i=0;i<KOCH_MAX; i++)
					{
						ii = (((double) rand() / 
							(double) RAND_MAX) * (KOCH_MAX-1) + 0);

						DCT_pos = /*pJSE->*/KochZhaoPosition(ii);

						if(bit==1)
						{
							if(min(MCU_data[blkn][0][DCT_pos.l1],MCU_data[blkn][0][DCT_pos.l2])+D
								< MCU_data[blkn][0][DCT_pos.l3])
							{
								continue;
							}

							pJSE->capacityBit++;							
							break;
						}else
						{
							if(max(MCU_data[blkn][0][DCT_pos.l1],MCU_data[blkn][0][DCT_pos.l2]) > 
								MCU_data[blkn][0][DCT_pos.l3]+D/*-1*/)
							{
								continue;
							}

							pJSE->capacityBit++;
							break;
						}

						/*MCU_data[blkn][0][DCT_pos.l1]=100;
						MCU_data[blkn][0][DCT_pos.l2]=200;
						MCU_data[blkn][0][DCT_pos.l3]=300;*/
					}
					//pJSE->mit++;
					//if(DCT_pos != -1)
					//{
					//	bit = pJSE->mit;
					//	//cerr << (int)bit;
					//	MCU_data[blkn][0][DCT_pos] = bit *				//set bit with sign
					//		/*pJSE->*/selectSign(MCU_data[blkn][0],DCT_pos);	//
					//	pJSE->mit++;
					//}
				}

				pJSE->Pi++;
				if(pJSE->Pi>4)
					pJSE->Pi=1;
			}

			if(pJSE->blog) 
			{
				pJSE->slog1->writeData(compptr->component_id);
				for(int i=0; i<8; i++)
				{
					for(int j=0; j<8; j++)
					{						
						pJSE->slog1->writeCoef(MCU_data[blkn][0][i*8+j]);
					}
					pJSE->slog1->endStr();
				}				
				pJSE->slog1->endBlock();
			}

		}

	}catch(OutOfRangeException oorExc)
	{
		//cerr<<oorExc.getMessage();
		ccinfo->stego.isStego=0;
		//pJSE->sData.isStego = 0;
	}
	catch(Exception exc)
	{
		cerr << exc.getMessage();
		//pOCD->isStego = 0;
	}
}

void JpegStegoEncoder::StegoTestContainer(void *cinfo, JBLOCKROW *MCU_data)
{
	/*JpegECallbackData *pJCD = static_cast<JpegECallbackData*>(cinfo->stegoEncoderData.stegoObjPtr);
	JpegStegoEncoder *pJSE = static_cast<JpegStegoEncoder*>(pJCD->stegoObjPtr);*/
	j_compress_ptr *ccinfop = static_cast<j_compress_ptr*>(cinfo);
	j_compress_ptr ccinfo = *ccinfop;
	JpegStegoEncoder *pJSE = static_cast<JpegStegoEncoder*>(ccinfo->stego.stegoObjPtr);

	try
	{
		int DCT_pos;
		jpeg_component_info *compptr;

		int blkn;
		for (blkn = 0; blkn < ccinfo->blocks_in_MCU; blkn++)
		{
			compptr = ccinfo->cur_comp_info[ccinfo->MCU_membership[blkn]];
			DCT_pos = /*pJSE->*/selectPosition(MCU_data[blkn][0]);
			if(DCT_pos != -1)
				pJSE->capacityBit++;

			//Writing LOG
			if(pJSE->blog) 
			{
				pJSE->slog1->writeData(compptr->component_id);
				for(int i=0; i<8; i++)
				{
					for(int j=0; j<8; j++)
					{						
						pJSE->slog1->writeCoef(MCU_data[blkn][0][i*8+j]);
					}
					pJSE->slog1->endStr();
				}				
				pJSE->slog1->endBlock();
			}
		}
		
	}catch(Exception exc)
	{
		pJSE->sData.isStego = 0;
	}
}

void JpegStegoEncoder::StegoTestContainer2(void* cinfo, int workspace[64],int blkn)
{
	j_compress_ptr *ccinfop = static_cast<j_compress_ptr*>(cinfo);
	j_compress_ptr ccinfo = *ccinfop;
	JpegStegoEncoder *pJSE = static_cast<JpegStegoEncoder*>(ccinfo->stego.stegoObjPtr);

	try
	{
		//int DCT_pos;
		jpeg_component_info *compptr;

		//int blkn;

		compptr = ccinfo->cur_comp_info[ccinfo->MCU_membership[blkn]];
		if(pJSE->blog) 
		{
			pJSE->slog2->writeData(compptr->component_id);
			for(int i=0; i<8; i++)
			{
				for(int j=0; j<8; j++)
				{						
					pJSE->slog2->writeCoef(workspace[i*8+j]);
				}
				pJSE->slog2->endStr();
			}				
			pJSE->slog2->endBlock();
		}
		//for (blkn = 0; blkn < ccinfo->blocks_in_MCU; blkn++)
		//{
		//	
		//	DCT_pos = pJSE->selectPosition(MCU_data[blkn][0]);
		//	if(DCT_pos != -1)
		//		pJSE->capacityBit++;

		//	//Writing LOG
		//	
		//}

	}catch(Exception exc)
	{
		pJSE->sData.isStegoAnalysis = 0;
	}
}


//int  JpegStegoEncoder::Encode(char **infiles, int count, char *dstdir, bool pasteMes)
//{
//	if(count <1)
//		throw Exception("Must be one or more input files");
//	char outfile[512];
//	int len=0;
//	for(int i=0; i<count; i++)
//	{
//		memset(outfile,0,512);
//		genOutFileName(infiles[i],outfile,dstdir,i,count);
//		len = strlen(outfile);
//		outfile[len-3]='j';
//		outfile[len-2]='p';
//		outfile[len-1]='g';
//		Encode(infiles[i], outfile, pasteMes);
//	}	
//}

int JpegStegoEncoder::Encode(char *infile, char *outfile, bool pasteMes)
{
	// test for existing input and output files
	FILE *fp;
	if ((fp = fopen(infile, READ_BINARY)) == NULL) {
		throw FileNotFoundException("File not found\n",infile);
	}
	fclose(fp);
	if ((fp = fopen(outfile, WRITE_BINARY)) == NULL) {
		throw FileNotFoundException("File not found\n",outfile);
	}
	fclose(fp);

	if(blog)
	{		
		slog1 = new JpegStegoLog;
		slog1->Bin(binary);
		slog1->setLogFileName(logfile);
		slog1->openLOG(work_component);
		/*slog2 = new JpegStegoLog;
		slog2->setLogFileName(infile,"DCT");
		slog2->openLOG(ALL);*/
	}

	InitJpegStego(true);
	size_t len = strlen(infile);
	char *ext = infile+(len-3);
	char extl[4]={0};
	for(int i=0;i<3;i++)
		extl[i] = tolower(ext[i]);
	if(!strcmp(extl,"bmp\0"))
		startBmpToJpeg(infile,outfile);
	else if(!strcmp(extl,"jpg\0"))
		startJpegToJpeg(infile,outfile);
	else
		throw Exception("Unsupported file type");
	return 0;
}

size_t JpegStegoEncoder::Test(char *infile, bool wrtLog, char *outfile)
{
	// test for existing input file
	blog = wrtLog;
#ifdef _WIN32
	char nullFile[] = "nul";
#else
	char nullFile[] = "/dev/null";
#endif
	FILE *fp;

	if ((fp = fopen(infile, READ_BINARY)) == NULL) {
		throw FileNotFoundException("File not found\n",infile);
	}
	fclose(fp);

	if ((fp = fopen(nullFile, READ_BINARY)) == NULL) {
		throw FileNotFoundException("File not found\n", "\\dev\\null");
	}
	fclose(fp);



	if(blog)
	{		
		slog1 = new JpegStegoLog;
		slog1->Bin(binary);
		if(outfile)
			slog1->setLogFileName(outfile);
		if(logfile)
			slog1->setLogFileName(logfile);
		else
			slog1->setLogFileName(infile);
		slog1->openLOG(work_component);	
	}

	InitJpegStego(false);
	string instr(infile);
	size_t extpos = instr.find_last_of(".");
	string ext(instr.substr(extpos+1, instr.length()-1));
	cout << ext << endl;

	if(!ext.compare("bmp"))
		startBmpToJpeg(infile, nullFile);
	else if (!ext.compare("jpg"))
		startJpegToJpeg(infile, nullFile);
	else
		throw Exception("Unsupported file type");

//	size_t len = strlen(infile);
//	char *ext = infile+(len-3);
//	char extl[4]={0};
//	for(int i=0;i<3;i++)
//		extl[i] = tolower(ext[i]);
//	printf("%s - %s", extl, "bmp\0");
//	if(!strcmp(extl,"bmp\0"))
//		startBmpToJpeg(infile, nullFile);
//	else if(!strcmp(extl,"jpg\0"))
//		startJpegToJpeg(infile, nullFile);
//	else
//		throw Exception("Unsupported file type");
	if(wrtLog)
	{
		slog1->close();
		delete slog1;
		/*slog2->close();
		delete slog2;*/
	}
	if(capacityBit/8 <= BEG_LEN + LEN_LEN + CRC_LEN + END_LEN)
		return 0;
	return ((size_t) capacityBit/8) - BEG_LEN - LEN_LEN - CRC_LEN - END_LEN;
}

//int JpegStegoEncoder::startHiding(char *inf, char *outf)
//{
//	size_t len = strlen(inf);
//	char *ext = inf+(len-3);
//	char extl[4]={0};
//	for(int i=0;i<3;i++)
//		extl[i] = tolower(ext[i]);
//	if(!strcmp(extl,"bmp\0"))
//		startBmpToJpeg(inf,outf);
//	else if(!strcmp(extl,"jpg\0"))
//		startJpegToJpeg(inf,outf);
//	else
//		return -1;
//}

int JpegStegoEncoder::startJpegToJpeg(char *inf, char *outf)
{
	int argc = 3;
	char *argv[3];
	char name[]="jpegtran";
	
	argv[0]=name;
	argv[1]=inf;
	argv[2]=outf;

	return main_tran(argc, argv, sData);
}

int JpegStegoEncoder::startBmpToJpeg(char *inf, char *outf)
{
	char name[]="cjpeg";
	char oout[]="-outfile";
	char qua[3];

	int argc = 4;
	char *argv[6];
	argv[0]=name;
	argv[1]=oout;
	argv[2]=outf;
	int ind = 3;
	if(quality)
	{
		argc = 6;
		argv[3]= (char*)"-quality";
#ifdef _WIN32
		itoa(quality,qua,10);
#else
		sprintf(qua,"%d",quality);
#endif
		argv[4]= qua;
		ind = 5;
	}

	argv[ind]=inf;


	return main_cjpeg(argc, argv, sData);
//	readBMP(inf);
//	struct jpeg_compress_struct cinfo;
//	struct jpeg_error_mgr jerr;
//	FILE* dst_jpeg;
//	JSAMPLE *image_buffer = (JSAMPLE*)rgb;
//
//	cinfo.err = jpeg_std_error(&jerr);
//	jpeg_create_compress(&cinfo);
//
//	if (fopen_s(&dst_jpeg, outf, "wb")!=0)
//	{
//		Exception exc;
//		char strerr[STR_LEN];
//		sprintf_s(strerr, STR_LEN, "can't open %s\n", outf);
//		exc.setMessage(strerr);
//		throw exc;
//	}
//
//		jpeg_stdio_dest(&cinfo, dst_jpeg);
//		cinfo.image_width = width; 	/* image width and height, in pixels */
//		cinfo.image_height = height;
//		cinfo.input_components = 3;		/* # of color components per pixel */
//		cinfo.in_color_space = JCS_RGB; 	/* colorspace of input image */
//
//	JSAMPROW row_pointer[1];	/* pointer to JSAMPLE row[s] */
//	int row_stride;		/* physical row width in image buffer */
//
//	jpeg_set_defaults(&cinfo);
//	
//	jpeg_set_quality(&cinfo, 100, TRUE /* limit to baseline-JPEG values */);
//	jpeg_start_compress(&cinfo, TRUE);
//
//	row_stride = width * 3;	/* JSAMPLEs per row in image_buffer */
//
///**************************************/
//	cinfo.stego = sData;
///**************************************/
//		
//	while (cinfo.next_scanline < cinfo.image_height)
//	{
//		row_pointer[0] = & image_buffer[cinfo.next_scanline * row_stride];
//		(void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
//	}
//	jpeg_finish_compress(&cinfo);
//
//	fclose(dst_jpeg);
	return 1;
}

