
#include "pch.h"

// lua
#include "lua.hpp"

#include "winProcHandler.h"

#include <vector>

namespace UCPwinProc
{
  static WNDPROC realWinProc{ nullptr };
  static std::vector<FuncCallNextProc> procVector{};

  /* helper */

  static __declspec(naked) LRESULT __stdcall CallRealProcFunc(UINT, HWND, UINT, WPARAM, LPARAM)
  {
    __asm
    {
      pop     ecx   // remove return address
      pop     eax   // removes index, since we are going to main func
      push    ecx   // add return address again

      jmp     realWinProc   // jump to actual main window proc
    }
  }

  static FuncCallNextProc __stdcall GetProcFuncAddress(unsigned int index)
  {
    return (index >= procVector.size()) ? CallRealProcFunc : procVector.at(index);
  }


  /* Export functions, CallNextProc */

  extern "C" __declspec(dllexport, naked) LRESULT __stdcall CallNextProc(UINT, HWND, UINT, WPARAM, LPARAM)
  {
    __asm
    {
      push    dword ptr[esp + 4]  // push index
      call    GetProcFuncAddress  // get new func address in eax

      add     dword ptr[esp + 4], 1   // increment index

      jmp     eax     // jump to next window proc
    }
  }


  /* Called window function */

  static __declspec(naked) LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM)
  {
    __asm
    {
      pop     ecx           // remove return address
      push    dword ptr 0   // adds 0 index on stack
      push    ecx           // add return address again

      jmp     CallNextProc   // jump to actual main window proc
    }
  }

  /*static LRESULT CALLBACK WindowProcTest(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
  {
    return 10;
  }*/


  /* Other export functions */

  extern "C" __declspec(dllexport) WNDPROC __stdcall GetMainProc()
  {
    return WindowProc;
  }

  extern "C" __declspec(dllexport) void __stdcall RegisterProc(FuncCallNextProc callNextProcFunc)
  {
    procVector.push_back(callNextProcFunc);
  }



  // lua module load
  extern "C" __declspec(dllexport) int __cdecl luaopen_winProcHandler(lua_State * L)
  {
    lua_newtable(L); // push a new table on the stack

    // need to write window callback func to the returned address
    lua_pushinteger(L, (DWORD)&realWinProc);
    lua_setfield(L, -2, "address_FillWithWindowProcCallback");

    // simple replace
    lua_pushinteger(L, (DWORD)WindowProc);
    lua_setfield(L, -2, "funcAddress_WindowProc");

    // test
    /*realWinProc = WindowProcTest;
    RegisterProc(CallNextProc);
    LRESULT a{ WindowProc(0, 0, 0, 0) };*/

    return 1;
  }

  // entry point
  BOOL APIENTRY DllMain(HMODULE,
    DWORD  ul_reason_for_call,
    LPVOID
  )
  {
    switch (ul_reason_for_call)
    {
      case DLL_PROCESS_ATTACH:
      case DLL_THREAD_ATTACH:
      case DLL_THREAD_DETACH:
      case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
  }
}