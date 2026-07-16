# Building C-- Compiler

## Windows (32-bit)

### Requirements

- [MSYS2](https://www.msys2.org/) with the **mingw32** toolchain

```sh
pacman -S mingw-w64-i686-gcc mingw-w64-i686-binutils
```

### Build

```powershell
$env:PATH = "C:\msys64\mingw32\bin;$env:PATH"
i686-w64-mingw32-g++ -w -fno-exceptions -static-libgcc -D_WIN32_ -D__CONSOLE__ -o c--.exe *.cpp
```

### Makefile

```powershell
$env:PATH = "C:\msys64\mingw32\bin;$env:PATH"
mingw32-make -f Makefile.win32
```

## Windows (64-bit)

### Requirements

- [MSYS2](https://www.msys2.org/) with the **mingw64** toolchain

```sh
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-binutils
```

### Build

```powershell
$env:PATH = "C:\msys64\mingw64\bin;$env:PATH"
x86_64-w64-mingw32-g++ -w -fno-exceptions -static-libgcc -D_WIN32_ -D__CONSOLE__ -o c--64.exe *.cpp
```

## Linux

### 32-bit

```sh
apt install g++-multilib
make -f Makefile.lin32
```

### 64-bit

```sh
apt install g++-x86-64-linux-gnu
# or just use the system g++ (no cross needed on x86_64 hosts)
g++ -w -fno-exceptions -static-libgcc -static-libstdc++ -D_UNIX_ -Dstricmp=strcasecmp -Dstrnicmp=strncasecmp -DO_BINARY=0 -o c-- *.cpp
```

> [!TIP]
> If you get *"multiple definition of `main`"*, check for extra `.cpp` files and move them aside.

## Quick test

```sh
./c-- test.c -lst
```
