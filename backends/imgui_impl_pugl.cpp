// The MIT License (MIT)

// Copyright (c) 2024 Martin Preinfalk

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

// dear imgui: Platform Backend for Pugl
//  - Pugl (PlUgin Graphics Library) is a minimal portability layer for GUIs which is suitable for use in plugins
//    and applications.  It works on X11, MacOS, and Windows, and includes optional support for drawing with Vulkan,
//    OpenGL, and Cairo.)
// This needs to be used along with a Renderer (e.g. OpenGL3, Vulkan, WebGPU..)
//
// You can use unmodified imgui_impl_* files in your project. See examples/ folder for examples of using this.
// Prefer including the entire imgui/ repository into your project (either as a copy or as a submodule), and only build
// the backends you need. Learn about Dear ImGui:
// - FAQ                  https://dearimgui.com/faq
// - Getting Started      https://dearimgui.com/getting-started
// - Documentation        https://dearimgui.com/docs (same as your local docs/ folder).
// - Introduction, links and more at the top of imgui.cpp
//
// CAUTION !!!
//  - currently only OpenGL3 is supported!
//  - currently only tested on Linux in the scope of Audio Plugin Development,
//    No tests done for other platforms yet, I don't know if it even builds on Windows or MacOS!
//  - you have been warned! ;-)

// CHANGELOG
// (minor and older changes stripped away, please see git history for details)
//  20??-??-??: initial version

#include "imgui.h"
#ifndef IMGUI_DISABLE
#include "imgui_impl_pugl.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

// pugl
#include <pugl/pugl.h>

// Pugl data
struct ImGui_ImplPugl_Data {
  PuglView* View = nullptr;
  double Time = 0.0;
  PuglView* MouseView = nullptr;
  ImVec2 LastValidMousePos = {0, 0};
  // bool InstalledEventFunc = false;
  // bool CallbacksChainForAllWindows = false;
  // PuglEventFunc PrevEventFunc = nullptr;

  ImGui_ImplPugl_Data() = default;
};

// Backend data stored in io.BackendPlatformUserData to allow support for multiple Dear ImGui contexts
// It is STRONGLY preferred that you use docking branch with multi-viewports (== single Dear ImGui context + multiple
// windows) instead of multiple Dear ImGui contexts.
// FIXME: multi-context support is not yet tested and probably dysfunctional in this backend.
static ImGui_ImplPugl_Data* ImGui_ImplPugl_GetBackendData() {
  return ImGui::GetCurrentContext() ? (ImGui_ImplPugl_Data*)ImGui::GetIO().BackendPlatformUserData : nullptr;
}

// static bool ImGui_ImplPugl_ShouldChainCallback(PuglView* view) {
//   ImGui_ImplPugl_Data* bd = ImGui_ImplPugl_GetBackendData();
//   return bd->CallbacksChainForAllWindows ? true : (view == bd->View);
// }

