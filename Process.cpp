#include "Process.h"
int flag = 0;
#include<fstream>
using namespace std;
void IntTo4Bits(int v, unsigned char*bits)
{
	bits[3] = (unsigned char)(v >> 24);
	v = v - ((v >> 24) << 24);
	bits[2] = (unsigned char)(v >> 16);
	v = v - ((v >> 16) << 16);
	bits[1] = (unsigned char)(v >> 8);
	v = v - ((v >> 8) << 8);
	bits[0] = (unsigned char)v;
}

//保存BMP图像
void SaveAsBmp(const char *fn, char *bgra, int w, int h)
{
	ofstream out;
	out.open(fn, ios::trunc);


	//写BMP文件头
	unsigned char fileHead[14] = { 0x42, 0x4D,                //BM
								0x36, 0xE0, 0x10, 0x00,    //文件大小  //change
								0x00, 0x00,                //保留1
								0x00, 0x00,                //保留2
								0x36, 0x00, 0x00, 0x00     //数据相对文件起始位置偏移量 54
	};
	int len = w * h * 4 + 54;
	IntTo4Bits(len, &fileHead[2]);          //修改文件大小
	out.write((char*)fileHead, 14);

	//写BMP信息头
	unsigned char infoHead[40] = { 0x28, 0x00, 0x00, 0x00,    //该结构体大小  40
								0x00, 0x03, 0x00, 0x00,    //图像宽
								0xE0, 0x01, 0x00, 0x00,    //图像高
								0x01, 0x00,                //biPlanes
								0x20, 0x00,                //biBitCount  32位 //change
								0x00, 0x00, 0x00, 0x00,    //压缩方式，不压缩
								0x00, 0xE0, 0x10, 0x00,    //数据大小
								0x00, 0x00, 0x00, 0x00,    //biXPelsPerMeter
								0x00, 0x00, 0x00, 0x00,    //biYPelsPerMeter
								0x00, 0x00, 0x00, 0x00,    //biClrUsed
								0x00, 0x00, 0x00, 0x00,    //biClrImportant
	};
	IntTo4Bits(w, &infoHead[4]);          //修改图像高度
	IntTo4Bits(h, &infoHead[8]);          //修改图像宽度
	IntTo4Bits(len - 54, &infoHead[20]);    //修改数据大小
	out.write((char*)infoHead, 40);

	//写数据
	bgra += w * 4 * (h - 1);
	for (int i = 0; i < h; i++)
	{
		out.write(bgra, w * 4);
		bgra -= w * 4;
	}

	out.close();
}
void Process::Black_and_white()
{
	for (int i = 0; i < width* height; i++)
	{
		pframe[i].rgbBlue = pframe[i].rgbRed = pframe[i].rgbGreen =
			(pframe[i].rgbBlue + pframe[i].rgbRed + pframe[i].rgbGreen) / 3;

	}
}
void Process::Nagation()
{
	for (int i = 0; i < width* height; i++)
	{
		pframe[i].rgbBlue = 255 -pframe[i].rgbBlue;
		pframe[i].rgbRed = 255 - pframe[i].rgbRed;
		pframe[i].rgbGreen = 255 - pframe[i].rgbGreen;
	}
}
void Process::Save()
{
	int w = width;
	int h = height;
	int i = 0, j = 0;
	//w = ((w * 32 + 31) / 32) * 4;
	int line_Width = ((w * 32 + 31) / 32) * 4;

	char * data = new char[w*h * 4];
	memset(data, 0, w*h * 4);

	//修改图像数据
	for (i = 0; i < w*h  ; i += 1)
	{
		data[4 * i] =  (pframe[i].rgbBlue);
		data[4 * i + 1] = (pframe[i].rgbGreen) ;
		data[4 * i + 2] = (pframe[i].rgbRed);
		data[4 * i + 3] = pframe[i].rgbReserved;
	}
	//for (i = 0; i < h; i++)
	//{
	//	for (j = 0; j < w ; j++)
	//	{
	//		data[4 * j] = (pframe[j + i * w].rgbBlue);
	//		data[4 * j + 1] = (pframe[j + i * w].rgbGreen);
	//		data[4 * j + 2] = (pframe[j + i * w].rgbRed);
	//		data[4 * j + 3] = 0;
	//	}
	//	/*for (int k = j; k < line_Width; k++)
	//	{
	//		data[4 * k] = data[4 * k + 1]= data[4 * k + 2]= data[4 * k + 3]=0;

	//	}*/
	//}
	SaveAsBmp("test1.bmp", data, w, h);
	free(data);
}