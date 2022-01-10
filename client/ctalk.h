#include <Python.h>

class Ctalk {
 private:
  PyObject* pClass;
  PyObject* pInstance;

 public:
  bool ctalk();
  void init_ctalk(uint8_t* data,  // height * width * 3
                  int width,
                  int height);
  bool load_reference(uint8_t* data, int width, int height);
  bool load_audio(float* data, int lth = 5119);
  uint8_t* ctalk_process();
  bool first = true;
};