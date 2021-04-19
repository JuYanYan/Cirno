# Cirno

![Platform](https://img.shields.io/badge/Platform-Any-blue)&nbsp;![Language](https://img.shields.io/badge/Language-C%2B%2B-green)&nbsp;![License](https://img.shields.io/badge/License-MIT-green)&nbsp;![CodeQuality](https://img.shields.io/lgtm/grade/cpp/github/JuYanYan/Cirno.svg?logo=lgtm&logoWidth=24)&nbsp;![Document](https://img.shields.io/badge/Document-Writing-orange)

&emsp;&emsp;⑨味十足的C++数学库，包括向量、四元数和4x4方阵。

&emsp;&emsp;A C++ library of Matrix and Vector with バガ style!

## How to use

It's easy to use this library:

 1. Add all the files in "./cirno" to your project
 2. Check your incude directories
 3. Add this code into your source/header file:
 ```c++
// x86/AMD64 ONLY, open SSE
#define _MATHLIB_USE_SSE   1
// then, include this file:
#include "cirno/cirno.hpp"
 ```
 4. Thas's all:
 ```c++
int main()
{
    cirno::Vector3 vec(1.0f, 2.0f, 3.0f);
    vec.SetNormalize();
    printf("(%g, %g, %g)\n", vec.X(), vec.Y(), vec.Z());
    return 0;
}
 ```

## Build sample

&emsp;&emsp;To build example.cpp, run this command:

```bash
make
```
