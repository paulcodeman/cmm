# Building C-- Compiler on Windows

## Requirements

- [MSYS2](https://www.msys2.org/) with **mingw32** (32-bit) toolchain

Install in MSYS2:

```sh
pacman -S mingw-w64-i686-gcc mingw-w64-i686-binutils
```

## Build

Open **PowerShell** (not MSYS2 shell) and run:

```powershell
$env:PATH = "C:\msys64\mingw32\bin;$env:PATH"
i686-w64-mingw32-g++ -w -fno-exceptions -static-libgcc -D_WIN32_ -D__CONSOLE__ -o c--.exe *.cpp
```

> [!TIP]
> If you get *"multiple definition of `main`"*, check for extra `.cpp` files (like `simple_test.cpp`) and move them aside, or list only the needed files.

## Why this works

The compiler is 32-bit code and uses `int` for pointers (e.g. `(int)itok.rec`). A 64-bit compiler rejects those casts with *"loses precision"*, and its `-m32` flag links against incompatible libraries.

The **i686-w64-mingw32-g++** cross-compiler is the correct 32-bit toolchain — it produces a 32-bit `.exe` and links against `mingw32` libraries natively.

## Original Makefile

`Makefile.win32` uses `i686-w64-mingw32-gcc` (C compiler) with `-m32`. To use it directly:

```powershell
$env:PATH = "C:\msys64\mingw32\bin;$env:PATH"
mingw32-make -f Makefile.win32
```

## Quick test

```powershell
$env:PATH = "C:\msys64\mingw32\bin;$env:PATH"
.\c--.exe test.c -lst
```
