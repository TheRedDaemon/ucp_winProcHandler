
#include "pch.h"

// lua
#include "lua.hpp"


namespace UCPwinProc
{

  // lua module load
  extern "C" __declspec(dllexport) int __cdecl luaopen_winProcHandler(lua_State * L)
  {
    return 0;
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