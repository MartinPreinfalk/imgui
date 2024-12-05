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

#pragma once
#include "imgui.h"  // IMGUI_IMPL_API
#ifndef IMGUI_DISABLE

#include <pugl/pugl.h>

// Follow "Getting Started" link and check examples/ folder to learn about using backends!
// IMGUI_IMPL_API bool ImGui_ImplPugl_InitForOpenGL(PuglView* view, bool install_event_handler);
IMGUI_IMPL_API bool ImGui_ImplPugl_InitForOpenGL(PuglView* view);
IMGUI_IMPL_API void ImGui_ImplPugl_Shutdown();
IMGUI_IMPL_API void ImGui_ImplPugl_NewFrame();

// Pugl event handler
// - When calling Init with 'install_event_handler=true': ImGui_ImplPugl_SetEventFunc() is called. Pugl event function
// pointer will be installed for you. It will chain-call user's previously installed one, if any.
// - When calling Init with 'install_event_handler=false': Pugl event function pointer callbacks won't be installed. You
// will need to call individual function yourself from your own Pugl event function pointer.
// IMGUI_IMPL_API void ImGui_ImplPugl_SetEventFunc(PuglView* view, PuglEventFunc eventFunc);
// IMGUI_IMPL_API void ImGui_ImplPugl_RestoreEventFunc(PuglView* window);

// Pugl event handler that is installed when calling Init with 'install_event_handler=true'
// It is part of the API only to expose it for third-party users if they want to to use that directly (but undocumented)
IMGUI_IMPL_API PuglStatus ImGui_ImplPugl_EventHandler(PuglView* view, const PuglEvent* event);

// Pugl event handler options:
// - Set 'chain_for_all_windows=true' to enable chaining callbacks for all views (including secondary viewports created
// by backends or by user)
// IMGUI_IMPL_API void ImGui_ImplPugl_SetEventFuncChainForAllViews(
//     bool chain_for_all_views);  // TODO: do we need this and if yes what is it?

// ImGui's Pugl event handler function
// (to call yourself if you didn't install handler)
// IMGUI_IMPL_API PuglStatus ImGui_ImplPugl_EventHandler(PuglView* view, const PuglEvent* event);

// Pugl helpers
IMGUI_IMPL_API void ImGui_ImplPugl_Sleep(int milliseconds);

IMGUI_IMPL_API ImGuiKey ImGui_ImplPugl_KeyToImGuiKey(int keycode, int scancode);

#endif  // #ifndef IMGUI_DISABLE
