#include "MTalk.h"
#include "BmpOperation.h"

MTalkSingleton* MTalkSingleton::mTalkSingleton = new MTalkSingleton;

MTalkSingleton* MTalkSingleton::getInstance() {
  return mTalkSingleton;
}

void MTalkSingleton::print() {
  RTC_LOG(INFO) << "MTalkSingleton::" << __FUNCTION__ << ": " << this;
}

void MTalkSingleton::resetVideoImage(unsigned long imageSize = 921601) {
  image_.reset(new uint8_t[imageSize]);
}

void MTalkSingleton::setVideoImage(uint8_t* image, int width, int height) {
  width_ = width;
  height_ = height;

  int length = width_ * height_ * 3;
  for (int i = 0; i < length; i++) {
    image_.get()[i] = image[i];
  }

  //SaveDIB2Bmp(1, "D:\\", width_, height_, image_.get());
}

uint8_t* MTalkSingleton::getSytheticImage(uint8_t* mtalkImage, uint8_t* output) {
  int length = width_ * height_ * 3;
  for (int i = 0; i < length; i++) {
    output[i] = mtalkImage[i];
  }

  return output;
}