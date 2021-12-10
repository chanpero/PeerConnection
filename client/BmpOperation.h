#include "examples/peerconnection/client/main_wnd.h"

#include <math.h>

#include "api/video/i420_buffer.h"
#include "third_party/libyuv/include/libyuv/convert_argb.h"
#include "third_party/libyuv/include/libyuv/convert_from_argb.h"

// @Author: chanper

// 构建BMP位图文件头
void ContructBhh(int nWidth,
                 int nHeight,
                 BITMAPFILEHEADER& bhh);

// 构建BMP文件信息头
void ConstructBih(int nWidth, int nHeight, BITMAPINFOHEADER& bih);

bool SaveDIB2Bmp(int fileNum,
                 const char* filePath,
                 int iWidth,
                 int iHeight,
                 BYTE* pBuffer);

BITMAPV5HEADER ReadBmpRGB(const char* bmpName, uint8_t* dst_argb);