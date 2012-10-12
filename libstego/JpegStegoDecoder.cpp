#ifdef _WIN32
#include "StdAfx.h"
#endif
#include "JpegStegoDecoder.h"

extern "C"{
	#include "jpegtran.h"
	#include "cdjpeg.h"
}

#define MCU_DATA(blkn, idx) MCU_data[blkn][0][idx]

static int id;

JpegStegoDecoder::JpegStegoDecoder(void)
{
	blog = false;
	binary = false;
	koch = false;
//	percent = 100;
//	perc = P100;
//	random = false;
	work_component = ALL;
	logfile = NULL;
}

JpegStegoDecoder::~JpegStegoDecoder(void)
{
}

void JpegStegoDecoder::InitJpegStego(bool decMes)
{
	// InitPercent();
	get_message = decMes;
	srand(1);
	if(get_message)											//
	{																	//
		sData.CallbackFunction = &StegoGetMessage;					//
		if(koch)
			sData.CallbackFunction = &StegoKochZhaoGet;
		sData.isStego = 1;											//
	}else																//
		sData.isStego = 0;											//
	sData.stegoObjPtr = this;		
}


void JpegStegoDecoder::StegoGetMessage(void *cinfo, JBLOCKROW *MCU_data)
{
	/*JpegDCallbackData *pJCD = static_cast<JpegDCallbackData*>(cinfo->stegoDecoderData.stegoObjPtr);
	JpegStegoDecoder *pJSD = static_cast<JpegStegoDecoder*>(pJCD->stegoObjPtr);*/
	j_decompress_ptr *dcinfop = static_cast<j_decompress_ptr*>(cinfo);
	j_decompress_ptr dcinfo = *dcinfop;
	JpegStegoDecoder *pJSD =
			static_cast<JpegStegoDecoder*>(dcinfo->stego.stegoObjPtr);
	if(!pJSD->get_message)// || !pJSD->mesArray.IsArraySet())
		return;
	try
	{
		int DCT_pos;
		jpeg_component_info *compptr;

		for (int blkn = 0; blkn < dcinfo->blocks_in_MCU; blkn++)
		{
			compptr = dcinfo->cur_comp_info[dcinfo->MCU_membership[blkn]];
			if(pJSD->work_component == ALL || pJSD->work_component == compptr->component_id)	//check current component
			{
				if (pJSD->get_message) {
					DCT_pos = /*pJSD->*/selectPosition(MCU_data[blkn][0]);
					BYTE bit = abs(MCU_data[blkn][0][DCT_pos]);
					//cerr << (int)bit;
					if (DCT_pos != -1 && (bit == 1 || bit == 0)) {
						pJSD->mit = bit;
						pJSD->mit++;
					}
				}
				if (pJSD->blog) {
					pJSD->slog->writeData(compptr->component_id);
					for (int i = 0; i < 8; i++) {
						for (int j = 0; j < 8; j++) {
							pJSD->slog->writeCoef(MCU_data[blkn][0][i * 8 + j]);
						}
						pJSD->slog->endStr();
					}
				pJSD->slog->endBlock();
				}
			}
		}


	}catch(EndOfMessageException exc)
	{
		cerr << exc.getMessage()<<endl;
		//pJSD->sData.isStego = 0;
		dcinfo->stego.isStego=0;
		pJSD->get_message = false;
	}catch(DamagedMessageException exc)
	{
		cerr << exc.getMessage()<<endl;
		//pJSD->sData.isStego = 0;
		dcinfo->stego.isStego=0;
		pJSD->get_message = false;	
	}catch(Exception exc)
	{		
		cerr << exc.getMessage()<<endl;		
	}
}

