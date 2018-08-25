////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2018, Ilya Kotelnikov (ilya.kotelnikov@gmail.com)
// All rights reserved.
//
// You may use, distribute and modify this code under the terms of GNU GPLv3.
//------------------------------------------------------------------------------

#include <stdint.h>

#include <windows.h>

#include "protozoa/stl/list"

namespace {

// A list of functions registered by atexit() routine to be called in reverse
// order at the app termination. Thread-safe access to the list is provided by
// a critical section.
using atexit_func_ptr = void(*)();
CRITICAL_SECTION g_atexit_critical_section;
std::list<atexit_func_ptr>* g_atexit_func_stack_list = nullptr;

// A single critical section controlling all function-level static
// initializations.
CRITICAL_SECTION g_init_static_critical_section;

}  // namespace

namespace protozoa {
namespace crt {

void init() {
  ::InitializeCriticalSection(&g_atexit_critical_section);
  g_atexit_func_stack_list = new std::list<atexit_func_ptr>();

  ::InitializeCriticalSection(&g_init_static_critical_section);
}

void term() {
  ::DeleteCriticalSection(&g_init_static_critical_section);

  // Call atexit functions and free related internal structures.
  ::EnterCriticalSection(&g_atexit_critical_section);
  for (atexit_func_ptr f : (*g_atexit_func_stack_list)) {
    f();
  }
  delete g_atexit_func_stack_list;
  g_atexit_func_stack_list = nullptr;
  ::LeaveCriticalSection(&g_atexit_critical_section);
  ::DeleteCriticalSection(&g_atexit_critical_section);
}

}  // namespace crt
}  // namespace protozoa

extern "C" int atexit(atexit_func_ptr f) noexcept {
  ::EnterCriticalSection(&g_atexit_critical_section);
  g_atexit_func_stack_list->push_front(f);
  ::LeaveCriticalSection(&g_atexit_critical_section);
  return 0;
}

extern "C" void __cxa_pure_virtual() {
  // Show a message box with the fatal error in another thread blocking the
  // current thread execution/message pump. When user dismisses the message,
  // terminate the process.
  struct MsgBoxInThread {
    static DWORD WINAPI Proc(void*) {
      constexpr const wchar_t* text =
          L"Pure virtual function call.\r\nThe application will be terminated.";
      constexpr const wchar_t* title = L"Fatal Error";
      ::MessageBox(nullptr, text, title, MB_OK | MB_ICONSTOP);
      return 0;
    }
  };

  HANDLE thread_handle =
      ::CreateThread(nullptr, 0, &MsgBoxInThread::Proc, 0, 0, nullptr);
  ::WaitForSingleObject(thread_handle, INFINITE);
  ::CloseHandle(thread_handle);

  ::TerminateProcess(::GetCurrentProcess(), 0);
}

// Returns 1 if the caller needs to run the initializer and then call
// __cxa_guard_release(). If 0 is returned, then the initializer has already
// been run. Blocks if another thread is currently running the initializer.
// The |guard| arg's first byte meaning: 0 - need init; 1 - init done.
//
extern "C" int __cxa_guard_acquire(uint8_t guard[8]) {
  if (guard[0] != 0)
    return 0;

  ::EnterCriticalSection(&g_init_static_critical_section);

  if (guard[0] != 0) {
    ::LeaveCriticalSection(&g_init_static_critical_section);
    return 0;
  }

  return 1;
}

extern "C" void __cxa_guard_release(uint8_t guard[8]) {
  guard[0] = 1;
  ::LeaveCriticalSection(&g_init_static_critical_section);
}