ImGuiKey ImGui_ImplPugl_KeyToImGuiKey(int keycode, int scancode) {
  IM_UNUSED(scancode);
  switch (keycode) {
    case PUGL_KEY_TAB:
      return ImGuiKey_Tab;
    case PUGL_KEY_LEFT:
      return ImGuiKey_LeftArrow;
    case PUGL_KEY_RIGHT:
      return ImGuiKey_RightArrow;
    case PUGL_KEY_UP:
      return ImGuiKey_UpArrow;
    case PUGL_KEY_DOWN:
      return ImGuiKey_DownArrow;
    case PUGL_KEY_PAGE_UP:
      return ImGuiKey_PageUp;
    case PUGL_KEY_PAGE_DOWN:
      return ImGuiKey_PageDown;
    case PUGL_KEY_HOME:
      return ImGuiKey_Home;
    case PUGL_KEY_END:
      return ImGuiKey_End;
    case PUGL_KEY_INSERT:
      return ImGuiKey_Insert;
    case PUGL_KEY_DELETE:
      return ImGuiKey_Delete;
    case PUGL_KEY_BACKSPACE:
      return ImGuiKey_Backspace;
    case PUGL_KEY_SPACE:
      return ImGuiKey_Space;
    case PUGL_KEY_ENTER:
      return ImGuiKey_Enter;
    case PUGL_KEY_ESCAPE:
      return ImGuiKey_Escape;
    case '\'':
      return ImGuiKey_Apostrophe;
    case ',':
      return ImGuiKey_Comma;
    case '-':
      return ImGuiKey_Minus;
    case '.':
      return ImGuiKey_Period;
    case '/':
      return ImGuiKey_Slash;
    case ';':
      return ImGuiKey_Semicolon;
    case '=':
      return ImGuiKey_Equal;
    case '[':
      return ImGuiKey_LeftBracket;
    case '\\':
      return ImGuiKey_Backslash;
    case ']':
      return ImGuiKey_RightBracket;
    case 0x60:
      return ImGuiKey_GraveAccent;
    case 0xFFFF:
      return ImGuiKey_GraveAccent;
    case PUGL_KEY_CAPS_LOCK:
      return ImGuiKey_CapsLock;
    case PUGL_KEY_SCROLL_LOCK:
      return ImGuiKey_ScrollLock;
    case PUGL_KEY_NUM_LOCK:
      return ImGuiKey_NumLock;
    case PUGL_KEY_PRINT_SCREEN:
      return ImGuiKey_PrintScreen;
    case PUGL_KEY_PAUSE:
      return ImGuiKey_Pause;
    case PUGL_KEY_PAD_0:
      return ImGuiKey_Keypad0;
    case PUGL_KEY_PAD_1:
      return ImGuiKey_Keypad1;
    case PUGL_KEY_PAD_2:
      return ImGuiKey_Keypad2;
    case PUGL_KEY_PAD_3:
      return ImGuiKey_Keypad3;
    case PUGL_KEY_PAD_4:
      return ImGuiKey_Keypad4;
    case PUGL_KEY_PAD_5:
      return ImGuiKey_Keypad5;
    case PUGL_KEY_PAD_6:
      return ImGuiKey_Keypad6;
    case PUGL_KEY_PAD_7:
      return ImGuiKey_Keypad7;
    case PUGL_KEY_PAD_8:
      return ImGuiKey_Keypad8;
    case PUGL_KEY_PAD_9:
      return ImGuiKey_Keypad9;
    case PUGL_KEY_PAD_DECIMAL:
      return ImGuiKey_KeypadDecimal;
    case PUGL_KEY_PAD_DIVIDE:
      return ImGuiKey_KeypadDivide;
    case PUGL_KEY_PAD_MULTIPLY:
      return ImGuiKey_KeypadMultiply;
    case PUGL_KEY_PAD_SUBTRACT:
      return ImGuiKey_KeypadSubtract;
    case PUGL_KEY_PAD_ADD:
      return ImGuiKey_KeypadAdd;
    case PUGL_KEY_PAD_ENTER:
      return ImGuiKey_KeypadEnter;
    case PUGL_KEY_PAD_EQUAL:
      return ImGuiKey_KeypadEqual;
    case PUGL_KEY_SHIFT_L:
      return ImGuiKey_LeftShift;
    case PUGL_KEY_CTRL_L:
      return ImGuiKey_LeftCtrl;
    case PUGL_KEY_ALT_L:
      return ImGuiKey_LeftAlt;
    case PUGL_KEY_SUPER_L:
      return ImGuiKey_LeftSuper;
    case PUGL_KEY_SHIFT_R:
      return ImGuiKey_RightShift;
    case PUGL_KEY_CTRL_R:
      return ImGuiKey_RightCtrl;
    case PUGL_KEY_ALT_R:
      return ImGuiKey_RightAlt;
    case PUGL_KEY_SUPER_R:
      return ImGuiKey_RightSuper;
    case PUGL_KEY_MENU:
      return ImGuiKey_Menu;
    case '0':
      return ImGuiKey_0;
    case '1':
      return ImGuiKey_1;
    case '2':
      return ImGuiKey_2;
    case '3':
      return ImGuiKey_3;
    case '4':
      return ImGuiKey_4;
    case '5':
      return ImGuiKey_5;
    case '6':
      return ImGuiKey_6;
    case '7':
      return ImGuiKey_7;
    case '8':
      return ImGuiKey_8;
    case '9':
      return ImGuiKey_9;
    case 'A':
      return ImGuiKey_A;
    case 'B':
      return ImGuiKey_B;
    case 'C':
      return ImGuiKey_C;
    case 'D':
      return ImGuiKey_D;
    case 'E':
      return ImGuiKey_E;
    case 'F':
      return ImGuiKey_F;
    case 'G':
      return ImGuiKey_G;
    case 'H':
      return ImGuiKey_H;
    case 'I':
      return ImGuiKey_I;
    case 'J':
      return ImGuiKey_J;
    case 'K':
      return ImGuiKey_K;
    case 'L':
      return ImGuiKey_L;
    case 'M':
      return ImGuiKey_M;
    case 'N':
      return ImGuiKey_N;
    case 'O':
      return ImGuiKey_O;
    case 'P':
      return ImGuiKey_P;
    case 'Q':
      return ImGuiKey_Q;
    case 'R':
      return ImGuiKey_R;
    case 'S':
      return ImGuiKey_S;
    case 'T':
      return ImGuiKey_T;
    case 'U':
      return ImGuiKey_U;
    case 'V':
      return ImGuiKey_V;
    case 'W':
      return ImGuiKey_W;
    case 'X':
      return ImGuiKey_X;
    case 'Y':
      return ImGuiKey_Y;
    case 'Z':
      return ImGuiKey_Z;
    case PUGL_KEY_F1:
      return ImGuiKey_F1;
    case PUGL_KEY_F2:
      return ImGuiKey_F2;
    case PUGL_KEY_F3:
      return ImGuiKey_F3;
    case PUGL_KEY_F4:
      return ImGuiKey_F4;
    case PUGL_KEY_F5:
      return ImGuiKey_F5;
    case PUGL_KEY_F6:
      return ImGuiKey_F6;
    case PUGL_KEY_F7:
      return ImGuiKey_F7;
    case PUGL_KEY_F8:
      return ImGuiKey_F8;
    case PUGL_KEY_F9:
      return ImGuiKey_F9;
    case PUGL_KEY_F10:
      return ImGuiKey_F10;
    case PUGL_KEY_F11:
      return ImGuiKey_F11;
    case PUGL_KEY_F12:
      return ImGuiKey_F12;
    default:
      return ImGuiKey_None;
  }
}

static void ImGui_ImplPugl_UpdateKeyModifiers(PuglMods const& state) {
  ImGuiIO& io = ImGui::GetIO();
  io.AddKeyEvent(ImGuiMod_Ctrl, state & PUGL_MOD_CTRL);
  io.AddKeyEvent(ImGuiMod_Shift, state & PUGL_MOD_SHIFT);
  io.AddKeyEvent(ImGuiMod_Alt, state & PUGL_MOD_ALT);
  io.AddKeyEvent(ImGuiMod_Super, state & PUGL_MOD_SUPER);
}

