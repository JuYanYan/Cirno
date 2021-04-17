/*
 | Cirno
 | 文件名称: rect.hpp
 | 文件作用: 矩形
 | 创建日期: 2021-03-26
 | 更新日期: 2021-03-27
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
    // 浮点数矩形
    struct RectF
    {
        float32 left, right;
        float32 top, bottom;
        RectF() = default;
        RectF(float32 l, float32 r, float32 t, float32 b) noexcept
        {
            left = l;
            right = r;

            top = t;
            bottom = b;
        }
    };
    // 整数矩形
    struct RectI32
    {
        int32_t left, right;
        int32_t top, bottom;
        RectI32() = default;
        RectI32(int32_t l, int32_t r, int32_t t, int32_t b) noexcept
        {
            left = l;
            right = r;

            top = t;
            bottom = b;
        }
        // 隐式转换
        operator RectF() const noexcept
        {
            return RectF(
                static_cast<float32>(left),
                static_cast<float32>(right),
                static_cast<float32>(top),
                static_cast<float32>(bottom)
            );
        }
    };
}

