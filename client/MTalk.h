#include<iostream>
#include "rtc_base/logging.h"


class MTalkSingleton {
 protected:
  MTalkSingleton() {}

 private:
  static MTalkSingleton* mTalkSingleton;

  std::unique_ptr<uint8_t[]> image_;
  int width_, height_;

 public:
  static MTalkSingleton* getInstance();

  void print();

  void resetVideoImage(unsigned long imageSize);
  // �洢�ο�ͼ��
  void setVideoImage(uint8_t* image, int width, int height);
  // ��ȡģ�������ͼƬ
  uint8_t* getSytheticImage(uint8_t* mtalkImage, uint8_t* output);

  void resetAudioData();
  void setAudioData();
};