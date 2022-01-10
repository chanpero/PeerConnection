/*
 *  Copyright 2012 The WebRTC Project Authors. All rights reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

// clang-format off
// clang formating would change include order.
#include <windows.h>
#include <shellapi.h>  // must come after windows.h
// clang-format on

#include <string>
#include <vector>

#include "absl/flags/parse.h"
#include "examples/peerconnection/client/conductor.h"
#include "examples/peerconnection/client/flag_defs.h"
#include "examples/peerconnection/client/main_wnd.h"
#include "examples/peerconnection/client/peer_connection_client.h"
#include "rtc_base/checks.h"
#include "rtc_base/constructor_magic.h"
#include "rtc_base/ssl_adapter.h"
#include "rtc_base/string_utils.h"  // For ToUtf8
#include "rtc_base/win32_socket_init.h"
#include "system_wrappers/include/field_trial.h"
#include "test/field_trial.h"
#include <Python.h>


namespace {
// A helper class to translate Windows command line arguments into UTF8,
// which then allows us to just pass them to the flags system.
// This encapsulates all the work of getting the command line and translating
// it to an array of 8-bit strings; all you have to do is create one of these,
// and then call argc() and argv().
class WindowsCommandLineArguments {
 public:
  WindowsCommandLineArguments();

  int argc() { return argv_.size(); }
  char** argv() { return argv_.data(); }

 private:
  // Owned argument strings.
  std::vector<std::string> args_;
  // Pointers, to get layout compatible with char** argv.
  std::vector<char*> argv_;

 private:
  RTC_DISALLOW_COPY_AND_ASSIGN(WindowsCommandLineArguments);
};

WindowsCommandLineArguments::WindowsCommandLineArguments() {
  // start by getting the command line.
  LPCWSTR command_line = ::GetCommandLineW();
  // now, convert it to a list of wide char strings.
  int argc;
  LPWSTR* wide_argv = ::CommandLineToArgvW(command_line, &argc);

  // iterate over the returned wide strings;
  for (int i = 0; i < argc; ++i) {
    args_.push_back(rtc::ToUtf8(wide_argv[i], wcslen(wide_argv[i])));
    // make sure the argv array points to the string data.
    argv_.push_back(const_cast<char*>(args_.back().c_str()));
  }
  LocalFree(wide_argv);
}
//
//// 初始化python文件，声明其中的类
//// 返回python类
//PyObject* ctalk() {
//  Py_SetPythonHome((wchar_t*)(L"E:\\tools\\anaconda\\anaconda\\envs\\mtalk"));
//  Py_Initialize();
//  import_array();
//
//  PyRun_SimpleString("import sys");
//  PyRun_SimpleString("sys.path.append('E:\\project\\makeittalk\\MakeItTalk')");
//
//  PyObject* pName = PyUnicode_FromString("interfacer");
//  PyObject* pModule = PyImport_Import(pName);
//  if (!pModule) {
//    printf("Module load error !");
//    exit(0);
//  }
//
//  PyObject* pDict = PyModule_GetDict(pModule);
//  if (pDict == NULL) {
//    printf("Can't find the dictionary!");
//    exit(0);
//  }
//
//  PyObject* pClass = PyDict_GetItemString(pDict, "Interfacer");
//  if (pClass == NULL) {
//    printf("Can't find class!");
//    exit(0);
//  }
//  return pClass;
//}
//
//// 读入初始参考图像，实例化类的对象
//// 图像需要(unsigned char) * height * width * 3, 一维数组即可。
//// 返回python类对应的实例
//PyObject* init_ctalk(PyObject* pClass,
//                     unsigned char* data,  // height * width * 3
//                     int width,
//                     int height) {
//  npy_intp Dims[3] = {height, width, 3};
//  PyObject* pArray = PyArray_SimpleNewFromData(3, Dims, NPY_UBYTE, data);
//  PyObject* ArgArray = PyTuple_New(1);
//  PyTuple_SetItem(ArgArray, 0, pArray);
//  PyObject* pInstance = PyObject_CallObject(pClass, ArgArray);
//  assert(pInstance != NULL);
//  /*Py_DECREF(pArray);
//  Py_DECREF(ArgArray);*/
//  return pInstance;
//}


}  // namespace
int PASCAL wWinMain(HINSTANCE instance,
                    HINSTANCE prev_instance,
                    wchar_t* cmd_line,
                    int cmd_show) {
  rtc::WinsockInitializer winsock_init;
  rtc::PhysicalSocketServer ss;
  rtc::AutoSocketServerThread main_thread(&ss);

  WindowsCommandLineArguments win_args;
  int argc = win_args.argc();
  char** argv = win_args.argv();

  absl::ParseCommandLine(argc, argv);

  // PyObject* pClass1 = ctalk();

  //unsigned char* referenceImage =
  //    (uint8_t*)malloc(sizeof(unsigned char) * 256 * 256 * 3);
  //int num = 0;
  //for (int i = 0; i < 256; i++) {
  //  for (int j = 0; j < 256; j++) {
  //    referenceImage[num] = i + j;
  //    num++;
  //    referenceImage[num] = i + j + 2;
  //    num++;
  //    referenceImage[num] = i + j + 1;
  //    num++;
  //  }
  //}
  //
  //PyObject* pClass = ctalk();
  //PyObject* pInstance = init_ctalk(pClass, referenceImage, 256, 256);

  ///*npy_intp Dims[3] = {256, 256, 3};
  //PyObject* pArray =
  //    PyArray_SimpleNewFromData(3, Dims, NPY_UBYTE, referenceImage);
  //assert(pArray != NULL);
  //PyObject* ArgArray = PyTuple_New(1);
  //PyTuple_SetItem(ArgArray, 0, pArray);
  //PyObject* pInstance = PyObject_CallObject(pClass, ArgArray);*/

  //assert(pInstance != NULL);
  //PyObject* pResult =
  //    PyObject_CallMethod(pInstance, "load_audio", NULL);
  //assert(pResult != NULL);

  // InitFieldTrialsFromString stores the char*, so the char array must outlive
  // the application.
  const std::string forced_field_trials =
      absl::GetFlag(FLAGS_force_fieldtrials);
  webrtc::field_trial::InitFieldTrialsFromString(forced_field_trials.c_str());

  // Abort if the user specifies a port that is outside the allowed
  // range [1, 65535].
  if ((absl::GetFlag(FLAGS_port) < 1) || (absl::GetFlag(FLAGS_port) > 65535)) {
    printf("Error: %i is not a valid port.\n", absl::GetFlag(FLAGS_port));
    return -1;
  }

  const std::string server = absl::GetFlag(FLAGS_server);
  MainWnd wnd(server.c_str(), absl::GetFlag(FLAGS_port),
              absl::GetFlag(FLAGS_autoconnect), absl::GetFlag(FLAGS_autocall));
  if (!wnd.Create()) {
    RTC_NOTREACHED();
    return -1;
  }

  rtc::InitializeSSL();
  PeerConnectionClient client;
  rtc::scoped_refptr<Conductor> conductor(
      new rtc::RefCountedObject<Conductor>(&client, &wnd));

  // Main loop.
  MSG msg;
  BOOL gm;
  while ((gm = ::GetMessage(&msg, NULL, 0, 0)) != 0 && gm != -1) {
    if (!wnd.PreTranslateMessage(&msg)) {
      ::TranslateMessage(&msg);
      ::DispatchMessage(&msg);
    }
  }

  if (conductor->connection_active() || client.is_connected()) {
    while ((conductor->connection_active() || client.is_connected()) &&
           (gm = ::GetMessage(&msg, NULL, 0, 0)) != 0 && gm != -1) {
      if (!wnd.PreTranslateMessage(&msg)) {
        ::TranslateMessage(&msg);
        ::DispatchMessage(&msg);
      }
    }
  }

  rtc::CleanupSSL();
  return 0;
}