void ImGui_ImplPugl_MouseButtonEventHandler(PuglView* /*view*/, PuglButtonEvent const& event) {
  ImGui_ImplPugl_UpdateKeyModifiers(event.state);
  ImGuiIO& io = ImGui::GetIO();
  if (event.button < ImGuiMouseButton_COUNT)
    io.AddMouseButtonEvent(event.button, event.type == PUGL_BUTTON_PRESS);
}

void ImGui_ImplPugl_ScrollEventHandler(PuglView* /*view*/, PuglScrollEvent event) {
  ImGui_ImplPugl_UpdateKeyModifiers(event.state);
  ImGuiIO& io = ImGui::GetIO();
  io.AddMouseWheelEvent(event.dx, event.dy);
}

void ImGui_ImplPugl_KeyEventHandler(PuglView* /*view*/, PuglKeyEvent const& event) {
  if (event.type != PUGL_KEY_PRESS && event.type != PUGL_KEY_RELEASE) {
    return;
  }
  ImGui_ImplPugl_UpdateKeyModifiers(event.state);
  ImGuiIO& io = ImGui::GetIO();
  ImGuiKey imgui_key = ImGui_ImplPugl_KeyToImGuiKey(event.keycode, 0);
  io.AddKeyEvent(imgui_key, (event.type == PUGL_KEY_PRESS));
  io.SetKeyEventNativeData(imgui_key, event.keycode, 0);  // To support legacy indexing (<1.87 user code)
}

void ImGui_ImplPugl_FocusEventHandler(PuglView* /*view*/, PuglFocusEvent const& event) {
  ImGuiIO& io = ImGui::GetIO();
  io.AddFocusEvent(event.type != PUGL_FOCUS_IN);
}

void ImGui_ImplPugl_MotionEventHandler(PuglView* /*view*/, PuglMotionEvent const& event) {
  ImGui_ImplPugl_Data* bd = ImGui_ImplPugl_GetBackendData();
  IM_ASSERT(bd != nullptr && "platform backend nullptr");
  ImGui_ImplPugl_UpdateKeyModifiers(event.state);
  ImGuiIO& io = ImGui::GetIO();
  io.AddMousePosEvent(event.x, event.y);
  bd->LastValidMousePos = ImVec2(event.x, event.y);
}

// Workaround: X11 seems to send spurious Leave/Enter events which would make us lose our position,
// so we back it up and restore on Leave/Enter (see https://github.com/ocornut/imgui/issues/4984)
void ImGui_ImplPugl_CrossingEventHandler2(PuglView* view, PuglCrossingEvent const& event) {
  ImGui_ImplPugl_Data* bd = ImGui_ImplPugl_GetBackendData();
  ImGuiIO& io = ImGui::GetIO();
  if (event.type == PUGL_POINTER_IN) {
    bd->MouseView = view;
    io.AddMousePosEvent(bd->LastValidMousePos.x, bd->LastValidMousePos.y);
  } else if (!event.type == PUGL_POINTER_IN && bd->MouseView == view) {
    bd->LastValidMousePos = io.MousePos;
    bd->MouseView = nullptr;
    io.AddMousePosEvent(-FLT_MAX, -FLT_MAX);
  }
}

void ImGui_ImplPugl_CrossingEventHandler(PuglView* /*view*/, PuglCrossingEvent const& event) {
  ImGuiIO& io = ImGui::GetIO();
  if (event.type == PUGL_POINTER_IN) {
    io.AddMousePosEvent(event.x, event.y);
  } else {
    io.AddMousePosEvent(-FLT_MAX, -FLT_MAX);
  }
}

void ImGui_ImplPugl_TextEventHandler(PuglView* /*view*/, PuglTextEvent const& event) {
    ImGuiIO& io = ImGui::GetIO();
    io.AddInputCharacter(event.character);
}

// void ImGui_ImplGlfw_MonitorCallback(GLFWmonitor*, int)
// {
// 	// Unused in 'master' branch but 'docking' branch will use this, so we declare it ahead of it so if you have to
// install callbacks you can install this one too.
// }

// #ifdef EMSCRIPTEN_USE_EMBEDDED_GLFW3
// static EM_BOOL ImGui_ImplEmscripten_WheelCallback(int, const EmscriptenWheelEvent* ev, void*)
// {
//     // Mimic Emscripten_HandleWheel() in SDL.
//     // Corresponding equivalent in GLFW JS emulation layer has incorrect quantizing preventing small values. See
//     #6096 float multiplier = 0.0f; if (ev->deltaMode == DOM_DELTA_PIXEL)       { multiplier = 1.0f / 100.0f; } // 100
//     pixels make up a step. else if (ev->deltaMode == DOM_DELTA_LINE)   { multiplier = 1.0f / 3.0f; }   // 3 lines
//     make up a step. else if (ev->deltaMode == DOM_DELTA_PAGE)   { multiplier = 80.0f; }         // A page makes up 80
//     steps. float wheel_x = ev->deltaX * -multiplier; float wheel_y = ev->deltaY * -multiplier; ImGuiIO& io =
//     ImGui::GetIO(); io.AddMouseWheelEvent(wheel_x, wheel_y);
//     //IMGUI_DEBUG_LOG("[Emsc] mode %d dx: %.2f, dy: %.2f, dz: %.2f --> feed %.2f %.2f\n", (int)ev->deltaMode,
//     ev->deltaX, ev->deltaY, ev->deltaZ, wheel_x, wheel_y); return EM_TRUE;
// }
// #endif

