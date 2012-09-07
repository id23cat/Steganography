#ifdef _WIN32
#include "StdAfx.h"
#endif
#include <memory.h>
#include "JpegStegoLog.h"

JpegStegoLog::JpegStegoLog(void)
{
	logfile = NULL;
	ok = false;
	bin = false;
}

JpegStegoLog::~JpegStegoLog(void)
{
	if(logfile!=NULL)
		fclose(logfile);
	ok = false;
}

void JpegStegoLog::setLogFileName(char fname[FNAME_LEN],char *ext)
{
	memset(log_fname, 0, FNAME_LEN);
#ifdef _WIN32
	strncat_s(log_fname, 80, fname, (strlen(fname)-4));
	if(ext)
		strcat_s(log_fname, 80, ext);
#else
	strncat(log_fname,fname, (strlen(fname)-4));
	if(ext)
		strcat(log_fname, ext);
#endif
	
	
}

int JpegStegoLog::openLOG(int component)
{
	/*if(component == Y)
		strcat_s(log_fname, 80, "Y.log");
	else if(component == Cb)
		strcat_s(log_fname, 80, "Cb.log");
	else if(component == Cr)
		strcat_s(log_fname, 80, "Cr.log");
	else*/
#ifdef _WIN32
		strcat_s(log_fname, 80, ".log");
#else
		strcat(log_fname, ".log");
#endif

	if(bin)
	{
#ifdef _WIN32
		if (fopen_s(&logfile, log_fname, "wb")!=0)
		{
			logfile = NULL;
			ok = false;
			Exception exc;
			char strerr[STR_LEN];
			sprintf_s(strerr, STR_LEN, "can't open LOG %s\n", log_fname);
			exc.setMessage(strerr);
			throw exc;
		}
#else
		if ((logfile=fopen(log_fname, "wb"))!=0)
		{
			logfile = NULL;
			ok = false;
			Exception exc;
			char strerr[STR_LEN];
			sprintf(strerr, "can't open LOG %s\n", log_fname);
			exc.setMessage(strerr);
			throw exc;
		}
#endif
	}
	else
	{
#ifdef _WIN32
		if (fopen_s(&logfile, log_fname, "w")!=0)
		{
			logfile = NULL;
			ok = false;
			Exception exc;
			char strerr[STR_LEN];
			sprintf_s(strerr, STR_LEN, "can't open LOG %s\n", log_fname);
			exc.setMessage(strerr);
			throw exc;
		}
#else
		if ((logfile=fopen(log_fname, "w"))==NULL)
		{
			logfile = NULL;
			ok = false;
			Exception exc;
			char strerr[STR_LEN];
			sprintf(strerr, "can't open LOG %s\n", log_fname);
			exc.setMessage(strerr);
			throw exc;
		}
#endif
	}
	counter=1;
	ok = true;
	return 0;
}

void JpegStegoLog::close()
{
	if(logfile!=NULL){
		fclose(logfile);
		logfile=NULL;
		ok = false;
	}
}

int JpegStegoLog::writeData(int work_component,
			  size_t writed,
			  size_t length,
			  int cur_component)
{
	if(logfile==NULL)
		return -1;
	if(!bin)
	{
		fprintf(logfile,"===========================%s=\n", toChar(work_component));
		fprintf(logfile,"Writed %d bits of %d: %d\n", writed, length, counter++);
		//fprintf(logfile,"Component ID( 1(Y), 2(Cb), 3(Cr) ): %d\n", toChar(cur_component));
		fprintf(logfile,"Current component: %s\n", toChar(cur_component));
	}else
	{
		short comp = (short)cur_component;
		fwrite(&comp, sizeof(short), 1, logfile);
	}
	return 0;
}

int JpegStegoLog::writeData(int cur_component)
{
	if(logfile==NULL)
		return -1;
	if(!bin)	fprintf(logfile,"Current component: %s\n", toChar(cur_component));
	else
	{
		short comp = (short)cur_component;
		fwrite(&comp, sizeof(short), 1, logfile);
	}
	return 0;
}

int JpegStegoLog::writeCoef(int coef)
{
	if(logfile==NULL)
		return -1;
	if(!bin)	fprintf(logfile,"%d\t", coef);
	else{short co = (short)coef;		fwrite(&co, sizeof(short), 1, logfile);}
	return 0;
}

int JpegStegoLog::endBlock()
{
	if(logfile==NULL)
		return -1;
	if(!bin)		fprintf(logfile,"\n============================\n\n");
	return 0;
}

int JpegStegoLog::endStr()
{
	if(logfile==NULL)
		return -1;
	if(!bin)		fprintf(logfile,"\n");
	return 0;
}

char* JpegStegoLog::toChar(int component)
{
	static char str[4] = {0,0,0,0};
#ifdef _WIN32
	if(component == Y)
		strcpy_s(str, 4, "Y");
	else if(component == Cb)
		strcpy_s(str, 4, "Cb");
	else if(component == Cr)
		strcpy_s(str, 4, "Cr");
	else
		strcpy_s(str, 4, "ALL");
#else
	if(component == Y)
		strcpy(str, "Y");
	else if(component == Cb)
		strcpy(str, "Cb");
	else if(component == Cr)
		strcpy(str, "Cr");
	else
		strcpy(str, "ALL");
#endif
	return str;
}
