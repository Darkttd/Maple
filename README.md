# Maple
Maple is project name of game "Moemoe Nemologic" which is square picross game.

## Download Game
- https://play.google.com/store/apps/details?id=com.Kradmoa.Maple

# How to build it
## Windows
1. You must clone and build Bibim ( https://github.com/Halak/Bibim ) first.
2. run Bibim project's "bin/Bibim.Baaaaaab.exe" and type "console /class:AssetServer" and press run button or, run "bin/Bibim.Baaaaaab.exe console /class:AssetServer"
3. Open Solution file which in Source/Game/Maple.VCxx.sln
4. Check property 'Bibim' is correctly load. if not, manually load Bibim project's lib/vs20xx/Win32/Debug or Release/Bibim.props
5. Build

## Linux, and Windows with MinGW-w64 (CMake)
The root `CMakeLists.txt` builds Maple together with the Bibim engine in `Source/bibim`, a git submodule
pinned to an engine commit. Clone with `git clone --recursive`, or run `git submodule update --init --recursive`
in an existing clone. The engine's requirements are listed in `Source/bibim/README.md` (CMake 3.16+, GCC, and on
Linux SDL3 3.4+ and the OpenGL ES 2.0/EGL headers).

Linux:

```
cmake -S . -B build-linux -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build-linux
```

64-bit Windows, cross-compiled on Linux with MinGW-w64:

```
cmake -S . -B build-mingw -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=Source/bibim/cmake/mingw-w64-x86_64.cmake
cmake --build build-mingw
```

Run the game from `Deploy/`, which holds the built assets, for example `cd Deploy && ../build-linux/Maple`.
On Windows, `Maple.exe` needs `d3dx9_43.dll` from the DirectX End-User Runtime.