// void ImGui_ImplPugl_InstallEventFunc(PuglView* view) {
//   ImGui_ImplPugl_Data* bd = ImGui_ImplPugl_GetBackendData();
//   IM_ASSERT(bd->InstalledEventFunc == false && "EventFunc already installed!");
//   IM_ASSERT(bd->View == view);

//   bd->PrevEventFunc = puglGetEventFunc(view);
//   puglSetEventFunc(view, ImGui_ImplPugl_EventHandler);
//   bd->InstalledEventFunc = true;
// }

// void ImGui_ImplPugl_RestoreEventFunc(PuglView* view) {
//   ImGui_ImplPugl_Data* bd = ImGui_ImplPugl_GetBackendData();
//   IM_ASSERT(bd->InstalledEventFunc == true && "Callbacks not installed!");
//   IM_ASSERT(bd->View == view);

//   puglSetEventFunc(view, bd->PrevEventFunc);
//   bd->InstalledEventFunc = false;
//   bd->PrevEventFunc = nullptr;
// }

// // Set to 'true' to enable chaining installed callbacks for all windows (including secondary viewports created by
// backends or by user.
// // This is 'false' by default meaning we only chain callbacks for the main viewport.
// // We cannot set this to 'true' by default because user callbacks code may be not testing the 'window' parameter of
// their callback.
// // If you set this to 'true' your user callback code will need to make sure you are testing the 'window' parameter.
// void ImGui_ImplGlfw_SetCallbacksChainForAllWindows(bool chain_for_all_windows)
// {
//     ImGui_ImplGlfw_Data* bd = ImGui_ImplGlfw_GetBackendData();
//     bd->CallbacksChainForAllWindows = chain_for_all_windows;
// }

// #ifdef __EMSCRIPTEN__
// #if EMSCRIPTEN_USE_PORT_CONTRIB_GLFW3 >= 34020240817
// void ImGui_ImplGlfw_EmscriptenOpenURL(const char* url) { if (url) emscripten::glfw3::OpenURL(url); }
// #else
// EM_JS(void, ImGui_ImplGlfw_EmscriptenOpenURL, (const char* url), { url = url ? UTF8ToString(url) : null; if (url)
// window.open(url, '_blank'); }); #endif #endif

// bool ImGui_ImplPugl_InitForOpenGL(PuglView* view, bool install_event_func) {
bool ImGui_ImplPugl_InitForOpenGL(PuglView* view) {
  ImGuiIO& io = ImGui::GetIO();
  IMGUI_CHECKVERSION();
  IM_ASSERT(io.BackendPlatformUserData == nullptr && "Already initialized a platform backend!");
  // printf("GLFW_VERSION: %d.%d.%d (%d)", GLFW_VERSION_MAJOR, GLFW_VERSION_MINOR, GLFW_VERSION_REVISION,
  // GLFW_VERSION_COMBINED);

  // Setup backend capabilities flags
  ImGui_ImplPugl_Data* bd = IM_NEW(ImGui_ImplPugl_Data)();
  io.BackendPlatformUserData = (void*)bd;
  io.BackendPlatformName = "imgui_impl_pugl";
  io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;  // We can honor GetMouseCursor() values (optional)
  io.BackendFlags |=
      ImGuiBackendFlags_HasSetMousePos;  // We can honor io.WantSetMousePos requests (optional, rarely used)

  bd->View = view;
  bd->Time = 0.0;

  // TODO clipboard support
  //     ImGuiPlatformIO& platform_io = ImGui::GetPlatformIO();
  //     platform_io.Platform_SetClipboardTextFn = [](ImGuiContext*, const char* text) { glfwSetClipboardString(NULL,
  //     text); }; platform_io.Platform_GetClipboardTextFn = [](ImGuiContext*) { return glfwGetClipboardString(NULL); };
  // #ifdef __EMSCRIPTEN__
  //     platform_io.Platform_OpenInShellFn = [](ImGuiContext*, const char* url) {
  //     ImGui_ImplGlfw_EmscriptenOpenURL(url); return true; };
  // #endif

  // TODO do we need this?
  //     // Create mouse cursors
  //     // (By design, on X11 cursors are user configurable and some cursors may be missing. When a cursor doesn't
  //     exist,
  //     // GLFW will emit an error which will often be printed by the app, so we temporarily disable error reporting.
  //     // Missing cursors will return nullptr and our _UpdateMouseCursor() function will use the Arrow cursor
  //     instead.) GLFWerrorfun prev_error_callback = glfwSetErrorCallback(nullptr);
  //     bd->MouseCursors[ImGuiMouseCursor_Arrow] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
  //     bd->MouseCursors[ImGuiMouseCursor_TextInput] = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
  //     bd->MouseCursors[ImGuiMouseCursor_ResizeNS] = glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR);
  //     bd->MouseCursors[ImGuiMouseCursor_ResizeEW] = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
  //     bd->MouseCursors[ImGuiMouseCursor_Hand] = glfwCreateStandardCursor(GLFW_HAND_CURSOR);
  // #if GLFW_HAS_NEW_CURSORS
  //     bd->MouseCursors[ImGuiMouseCursor_ResizeAll] = glfwCreateStandardCursor(GLFW_RESIZE_ALL_CURSOR);
  //     bd->MouseCursors[ImGuiMouseCursor_ResizeNESW] = glfwCreateStandardCursor(GLFW_RESIZE_NESW_CURSOR);
  //     bd->MouseCursors[ImGuiMouseCursor_ResizeNWSE] = glfwCreateStandardCursor(GLFW_RESIZE_NWSE_CURSOR);
  //     bd->MouseCursors[ImGuiMouseCursor_NotAllowed] = glfwCreateStandardCursor(GLFW_NOT_ALLOWED_CURSOR);
  // #else
  //     bd->MouseCursors[ImGuiMouseCursor_ResizeAll] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
  //     bd->MouseCursors[ImGuiMouseCursor_ResizeNESW] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
  //     bd->MouseCursors[ImGuiMouseCursor_ResizeNWSE] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
  //     bd->MouseCursors[ImGuiMouseCursor_NotAllowed] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
  // #endif
  //     glfwSetErrorCallback(prev_error_callback);
  // #if GLFW_HAS_GETERROR && !defined(__EMSCRIPTEN__) // Eat errors (see #5908)
  //     (void)glfwGetError(nullptr);
  // #endif

  // Chain GLFW callbacks: our callbacks will call the user's previously installed callbacks, if any.
  // if (install_event_func) ImGui_ImplPugl_InstallEventFunc(view);

  // Set platform dependent data in viewport
  ImGuiViewport* main_viewport = ImGui::GetMainViewport();
  main_viewport->PlatformHandle = (void*)bd->View;

#ifdef _WIN32
  // FIXME / TODO windows and apple stuff is completely untested, I only have a Linux machine
  main_viewport->PlatformHandleRaw = puglGetNativeView(bd->View);
#elif defined(__APPLE__)
  // FIXME / TODO windows and apple stuff is completely untested, I only have a Linux machine
  main_viewport->PlatformHandleRaw = (void*)puglGetNativeView(bd->View);
#else
  IM_UNUSED(main_viewport);
#endif

  return true;
}

