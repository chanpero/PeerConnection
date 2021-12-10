#include "examples/peerconnection/client/main_wnd.h"

#include<iostream>
#include <math.h>

#include "api/video/i420_buffer.h"
#include "third_party/libyuv/include/libyuv/convert_argb.h"
#include "third_party/libyuv/include/libyuv/convert_from_argb.h"

#include "BmpOperation.h"

// @Author: chanper

// 构建BMP位图文件头
void ContructBhh(int nWidth,
                 int nHeight,
                 BITMAPFILEHEADER& bhh)  // add 2010-9-04
{
  int widthStep = (((nWidth * 24) + 31) & (~31)) / 8;  //每行实际占用的大小（每行都被填充到一个4字节边界）
  bhh.bfType = ((WORD)('M' << 8) | 'B');  //'BM'
  bhh.bfSize = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER) + widthStep * nHeight;
  bhh.bfReserved1 = 0;
  bhh.bfReserved2 = 0;
  bhh.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER);
}

// 构建BMP文件信息头
void ConstructBih(int nWidth, int nHeight, BITMAPINFOHEADER& bih) {
  int widthStep = (((nWidth * 24) + 31) & (~31)) / 8;

  bih.biSize = 40;  // header size
  bih.biWidth = nWidth;
  bih.biHeight = -nHeight; //chanper: specify the height while specify the direction of the bitmap
  bih.biPlanes = 1;
  bih.biBitCount = 24;         // RGB encoded, 24 bit
  bih.biCompression = BI_RGB;  // no compression 非压缩
  bih.biSizeImage = widthStep * nHeight * 3;
  bih.biXPelsPerMeter = 0;
  bih.biYPelsPerMeter = 0;
  bih.biClrUsed = 0;
  bih.biClrImportant = 0;
}

bool SaveDIB2Bmp(int fileNum,
                 const char* filePath,
                 int iWidth,
                 int iHeight,
                 BYTE* pBuffer) {
  BITMAPINFOHEADER bih;
  ConstructBih(iWidth, iHeight, bih);
  BITMAPFILEHEADER bhh;
  ContructBhh(iWidth, iHeight, bhh);

  CHAR BMPFileName[1024];
  int widthStep = (((iWidth * 24) + 31) & (~31)) /
                  8;  //每行实际占用的大小（每行都被填充到一个4字节边界）
  int DIBSize = widthStep * iHeight;  // buffer的大小 （字节为单位）

  char path[1024];
  char str[1024];
  char num[10];
  sprintf(num, "%d", fileNum);
  sprintf(str, "RGB_Frame_");
  strcat(str, num);
  strcat(str, ".bmp");
  sprintf(path, "%s", filePath);
  strcat(path, str);  // Path//RGB_Frame_1.bmp

  strcpy(BMPFileName, path);
  FILE* file;

  if ((file = fopen(BMPFileName, "wb"))) {
    //写入文件
    fwrite((LPSTR)&bhh, sizeof(BITMAPFILEHEADER), 1, file);
    fwrite((LPSTR)&bih, sizeof(BITMAPINFOHEADER), 1, file);
    fwrite(pBuffer, sizeof(char), DIBSize, file);
    fclose(file);
    return true;
  }
  return false;
}

BITMAPV5HEADER ReadBmpRGB(const char* bmpName, uint8_t* dst_argb) {
  int bmpWidth;            //图像的宽
  int bmpHeight;           //图像的高
  RGBQUAD* pColorTable;    //颜色表指针
  int biBitCount;          //图像类型，每像素位数

  //定义位图信息头结构变量，读取位图信息头进内存,注意bitmapinfoheader版本
  BITMAPFILEHEADER bitmapFileHeader;
  BITMAPV5HEADER bitmapInfoHeader;

  //二进制读方式打开指定的图像文件
  FILE* fp = fopen(bmpName, "rb");
  if (fp == 0)
    return bitmapInfoHeader;
    
  
  // chanper: 注意bitmapinfoheader的版本！！！
  fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, fp);
  fread(&bitmapInfoHeader, sizeof(BITMAPV5HEADER), 1, fp);

  //获取图像宽、高、每像素所占位数等信息
  bmpWidth = bitmapInfoHeader.bV5Width;
  bmpHeight = bitmapInfoHeader.bV5Height;
  biBitCount = bitmapInfoHeader.bV5BitCount;

  //定义变量，计算图像每行像素所占的字节数（必须是4的倍数）
  // int lineByte=(bmpWidth * biBitCount/8+3)/4*4;
  int lineByte = (bmpWidth * biBitCount / 8 + 3) / 4 * 4;

  //灰度图像有颜色表，且颜色表表项为256
  if (biBitCount == 8) {
    //申请颜色表所需要的空间，读颜色表进内存
    pColorTable = new RGBQUAD[256];
    fread(pColorTable, sizeof(RGBQUAD), 256, fp);
  }

  //申请位图数据所需要的空间，读位图数据进内存
  fread(dst_argb, 1, lineByte * bmpHeight, fp);

  //关闭文件
  fclose(fp);
  return bitmapInfoHeader;
}
