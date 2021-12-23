#include <iostream>
#include <cstdint>
#include "rtc_base/logging.h"
#include "api/audio/audio_frame.h"


class MTalkSingleton {
 protected:
  MTalkSingleton() {}

 private:
  static MTalkSingleton* mTalkSingleton;

  // video data
  std::unique_ptr<uint8_t[]> referenceImage;
  std::unique_ptr<uint8_t[]> mTalkImage;
  int width_, height_;

  // audio data
  float_t* audioData;
  int audioLength = -1;
  int startIndex = -1;

 public:
  static const int kMAXAUDIODATASIZE = 30000;  // 600ms for 16K sample rate
  int kNEEDDATASIZE = 25599;      // Mtalk ��Ҫ��������,��һ����������5119

  //int kFIRSTAUDIOSIZE = 25599;

  static MTalkSingleton* getInstance();

  bool savedImage = false;

  // for test
  void print();

  void resetReferenceImage(unsigned long imageSize);
  void resetMTalkImage(unsigned long imageSize);

  // �洢�ο�ͼ��
  void setReferenceImage(uint8_t* image, int width, int height);

  // ��ȡģ�������ͼƬ
  uint8_t* getMTalkImage(uint8_t* output);

  void resetAudioData();

  void setAudioData(const webrtc::AudioFrame& audio_frame);
};