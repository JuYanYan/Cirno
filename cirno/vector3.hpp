/*
 | Cirno
 | 文件名称: vector3.hpp
 | 文件作用: 空间向量
 | 创建日期: 2021-03-14
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
    // 空间向量
    struct CompactVector3
    {
        float32 x, y, z;
    };
    // 空间向量, 16字节对齐分配, 大小16字节
    class Vector3 final
    {
    public:
        Vector3() noexcept : x(0.0f), y(0.0f), z(0.0f)
        {
            // nothing to do
        }
        Vector3(float32 _v) noexcept : x(_v), y(_v), z(_v)
        {
            // nothing to do
        }
        Vector3(float32 _x, float32 _y, float32 _z) noexcept : x(_x), y(_y), z(_z)
        {
            // nothing to do
        }
        Vector3(const CompactVector3 &vec3) noexcept : x(vec3.x), y(vec3.y), z(vec3.z)
        {
            // nothing to do
        }
    #if defined(_MATHLIB_USE_SSE)
        Vector3(__m128 _val) noexcept : val{ _val }
        {
            // nothing to do
        }
    #endif // _MATHLIB_USE_SSE
        Vector3(const Vector3 &) = default;
        Vector3& operator=(const Vector3 &) = default;
        ~Vector3() = default;
        // 取得模长的平方
        float32 GetNormL2Square() const noexcept
        {
        #if defined(_MATHLIB_USE_SSE)
            float32 tmp[4];
            __m128 t = _mm_load_ps(buff);                   // t[0 .. 3] = (x, y, z, _)
            __m128 r = _mm_mul_ps(t, t);
            _mm_store_ps(tmp, r);
            return tmp[0] + tmp[1] + tmp[2];
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
        Vector3& SetNormalize() noexcept
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
        Vector3 GetNormalize() const noexcept
        {
            Vector3 t(*this);
            return t.SetNormalize();
        }
        // 点乘
        MATHLIB_CALL(float32) DotMul(const Vector3 b) const noexcept
        {
        #if defined(_MATHLIB_USE_SSE)
            float32 buff[4];
            __m128 t = _mm_mul_ps(val, b.val);              // t[k] = val[k] * b.val[k]
            _mm_store_ps(buff, t);
            return buff[0] + buff[1] + buff[2];
        #else
            return x * b.x + y * b.y + z * b.z;
        #endif // _MATHLIB_USE_SSE
        }
        // 叉乘
        MATHLIB_CALL(Vector3) CrossMul(const Vector3 b) const noexcept
        {
        #if defined(_MATHLIB_USE_SSE)
            __m128 t1 = _mm_shuffle_ps(val, val, 0xc9);     // t1(y, z, x) := val(x, y, z)
            __m128 t2 = _mm_shuffle_ps(b.val, b.val, 0xd2); // t2(z, x, y) := b(x, y, z)
            __m128 r1 = _mm_mul_ps(t1, t2);                 // r1[k] = t1[k] * t2[k]

            __m128 t3 = _mm_shuffle_ps(val, val, 0xd2);     // t3(z, x, y) := val(x, y, z)
            __m128 t4 = _mm_shuffle_ps(b.val, b.val, 0xc9); // t4(y, z, x) := val(x, y, z)
            __m128 r2 = _mm_mul_ps(t3, t4);                 // r2[k] = t3[k] * t4[k]

            __m128 res = _mm_sub_ps(r1, r2);                // res[k] = r1[k] - r2[k]   
            return Vector3(res);
        #else
            Vector3 r;
            r.x = y * b.z - z * b.y;
            r.y = z * b.x - x * b.z;
            r.z = x * b.y - y * b.x;
            return r;
        #endif // _MATHLIB_USE_SSE
        }
        // 空间向量加法
        MATHLIB_CALL(Vector3&) operator+=(const Vector3 b) noexcept
        {
        #if defined(_MATHLIB_USE_SSE)
            val = _mm_add_ps(val, b.val);
        #else
            x += b.x;
            y += b.y;
            z += b.z;
        #endif // _MATHLIB_USE_SSE
            return *this;
        }
        // 空间向量加法
        MATHLIB_CALL(Vector3) operator+(const Vector3 b) const noexcept
        {
        #if defined(_MATHLIB_USE_SSE)
            return Vector3(_mm_add_ps(val, b.val));
        #else
            return Vector3(x + b.x, y + b.y, z + b.z);
        #endif // _MATHLIB_USE_SSE
        }
        // 空间向量减法
        MATHLIB_CALL(Vector3&) operator-=(const Vector3 b) noexcept
        {
        #if defined(_MATHLIB_USE_SSE)
            val = _mm_sub_ps(val, b.val);
        #else
            x -= b.x;
            y -= b.y;
            z -= b.z;
        #endif // _MATHLIB_USE_SSE
            return *this;
        }
        // 空间向量减法
        MATHLIB_CALL(Vector3) operator-(const Vector3 b) const noexcept
        {
        #if defined(_MATHLIB_USE_SSE)
            return Vector3(_mm_sub_ps(val, b.val));
        #else
            return Vector3(x - b.x, y - b.y, z - b.z);
        #endif // _MATHLIB_USE_SSE
        }
        // 空间向量数乘
        MATHLIB_CALL(Vector3&) operator*=(const float32 v) noexcept
        {
        #if defined(_MATHLIB_USE_SSE)
            __m128 t = _mm_load_ps1(&v);
            val = _mm_mul_ps(val, t);
        #else
            x *= v;
            y *= v;
            z *= v;
        #endif // _MATHLIB_USE_SSE
            return *this;
        }
        // 空间向量数乘
        friend MATHLIB_CALL(Vector3) operator*(const Vector3 vec, const float32 v) noexcept
        {
        #if defined(_MATHLIB_USE_SSE)
            __m128 t = _mm_load_ps1(&v);
            __m128 r = _mm_mul_ps(vec.val, t);
            return Vector3(r);
        #else
            return Vector3(vec.x * v, vec.y * v, vec.z * v);
        #endif // _MATHLIB_USE_SSE
        }
        // 取反
        MATHLIB_CALL(Vector3) operator-() const noexcept
        {
            return Vector3(-x, -y, -z);
        }
        // 按照下标取得值, [0] = x, [1] = y, [2] = z
        float32& operator[](unsigned int i) noexcept
        {
            assert(i < 3);
            return buff[i];
        }
        // 按照下标取得值, [0] = x, [1] = y, [2] = z
        float32 operator[](unsigned int i) const noexcept
        {
            assert(i < 3);
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
        // 取得ptr, 原始类型是float32[3]
        inline float32* GetPtr() noexcept
        {
            return buff;
        }
        // 取得ptr, 原始类型是float32[3]
        inline const float32* GetPtr() const noexcept
        {
            return buff;
        }
        // 取得更紧凑的空间向量
        inline CompactVector3 CompactVector() const noexcept
        {
            return CompactVector3{ x, y, z };
        }
    protected:
        // 内存布局:
        // union
        // { | ---- 32 ---- | ---- 32 ---- | ---- 32 ---- | ---- 32 ---- |
        //   |                         __m128 val                        |
        //   |                        float32 buff[3]                    |
        //   |   float32 x  |   float32 y  |   float32 z  |   (noused)   |
        // }
        union
        {
            struct
            {
                float32 x, y, z;
            };
            float32 buff[4];
        #if defined(_MATHLIB_USE_SSE)
            __m128 val;
        #endif // _MATHLIB_USE_SSE
        };
    };
}
