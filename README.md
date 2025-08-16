# FusionLibrary

**FusionLibrary — A simple and fast C++ library for creating 2D games and interfaces.**  
 FusionLibrary combines a clear API with a flexible architecture for modern graphics development.


---

## ✨ Features

- **Simple and intuitive API** — inspired by Raylib, letting you get something on screen with just a few lines of code.
- **Modular organization** — following SFML’s structure for easier maintenance and scalability.
- **Cross-platform support** — Windows, Linux, and Web (via Emscripten).
- **Modern 2D rendering** using OpenGL 3.3 (desktop) and WebGL 2.0 (web).
- **Render batching** to minimize draw calls and boost performance.
- **UTF-8 text rendering** with *stb_truetype*.
- **Ready-to-use features**:
  - Sprites with transformations
  - Basic shapes (rectangles, circles, lines, triangles)
  - RenderTexture (draw directly to textures)
  - 2D camera with zoom and rotation
  - Keyboard, mouse, and gamepad input

---

<!-- 

## ⚠️ Project Status

> **Warning:** This project is currently **experimental** and mainly aimed at **learning and prototyping**.  
> For production-ready applications, consider well-established libraries like [Raylib](https://www.raylib.com/) and [SFML](https://www.sfml-dev.org/).



---

## 📦 Supported Platforms

- **Windows** — OpenGL 3.3 Core
- **Linux** — OpenGL 3.3 Core
- **Web** — WebAssembly / WebGL 2.0 via Emscripten

---

## 🛠 Building the Project

**Requirements:**
- CMake >= 3.25
- C++ compiler with C++20 support
- [GLFW](https://www.glfw.org/) (included in the project)

**Build example:**
```bash
git clone https://github.com/marconaveni/FusionLibrary.git
cd FusionLibrary
cmake -B build
cmake --build build
```

---

## 🚀 Quick Example

```cpp
#include "window.h"

int main() {
    Fusion::Window window;
    window.InitWindow("Hello it is work", 800, 600);
    window.SetTargetFPS(60);

    while (!window.WindowShouldClose()) {
        window.BeginDrawing();
        window.Clear({0.2f, 0.3f, 0.3f, 1.0f});
        window.DrawCircle({400, 300}, 50, {1.0f, 0.5f, 0.2f, 1.0f});
        window.EndDrawing();
    }

    window.Close();
    return 0;
}
```

---

## 📚 Inspirations

FusionLibrary draws strong inspiration from two standout libraries in the game development world:

- **[Raylib](https://www.raylib.com/)** — for its extreme simplicity and beginner-friendly approach, enabling quick results with minimal learning curve.  
- **[SFML](https://www.sfml-dev.org/)** — for its modular architecture and clear separation of responsibilities, resulting in clean and scalable code.

---

## 🛣️ Roadmap

- [ ] **Audio (miniaudio)**  
  - Integrate miniaudio (WAV/OGG/MP3) with a simple API: load/play/stop/loop, volume, and pan.

- [ ] **Text – immediate improvements**  
  - Remove the hardcoded atlas size (1024×1024): dynamically calculate atlas size based on `fontSize`/`charCount`.

- [ ] **Multiple windows**  
  - Refactor `Core::PollEvents()` and shared context to support multiple windows without repeating `glfwPollEvents()`.

- [ ] **Debug tools**  
  - FPS overlay (quickly render on screen).  
  - Logging system with levels.

- [ ] **Build as a library**  
  - Generate a library target: e.g., FusionLibrary as static (`.a`/`.lib`) and shared (`.so`/`.dll`).  
  - CMake options: e.g., `FUSION_BUILD_SHARED` (ON/OFF) and `FUSION_BUILD_EXAMPLES` (ON/OFF).  
  - Installation and consumption.

- [ ] **Platform Web (parity with Desktop)**  
  - Full input support (keyboard, mouse, wheel, gamepad) with current/previous state tracking (same as desktop), mirroring the `Platform` interface.  
  - Time/FPS: measure frame time (using `emscripten_get_now()` or `glfwGetTime()`) and expose `GetFrameTime()`, `GetFPS()`, `GetTime()`.  
  - Window/resize: return `GetWindowSize()`, signal `IsWindowResized()`, and adjust viewport/projection.  
  - Main loop already exists (`emscripten_set_main_loop_arg`), but integrate with:
    - `PollEventsAndUpdate()` (swap + input update + time).  
    - `SetTargetFPS()` (web mode: rely on VSYNC/Emscripten; document limitations).  
  - Callbacks: register key/mouse/scroll/resize in GLFW (works in Emscripten) and update state arrays, same as desktop.  
  - Gamepad: map `glfwGetGamepadState` on web (when available) and apply deadzone.
- [ ] **Code Improvements / Refactor**
  - Translate all in-code comments and log messages from Portuguese to English.
  - Standardize naming conventions, includes order, and formatting across the codebase.


---

## 📄 License

*(To be defined)*