// bool ImGui_ImplGlfw_InitForVulkan(GLFWwindow* window, bool install_callbacks)
// {
//     return ImGui_ImplGlfw_Init(window, install_callbacks, GlfwClientApi_Vulkan);
// }

// bool ImGui_ImplGlfw_InitForOther(GLFWwindow* window, bool install_callbacks)
// {
//     return ImGui_ImplGlfw_Init(window, install_callbacks, GlfwClientApi_Unknown);
// }

void ImGui_ImplPugl_Shutdown() {
  ImGui_ImplPugl_Data* bd = ImGui_ImplPugl_GetBackendData();
  IM_ASSERT(bd != nullptr && "No platform backend to shutdown, or already shutdown?");
  ImGuiIO& io = ImGui::GetIO();

  // if (bd->InstalledEventFunc) ImGui_ImplPugl_RestoreEventFunc(bd->View);

  // FIXME TODO wtf needed?
  // for (ImGuiMouseCursor cursor_n = 0; cursor_n < ImGuiMouseCursor_COUNT; cursor_n++)
  //     glfwDestroyCursor(bd->MouseCursors[cursor_n]);

  io.BackendPlatformName = nullptr;
  io.BackendPlatformUserData = nullptr;
  io.BackendFlags &=
      ~(ImGuiBackendFlags_HasMouseCursors | ImGuiBackendFlags_HasSetMousePos | ImGuiBackendFlags_HasGamepad);
  IM_DELETE(bd);
}

// static void ImGui_ImplPugl_UpdateMouseData()
// {
//     ImGui_ImplPugl_Data* bd = ImGui_ImplPugl_GetBackendData();
//     ImGuiIO& io = ImGui::GetIO();

//     // (those braces are here to reduce diff with multi-viewports support in 'docking' branch)
//     {
//         PuglView* window = bd->View;
//         const bool is_window_focused = puglHasFocus(view);
//         if (is_window_focused)
//         {
//             // FIXME: pugl does not seem to support this out of the box
//             // (Optional) Set OS mouse position from Dear ImGui if requested (rarely used, only when
//             io.ConfigNavMoveSetMousePos is enabled by user)
//             // if (io.WantSetMousePos)
//             //     glfwSetCursorPos(window, (double)io.MousePos.x, (double)io.MousePos.y);

//             // FIXME: pugl does not seem to support this out of the box
//             // // (Optional) Fallback to provide mouse position when focused (ImGui_ImplGlfw_CursorPosCallback
//             already provides this when hovered or captured)
//             // if (bd->MouseWindow == nullptr)
//             // {
//             //     double mouse_x, mouse_y;
//             //     glfwGetCursorPos(window, &mouse_x, &mouse_y);
//             //     bd->LastValidMousePos = ImVec2((float)mouse_x, (float)mouse_y);
//             //     io.AddMousePosEvent((float)mouse_x, (float)mouse_y);
//             // }
//         }
//     }
// }

// static void ImGui_ImplPugl_UpdateMouseCursor()
// {
//     ImGuiIO& io = ImGui::GetIO();
//     ImGui_ImplGlfw_Data* bd = ImGui_ImplGlfw_GetBackendData();
//     if ((io.ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange) || glfwGetInputMode(bd->Window, GLFW_CURSOR) ==
//     GLFW_CURSOR_DISABLED)
//         return;

//     ImGuiMouseCursor imgui_cursor = ImGui::GetMouseCursor();
//     // (those braces are here to reduce diff with multi-viewports support in 'docking' branch)
//     {
//         GLFWwindow* window = bd->Window;
//         if (imgui_cursor == ImGuiMouseCursor_None || io.MouseDrawCursor)
//         {
//             // Hide OS mouse cursor if imgui is drawing it or if it wants no cursor
//             glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
//         }
//         else
//         {
//             // Show OS mouse cursor
//             // FIXME-PLATFORM: Unfocused windows seems to fail changing the mouse cursor with GLFW 3.2, but 3.3 works
//             here. glfwSetCursor(window, bd->MouseCursors[imgui_cursor] ? bd->MouseCursors[imgui_cursor] :
//             bd->MouseCursors[ImGuiMouseCursor_Arrow]); glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
//         }
//     }
// }

