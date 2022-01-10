#include <Python.h>
#include <numpy/arrayobject.h>
#include "ctalk.h"
#include "rtc_base/logging.h"

// PyObject* pClass = ctalk();
  // 初始化python文件，声明其中的类
  // 返回python类
  bool Ctalk::ctalk() {
    Py_SetPythonHome((wchar_t*)(L"C:\\Users\\chanper\\anaconda3\\envs\\mtalk"));
    Py_Initialize();
    import_array();


    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('D:\\Study\\mtalk\\MakeItTalk')");

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

  // 读入初始参考图像，实例化类的对象
  // 图像需要(unsigned char) * height * width * 3, 一维数组即可。
  // 返回python类对应的实例
  void Ctalk::init_ctalk(uint8_t* data, int width, int height) {
    npy_intp Dims[3] = {height, width, 3};
    PyObject* pArray = PyArray_SimpleNewFromData(3, Dims, NPY_UBYTE, data);
    PyObject* ArgArray = PyTuple_New(1);
    PyTuple_SetItem(ArgArray, 0, pArray);
    pInstance = PyObject_CallObject(pClass, ArgArray);
    assert(pInstance != NULL);
  }

  // 向实例中载入参考图像
  // 图像需要(unsigned char) * height * width * 3, 一维数组即可。
  bool Ctalk::load_reference(uint8_t* data, int width, int height) {
    npy_intp Dims[3] = {height, width, 3};
    PyObject* pArray = PyArray_SimpleNewFromData(3, Dims, NPY_UBYTE, data);
    PyObject* ArgArray = PyTuple_New(1);
    PyTuple_SetItem(ArgArray, 0, pArray);
    PyObject* Result =
        PyObject_CallMethod(pInstance, "load_reference", "O", pArray);

    if (Result == NULL) {
      return false;
    }

    return true;
  }

  // 向实例中载入音频
  // lth初次需设置成25599，对应的步长为20992（82×256，256为一帧，第一次需要较多的帧）
  // 后续设置成5119，对应的步长为512（2×256，256为一帧，即每次跳跃两帧，每次处理20帧：窗口18+帧2）
  bool Ctalk::load_audio(float* data, int lth) {
    npy_intp Dims[1] = {lth};
    PyObject* pArray = PyArray_SimpleNewFromData(1, Dims, NPY_FLOAT32, data);
    PyObject* pResult =
        PyObject_CallMethod(pInstance, "load_audio", "Oi", pArray, 16000);
    
    if (pResult == NULL) {
      return false;
    }

    return true;
  }

  // 处理过程，返回生成的假图像(unsigned char) * 256 * 256 * 3
  uint8_t* Ctalk::ctalk_process() {
    PyObject* pResult1 =
        PyObject_CallMethod(pInstance, "audio2landmark", nullptr);
    assert(pResult1 != NULL);

    PyObject* pResult2 =
        PyObject_CallMethod(pInstance, "landmark2image", nullptr);
    assert(pResult2 != NULL);

    uint8_t* fake_image = (uint8_t*)malloc(sizeof(uint8_t) * 640 * 480 * 3);  //返回的图像大小为640×480
    PyArrayObject* fake_image_array;
    PyArray_OutputConverter(pResult2, &fake_image_array);
    int t = 0;
    for (int i = 0; i < 480; i++) {
      for (int j = 0; j < 640; j++) {
        uint8_t b = *(uint8_t*)PyArray_GETPTR3(fake_image_array, i, j, 0);
        uint8_t g = *(uint8_t*)PyArray_GETPTR3(fake_image_array, i, j, 1);
        uint8_t r = *(uint8_t*)PyArray_GETPTR3(fake_image_array, i, j, 2);
        fake_image[t++] = b;
        fake_image[t++] = g;
        fake_image[t++] = r;
      }
    }

    return fake_image;
  }