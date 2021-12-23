#include <Python.h>
#include <numpy/arrayobject.h>
#include "ctalk.h"
#include "rtc_base/logging.h"

//PyObject* pClass = ctalk();
// ��ʼ��python�ļ����������е���
// ����python��
bool Ctalk::ctalk() {
  Py_SetPythonHome((wchar_t*)(L"E:\\tools\\anaconda\\anaconda\\envs\\mtalk"));
  Py_Initialize();
  import_array();

  PyRun_SimpleString("import sys");
  PyRun_SimpleString("sys.path.append('E:\\project\\makeittalk\\MakeItTalk')");

  PyObject* pName = PyUnicode_FromString("interfacer");
  PyObject* pModule = PyImport_Import(pName);
  if (!pModule) {
    printf("Module load error !");
    return false;
  }

  PyObject* pDict = PyModule_GetDict(pModule);
  if (pDict == NULL) {
    printf("Can't find the dictionary!");
    return false;
  }

  pClass = PyDict_GetItemString(pDict, "Interfacer");
  if (pClass == NULL) {
    printf("Can't find class!");
    return false;
  }
  return true;
}

// �����ʼ�ο�ͼ��ʵ������Ķ���
// ͼ����Ҫ(unsigned char) * height * width * 3, һά���鼴�ɡ�
// ����python���Ӧ��ʵ��
void Ctalk::init_ctalk(uint8_t* data,  // height * width * 3
                     int width,
                     int height) {
  npy_intp Dims[3] = {height, width, 3};
  PyObject* pArray = PyArray_SimpleNewFromData(3, Dims, NPY_UBYTE, data);
  PyObject* ArgArray = PyTuple_New(1);
  PyTuple_SetItem(ArgArray, 0, pArray);
  pInstance = PyObject_CallObject(pClass, ArgArray);
  assert(pInstance != NULL);
}

// ��ʵ��������ο�ͼ��
// ͼ����Ҫ(unsigned char) * height * width * 3, һά���鼴�ɡ�
bool Ctalk::load_reference(  // python���Ӧ��ʵ��
                    uint8_t* data,       // ͼ��
    int width, //ͼ��Ŀ�
    int height) { // ͼ��ĸ�
    npy_intp Dims[3] = {height, width, 3};
    PyObject* pArray = PyArray_SimpleNewFromData(3, Dims, NPY_UBYTE, data);
    PyObject* ArgArray = PyTuple_New(1);
    PyTuple_SetItem(ArgArray, 0, pArray);
    PyObject* Result =
        PyObject_CallMethod(pInstance, "load_reference", "O", pArray);

    /*Py_DECREF(pArray);
    Py_DECREF(ArgArray);*/
    if (Result == NULL) {
      return false;
    }
    //Py_DECREF(Result);
    return true;
}

// ��ʵ����������Ƶ
// lth
// ���������ó�25599����Ӧ�Ĳ���Ϊ20992��82��256��256Ϊһ֡����һ����Ҫ�϶��֡��
// lth
// �������ó�5119����Ӧ�Ĳ���Ϊ512��2��256��256Ϊһ֡����ÿ����Ծ��֡��ÿ�δ���20֡������18+֡2��
bool Ctalk::load_audio(float* data, int lth) {
  
  npy_intp Dims[1] = {lth};
  PyObject* pArray = PyArray_SimpleNewFromData(1, Dims, NPY_FLOAT32, data);
  /*PyObject* ArgArray = PyTuple_New(1);
  PyTuple_SetItem(ArgArray, 0, pArray);*/
  PyObject* pResult =
      PyObject_CallMethod(pInstance, "load_audio", "Oi", pArray, 16000);

  //Py_DECREF(pArray);
  //Py_DECREF(ArgArray);
  if (pResult == NULL) {
    return false;
  }
  //Py_DECREF(pResult);
  return true;
}

// ������̣��������ɵļ�ͼ��(unsigned char) * 256 * 256 * 3
uint8_t* Ctalk::ctalk_process() {

  PyObject* pResult1 =
      PyObject_CallMethod(pInstance, "audio2landmark", nullptr);
  assert(pResult1 != NULL);

  PyObject* pResult2 =
      PyObject_CallMethod(pInstance, "landmark2image", nullptr);
  assert(pResult2 != NULL);

  unsigned char* fake_image = (uint8_t*)malloc(
      sizeof(unsigned char) * 256 * 256 * 3);  //���ص�ͼ���СΪ256��256
  PyArg_Parse(pResult2, "O", &fake_image);
  return fake_image;
}