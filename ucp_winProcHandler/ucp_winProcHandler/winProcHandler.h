
#ifndef WIN_PROC_HEADER
#define WIN_PROC_HEADER

// currently only helpers -> sharing a header with imports requires .def or .lib (or .dll?) to link
// at the moment, the intention is to use GetModuleHandle and GetProcAddress
namespace WinProcHeader
{
  using FuncGetMainProc = WNDPROC(__stdcall*)();
  using FuncCallNextProc = LRESULT(__stdcall*)(UINT reservedCurrentPrio, HWND, UINT, WPARAM, LPARAM);
  using FuncRegisterProc = int(__stdcall*)(FuncCallNextProc funcToCall, int priority);

  inline constexpr char const* NAME_VERSION{ "0.1.0" };

  inline constexpr char const* NAME_MODULE{ "winProcHandler.dll" };
  inline constexpr char const* NAME_GET_MAIN_PROC{ "_GetMainProc@0" };
  inline constexpr char const* NAME_CALL_NEXT_PROC{ "_CallNextProc@20" };
  inline constexpr char const* NAME_REGISTER_PROC{ "_RegisterProc@8" };

  // used at start for first call, can not carry function, returned by RegisterProc indicates placement failed
  inline constexpr int NO_VALID_PRIO{ INT_MIN };

  inline FuncGetMainProc GetMainProc{ nullptr };
  inline FuncCallNextProc CallNextProc{ nullptr };
  inline FuncRegisterProc RegisterProc{ nullptr };

  // returns true if the function variables of this header were successfully filled
  inline bool initModuleFunctions()
  {
    HMODULE winProcModule{ GetModuleHandleA(NAME_MODULE) };
    if (!winProcModule)
    {
      return false;
    }

    GetMainProc = (FuncGetMainProc)GetProcAddress(winProcModule, NAME_GET_MAIN_PROC);
    CallNextProc = (FuncCallNextProc)GetProcAddress(winProcModule, NAME_CALL_NEXT_PROC);
    RegisterProc = (FuncRegisterProc)GetProcAddress(winProcModule, NAME_REGISTER_PROC);

    return GetMainProc && CallNextProc && RegisterProc;
  }
}

#endif //WIN_PROC_HEADER