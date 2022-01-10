#include "MTalk.h"
#include "ctalk.h"

MTalkSingleton* MTalkSingleton::mTalkSingleton = new MTalkSingleton;
Ctalk ct = Ctalk();

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
  ct.ctalk();
  ct.init_ctalk(MTalkSingleton::referenceImage.get(), MTalkSingleton::width_,
                MTalkSingleton::height_);
}

// 这里要将模型输出的图像放到mTalkImage里
uint8_t* MTalkSingleton::getMTalkImage(uint8_t* output) {
  resetMTalkImage(width_ * height_ * 3);

  assert(audioLength > 5119);
  ct.load_audio(MTalkSingleton::audioData, kNEEDDATASIZE);
  if (ct.first) {
    kNEEDDATASIZE = 5119;
    ct.first = false;
  }
  unsigned char* fake_image = ct.ctalk_process();

  mTalkImage.reset(fake_image);

  int length = width_ * height_ * 3;
  for (int i = 0; i < length; i++) {
    output[i] = mTalkImage[i];
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

  const int16_t* data = audio_frame.data();
  for (int i = 0; i < length; i++) {
    int indexToPut = (startIndex + audioLength + i) % kMAXAUDIODATASIZE;
    audioData[indexToPut] = (float)data[i] / 32768.0;
  }

  audioLength += length;
  while (audioLength > kMAXAUDIODATASIZE) {
    ++startIndex;
    --audioLength;
  }
}