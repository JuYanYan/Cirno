/*
 | Cirno
 | 文件名称: quater.hpp
 | 文件作用: 四元数
 | 创建日期: 2021-03-26
 | 更新日期: 2021-04-01
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
#include "vector3.hpp"
#include "vector4.hpp"
namespace cirno
{
    // 四元数
    class Quaternion final : public Vector4
    {
    public:
        // 虚数部分
        using ImaginaryNum = Vector3;

        Quaternion() noexcept : Vector4(1.0f, 0.0f, 0.0f, 0.0f)
        {
            // nothing to do...
        }
        Quaternion(const Vector3 imag) noexcept : Vector4(0.0f, imag.X(), imag.Y(), imag.Z())
        {
            // nothing to do...
        }
        Quaternion(const float32 angle, const Vector3 axis) noexcept
        {
            SetByRotateAxis(angle, axis);
        }
        Quaternion(const float32 pitch, const float32 yaw, const float32 roll) noexcept
        {
            SetByEulerAngle(pitch, yaw, roll);
        }
        Quaternion(float32 _a, float32 _b, float32 _c, float32 _d) noexcept : Vector4(_a, _b, _c, _d)
        {
            // nothing to do...
        }
    #if defined(_MATHLIB_USE_SSE)
        Quaternion(__m128 _val) noexcept : Vector4(_val)
        {
            // nothing to do...
        }
    #endif // _MATHLIB_USE_SSE
        Quaternion(float32 v) = delete;
        ~Quaternion() = default;
        Quaternion(const Quaternion &) = default;
        Quaternion& operator=(const Quaternion &) = default;
        // 从欧拉角那设置四元数, 旋转顺序: Roll -> Pitch -> Yaw, 即ZXY
        MATHLIB_CALL(Quaternion&) SetByEulerAngle(float32 pitch, float32 yaw, float32 roll) noexcept
        {
            float32 sx, sy, sz, cx, cy, cz;
            sx = sinf(0.5f * pitch);                            // X: pitch (俯仰)角, 物体从屏幕里面飞出来(Z正方向)
            sy = sinf(0.5f * yaw);                              // Y: yaw   (航向)角
            sz = sinf(0.5f * roll);                             // Z: roll  (滚转)角
            cx = cosf(0.5f * pitch);
            cy = cosf(0.5f * yaw);
            cz = cosf(0.5f * roll);

            a = cx * cy * cz + sx * sy * sz;
            b = sx * cy * cz + cx * sy * sz;
            c = cx * sy * cz - sx * cy * sz;
            d = cx * cy * sz - sx * sy * cz;

            return *this;
        }
        // 从欧拉角那创建四元数, 旋转顺序: Roll -> Pitch -> Yaw, 即ZXY
        static MATHLIB_CALL(Quaternion) EulerAngle(float32 pitch, float32 yaw, float32 roll) noexcept
        {
            return Quaternion().SetByEulerAngle(pitch, yaw, roll);
        }
        // 从绕某个向量旋转一个角度来四元数
        MATHLIB_CALL(Quaternion&) SetByRotateAxis(const float32 angle, const Vector3 _axis) noexcept
        {
            Vector3 axis = _axis.GetNormalize();

            const float32 si = sinf(0.5f * angle);
            const float32 co = cosf(0.5f * angle);

            a = co;
            b = si * axis.X();
            c = si * axis.Y();
            d = si * axis.Z();

            return *this;
        }
        // 从绕某个向量旋转一个角度来创建四元数
        static MATHLIB_CALL(Quaternion) RotateAxis(const float32 angle, const Vector3 _axis) noexcept
        {
            return Quaternion().SetByRotateAxis(angle, _axis);
        }
        // 取得虚数部分
        ImaginaryNum GetImaginaryPart() const noexcept
        {
            return ImaginaryNum(b, c, d);
        }
        // 共轭
        Quaternion& SetConjugate() noexcept
        {
            b = -b;                                             // 反转虚部得到共轭
            c = -c;
            d = -d;
            return *this;
        }
        // 取得四元数的共轭结果
        MATHLIB_CALL(Quaternion) GetConjugate() const noexcept
        {
            Quaternion r;
            r.a = a;                                            // 实部不反转
            r.b = -b;                                           // 虚部反转
            r.c = -c;
            r.d = -d;
            return r;
        }
        // 逆
        Quaternion& SetInverse() noexcept
        {
        #if defined(_MATHLIB_USE_SSE)
            float32 tmp[4];
            // 首先计算||this||^2, 结果存放在 tmp[0]中
            __m128 r = _mm_mul_ps(val, val);
            _mm_store_ps(tmp, r);
            tmp[0] = tmp[0] + tmp[1] + tmp[2] + tmp[3];
            // 得到1 / length2
            __m128 t = _mm_load_ps1(tmp);                       // t[0 .. 3] = length
            __m128 k = _mm_rcp_ps(t);                           // k = 1 / length
            // 计算数乘结果
            __m128 o = _mm_mul_ps(k, t);
            // 取得结果
            _mm_store_ps(tmp, o);
            // 最后取共轭, 因为q^-1 = q共轭 / (||q||^2), 因此就是最终答案
            a = tmp[0];
            b = -tmp[1];                                        // 反转虚部得到共轭
            c = -tmp[2];
            d = -tmp[3];
        #else
            float32 lensq = GetNormL2Square();                  // 模长的平方
            SetConjugate();                                     // 求得共轭
            this->operator*=(1.0f / lensq);                     // q^-1 = q共轭 / (||q||^2)
        #endif // _MATHLIB_USE_SSE
            return *this;
        }
        // 取得四元数的逆
        MATHLIB_CALL(Quaternion) GetInverse() const noexcept
        {
        #if defined(_MATHLIB_USE_SSE)
            float32 tmp[4];
            __m128 r = _mm_mul_ps(val, val);
            _mm_store_ps(tmp, r);
            tmp[0] = tmp[0] + tmp[1] + tmp[2] + tmp[3];

            __m128 t = _mm_load_ps1(tmp);
            __m128 k = _mm_rcp_ps(t);

            __m128 o = _mm_mul_ps(k, t);

            _mm_store_ps(tmp, o);

            return Quaternion(tmp[0], -tmp[1], -tmp[2], -tmp[3]);
        #else
            Quaternion r(*this);
            r.SetInverse();
            return r;
        #endif // _MATHLIB_USE_SSE
        }
        // 四元数加法
        MATHLIB_CALL(Quaternion&) operator+=(const Quaternion b) noexcept
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
        // 四元数加法
        MATHLIB_CALL(Quaternion) operator+(const Quaternion b) const noexcept
        {
        #if defined(_MATHLIB_USE_SSE)
            return Quaternion(_mm_add_ps(val, b.val));
        #else
            return Quaternion(x + b.x, y + b.y, z + b.z, w + b.w);
        #endif // _MATHLIB_USE_SSE
        }
        // 四元数减法
        MATHLIB_CALL(Quaternion&) operator-=(const Quaternion b) noexcept
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
        // 四元数减法
        MATHLIB_CALL(Quaternion) operator-(const Quaternion b) const noexcept
        {
        #if defined(_MATHLIB_USE_SSE)
            return Quaternion(_mm_sub_ps(val, b.val));
        #else
            return Quaternion(x - b.x, y - b.y, z - b.z, w - b.w);
        #endif // _MATHLIB_USE_SSE
        }
        // 四元数数乘
        MATHLIB_CALL(Quaternion&) operator*=(const float32 v) noexcept
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
        // 四元数乘法(GraBmann积)
        MATHLIB_CALL(Quaternion&) operator*=(const Quaternion _b) noexcept
        {
        #if defined(_MATHLIB_USE_SSE)
            float32 buff_v[4] = { b, c, d };
            float32 buff_u[4] = { _b.b, _b.c, _b.d };
            // 取得q1(b), q2(this)的虚部u, v
            __m128 u = _mm_load_ps(buff_u);                     // q1 = [b.a u] u[0 .. 3] = (_b.b, _b.c. _b.d, _)
            __m128 v = _mm_load_ps(buff_v);                     // q1 = [this.a v], v[0 .. 3] = (this.b, this.c. this.d, _)
            // uleng = ||u||^2
            float32 tmp_uleng[4];
            __m128  ulengsq = _mm_mul_ps(u, u);                 // ulengsq[0 .. 3] = u[k] * u[k] = (u[k])^2
            _mm_store_ps(tmp_uleng, ulengsq);                   // tmp_uleng[0]是计算得到的u的模长的平方
            tmp_uleng[0] = tmp_uleng[0] + tmp_uleng[1] + tmp_uleng[2];
            __m128 uleng = _mm_load_ss(tmp_uleng);
            //   v CROSSMUL u
            // = r1 - r2
            // = v(y, z, x) MUL u(z, x, y) - v(z, x, y) MUL u(y, z, x)
            // = t1 * t2 - t3 * t4
            __m128 t1 = _mm_shuffle_ps(v, v, 0xc9);             // t1(y, z, x) := v(x, y, z)
            __m128 t2 = _mm_shuffle_ps(u, u, 0xd2);             // t2(z, x, y) := u(x, y, z)
            __m128 r1 = _mm_mul_ps(t1, t2);                     // r1[k] = t1[k] * t2[k]
            __m128 t3 = _mm_shuffle_ps(v, v, 0xd2);             // t3(z, x, y) := v(x, y, z)
            __m128 t4 = _mm_shuffle_ps(u, u, 0xc9);             // t4(y, z, x) := u(x, y, z)
            __m128 r2 = _mm_mul_ps(t3, t4);                     // r2[k] = t3[k] * t4[k]
            __m128 cr = _mm_sub_ps(r1, r2);                     // cr[k] = r1[k] - r2[k]   
            //   this.a NUMBER_MUL u + b.a NUMBER_MUL v + v CROSSMUL u
            // = this.a NUMBER_MUL u + b.a NUMBER_MUL v + cr
            // = tv + tu + cr
            // = tr
            __m128 this_a = _mm_load_ps1(&a);
            __m128 b_a = _mm_load_ps1(&_b.a);
            __m128 tv = _mm_mul_ps(b_a, v);                     // tv[k] = b_a[k] * v[k]
            __m128 tu = _mm_mul_ps(this_a, u);                  // tu[k] = this_a[k] * u[k]
            __m128 ts = _mm_add_ps(tv, tu);                     // ts[k] = tu[k] + tv[k]
            __m128 tr = _mm_add_ps(ts, cr);                     // tr[k] = ts[k] + cr[k]
            //   a * _b.a - tmp_uleng[0]
            // = this_a[0] * b_a[0] - tmp_uleng[0]
            // = tm - uleng
            // = ra
            __m128 tm = _mm_mul_ss(this_a, b_a);
            __m128 ra = _mm_sub_ss(tm, uleng);
            //   tr(x, y, z, w)
            // = ri(w, x, y, z)
            __m128 ri = _mm_shuffle_ps(tr, tr, 0x93);           // ri(w, x, y, z) = tr(x, y, z, w)
            // store
            _mm_store_ps(buff, ri);                             // a, b, c, d = ri[0 .. 3]
            _mm_store_ss(buff, ra);                             // a = ra[0]
        #else
            ImaginaryNum v = GetImaginaryPart();                // q1 = [this.a v]
            ImaginaryNum u = _b.GetImaginaryPart();             // q2 = [b.a u]

            float32 r = a * _b.a - u.GetNormL2Square();

            ImaginaryNum t = v.CrossMul(u);                     // t = v CROSSMUL u
            u *= a;
            v *= _b.a;
            t += u;
            t += v;                                             // t = this.a * u + b.a * v + v * u

            a = r;                                              // q1*q2 = [r t]
            b = t.X();
            c = t.Y();
            d = t.Z();
        #endif // _MATHLIB_USE_SSE
            return *this;
        }
        // 四元数数乘
        friend MATHLIB_CALL(Quaternion) operator*(const Quaternion vec, const float32 v) noexcept
        {
        #if defined(_MATHLIB_USE_SSE)
            __m128 t = _mm_load_ps1(&v);
            __m128 r = _mm_mul_ps(vec.val, t);
            return Quaternion(r);
        #else
            return Quaternion(vec.x * v, vec.y * v, vec.z * v, vec.w * v);
        #endif // _MATHLIB_USE_SSE
        }
        // 四元数乘法
        MATHLIB_CALL(Quaternion) operator*(const Quaternion _b) noexcept
        {
            Quaternion t(*this);
            t *= _b;
            return t;
        }
        // 按照下标取得值, [0] = x, [1] = y, [2] = z, [3] = w
        float32& operator[](unsigned int i) noexcept
        {
            assert(i < 4);
            return buff[i];
        }
        // 按照下标取得值, [0] = x, [1] = y, [2] = z, [3] = w
        float32 operator[](unsigned int i) const noexcept
        {
            assert(i < 4);
            return buff[i];
        }
    private:
        // 内存布局:
        // Z = a + bi + cj + dk
        // union
        // { | ---- 32 ---- | ---- 32 ---- | ---- 32 ---- | ---- 32 ---- |
        //   |                         Vector4 father                    |
        //   |   float32 a  |   float32 b  |   float32 c  |   float32 d  |
        // }
        //
        // nothing ... 参考Vector4类
        //
    };
}
