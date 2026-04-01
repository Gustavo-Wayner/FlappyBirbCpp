***ATENTION*: I only have access to Windows and therefore am unable to confirm if my Makefile works on linux/mac. Feedback and fixes/tips from linux/mac users would be very much apreciated**
# 🎮 FlappyBirbCpp

A small flappy bird clone written in C++ using [raylib](https://www.raylib.com/)  

> Purpose: practicing C++, game architecture, cross-platform builds but mainly, game optimization.  

in my python flappy bird clone, i have multiple inoptimal operations like reloading textures every frame and recreating my text font every frame. Here, i'll apply concepts of optimization such as pre-loadig textures and applying scale before entering the loop

## 📦 Dependencies

Required tools:

- **C++11** or newer
- **GNU make**
- **Lua libs** (included with the project)
- **raylib** (included with the project)
- **gcc / clang / mingw-w64**

You **do NOT** need to install raylib manually.  
It is built from the repository source.

---

## 🛠️ Installing Build Tools

### 🐧 Linux (Debian/Ubuntu/Zorin)

```bash
sudo apt update
```

```bash
sudo apt install build-essential git libx11-dev libxrandr-dev libxinerama-dev libxi-dev libgl1-mesa-dev libxcursor-dev mesa-utils
```

### 🐧 Arch Linux
```bash
sudo pacman -S base-devel libx11 libxrandr libxi libxcursor mesa
```

### 🍎 macOS

Install Xcode Command Line Tools (provides clang, make, and other build tools):

```bash
xcode-select --install
```

Click "Install" when prompted. This may take a few minutes.

**Note:** macOS has the required OpenGL and system frameworks (Cocoa, IOKit, CoreVideo) built-in, so no additional library installation is needed for raylib.

### 🪟 Windows (MSYS2 + MinGW)
1 - Install MSYS2 → https://www.msys2.org/

2 - Open MSYS2 UCRT64, MSYS2 MINGW64 or your MSYS2 terminal of choice and run:
```bash
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-make git
```

---

## 📚 Building libraylib.a from Source

**Important:** This project includes raylib source code in `third_party/raylib/`, but the main Makefile does **not** build raylib automatically. You must build `libraylib.a` manually before building the project.

### Method 1: Using Make (Recommended)

The included raylib source has a Makefile - this is the recommended method since this project uses make.

**Linux / macOS:**
```bash
cd third_party/raylib/src
make PLATFORM=PLATFORM_DESKTOP
```

**Windows (MinGW):**
```bash
cd third_party/raylib/src
mingw32-make PLATFORM=PLATFORM_DESKTOP
```

The `libraylib.a` static library will be created in `third_party/raylib/src/`.

### Method 2: Using CMake (Alternative)

If you prefer using CMake, raylib also supports building with it.

Make sure you have CMake installed:
- **Linux (Debian/Ubuntu):** `sudo apt install cmake`
- **Arch Linux:** `sudo pacman -S cmake`
- **macOS:** `brew install cmake`
- **Windows (MSYS2):** `pacman -S mingw-w64-x86_64-cmake`

**Linux / macOS:**
```bash
cd third_party/raylib
mkdir build && cd build
cmake .. -DBUILD_SHARED_LIBS=OFF -DSTATIC=ON
make
```

**Windows (MinGW):**
```bash
cd third_party/raylib
mkdir build && cd build
cmake .. -G "MinGW Makefiles" -DBUILD_SHARED_LIBS=OFF -DSTATIC=ON
mingw32-make
```

The `libraylib.a` static library will be created in `third_party/raylib/build/`.

---

## 🚀 Building and Running

### In the terminal

▶️ Linux / macOS:  
To build:
```bash
make
```

To run:
```bash
make run
```

to clean:
```bash
make clean
```


🪟 Windows (MinGW):

To build:
```bash
mingw32-make
```

To run:
```bash
mingw32-make run
```

to clean:
```bash
mingw32-make clean
```

### Through tasks

alternatively you can run the repective tasks in tasks.json, if in vs code

▶️ Linux / macOS:

build (make);  
run (make);  
clean (make)

🪟 Windows (MinGW):

windows build (make);  
windows run (make);  
windows clean (make)

#### No make
My college computers have no make nor can i install stuff in them. for that, and for those who are in a similar situation, i made tasks to compile without using make, just the terminal
**This removes the need for make but other requirements still stand**

🪟 windows compile (no-make);  
🐧 linux compile (no-make);  
🍎 osx compile (no-make)

## 📁 Project Structure
FlappyBirbCpp/  
 ├── .vscode/  
 │   ├── tasks.json  
 │   └── settings.json  
 ├── assets/  
 │   ├── birb.png  
 │   ├── cloud.png  
 │   ├── pipe.png  
 │   ├── wing_down.png  
 │   └── wing_up.png  
 ├── src/  
 │   ├── main.cpp  
 │   ├── Rooms.cpp / .h  
 │   ├── Objects.cpp / .h  
 |   ├── Static.cpp / .h  
 │   └── Structs.cpp / .h  
 ├── raylib/ ← bundled raylib source  
 ├── Makefile  
 └── README.md