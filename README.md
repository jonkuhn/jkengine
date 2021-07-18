# Learn OpenGL

## Summary
Project for me to write a simple 2D graphics engine for learning modern OpenGL graphics programming.  I am initially following the tutorial here: https://learnopengl.com.  I may eventually expand it from just graphics into a game engine and potentially expand the graphics to support 3D at some point as well.  However, I have very limited time to spend on this so progress is going to be very slow.

## Project Goals
1. The number one goal is to have fun learning about graphics programming and to keep my C++ skills fresh.
2. Have a high level of automated test coverage with unit and integration tests.
3. Prefer to focus on simplicity, testability, readability, and maintainability over performance optimizations.
   - When writing in C++ I can get distracted by reading and learning about things that _can_ be done to optimize code, but likely don't really matter for the goals of this project.  I am _interested_ in learning about these ideas and playing with them, but I'd like to avoid letting unnecessary optimizations from creeping into this project.
   - To the extent that I do read up on these types of optimizations my goal should just be to understand them enough to not design myself into a corner that would make it difficult to optimize later.
   - Basically: avoid premature optimization!  Unless there is a real reason to do otherwise:
     - Don't worry about having lots of interfaces with virtual functions to enable testing.
     - Don't worry too much about memory locality and just allocate individual object on the heap to get things going.  Consider trying to abstract object creation and scene loading concerns behind an interface so that I could optimize in in the future if needed.
     - Defer (but try to leave room in the design for) work around culling objects before making draw calls.  It seems very likely that any GPU can handle me throwing a whole 2D game level's worth of 2D rectangles at it even if most of them are off screen.  If I am proven wrong I will adjust and learn from it.

## Rough Project Roadmap and Ideas
1. ✓ ~~Build wrappers around basic OpenGL constructs~~ (probably have enough for now)
2. ✓ ~~Prototype 2D tile maps~~ (prototype exists)
2. Build support for 2D tile maps that is decoupled from OpenGL.
3. Build support for 2D sprites that is decoupled from OpenGL.
4. Build something like an object factory or scene builder / loader to encapsulate loading, allocation, and instantiation logic and to serve as the Dependency Injection root. (maybe could defer this until after I have hard coded a basic game level prototype)
5. What is next may vary on what I feel like doing, but likely should be:
   - Use 2D tile map and sprites to prototype a simple game level that uses them.  This would not be designed to be a "good" game but just to see how it is to use the tools I built and make sure I can integrate them well with basic input and basic physics and game logic.
6. Formalize the design of some of the non-graphics aspects from the simple 2D game level prototype
6. Things I have thought of doing but should probably defer:
   - Make a tile map editor.
   - Implement instanced rendering for drawing multiple instances of the same sprite.
   - Writing a WebGL renderer and allowing games written with the engine to be deployed to the web.
   - Writing a Metal or DirectX renderer to experience a different graphics library
   - use a linter and formatter to the project via github actions and vscode extensions

## Prerequisites
1. CMake (using 3.19.0) (`brew install cmake`)
2. Conan (using 1.31.3) (`brew install conan`)

## Devlopment Environment
The development environment I am using is as follows:
- Visual Studio Code with the following extensions:
  - C/C++ (ms-vscode.cpptools)
  - CMake Tools (twxs.cmake)
  - CMake (twxs.cmake)
  - Vim (vscodevim.vim)
  - Markdown All in One (yzhang.markdown-all-in-one)
  - Shader Languages Support for VSCode (slevesque.shader)

## Conan setup notes
1. `brew install conan`
2. create `conanfile.txt` (see file for details)
3. Note that `build.sh` does the following steps already but the following
   here to document how the project was set up.
4. Do the following (resulting files should not be committed -- they are system specific):
   ```
   mkdir build && cd build
   conan install .. --build=missing
   ```
5. Add the following to CMakeLists.txt:
   ```
   include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
   conan_basic_setup()

   add_executable(myexecutable main.cpp)
   target_link_libraries(myexecutable ${CONAN_LIBS})
   ```
6. Build with cmake as normal (from `build` directory):
   ```
   cmake ..
   cmake --build .
   ```
7. For VSCode: Add `"~/.conan/data/**"` to `.vscode/c_cpp_properties.json` at `configurations[0].["includePath"]` so that VSCode will look for headers in installed conan packages.

## Issue with glfw conan package
As of Nov 21, 2020 the glfw 3.3.2 package would not build for Mac with CMake 3.19.  This issue was fixed in master of glfw in commit 3327050c.  In order to still use conan (and to learn more about it), I copied the conan recipe files for glfw from https://github.com/conan-io/conan-center-index and modified them to build from the above commit of glfw.  These files are under the `patched-dependencies` folder.  In order to get `glfw/3.3.2-3327050c@jkuhn/patched` (as it is referenced in conanfile.txt) into the conan local cache, run the following command in the `patched-dependencies/glfw` directory:
```
conan create . jkuhn/patched
```

## OpenGL Resources:
- [Learn OpenGL](https://learnopengl.com)

## C++ Ecosystem Resources
- [CMake Tutorial](https://cmake.org/cmake/help/latest/guide/tutorial/index.html)
- [Conan Getting Started](https://docs.conan.io/en/latest/getting_started.html)

## Development Environment Resources:
- [Get Started With CMake Tools On Linux](https://code.visualstudio.com/docs/cpp/cmake-linux)
- [Using Clang in VSCode](https://code.visualstudio.com/docs/cpp/config-clang-mac)
