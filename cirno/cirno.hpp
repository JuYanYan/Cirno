/*
 | Cirno
 | 文件名称: cirno.hpp
 | 文件作用: 共同的头文件
 | 创建日期: 2021-04-17
 | 更新日期: 2021-04-17
 | 开发人员: JuYan
+----------------------------
 Copyright (C) JuYan, all rights reserved.
 只应该通过此头文件包括该库
 
 MIT License

 Copyright (C) JuYan

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
*/
#pragma once
// 数学库的函数的调用方式
#if defined(_MATHLIB_USE_SSE) && defined(_MSC_VER)
    #define MATHLIB_CALL(ret)   ret __vectorcall
#elif defined(_MSC_VER)
    #define MATHLIB_CALL(ret)   ret __vectorcall
#else
    #define MATHLIB_CALL(ret)   ret
#endif // _MATHLIB_USE_SSE, _MSC_VER
#if defined(_MATHLIB_USE_SSE)
    #include <xmmintrin.h>
#endif // _MATHLIB_USE_SSE
//
#include <math.h>
#include <utility>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
// 运算数据类型
namespace cirno
{
    using float32 = float;
    using float64 = double;
}
// Vector2
#include "vector2.hpp"
// Vector3
#include "vector3.hpp"
// Vector4
#include "vector4.hpp"
// Quaternion
#include "quater.hpp"
// Matrix 4x4
#include "matrix4.hpp"
// Utils
#include "rect.hpp"

