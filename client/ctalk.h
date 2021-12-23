#include <Python.h>

//
//PyObject* ctalk();
//PyObject* init_ctalk(uint8_t* data,  // height * width * 3
//                     int width,
//                     int height);
//
//bool load_reference(PyObject* pInstance,  // python���Ӧ��ʵ��
//                    uint8_t* data,        // ͼ��
//                    int width,            //ͼ��Ŀ�
//                    int height);
//bool load_audio(PyObject* pInstance, float* data, int lth = 5119);
//uint8_t* ctalk_process(PyObject* pInstance);






//PyObject* talk_Class = ctalk();
class Ctalk {
 private:
  PyObject* pClass;
  PyObject* pInstance;

 public:
  bool ctalk();
  void init_ctalk(uint8_t* data,  // height * width * 3
                   int width,
                   int height);
  bool load_reference(  // python���Ӧ��ʵ��
                      uint8_t* data,        // ͼ��
                      int width,            //ͼ��Ŀ�
                      int height);
  bool load_audio(float* data, int lth = 5119);
  uint8_t* ctalk_process();
};