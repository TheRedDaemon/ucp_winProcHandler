#pragma once

// currently only helpers -> sharing the header requires .def or .lib (or .dll?) to link
// at the moment, the intention is to use GetModuleHandle and GetProcAddress
namespace UCPwinProc
{
  inline constexpr char const* NAME_GET_MAIN_PROC{ "_GetMainProc@0" };
  inline constexpr char const* NAME_CALL_NEXT_PROC{ "_CallNextProc@20" };
  inline constexpr char const* NAME_REGISTER_PROC{ "_RegisterProc@4" };

  using FuncGetMainProc = WNDPROC(__stdcall*)();
  using FuncCallNextProc = LRESULT(__stdcall*)(UINT, HWND, UINT, WPARAM, LPARAM);
  using FuncRegisterProc = void(__stdcall*)(FuncCallNextProc);
}