// // Update gamepad inputs
// static inline float Saturate(float v) { return v < 0.0f ? 0.0f : v  > 1.0f ? 1.0f : v; }
// static void ImGui_ImplGlfw_UpdateGamepads()
// {
//     ImGuiIO& io = ImGui::GetIO();
//     if ((io.ConfigFlags & ImGuiConfigFlags_NavEnableGamepad) == 0) // FIXME: Technically feeding gamepad shouldn't
//     depend on this now that they are regular inputs.
//         return;

//     io.BackendFlags &= ~ImGuiBackendFlags_HasGamepad;
// #if GLFW_HAS_GAMEPAD_API && !defined(EMSCRIPTEN_USE_EMBEDDED_GLFW3)
//     GLFWgamepadstate gamepad;
//     if (!glfwGetGamepadState(GLFW_JOYSTICK_1, &gamepad))
//         return;
//     #define MAP_BUTTON(KEY_NO, BUTTON_NO, _UNUSED)          do { io.AddKeyEvent(KEY_NO, gamepad.buttons[BUTTON_NO] !=
//     0); } while (0) #define MAP_ANALOG(KEY_NO, AXIS_NO, _UNUSED, V0, V1)    do { float v = gamepad.axes[AXIS_NO]; v =
//     (v - V0) / (V1 - V0); io.AddKeyAnalogEvent(KEY_NO, v > 0.10f, Saturate(v)); } while (0)
// #else
//     int axes_count = 0, buttons_count = 0;
//     const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axes_count);
//     const unsigned char* buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &buttons_count);
//     if (axes_count == 0 || buttons_count == 0)
//         return;
//     #define MAP_BUTTON(KEY_NO, _UNUSED, BUTTON_NO)          do { io.AddKeyEvent(KEY_NO, (buttons_count > BUTTON_NO &&
//     buttons[BUTTON_NO] == GLFW_PRESS)); } while (0) #define MAP_ANALOG(KEY_NO, _UNUSED, AXIS_NO, V0, V1)    do {
//     float v = (axes_count > AXIS_NO) ? axes[AXIS_NO] : V0; v = (v - V0) / (V1 - V0); io.AddKeyAnalogEvent(KEY_NO, v >
//     0.10f, Saturate(v)); } while (0)
// #endif
//     io.BackendFlags |= ImGuiBackendFlags_HasGamepad;
//     MAP_BUTTON(ImGuiKey_GamepadStart,       GLFW_GAMEPAD_BUTTON_START,          7);
//     MAP_BUTTON(ImGuiKey_GamepadBack,        GLFW_GAMEPAD_BUTTON_BACK,           6);
//     MAP_BUTTON(ImGuiKey_GamepadFaceLeft,    GLFW_GAMEPAD_BUTTON_X,              2);     // Xbox X, PS Square
//     MAP_BUTTON(ImGuiKey_GamepadFaceRight,   GLFW_GAMEPAD_BUTTON_B,              1);     // Xbox B, PS Circle
//     MAP_BUTTON(ImGuiKey_GamepadFaceUp,      GLFW_GAMEPAD_BUTTON_Y,              3);     // Xbox Y, PS Triangle
//     MAP_BUTTON(ImGuiKey_GamepadFaceDown,    GLFW_GAMEPAD_BUTTON_A,              0);     // Xbox A, PS Cross
//     MAP_BUTTON(ImGuiKey_GamepadDpadLeft,    GLFW_GAMEPAD_BUTTON_DPAD_LEFT,      13);
//     MAP_BUTTON(ImGuiKey_GamepadDpadRight,   GLFW_GAMEPAD_BUTTON_DPAD_RIGHT,     11);
//     MAP_BUTTON(ImGuiKey_GamepadDpadUp,      GLFW_GAMEPAD_BUTTON_DPAD_UP,        10);
//     MAP_BUTTON(ImGuiKey_GamepadDpadDown,    GLFW_GAMEPAD_BUTTON_DPAD_DOWN,      12);
//     MAP_BUTTON(ImGuiKey_GamepadL1,          GLFW_GAMEPAD_BUTTON_LEFT_BUMPER,    4);
//     MAP_BUTTON(ImGuiKey_GamepadR1,          GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER,   5);
//     MAP_ANALOG(ImGuiKey_GamepadL2,          GLFW_GAMEPAD_AXIS_LEFT_TRIGGER,     4,      -0.75f,  +1.0f);
//     MAP_ANALOG(ImGuiKey_GamepadR2,          GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER,    5,      -0.75f,  +1.0f);
//     MAP_BUTTON(ImGuiKey_GamepadL3,          GLFW_GAMEPAD_BUTTON_LEFT_THUMB,     8);
//     MAP_BUTTON(ImGuiKey_GamepadR3,          GLFW_GAMEPAD_BUTTON_RIGHT_THUMB,    9);
//     MAP_ANALOG(ImGuiKey_GamepadLStickLeft,  GLFW_GAMEPAD_AXIS_LEFT_X,           0,      -0.25f,  -1.0f);
//     MAP_ANALOG(ImGuiKey_GamepadLStickRight, GLFW_GAMEPAD_AXIS_LEFT_X,           0,      +0.25f,  +1.0f);
//     MAP_ANALOG(ImGuiKey_GamepadLStickUp,    GLFW_GAMEPAD_AXIS_LEFT_Y,           1,      -0.25f,  -1.0f);
//     MAP_ANALOG(ImGuiKey_GamepadLStickDown,  GLFW_GAMEPAD_AXIS_LEFT_Y,           1,      +0.25f,  +1.0f);
//     MAP_ANALOG(ImGuiKey_GamepadRStickLeft,  GLFW_GAMEPAD_AXIS_RIGHT_X,          2,      -0.25f,  -1.0f);
//     MAP_ANALOG(ImGuiKey_GamepadRStickRight, GLFW_GAMEPAD_AXIS_RIGHT_X,          2,      +0.25f,  +1.0f);
//     MAP_ANALOG(ImGuiKey_GamepadRStickUp,    GLFW_GAMEPAD_AXIS_RIGHT_Y,          3,      -0.25f,  -1.0f);
//     MAP_ANALOG(ImGuiKey_GamepadRStickDown,  GLFW_GAMEPAD_AXIS_RIGHT_Y,          3,      +0.25f,  +1.0f);
//     #undef MAP_BUTTON
//     #undef MAP_ANALOG
// }

