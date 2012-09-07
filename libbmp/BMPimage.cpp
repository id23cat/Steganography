#ifdef _WIN32
#include "StdAfx.h"
#endif
#include "BMPimage.h"
#include <stdlib.h>
#include <stdio.h>

#define PRT_ERR_STRING(str) printf("In libbmp %s %d: %s\n",__FILE__,__LINE__ , str)

BMPimage::BMPimage(void)
{
	//width = 0;
	//height = 0;
	rgb=NULL;
	//rgblen=0;
}

BMPimage::~BMPimage(void)
{
}

/**** privates *******/

//int BMPimage::ReadHeader(fstream &in)throw(...)
//{
//	//in.seekg(0,ios::beg);
//	if(in.fail())
//		throw Exception("Failed to read BMP header");
//	in.read((char*)&filHead, sizeof(filHead));
//	if(in.fail())
//		throw Exception("Failed to read BMP header");
//	in.read((char*)&infHead, sizeof(infHead));
//	if(in.fail())
//		throw Exception("Failed to read BMP header");
//	if(infHead.biBitCount<16)
//		return -1;
//	height = infHead.biHeight;
//	width = infHead.biWidth;
//	return 0;
//}
//
//int BMPimage::ReadData(fstream &in)throw(...)
//{
//	in.seekg(filHead.bfOffBits,ios::beg);
//	if(in.fail())
//		throw Exception("Failed to read BMP data");
//	if(infHead.biBitCount<16)
//		return -1;
//	rgblen = width * height * (infHead.biBitCount/8);
//	if(!(rgb = new BYTE[rgblen]))					// !!!!!!!! may be fault!!!!!!!!!!!!!!
//		throw Exception("Failed to allocate memory for RGB array");
//
//	in.read((char*)rgb,rgblen);
//	if(in.fail())
//		throw Exception("Failed to read BMP data");
//	return 0;
//}
//int BMPimage::WriteHeader(fstream &out)throw(...)
//{
//	out.seekg(0,ios::beg);
//	if(out.fail())
//		throw Exception("Failed to write BMP header");
//	out.write((char*)&filHead, sizeof(filHead));
//	if(out.fail())
//		throw Exception("Failed to write BMP header");
//	out.write((char*)&infHead, sizeof(infHead));
//	if(out.fail())
//		throw Exception("Failed to write BMP header");
//	return 0;
//}
//
//int BMPimage::writeData(fstream &out)throw(...)
//{
//	out.seekg(filHead.bfOffBits,ios::beg);
//	if(out.fail())
//		throw Exception("Failed to write BMP data");
//	out.write((char*)rgb,rgblen);
//	if(out.fail())
//		throw Exception("Failed to write BMP data");
//	return 0;
//}
//
//
void BMPimage::TopToBottom()
{
	BYTE *nr;
	size_t wwidth = Width*3;

	nr = new BYTE[wwidth];	
	
	int k1,k2;
	for(int i=0; i<Height/2; i++)
	{
		k1 = i*wwidth;
		k2 = (Height-i-1)*wwidth;
		memcpy(nr,&(rgb[k1]),wwidth);		
		memcpy(&(rgb[k1]),&(rgb[k2]),wwidth);
		memcpy(&(rgb[k2]),nr,wwidth);

	}
	delete nr;	
}
//
////void BMPimage::MAStoRGB()
////{
////	B = new BYTE[height * width];
////	R = new BYTE[height * width];
////	G = new BYTE[height * width];
////	for(int i=0,j=0; i < height * width; i++)
////	{
////		B[i] = rgb[j++] ;
////		G[i] = rgb[j++];
////		R[i] = rgb[j++];
////	}
////}
//void BMPimage::BGRtoRGB()
//{
//	//B = new BYTE[height * width];
//	//R = new BYTE[height * width];
//	//G = new BYTE[height * width];
//	//for(int i=0,j=0; i < height * width; i++)
//	//{
//	//	B[i] = rgb[j++] ;
//	//	G[i] = rgb[j++];
//	//	R[i] = rgb[j++];
//	//}
//	BYTE tmp;
//	for(int i=0; i<Height*Width*3; i+=3)
//	{
//		tmp = rgb[i];
//		rgb[i]=rgb[i+2];
//		rgb[i+2]=tmp;
//	}
//}

//bool BMPimage::GetPointerToRGB(BYTE *arr, DESCR src)
//{
//	//if((*arr = new BYTE[src.width * src.height*3])==NULL)
//	//	return false;
//	if(arr == NULL)
//		return false;
//	TopToBottom(src);
//
//	int i,j;
//	for(i=0,j=0; i<src.width * src.height; i++)
//	{
//		arr[j++] = src.R[i];
//		arr[j++] = src.G[i];
//		arr[j++] = src.B[i];
//	}
//	return true;
//}

void BMPimage::BGRtoRGB(BYTE *bgr)
{
	BYTE b;
	b = bgr[0];
	bgr[0]=bgr[2];
	bgr[2]=b;
}


/**** publics *******/
int BMPimage::ReadRGB(char *fname)/*throw(...)*/
{
	//fstream in=fstream(fname,ios::in);
	//if(!in)
	//	throw Exception("BMPimage::ReadRGB: Failed to open BMP file\n");
	//int ret = ReadHeader(in);
	//if(ret==-1)
	//	throw Exception("BMPimage::ReadRGB: May be damaged BMP file\n");
	//ReadData(in);
	//in.close();

	////convert data
	//BGRtoRGB();
	//TopToBottom();
	
	ReadFromFile(fname);	
	return 0;
}

BYTE * BMPimage::GetRGBptr(size_t &h, size_t &w)
{
	h=Height;
	w=Width;
	size_t rgblen=h*w*BitDepth/8;
	if(rgb)
		return rgb;
	if(!(rgb = new BYTE[rgblen])){					// !!!!!!!! may be fault!!!!!!!!!!!!!!
//		throw Exception("Can not allocate memory for RGB array");
		PRT_ERR_STRING("Can not allocate memory for RGB array");
		exit(1);

	}
	BYTE *ptr;
	for(int i=0;i<Height;i++)
		for(int j=0;j<Width;j++)
		{
			ptr = rgb+(Height-i-1)*Width+j*3;
			if(ptr<rgb||ptr>(rgb+rgblen-1)){
//				throw Exception("BMPimage::GetRGBptr(): out of array\n");
				PRT_ERR_STRING("BMPimage::GetRGBptr(): out of array");
				exit(1);
			}
		
			//memcpy(ptr,&GetPixel(j,i),3);
			memcpy(ptr,(*this)(j,i),3);
			BGRtoRGB(ptr);

		}
	//TopToBottom();
	return rgb;
}

//void BMPimage::SetCallbackFunction(void (func*)(BYTE *, size_t, size_t))
//{
//}

void BMPimage::ObjPtr(void *prt)
{
}

//int BMPimage::ReadBMPheader(char *fname, size_t &h, size_t &w)throw(...)
//{
//	/*fstream in=fstream(fname,ios::in);
//	if(!in)	
//		throw Exception("Failed to open BMP file");
//	ReadHeader(in);	
//	in.close();*/
//	return 0;
//}

int BMPimage::WriteRGB(char *fname)/*throw(...)*/
{
	/*fstream out=std::fstream(fname,ios::out);
	if(!out)
		throw Exception("Failed to open BMP file");
	WriteHeader(out);
	writeData(out);
	out.close();*/
	WriteToFile(fname);
	return 0;
}

//int BMPimage::ReWriteRGB(char *fname)
//{
//	return 0;
//}
