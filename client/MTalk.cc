#include "MTalk.h"
#include "BmpOperation.h"

MTalkSingleton* MTalkSingleton::mTalkSingleton = new MTalkSingleton;

MTalkSingleton* MTalkSingleton::getInstance() {
  return mTalkSingleton;
}

void MTalkSingleton::print() {
  RTC_LOG(INFO) << "MTalkSingleton::" << __FUNCTION__ << ": " << this;
}

void MTalkSingleton::resetReferenceImage(unsigned long imageSize = 921601) {
  referenceImage.reset(new uint8_t[imageSize]);
}

void MTalkSingleton::resetMTalkImage(unsigned long imageSize = 921601) {
  mTalkImage.reset(new uint8_t[imageSize]);
}

void MTalkSingleton::setReferenceImage(uint8_t* image, int width, int height) {
  width_ = width;
  height_ = height;

  resetReferenceImage(width_ * height_ * 3);

  int length = width_ * height_ * 3;
  for (int i = 0; i < length; i++) {
    referenceImage.get()[i] = image[i];
  }
}

uint8_t* MTalkSingleton::getMTalkImage(uint8_t* output) {
  resetMTalkImage(width_ * height_ * 3);

  // 这里要将模型输出的图像放到mTalkImage里

  int length = width_ * height_ * 3;
  for (int i = 0; i < length; i++) {
    // 上面改完mTalkImage后，将下面改成 output[i] = mTalkImage[i];
    output[i] = referenceImage[i];
  }

  return output;
}

void MTalkSingleton::resetAudioData() {
  audioData = new float[kMAXAUDIODATASIZE];
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
    audioData[startIndex + audioLength + i] = (float)data[i];

  while (audioLength > KNEEDDATASIZE) {
    ++startIndex;
    --audioLength;
  }

  if (audioLength > 5119 || startIndex < 0 || startIndex >= kMAXAUDIODATASIZE)
    // something wrong
    return;
}