PuglStatus ImGui_ImplPugl_EventHandler(PuglView* view, const PuglEvent* event) {
  if (!event) return PUGL_FAILURE;  // just in case
  // ImGui_ImplPugl_Data* bd = ImGui_ImplPugl_GetBackendData();
  // if (bd->PrevEventFunc != nullptr && ImGui_ImplPugl_ShouldChainCallback(view)) {
  //   auto status = bd->PrevEventFunc(view, event);
  //   if (status != PUGL_SUCCESS) {
  //     return status;
  //   }
  // }
  switch (event->type) {
    case PUGL_NOTHING:  ///< No event
      // TODO : handle event
      break;
    case PUGL_REALIZE:  ///< View realized, a #PuglRealizeEvent
      // TODO : handle event
      break;
    case PUGL_UNREALIZE:  ///< View unrealizeed, a #PuglUnrealizeEvent
      // TODO : handle event
      break;
    case PUGL_CONFIGURE:  ///< View configured, a #PuglConfigureEvent
      // TODO : handle event
      break;
    case PUGL_UPDATE:  ///< View ready to draw, a #PuglUpdateEvent
      // TODO : handle event
      break;
    case PUGL_EXPOSE:  ///< View must be drawn, a #PuglExposeEvent
      // TODO : handle event
      break;
    case PUGL_CLOSE:  ///< View will be closed, a #PuglCloseEvent
      // TODO : handle event
      break;
    case PUGL_FOCUS_IN:  ///< Keyboard focus entered view, a #PuglFocusEvent
      ImGui_ImplPugl_FocusEventHandler(view, event->focus);
      break;
    case PUGL_FOCUS_OUT:  ///< Keyboard focus left view, a #PuglFocusEvent
      ImGui_ImplPugl_FocusEventHandler(view, event->focus);
      break;
    case PUGL_KEY_PRESS:  ///< Key pressed, a #PuglKeyEvent
      ImGui_ImplPugl_KeyEventHandler(view, event->key);
      break;
    case PUGL_KEY_RELEASE:  ///< Key released, a #PuglKeyEvent
      ImGui_ImplPugl_KeyEventHandler(view, event->key);
      break;
    case PUGL_TEXT:  ///< Character entered, a #PuglTextEvent
      ImGui_ImplPugl_TextEventHandler(view, event->text);
      break;
    case PUGL_POINTER_IN:  ///< Pointer entered view, a #PuglCrossingEvent
      ImGui_ImplPugl_CrossingEventHandler(view, event->crossing);
      break;
    case PUGL_POINTER_OUT:  ///< Pointer left view, a #PuglCrossingEvent
      ImGui_ImplPugl_CrossingEventHandler(view, event->crossing);
      break;
    case PUGL_BUTTON_PRESS:  ///< Mouse button pressed, a #PuglButtonEvent
      ImGui_ImplPugl_MouseButtonEventHandler(view, event->button);
      break;
    case PUGL_BUTTON_RELEASE:  ///< Mouse button released, a #PuglButtonEvent
      ImGui_ImplPugl_MouseButtonEventHandler(view, event->button);
      break;
    case PUGL_MOTION:  ///< Pointer moved, a #PuglMotionEvent
      ImGui_ImplPugl_MotionEventHandler(view, event->motion);
      break;
    case PUGL_SCROLL:  ///< Scrolled, a #PuglScrollEvent
      ImGui_ImplPugl_ScrollEventHandler(view, event->scroll);
      break;
    case PUGL_CLIENT:  ///< Custom client message, a #PuglClientEvent
      // TODO : handle event
      break;
    case PUGL_TIMER:  ///< Timer triggered, a #PuglTimerEvent
      // TODO : handle event
      break;
    case PUGL_LOOP_ENTER:  ///< Recursive loop entered, a #PuglLoopEnterEvent
      // TODO : handle event
      break;
    case PUGL_LOOP_LEAVE:  ///< Recursive loop left, a #PuglLoopLeaveEvent
      // TODO : handle event
      break;
    case PUGL_DATA_OFFER:  ///< Data offered from clipboard, a #PuglDataOfferEvent
      // TODO : handle event
      break;
    case PUGL_DATA:  ///< Data available from clipboard, a #PuglDataEvent
      // TODO : handle event
      break;
    default:
      break;
  }
  return PUGL_SUCCESS;
}

