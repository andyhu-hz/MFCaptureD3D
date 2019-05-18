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

//����BMPͼ��
void SaveAsBmp(const char *fn, char *bgra, int w, int h)
{
	ofstream out;
	out.open(fn, ios::trunc);


	//дBMP�ļ�ͷ
	unsigned char fileHead[14] = { 0x42, 0x4D,                //BM
								0x36, 0xE0, 0x10, 0x00,    //�ļ���С  //change
								0x00, 0x00,                //����1
								0x00, 0x00,                //����2
								0x36, 0x00, 0x00, 0x00     //��������ļ���ʼλ��ƫ���� 54
	};
	int len = w * h * 4 + 54;
	IntTo4Bits(len, &fileHead[2]);          //�޸��ļ���С
	out.write((char*)fileHead, 14);

	//дBMP��Ϣͷ
	unsigned char infoHead[40] = { 0x28, 0x00, 0x00, 0x00,    //�ýṹ���С  40
								0x00, 0x03, 0x00, 0x00,    //ͼ���
								0xE0, 0x01, 0x00, 0x00,    //ͼ���
								0x01, 0x00,                //biPlanes
								0x20, 0x00,                //biBitCount  32λ //change
								0x00, 0x00, 0x00, 0x00,    //ѹ����ʽ����ѹ��
								0x00, 0xE0, 0x10, 0x00,    //���ݴ�С
								0x00, 0x00, 0x00, 0x00,    //biXPelsPerMeter
								0x00, 0x00, 0x00, 0x00,    //biYPelsPerMeter
								0x00, 0x00, 0x00, 0x00,    //biClrUsed
								0x00, 0x00, 0x00, 0x00,    //biClrImportant
	};
	IntTo4Bits(w, &infoHead[4]);          //�޸�ͼ��߶�
	IntTo4Bits(h, &infoHead[8]);          //�޸�ͼ����
	IntTo4Bits(len - 54, &infoHead[20]);    //�޸����ݴ�С
	out.write((char*)infoHead, 40);

	//д����
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

	//�޸�ͼ������
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