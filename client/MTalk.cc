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
  mTalkImage.reset(new uint8_t[imageSize]);
}

void MTalkSingleton::setVideoImage(uint8_t* image, int width, int height) {
  width_ = width;
  height_ = height;

  resetVideoImage(width * height * 3);

  int length = width_ * height_ * 3;
  for (int i = 0; i < length; i++) {
    mTalkImage.get()[i] = image[i];
  }

  //SaveDIB2Bmp(1, "D:\\", width_, height_, image_.get());
}

uint8_t* MTalkSingleton::getMTalkImage(uint8_t* output) {
  // For test
  // ReadBmpRGB("D:\\RGB_Frame_3.bmp", mTalkImage.get());

  int length = width_ * height_ * 3;
  for (int i = 0; i < length; i++) {
    output[i] = mTalkImage[i];
  }

  return output;
}

void MTalkSingleton::resetAudioData() {
  audioData = new int16_t[kMAXAUDIODATASIZE];
  audioLength = 0;
  startIndex = 0;
}

void MTalkSingleton::setAudioData(const webrtc::AudioFrame& audio_frame) {
  if (audioLength == -1 || startIndex == -1)
    resetAudioData();

  int length = audio_frame.samples_per_channel_ * audio_frame.num_channels_;

  if (startIndex + audioLength + length > kMAXAUDIODATASIZE) {
    for (int i = 0; i < audioLength; i++) {
      audioData[i] = audioData[startIndex + i];
    }
    startIndex = 0;
  }

  const int16_t* data = audio_frame.data();
  for (int i = 0; i < length; i++)
    audioData[startIndex + audioLength + i] = data[i];

  while (audioLength > 5119) {
    ++startIndex;
    --audioLength;
  }

  if (audioLength > 5119 || startIndex < 0 || startIndex >= kMAXAUDIODATASIZE)
    // something wrong
    return;
}
