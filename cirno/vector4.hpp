/*
 | Cirno
 | 文件名称: vector4.hpp
 | 文件作用: 四维向量
 | 创建日期: 2021-03-26
 | 更新日期: 2021-03-26
 | 开发人员: JuYan
 +----------------------------
 Copyright (C) JuYan, all rights reserved.
 
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
namespace cirno
{
    // 四维向量
    class Vector4;

    using CompactVector4 = Vector4;

    class Vector4
    {
    public:
        Vector4() noexcept : x(0.0f), y(0.0f), z(0.0f), w(0.0f)
        {
            // nothing to do...
        }
        Vector4(float32 _v) noexcept : x(_v), y(_v), z(_v), w(_v)
        {
            // nothing to do
        }
        Vector4(float32 _x, float32 _y, float32 _z, float32 _w) noexcept : x(_x), y(_y), z(_z), w(_w)
        {
            // nothing to do...
        }
    #if defined(_MATHLIB_USE_SSE)
        Vector4(__m128 _val) noexcept : val{ _val }
        {
            // nothing to do...
        }
    #endif // _MATHLIB_USE_SSE
        ~Vector4() = default;
        Vector4(const Vector4 &) = default;
        Vector4& operator=(const Vector4 &) = default;
        // 取得模长的平方
        float32 GetNormL2Square() const noexcept
        {
        #if defined(_MATHLIB_USE_SSE)
            float32 tmp[4];
            __m128 r = _mm_mul_ps(val, val);
            _mm_store_ps(tmp, r);
            return tmp[0] + tmp[1] + tmp[2] + tmp[3];
        #else
            return x * x + y * y + z * z;
        #endif // _MATHLIB_USE_SSE
        }
        // 取得L2范数模长
        inline float32 GetNormL2() const noexcept
        {
            return sqrtf(GetNormL2Square());
        }
        // 取得模长
        inline float32 Length() const noexcept
        {
            return GetNormL2();
        }
        // 归一化
        Vector4& SetNormalize() noexcept
        {
            float32 s = Length();
            if (s == 0.0f)                                  // 长度精确为0, 不进行计算
            {
                // nothing to do
            }
            else {
            #if defined(_MATHLIB_USE_SSE)
                __m128 t = _mm_load_ps1(&s);                // t[0 .. 3] = length
                __m128 k = _mm_rcp_ps(t);                   // k = 1 / length
                val = _mm_mul_ps(val, k);
            #else
                s = 1.0f / s;
                x *= s;
                y *= s;
                z *= s;
            #endif // _MATHLIB_USE_SSE
            }
            return *this;
        }
        // 归一化
        Vector4 GetNormalize() const noexcept
        {
            Vector4 t(*this);
            return t.SetNormalize();
        }
        // 点乘
        MATHLIB_CALL(float32) DotMul(const Vector4 b) const noexcept
        {
        #if defined(_MATHLIB_USE_SSE)
            float32 buff[4];
            __m128 t = _mm_mul_ps(val, b.val);              // t[k] = val[k] * b.val[k]
            _mm_store_ps(buff, t);
            return buff[0] + buff[1] + buff[2] + buff[3];
        #else
            return x * b.x + y * b.y + z * b.z + w * b.w;
        #endif // _MATHLIB_USE_SSE
        }
        // 四维向量加法
        MATHLIB_CALL(Vector4&) operator+=(const Vector4 b) noexcept
        {
        #if defined(_MATHLIB_USE_SSE)
            val = _mm_add_ps(val, b.val);
        #else
            x += b.x;
            y += b.y;
            z += b.z;
            w += b.w;
        #endif // _MATHLIB_USE_SSE
            return *this;
        }
        // 四维向量加法
        MATHLIB_CALL(Vector4) operator+(const Vector4 b) const noexcept
        {
        #if defined(_MATHLIB_USE_SSE)
            return Vector4(_mm_add_ps(val, b.val));
        #else
            return Vector4(x + b.x, y + b.y, z + b.z, w + b.w);
        #endif // _MATHLIB_USE_SSE
        }
        // 四维向量减法
        MATHLIB_CALL(Vector4&) operator-=(const Vector4 b) noexcept
        {
        #if defined(_MATHLIB_USE_SSE)
            val = _mm_sub_ps(val, b.val);
        #else
            x -= b.x;
            y -= b.y;
            z -= b.z;
            w -= b.w;
        #endif // _MATHLIB_USE_SSE
            return *this;
        }
        // 四维向量减法
        MATHLIB_CALL(Vector4) operator-(const Vector4 b) const noexcept
        {
        #if defined(_MATHLIB_USE_SSE)
            return Vector4(_mm_sub_ps(val, b.val));
        #else
            return Vector4(x - b.x, y - b.y, z - b.z, w - b.w);
        #endif // _MATHLIB_USE_SSE
        }
        // 四维向量数乘
        MATHLIB_CALL(Vector4&) operator*=(const float32 v) noexcept
        {
        #if defined(_MATHLIB_USE_SSE)
            __m128 t = _mm_load_ps1(&v);
            val = _mm_mul_ps(val, t);
        #else
            x *= v;
            y *= v;
            z *= v;
            w *= v;
        #endif // _MATHLIB_USE_SSE
            return *this;
        }
        // 空间向量数乘
        friend MATHLIB_CALL(Vector4) operator*(const Vector4 vec, const float32 v) noexcept
        {
        #if defined(_MATHLIB_USE_SSE)
            __m128 t = _mm_load_ps1(&v);
            __m128 r = _mm_mul_ps(vec.val, t);
            return Vector4(r);
        #else
            return Vector4(vec.x * v, vec.y * v, vec.z * v, vec.w * v);
        #endif // _MATHLIB_USE_SSE
        }
        // 取反
        MATHLIB_CALL(Vector4) operator-() const noexcept
        {
            return Vector4(-x, -y, -z, -w);
        }
        // 按照下标取得值, [0] = x, [1] = y, [2] = z, [3] = w
        float32& operator[](unsigned int i) noexcept
        {
            assert(i >= 0 && i < 4);
            return buff[i];
        }
        // 按照下标取得值, [0] = x, [1] = y, [2] = z, [3] = w
        float32 operator[](unsigned int i) const noexcept
        {
            assert(i >= 0 && i < 4);
            return buff[i];
        }
        // 取得X值
        inline float32 X() const noexcept
        {
            return x;
        }
        // 取得Y值
        inline float32 Y() const noexcept
        {
            return y;
        }
        // 取得Z值
        inline float32 Z() const noexcept
        {
            return z;
        }
        // 取得W值
        inline float32 W() const noexcept
        {
            return w;
        }
        // 取得ptr, 原始类型是float32[4]
        inline float32* GetPtr() noexcept
        {
            return buff;
        }
        // 取得ptr, 原始类型是float32[4]
        inline const float32* GetPtr() const noexcept
        {
            return buff;
        }
        // 取得更紧凑的四维向量
        inline CompactVector4 CompactVector() const noexcept
        {
            return CompactVector4{ x, y, z, w };
        }
    protected:
        // 内存布局:
        // union
        // { | ---- 32 ---- | ---- 32 ---- | ---- 32 ---- | ---- 32 ---- |
        //   |                         __m128 val                        |
        //   |                        float32 buff[4]                    |
        //   |   float32 x  |   float32 y  |   float32 z  |   float32 w  |
        //   |   float32 a  |   float32 b  |   float32 c  |   float32 d  |
        // }
        union
        {
            struct
            {
                float32 x, y, z, w;
            };
            struct
            {
                float32 a, b, c, d;
            };
            float32 buff[4];
        #if defined(_MATHLIB_USE_SSE)
            __m128 val;
        #endif // _MATHLIB_USE_SSE
        };
    };
}

