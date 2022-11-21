# DOOM NodeBuilder

![license](https://img.shields.io/github/license/7thSamurai/nodebuilder)
![build](https://img.shields.io/github/workflow/status/7thSamurai/nodebuilder/CMake)

An experimental **DOOM Node-Builder** that preprocesses DOOM 1 & 2 maps and renders cool animations of the **BSPs** and **Block Maps** being generated. 
Written in **C++ 17** with *SDL2* and *Cairo*. For both Windows and Unix based systems.

## Building

### Building (Linux)

To build on Linux, the following dependencies are required:

- CMake (3.8 or newer)
- G++
- SDL2
- Cairo

You may install these on **Ubuntu** with:

```
$ sudo apt-get update
$ sudo apt-get install cmake g++ libsdl2-dev libcairo2-dev
```

Or install them on **Arch** with:

```
$ sudo pacman -Syy
$ sudo pacman -S cmake g++ sdl2 cairo
```

And then to actaully build the program, for all distros, run the following commands in the Terminal:

```
$ mkdir build
$ cd build
$ cmake .. -DCMAKE_BUILD_TYPE=Release
$ make -j 4
```

### Building (Windows)

To build on Windows, the following dependencies are required:

- [CMake (3.8 or newer)](https://cmake.org/download/)
- [Visual Studio 2022](https://visualstudio.microsoft.com/downloads/)
- [SDL2](https://github.com/libsdl-org/SDL/releases/latest)
- [Cairo](https://github.com/preshing/cairo-windows/releases/latest)

And then run the following commands in the Command Prompt:

```
$ mkdir build
$ cd build
$ cmake .. -G "Visual Studio 17 2022" -DCMAKE_PREFIX="C:\path\to\SDL2;C:\path\to\Cairo"
$ cmake --build . --config Release
```

*Note: Instead of running the very last command, you have the option to alternatively open the generated .sln file and use Visual Studio directly.*

## Running

Todo

## Running Tests

TODO

## Roadmap

- [x] Add WAD loading/saving
- [x] Add BSP generation
- [x] Add Blockmap generation
- [x] Add build animation rendering
- [ ] Add Reject generation
- [x] Improve rendering quality
- [x] Add text rendering messages

## Copyright

This software is licensed under the GPL3 license. Copyright &copy; 2022 Zach Collins

## Acknowledgments

Special thanks to ID software for releasing the **Doom** engine as open-source. It's been a great tool for learning!