void JpegStegoDecoder::StegoKochZhaoGet(void *cinfo, JBLOCKROW *MCU_data)
{
	j_decompress_ptr *dcinfop = static_cast<j_decompress_ptr*> (cinfo);
	j_decompress_ptr dcinfo = *dcinfop;
	JpegStegoDecoder *pJSD =
			static_cast<JpegStegoDecoder*> (dcinfo->stego.stegoObjPtr);

	if(!pJSD->get_message)
		return;
	try {
		LKoch DCT_pos;
		jpeg_component_info *compptr;
//		int D = pJSD->D;
//		int d = D >> 1;
//		int d3 = D - d * 2;
		int ii = 0;

		for (int blkn = 0; blkn < dcinfo->blocks_in_MCU; blkn++) {
			compptr = dcinfo->cur_comp_info[dcinfo->MCU_membership[blkn]];
			if (pJSD->blog) {
				pJSD->slog->writeData(compptr->component_id);
				for (int i = 0; i < 8; i++) {
					for (int j = 0; j < 8; j++) {
						pJSD->slog->writeCoef(MCU_DATA(blkn, i*8 + j));
					}
					pJSD->slog->endStr();
				}
				pJSD->slog->endBlock();
			}

			if (pJSD->work_component == ALL || pJSD->work_component
					== compptr->component_id) //check current component
			{
				if (pJSD->get_message) {
//					if (pJSD->paste_message && pJSD->perc >= pJSD->Pi) {
						for (int i = 0; i < KOCH_MAX; i++) {
							ii = (((double) rand() / (double) RAND_MAX)
									* (KOCH_MAX - 1) + 0);

							// get position from 'Possible location sets' table
							DCT_pos = KochZhaoPosition(ii);
#ifdef DEBUG
							cout <<id++ <<": ii=" <<ii
									<<", l1=" <<MCU_DATA(blkn, DCT_pos.l1)
									<<", l2=" <<MCU_DATA(blkn, DCT_pos.l2)
									<<", l3=" <<MCU_DATA(blkn, DCT_pos.l3);
#endif
							// check_read
							if (
									// l1=H, l2=L, l3=M
									(MCU_DATA(blkn, DCT_pos.l1) > MCU_DATA(blkn, DCT_pos.l3) &&
											MCU_DATA(blkn, DCT_pos.l2) < MCU_DATA(blkn, DCT_pos.l3)) ||

									// l1=L, l2=H, l3=M
									(MCU_DATA(blkn, DCT_pos.l1) < MCU_DATA(blkn, DCT_pos.l3) &&
											MCU_DATA(blkn, DCT_pos.l2) > MCU_DATA(blkn, DCT_pos.l3)) ||

									// l1=M, l2=M, l3=M
									(MCU_DATA(blkn, DCT_pos.l1) == MCU_DATA(blkn, DCT_pos.l3)&&
											MCU_DATA(blkn, DCT_pos.l2) == MCU_DATA(blkn, DCT_pos.l3))
									) {//continue;		// invalid combination
#ifdef DEBUG
									cout <<" -- break" <<endl;
#endif
									break;
							}

							if (MCU_DATA(blkn, DCT_pos.l1) > MCU_DATA(blkn, DCT_pos.l3)/*+D*/ &&
									MCU_DATA(blkn, DCT_pos.l2) > MCU_DATA(blkn, DCT_pos.l3))/*+D*/ {
								pJSD->mit = 1;
								pJSD->mit++;
#ifdef DEBUG
								cout <<" bit=" <<1 <<endl;
#endif
								break;
							} else if (MCU_DATA(blkn, DCT_pos.l1) < MCU_DATA(blkn, DCT_pos.l3)/*-D*/ &&
									MCU_DATA(blkn, DCT_pos.l2) < MCU_DATA(blkn, DCT_pos.l3)/*-D*/) {
								pJSD->mit = 0;
								pJSD->mit++;
#ifdef DEBUG
								cout <<" bit=" <<0 <<endl;
#endif
								break;
							} else{
#ifdef DEBUG
								cerr <<"bit index: " <<pJSD->mit.GetCurIdx() <<endl;
#endif
								throw DamagedMessageException("Bit in KZ block has been damaged",0,0);
							}

						}
//					}

//					pJSD->Pi++;
//					if (pJSD->Pi > 4)
//						pJSD->Pi = 1;
				}
			}
		}

	} catch (EndOfMessageException exc) {
		cerr << exc.getMessage() << endl;
		//pJSD->sData.isStego = 0;
		dcinfo->stego.isStego = 0;
		pJSD->get_message = false;
	} catch (DamagedMessageException exc) {
		cerr << exc.getMessage() << endl;
		//pJSD->sData.isStego = 0;
		dcinfo->stego.isStego = 0;
		pJSD->get_message = false;
	} catch (Exception exc) {
		cerr << exc.getMessage() << endl;
	}
}



int JpegStegoDecoder::Decode(char *infile, char *outfile, bool getMes)
{
//	if(!outfile)
#ifdef _WIN32
		outfile = (char*)"nul";
#else
		outfile = (char*)"/dev/null";
#endif

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

	if (blog) {
		slog = new JpegStegoLog;
		slog->Bin(binary);
		slog->setLogFileName(logfile);
		slog->openLOG(work_component);
	}

	InitJpegStego(true);

	int argc = 4;
	char *argv[4];
	char name[]="djpeg";
	
	argv[0]=name;
	argv[1]="-outfile";
	argv[2]=outfile;
	argv[3]=infile;

	return main_djpeg(argc, argv, sData);
}
