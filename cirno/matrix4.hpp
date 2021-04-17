/*
 | Cirno
 | 文件名称: matrix4.hpp
 | 文件作用: 矩阵
 | 创建日期: 2021-03-26
 | 更新日期: 2021-04-02
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
#include "rect.hpp"
#include "quater.hpp"
#include "vector4.hpp"
namespace cirno
{
    class Matrix4 final
    {
    public:
        Matrix4() noexcept : buff{ 0.0f }
        {
            static_assert(sizeof(Matrix4) == 4 * sizeof(Vector4), "sizeof(Vector4) * 4 must equ sizeof(*this)");

            buff[0][0] = 1.0f;
            buff[1][1] = 1.0f;
            buff[2][2] = 1.0f;
            buff[3][3] = 1.0f;
        }
        Matrix4(const Matrix4 &b) noexcept
        {
            memcpy(buff, b.buff, sizeof(buff));
        }
        Matrix4& operator=(const Matrix4 &b) noexcept
        {
            memcpy(buff, b.buff, sizeof(buff));
            return *this;
        }
        ~Matrix4() = default;
        // 设置为零矩阵
        Matrix4& SetZero() noexcept
        {
            memset(buff, 0, sizeof(buff));
            return *this;
        }
        // 设置为单位矩阵
        Matrix4& SetIdentity() noexcept
        {
            SetZero();

            int i = 4;
            while (i-- > 0)
            {
                buff[i][i] = 1.0f;
            }

            return *this;
        }
        // 设置缩放矩阵
        MATHLIB_CALL(Matrix4&) SetScaleTransform(const float32 r) noexcept
        {
            SetZero();

            int i = 4;
            while (i-- > 0)
            {
                buff[i][i] = r;
            }
            buff[3][3] = 1.0f;

            return *this;
        }
        // 设置缩放矩阵(static)
        static MATHLIB_CALL(Matrix4) ScaleTransform(const float32 r) noexcept
        {
            return Matrix4().SetScaleTransform(r);
        }
        // 从四元数载入旋转矩阵, q必须是归一化的
        MATHLIB_CALL(Matrix4&) SetRotateTransform(const Quaternion q) noexcept
        {
            SetZero();

            const float32 a = q.X();
            const float32 b = q.Y();
            const float32 c = q.Z();
            const float32 d = q.W();

            buff[0][0] = 1.0f - 2.0f * (c * c + d * d);             // a11 = 1 - 2(c^2 + d^2)
            buff[1][0] = 2.0f * (b * c - a * d);                    // a12 = 2(bc - ad)
            buff[2][0] = 2.0f * (a * c + b * d);                    // a13 = 2(ac + bd)
            buff[0][1] = 2.0f * (b * c + a * d);                    // a21 = 2(bc + ad)
            buff[1][1] = 1.0f - 2.0f * (b * b + d * d);             // a22 = 1 - 2(b^2 + d^2)
            buff[2][1] = 2.0f * (c * d - a * b);                    // a23 = 2(cd - ab)
            buff[0][2] = 2.0f * (b * d - a * c);                    // a31 = 2(bd - ac)
            buff[1][2] = 2.0f * (a * b + c * d);                    // a32 = 2(ab + cd)
            buff[2][2] = 1.0f - 2.0f * (b * b + c * c);             // a33 = 1 - 2(b^2 + c^2)

            buff[3][3] = 1.0f;                                      // a44 = 1 (不处理齐次坐标参数w)

            return *this;
        }
        // 绕轴旋转(static), q必须是归一化的
        static MATHLIB_CALL(Matrix4) RotateTransform(const Quaternion q) noexcept
        {
            return Matrix4().SetRotateTransform(q);
        }
        // 绕轴旋转
        MATHLIB_CALL(Matrix4&) SetRotateTransform(const float32 angle, const Vector3 ax) noexcept
        {
            const float32 c = cosf(angle);                          // cos和sin值
            const float32 s = sinf(angle);

            Vector3 r(ax.GetNormalize());
            Vector3 t(r * (1.0f - c));

            const float32 tx = t.X();
            const float32 ty = t.Y();
            const float32 tz = t.Z();

            const float32 rx = r.X();
            const float32 ry = r.Y();
            const float32 rz = r.Z();

            buff[0][0] = c + tx * rx;
            buff[0][1] = tx * ry + s * rz;
            buff[0][2] = tx * rz - s * ry;
            buff[0][3] = 0.0f;

            buff[1][0] = ty * rx - s * rz;
            buff[1][1] = c + ty * ry;
            buff[1][2] = ty * rz + s * rx;
            buff[1][3] = 0.0f;

            buff[2][0] = tz * rx + s * ry;
            buff[2][1] = tz * ry - s * rx;
            buff[2][2] = c + tz * rz;
            buff[2][3] = 0.0f;

            buff[3][0] = 0.0f;
            buff[3][1] = 0.0f;
            buff[3][2] = 0.0f;
            buff[3][3] = 1;

            return *this;
        }
        // 绕轴旋转(static)
        static MATHLIB_CALL(Matrix4) RotateTransform(const float32 angle, const Vector3 ax) noexcept
        {
            return Matrix4().SetRotateTransform(angle, ax);
        }
        // 平移变换
        MATHLIB_CALL(Matrix4&) SetTranslationTransform(const float32 offset_x, const float32 offset_y, const float32 offset_z) noexcept
        {
            SetIdentity();

            buff[3][0] = offset_x;
            buff[3][1] = offset_y;
            buff[3][2] = offset_z;

            return *this;
        }
        // 平移变换(static)
        static MATHLIB_CALL(Matrix4) TranslationTransform(const float32 x, const float32 y, const float32 z) noexcept
        {
            return Matrix4().SetTranslationTransform(x, y, z);
        }
        // 平移变换
        MATHLIB_CALL(Matrix4&) SetTranslationTransform(const Vector3 offset) noexcept
        {
            SetTranslationTransform(offset.X(), offset.Y(), offset.Z());
            return *this;
        }
        // 平移变换(static)
        static MATHLIB_CALL(Matrix4) TranslationTransform(const Vector3 offset) noexcept
        {
            return Matrix4().SetTranslationTransform(offset);
        }
        // 正射投影
        MATHLIB_CALL(Matrix4&) SetOrthProject(const uint32_t view_w, const uint32_t view_h, const float32 near_plane, const float32 far_plane) noexcept
        {
            SetZero();

            const float32 l = (2.0f * view_w) / (view_w + view_h);  // l = w/2, t=h/2,w和h映射到0-1
            const float32 t = (2.0f * view_h) / (view_w + view_h);

            buff[0][0] = 1.0f / l;
            buff[1][1] = 1.0f / t;
            buff[2][2] = -2.0f / (far_plane - near_plane);
            buff[3][2] = -(far_plane + near_plane) / (far_plane - near_plane);
            buff[3][3] = 1.0f;

            return *this;
        }
        // 正射投影(static)
        static MATHLIB_CALL(Matrix4) OrthProject(const uint32_t view_w, const uint32_t view_h, float32 near_plane, float32 far_plane) noexcept
        {
            return Matrix4().SetOrthProject(view_w, view_h, near_plane, far_plane);
        }
        // 正射投影
        MATHLIB_CALL(Matrix4&) SetOrthProject(const RectF &rc, float32 near_plane, float32 far_plane) noexcept
        {
            SetZero();

            buff[0][0] = 2.0f / (rc.right - rc.left);
            buff[1][1] = 2.0f / (rc.top - rc.bottom);
            buff[2][2] = -2.0f / (far_plane - near_plane);
            buff[3][3] = 1.0f;
            buff[3][0] = -(rc.right + rc.left) / (rc.right - rc.left);
            buff[3][1] = -(rc.top + rc.bottom) / (rc.top - rc.bottom);
            buff[3][2] = -(far_plane + near_plane) / (far_plane - near_plane);

            return *this;
        }
        // 正射投影(static)
        static MATHLIB_CALL(Matrix4) OrthProject(const RectF &rc, float32 near_plane, float32 far_plane) noexcept
        {
            return Matrix4().SetOrthProject(rc, near_plane, far_plane);
        }
        // 透视投影, wdivh指定宽高比
        MATHLIB_CALL(Matrix4&) SetPerspectiveProject(const float32 fovy, const float32 wdivh, const float32 zNear, const float32 zFar, bool flip = false) noexcept
        {
            assert(zNear < zFar);                                       // 近视平面必须比远视平面近(不然可能带来问题)

            const float32 h = cosf(0.5f * fovy) / sinf(0.5f * fovy);    // cot(0.5 * fovy)
            const float32 w = h / wdivh;

            SetZero();                                                  // 零矩阵

            buff[0][0] = w;
            buff[1][1] = flip ? -h : h;                                 // flip指定是否需要翻转投影矩阵, 例如Vulkan就需要翻转
            buff[2][2] = -(zFar + zNear) / (zFar - zNear);
            buff[2][3] = -1.0f;
            buff[3][2] = -(2.0f * zFar * zNear) / (zFar - zNear);

            return *this;
        }
        // 透视投影(static)
        static MATHLIB_CALL(Matrix4) PerspectiveProject(const float32 fovy, const float32 wdivh, const float32 zNear, const float32 zFar, bool flip = false) noexcept
        {
            return Matrix4().SetPerspectiveProject(fovy, wdivh, zNear, zFar, flip);
        }
        // 透视投影
        MATHLIB_CALL(Matrix4&) SetPerspectiveProject(const float32 fovy, const uint32_t width, const uint32_t height, const float32 zNear, const float32 zFar, bool flip = false) noexcept
        {
            return SetPerspectiveProject(fovy, static_cast<float32>(width) / static_cast<float32>(height), zNear, zFar, flip);
        }
        // 透视投影(static)
        static MATHLIB_CALL(Matrix4) PerspectiveProject(const float32 fovy, const uint32_t width, const uint32_t height, const float32 zNear, const float32 zFar, bool flip = false) noexcept
        {
            return Matrix4().SetPerspectiveProject(fovy, static_cast<float32>(width) / static_cast<float32>(height), zNear, zFar, flip);
        }
        // 观察矩阵
        MATHLIB_CALL(Matrix4&) SetLookAt(const Vector3 eye, const Vector3 at, const Vector3 up) noexcept
        {
            Vector3 f, s, u;
            f = (at - eye).SetNormalize();
            s = f.CrossMul(up).SetNormalize();                      // s = f CROSSMUL up
            u = s.CrossMul(f);                                      // u = s CROSSMUL f

            buff[0][0] = s.X();                                     // 第一行是右向量(这三行全部按照摄像机位置eY()e进行平移)
            buff[1][0] = s.Y();
            buff[2][0] = s.Z();
            buff[3][0] = 0.0f;

            buff[0][1] = u.X();                                     // 第二行是上向量
            buff[1][1] = u.Y();
            buff[2][1] = u.Z();
            buff[3][1] = 0.0f;

            buff[0][2] = -f.X();                                    // 第三行是方向向量
            buff[1][2] = -f.Y();
            buff[2][2] = -f.Z();
            buff[3][3] = 0.0f;

            buff[3][0] = -s.DotMul(eye);                            // 这列负责平移摄像机
            buff[3][1] = -u.DotMul(eye);
            buff[3][2] = +f.DotMul(eye);
            buff[3][3] = 1.0f;

            return *this;
        }
        // 观察矩阵(static)
        static MATHLIB_CALL(Matrix4) LookAt(const Vector3 eye, const Vector3 at, const Vector3 up) noexcept
        {
            return Matrix4().SetLookAt(eye, at, up);
        }
        // 叠加一个新变换, 并返回该矩阵
        // r = a.AddTransform(b)相当于r = b * a
        MATHLIB_CALL(Matrix4) AddTransform(const Matrix4 b) const noexcept
        {
            Matrix4 res;
        #if defined(_MATHLIB_USE_SSE)
            for (int i = 0; i < 4; i++)
            {
                __m128 brod1 = _mm_set_ps1(b.buff[i][0]);           // 参见 MATHLIB_CALL(Matrix4) operator*(const Matrix4 &b) 
                __m128 brod2 = _mm_set_ps1(b.buff[i][1]);           // 这里调换了操作数a, b的顺序
                __m128 brod3 = _mm_set_ps1(b.buff[i][2]);
                __m128 brod4 = _mm_set_ps1(b.buff[i][3]);

                __m128 t1 = _mm_mul_ps(brod1, mval[0]);
                __m128 t2 = _mm_mul_ps(brod2, mval[1]);
                __m128 t3 = _mm_mul_ps(brod3, mval[2]);
                __m128 t4 = _mm_mul_ps(brod4, mval[3]);

                __m128 t5 = _mm_add_ps(t1, t2);
                __m128 t6 = _mm_add_ps(t3, t4);

                res.mval[i] = _mm_add_ps(t5, t6);
            }
        #else
            res.SetZero();                                          // 全零的缓冲区

            for (int i = 0; i < 4; i += 1)
            {
                for (int j = 0; j < 4; j += 1)
                {
                    for (int k = 0; k < 4; k += 1)
                    {
                        res[j][i] += b.buff[k][i] * buff[j][k];
                    }
                }
            }
        #endif // _MATHLIB_USE_SSE
            return res;
        }
        // 在本次变换后添加一个变换
        // a.AppendTransform(b)相当于a = b * a
        MATHLIB_CALL(Matrix4&) AppendTransform(const Matrix4 b) noexcept
        {
            Matrix4 res;
        #if defined(_MATHLIB_USE_SSE)
            for (int i = 0; i < 4; i++)
            {
                __m128 brod1 = _mm_set_ps1(b.buff[i][0]);           // 参见 MATHLIB_CALL(Matrix4) operator*(const Matrix4 &b) 
                __m128 brod2 = _mm_set_ps1(b.buff[i][1]);           // 这里调换了操作数a, b的顺序, a.AppendTransform(b)相当于a = b * a
                __m128 brod3 = _mm_set_ps1(b.buff[i][2]);
                __m128 brod4 = _mm_set_ps1(b.buff[i][3]);

                __m128 t1 = _mm_mul_ps(brod1, mval[0]);
                __m128 t2 = _mm_mul_ps(brod2, mval[1]);
                __m128 t3 = _mm_mul_ps(brod3, mval[2]);
                __m128 t4 = _mm_mul_ps(brod4, mval[3]);

                __m128 t5 = _mm_add_ps(t1, t2);
                __m128 t6 = _mm_add_ps(t3, t4);

                res.mval[i] = _mm_add_ps(t5, t6);
            }

            mval[0] = res.mval[0];
            mval[1] = res.mval[1];
            mval[2] = res.mval[2];
            mval[3] = res.mval[3];
        #else
            res.SetZero();

            for (int i = 0; i < 4; i += 1)
            {
                for (int j = 0; j < 4; j += 1)
                {
                    for (int k = 0; k < 4; k += 1)
                    {                                               // r_ij = 求和::(b_ik * a_kj)
                        res[j][i] += b.buff[k][i] * buff[j][k];     // 注意是反过来的, 矩阵乘法从右往左读
                    }
                }
            }
            for (int i = 0; i < 4; i += 1)
            {
                for (int j = 0; j < 4; j += 1)
                {
                    buff[i][j] = res[i][j];
                }
            }
        #endif // _MATHLIB_USE_SSE
            return *this;
        }
        // 矩阵乘法
        MATHLIB_CALL(Matrix4) operator*(const Matrix4 &b) const noexcept
        {
            Matrix4 res;
        #if defined(_MATHLIB_USE_SSE)
            for (int i = 0; i < 4; i++)
            {
                __m128 brod1 = _mm_set_ps1(buff[i][0]);             // brod[0 .. 3]_k = buff[i列][k行]
                __m128 brod2 = _mm_set_ps1(buff[i][1]);
                __m128 brod3 = _mm_set_ps1(buff[i][2]);
                __m128 brod4 = _mm_set_ps1(buff[i][3]);

                __m128 t1 = _mm_mul_ps(brod1, b.mval[0]);           // t_k = buff[i列][k行] * b.buff[k列][0, 1, 2, 3行]
                __m128 t2 = _mm_mul_ps(brod2, b.mval[1]);
                __m128 t3 = _mm_mul_ps(brod3, b.mval[2]);
                __m128 t4 = _mm_mul_ps(brod4, b.mval[3]);

                __m128 t5 = _mm_add_ps(t1, t2);
                __m128 t6 = _mm_add_ps(t3, t4);

                res.mval[i] = _mm_add_ps(t5, t6);                   // res = Σ(t_k)
            }
        #else
            res.SetZero();                                          // 全零的缓冲区

            for (int i = 0; i < 4; i += 1)
            {
                for (int j = 0; j < 4; j += 1)
                {
                    for (int k = 0; k < 4; k += 1)
                    {                                               // r_ij = 求和::(a_ik * b_kj)
                        res[j][i] += buff[k][i] * b.buff[j][k];
                    }
                }
            }
        #endif // _MATHLIB_USE_SSE
            return res;
        }
        // 应用变换: Vector4
        MATHLIB_CALL(Vector4) operator*(const Vector4 b) noexcept
        {
        #if defined(_MATHLIB_USE_SSE)
            __m128 x = _mm_set_ps1(b.X());                          // x[0 .. 3] = b.x
            __m128 y = _mm_set_ps1(b.Y());                          // y[0 .. 3] = b.y
            __m128 z = _mm_set_ps1(b.Z());                          // z[0 .. 3] = b.z
            __m128 w = _mm_set_ps1(b.W());                          // w[0 .. 3] = b.w

            __m128 col1 = _mm_mul_ps(mval[0], x);                   // col1[k] = mval[0][k] * x[k] = mval[0][k] * b.x
            __m128 col2 = _mm_mul_ps(mval[1], y);                   // col2[k] = mval[1][k] * y[k] = mval[1][k] * b.y
            __m128 col3 = _mm_mul_ps(mval[2], z);                   // col3[k] = mval[2][k] * z[k] = mval[2][k] * b.z
            __m128 col4 = _mm_mul_ps(mval[3], w);                   // col4[k] = mval[3][k] * w[k] = mval[3][k] * b.w

            __m128 tmp1 = _mm_add_ps(col1, col2);
            __m128 tmp2 = _mm_add_ps(col3, col4);
            __m128 tmp3 = _mm_add_ps(tmp1, tmp2);                   // tmp3[k] = col1[k] + col2[k] + col3[k] + col4[k]

            float32 res[4];
            _mm_store_ps(res, tmp3);                                // 存放结果

            return Vector4(res[0], res[1], res[2], res[3]);
        #else
            const float32 rx = buff[0][0] * b.X() + buff[1][0] * b.Y() + buff[2][0] * b.Z() + buff[3][0] * b.W();
            const float32 ry = buff[0][1] * b.X() + buff[1][1] * b.Y() + buff[2][1] * b.Z() + buff[3][1] * b.W();
            const float32 rz = buff[0][2] * b.X() + buff[1][2] * b.Y() + buff[2][2] * b.Z() + buff[3][2] * b.W();
            const float32 rw = buff[0][3] * b.X() + buff[1][3] * b.Y() + buff[2][3] * b.Z() + buff[3][3] * b.W();
            return Vector4(rx, ry, rz, rw);
        #endif // _MATHLIB_USE_SSE
        }
        // 应用变换: Vector3, 会补全为齐次坐标(补为1, 矩阵最后一列不参与乘法), 并进行裁剪
        MATHLIB_CALL(Vector3) operator*(const Vector3 b) noexcept
        {
        #if defined(_MATHLIB_USE_SSE)
            __m128 x = _mm_set_ps1(b.X());                          // x[0 .. 3] = b.x
            __m128 y = _mm_set_ps1(b.Y());                          // y[0 .. 3] = b.y
            __m128 z = _mm_set_ps1(b.Z());                          // z[0 .. 3] = b.z

            __m128 col1 = _mm_mul_ps(mval[0], x);                   // col1[k] = mval[0][k] * x[k] = mval[0][k] * b.x
            __m128 col2 = _mm_mul_ps(mval[1], y);                   // col2[k] = mval[1][k] * y[k] = mval[0][k] * b.y
            __m128 col3 = _mm_mul_ps(mval[2], z);                   // col3[k] = mval[2][k] * z[k] = mval[0][k] * b.z

            __m128 tmp1 = _mm_add_ps(col1, col2);                   // tmp1[k] = col1[k] + col2[k]
            __m128 tmp2 = _mm_add_ps(col3, mval[3]);                // tmp2[k] = col3[k] + mval[3][k]
            __m128 tmp3 = _mm_add_ps(tmp1, tmp2);                   // tmp3[k] = tmp1[k] + tmp2[k]

            float32 res[4];
            _mm_store_ps(res, tmp3);                                // 缓存结果

            __m128 k = _mm_set_ps1(res[3]);                         // k[0 .. 3] = res[3](齐次坐标参数w)
            __m128 r = _mm_div_ps(tmp3, k);                         // r[k] = tmp3[k] / k[k], 进行齐次坐标裁剪
            _mm_store_ps(res, r);                                   // r的前三个就是变换结果

            return Vector3(res[0], res[1], res[2]);
        #else
            const float32 rx = buff[0][0] * b.X() + buff[1][0] * b.Y() + buff[2][0] * b.Z() + buff[3][0];
            const float32 ry = buff[0][1] * b.X() + buff[1][1] * b.Y() + buff[2][1] * b.Z() + buff[3][1];
            const float32 rz = buff[0][2] * b.X() + buff[1][2] * b.Y() + buff[2][2] * b.Z() + buff[3][2];
            const float32 rw = buff[0][3] * b.X() + buff[1][3] * b.Y() + buff[2][3] * b.Z() + buff[3][3];
            return Vector3(rx / rw, ry / rw, rz / rw);
        #endif // _MATHLIB_USE_SSE
        }
        // 设置转置
        Matrix4& SetTransposition() noexcept
        {
            for (uint32_t i = 0; i < 4; i += 1)
            {
                for (uint32_t j = 0; j < i; j += 1)
                {
                    std::swap(data[i][j], data[j][i]);
                }
            }
            return *this;
        }
        // 取得该矩阵的转置
        Matrix4 GetTransposition() const noexcept
        {
            return Matrix4(*this).SetTransposition();
        }
        // 取得第几行的向量
        inline Vector4 &operator[](unsigned int i) noexcept
        {
            assert(i >= 0 && i < 4);
            return data[i];
        }
        // 取得第几行的向量
        inline const Vector4 &operator[](unsigned int i) const noexcept
        {
            assert(i >= 0 && i < 4);
            return data[i];
        }
        // 取得数据区内存
        inline float32* DataPtr() noexcept
        {
            return &buff[0][0];
        }
        // 取得数据区内存
        inline const float32* DataPtr() const noexcept
        {
            return &buff[0][0];
        }
    private:
        // 内存布局:
        // union
        // { | ---- 128 ---- | ---- 128 ---- | ---- 128 ---- | ---- 128 ---- |
        //   |    Vector4    |    Vector4    |    Vector4    |    Vector4    |
        //   |     __m128    |     __m128    |     __m128    |     __m128    |
        //   |    buff[4]    |    buff[4]    |    buff[4]    |    buff[4]    |
        // }
        // 数组结构: buff[第k列][第k行]
        //          Vector4 data[], __m128 mval[]均表示每一行的值
        union
        {
            Vector4 data[4];
        #if defined(_MATHLIB_USE_SSE)
            __m128  mval[4];
        #endif // _MATHLIB_USE_SSE
            float32 buff[4][4];
        };
    };
}
