#include "../src/function.hpp"

#include <eel2_import.hpp>
#include <gtest/gtest.h>
#include <reaper_plugin_functions.h>

#undef NSEEL_addfunc_ret_type
#undef NSEEL_addfunc_varparm_ex
#undef NSEEL_code_compile_ex
#undef NSEEL_code_execute
#undef NSEEL_code_free
#undef NSEEL_code_getcodeerror
#undef nseel_int_register_var
#undef NSEEL_PProc_THIS
#undef nseel_stringsegments_tobuf
#undef NSEEL_VM_alloc
#undef NSEEL_VM_enumallvars
#undef NSEEL_VM_free
#undef NSEEL_VM_getramptr
#undef NSEEL_VM_SetCustomFuncThis
#undef NSEEL_VM_SetFunctionTable
#undef NSEEL_VM_SetStringFunc

void NSEEL_HOSTSTUB_EnterMutex() {}
void NSEEL_HOSTSTUB_LeaveMutex() {}

struct Environment : testing::Environment { void SetUp() override; };

static const auto g_setup
  { testing::AddGlobalTestEnvironment(new Environment) };

void Environment::SetUp()
{
  GetMainHwnd     = []() -> HWND { return nullptr; };
  plugin_register = [](const char *, void *) { return 0; };

#ifndef _WIN32
  GetWindowLong = [](HWND, int)           -> LONG_PTR { return 0; };
  SetWindowLong = [](HWND, int, LONG_PTR) -> LONG_PTR { return 0; };
#endif

  eel2_import::NSEEL_addfunc_ret_type     = &NSEEL_addfunc_ret_type;
  eel2_import::NSEEL_addfunc_varparm_ex   = &NSEEL_addfunc_varparm_ex;
  eel2_import::NSEEL_code_compile_ex      = &NSEEL_code_compile_ex;
  eel2_import::NSEEL_code_execute         = &NSEEL_code_execute;
  eel2_import::NSEEL_code_free            = &NSEEL_code_free;
  eel2_import::NSEEL_code_getcodeerror    = &NSEEL_code_getcodeerror;
  eel2_import::nseel_int_register_var     = nseel_int_register_var;
  eel2_import::NSEEL_PProc_THIS           = &NSEEL_PProc_THIS;
  eel2_import::nseel_stringsegments_tobuf = &nseel_stringsegments_tobuf;
  eel2_import::NSEEL_VM_alloc             = &NSEEL_VM_alloc;
  eel2_import::NSEEL_VM_enumallvars       = &NSEEL_VM_enumallvars;
  eel2_import::NSEEL_VM_free              = &NSEEL_VM_free;
  eel2_import::NSEEL_VM_getramptr         = &NSEEL_VM_getramptr;
  eel2_import::NSEEL_VM_SetCustomFuncThis = &NSEEL_VM_SetCustomFuncThis;
  eel2_import::NSEEL_VM_SetFunctionTable  = &NSEEL_VM_SetFunctionTable;
  eel2_import::NSEEL_VM_SetStringFunc     = &NSEEL_VM_SetStringFunc;

  Function::setup();
}
