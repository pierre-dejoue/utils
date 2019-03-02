Utils
=====

hex/bin utils written in C/C++

## Build

Build using CMake: https://cmake.org/download/

For example on Windows:

```
$ mkdir build
$ cd build
$ cmake -G "Visual Studio 14 2015 Win64" ..
$ cmake --build . --config Release
```

## Usage

### ascii2bin

```
$ echo 5574696c730a > foo.hex
$ ascii2bin.exe foo.hex > foo.bin
$ cat foo.bin
Utils
```

### hex

```
$ hex README.md
00000000  5574696c 730a3d3d 3d3d3d0a 0a686578    Utils.=====..hex
00000010  2f62696e 20757469 6c732077 72697474    /bin utils writt
00000020  656e2069 6e20432f 432b2b0a 0a232320    en in C/C++..##
00000030  4275696c 640a0a42 75696c64 20757369    Build..Build usi
00000040  6e672043 4d616b65 3a206874 7470733a    ng CMake: https:
...       ...                                    ...
...       ...                                    ...
```

### parsefloat32

Parse numerical values and display detailed information about their encoding in 32-bit floats (IEEE 754).

Example:
```
$ parsefloat32 -0.002
-----------------------------------------------------------------------------------
Input: -0.002
    Normal floating-point:
    Exponent              = 2^-9
    Significand           = 1.00000110001001001101111 (1.[mantissa], in binary)
                          = 1.000046875094995E+00     (in decimal)
    Value64               = -2.000000094994903E-03
    Hex32                 = 0xBB03126F
     [sign]     (1  bit)  = 0x1
     [exponent] (8  bits) = 0x76     = 118
     [mantissa] (23 bits) = 0x03126F = 201327

    ULP (Unit in the Last Place) = 2^-32 = 2.328306436538696E-10

    -2.000000327825546E-03   0xBB031270   (prev)
    -2.000000094994903E-03   0xBB03126F   (this)
    -1.999999862164259E-03   0xBB03126E   (next)

```

The input can be the hexadecimal representation of a float. For example the following command will give the same output as above:

```
$ parsefloat32 0xBB03126F
... same as above ...
```

### rm_cr

Remove carriage return character from DOS text files.

`Usage: rm_cr FILE [FILE ...]`


## License

[![License](http://img.shields.io/:license-mit-blue.svg?style=flat-square)](./LICENSE)
