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
  std::unique_ptr<uint8_t[]> mTalkImage;
  int width_, height_;

  // audio data
  float_t* audioData;
  int audioLength = -1;
  int startIndex = -1;

 public:
  static const int kMAXAUDIODATASIZE = 9600;  // 600ms for 16K sample rate
  static const int KNEEDDATASIZE = 5119;      // Mtalk 需要的数据量

  static MTalkSingleton* getInstance();

  bool savedImage = false;

  // for test
  void print();

  void resetVideoImage(unsigned long imageSize);

  // 存储参考图像
  void setVideoImage(uint8_t* image, int width, int height);

  // 获取模型输出的图片
  uint8_t* getMTalkImage(uint8_t* output);

  void resetAudioData();

  void setAudioData(const webrtc::AudioFrame& audio_frame);
};