void ImGui_ImplPugl_NewFrame() {
  ImGuiIO& io = ImGui::GetIO();
  ImGui_ImplPugl_Data* bd = ImGui_ImplPugl_GetBackendData();
  IM_ASSERT(bd != nullptr && "Context or backend not initialized! Did you call ImGui_ImplPugl_InitForXXX()?");

  // Setup display size (every frame to accommodate for window resizing)
  PuglRect frame = puglGetFrame(bd->View);
  io.DisplaySize = ImVec2(frame.width, frame.height);

  // Setup time step
  // (Accept glfwGetTime() not returning a monotonically increasing value. Seems to happens on disconnecting peripherals
  // and probably on VMs and Emscripten, see #6491, #6189, #6114, #3644)
  double current_time = puglGetTime(puglGetWorld(bd->View));
  // TODO since this was in the glfw backend code and I am not sure if it is also an issue for pugl and since it does no
  // harm having it, at least I hope so, let's keep it for now Once we are sure we don't need this for pugl we remove
  // it.
  if (current_time <= bd->Time) current_time = bd->Time + 0.00001f;
  io.DeltaTime = bd->Time > 0.0 ? (float)(current_time - bd->Time) : (float)(1.0f / 60.0f);
  bd->Time = current_time;

  // ImGui_ImplPugl_UpdateMouseData(); // FIXME: pugl does not seem to support this out of the box
  // ImGui_ImplPugl_UpdateMouseCursor(); // FIXME: pugl does not seem to support this out of the box

  // Update game controllers (if enabled and available)
  // TODO
  // ImGui_ImplGlfw_UpdateGamepads()
}

// GLFW doesn't provide a portable sleep function
void ImGui_ImplGlfw_Sleep(int milliseconds) {
#ifdef _WIN32
  ::Sleep(milliseconds);
#else
  usleep(milliseconds * 1000);
#endif
}

// #ifdef EMSCRIPTEN_USE_EMBEDDED_GLFW3
// static EM_BOOL ImGui_ImplGlfw_OnCanvasSizeChange(int event_type, const EmscriptenUiEvent* event, void* user_data)
// {
//     ImGui_ImplGlfw_Data* bd = (ImGui_ImplGlfw_Data*)user_data;
//     double canvas_width, canvas_height;
//     emscripten_get_element_css_size(bd->CanvasSelector, &canvas_width, &canvas_height);
//     glfwSetWindowSize(bd->Window, (int)canvas_width, (int)canvas_height);
//     return true;
// }

// static EM_BOOL ImGui_ImplEmscripten_FullscreenChangeCallback(int event_type, const EmscriptenFullscreenChangeEvent*
// event, void* user_data)
// {
//     ImGui_ImplGlfw_Data* bd = (ImGui_ImplGlfw_Data*)user_data;
//     double canvas_width, canvas_height;
//     emscripten_get_element_css_size(bd->CanvasSelector, &canvas_width, &canvas_height);
//     glfwSetWindowSize(bd->Window, (int)canvas_width, (int)canvas_height);
//     return true;
// }

// // 'canvas_selector' is a CSS selector. The event listener is applied to the first element that matches the query.
// // STRING MUST PERSIST FOR THE APPLICATION DURATION. PLEASE USE A STRING LITERAL OR ENSURE POINTER WILL STAY VALID.
// void ImGui_ImplGlfw_InstallEmscriptenCallbacks(GLFWwindow*, const char* canvas_selector)
// {
//     IM_ASSERT(canvas_selector != nullptr);
//     ImGui_ImplGlfw_Data* bd = ImGui_ImplGlfw_GetBackendData();
//     IM_ASSERT(bd != nullptr && "Context or backend not initialized! Did you call ImGui_ImplGlfw_InitForXXX()?");

//     bd->CanvasSelector = canvas_selector;
//     emscripten_set_resize_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, bd, false, ImGui_ImplGlfw_OnCanvasSizeChange);
//     emscripten_set_fullscreenchange_callback(EMSCRIPTEN_EVENT_TARGET_DOCUMENT, bd, false,
//     ImGui_ImplEmscripten_FullscreenChangeCallback);

//     // Change the size of the GLFW window according to the size of the canvas
//     ImGui_ImplGlfw_OnCanvasSizeChange(EMSCRIPTEN_EVENT_RESIZE, {}, bd);

//     // Register Emscripten Wheel callback to workaround issue in Emscripten GLFW Emulation (#6096)
//     // We intentionally do not check 'if (install_callbacks)' here, as some users may set it to false and call GLFW
//     callback themselves.
//     // FIXME: May break chaining in case user registered their own Emscripten callback?
//     emscripten_set_wheel_callback(bd->CanvasSelector, nullptr, false, ImGui_ImplEmscripten_WheelCallback);
// }
// #elif defined(EMSCRIPTEN_USE_PORT_CONTRIB_GLFW3)
// // When using --use-port=contrib.glfw3 for the GLFW implementation, you can override the behavior of this call
// // by invoking emscripten_glfw_make_canvas_resizable afterward.
// // See
// https://github.com/pongasoft/emscripten-glfw/blob/master/docs/Usage.md#how-to-make-the-canvas-resizable-by-the-user
// for an explanation void ImGui_ImplGlfw_InstallEmscriptenCallbacks(GLFWwindow* window, const char* canvas_selector)
// {
//   GLFWwindow* w = (GLFWwindow*)(EM_ASM_INT({ return Module.glfwGetWindow(UTF8ToString($0)); }, canvas_selector));
//   IM_ASSERT(window == w); // Sanity check
//   IM_UNUSED(w);
//   emscripten_glfw_make_canvas_resizable(window, "window", nullptr);
// }
// #endif // #ifdef EMSCRIPTEN_USE_PORT_CONTRIB_GLFW3

// //-----------------------------------------------------------------------------

// #if defined(__clang__)
// #pragma clang diagnostic pop
// #endif

#endif  // #ifndef IMGUI_DISABLE
