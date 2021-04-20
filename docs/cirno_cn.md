# Cirno

&emsp;&emsp;Cirno是一个用C++编写的、纯头文件实现且易读懂的数学库，用于图形学方面。提供四元数、空间向量、4x4方阵以及一些常用变换的实现。

&emsp;&emsp;这里是它的文档。

[TOC]

## 使用

### Getting start

#### 依赖项

&emsp;&emsp;暂无

#### 添加到项目中

&emsp;&emsp;目前只能使用一个头文件来include到你的项目中：

```c++
#include <cirno/cirno.hpp>
```

&emsp;&emsp;其余文件不应该单独被include，不然可能带来编译错误。目前而言，include这个文件并不会严重影响编译时间，且不会引入严重的命名污染。

&emsp;&emsp;接着，在命名空间"cirno"下，可以使用这个库的所有内容。下面的例子计算![](http://latex.codecogs.com/svg.latex?\vec{c}=\vec{a}\times\vec{b})：

```c++
// ...
void do_something()
{
    cirno::Vector3 a(1.0f, 2.0f, 3.0f);
    cirno::Vector3 b(3.0f, 4.0f, 5.0f);
    cirno::Vector3 c = a.CrossMul(b);
}
```

&emsp;&emsp;更多内容可以参照后面对于各个类的文档。

### 预处理宏

#### _MATHLIB_USE_SSE

&emsp;&emsp;该宏指定打开SSE指令集：

```c++
#define  _MATHLIB_USE_SSE    1      // use SSE
#include <cirno/cirno.hpp>
```

&emsp;&emsp;这个宏只能在IA32/AMD6等支持SSE指令集的地方使用。

## rect.hpp

&emsp;&emsp;本文件提供一些简单的矩形，方便个别函数使用。

### namespace

```c++
namespace cirno
{
   // here
}
```

### struct RectF

&emsp;&emsp;该结构体描述一个矩形：

<img src="images/rect.png" alt="RectF" style="zoom:50%;" />

&emsp;&emsp;通常情况下，我们默认![](http://latex.codecogs.com/svg.latex?left\leq right)：而且![](http://latex.codecogs.com/svg.latex?top\leq bottom)。

#### 成员

```c++
struct RectF
{
    float32 left, right;
    float32 top, bottom;
};
```

#### 成员函数

##### RectF::RectF()

&emsp;&emsp;默认构造函数。

&emsp;&emsp;参数：无。

&emsp;&emsp;异常：无。

##### RectF::RectF(float32 l, float32 r, float32 t, float32 b)

&emsp;&emsp;构造一个矩形。该方法不检查参数的有效性。

&emsp;&emsp;参数：

| 参数类型 | 参数名称 | 说明             |
| -------- | -------- | ---------------- |
| float32  | l        | 指定矩形的left   |
| float32  | r        | 指定矩形的right  |
| float32  | t        | 指定矩形的top    |
| float32  | b        | 指定矩形的bottom |

&emsp;&emsp;异常：无。

### struct RectI32

&emsp;&emsp;该结构体描述一个矩形，其中所有坐标均为32位有符号整数：

<img src="images/rect.png" alt="RectF" style="zoom:50%;" />

&emsp;&emsp;通常情况下，我们默认![](http://latex.codecogs.com/svg.latex?left\leq right)：而且![](http://latex.codecogs.com/svg.latex?top\leq bottom)。

#### 成员

```c++
struct RectI32
{
    int32_t left, right;
    int32_t top, bottom;
};
```

#### 成员函数

##### RectI32::RectI32()

&emsp;&emsp;默认构造函数。

&emsp;&emsp;参数：无。

&emsp;&emsp;异常：无。

##### RectI32::RectI32(int32_t l, int32_t r, int32_t t, int32_t b)

&emsp;&emsp;构造一个矩形。该方法不检查参数的有效性。

&emsp;&emsp;参数：

| 参数类型 | 参数名称 | 说明             |
| -------- | -------- | ---------------- |
| int32_t  | l        | 指定矩形的left   |
| int32_t  | r        | 指定矩形的right  |
| int32_t  | t        | 指定矩形的top    |
| int32_t  | b        | 指定矩形的bottom |

&emsp;&emsp;异常：无。

##### RectI32::operator RectF()

&emsp;&emsp;将该结构转换为RectF结构。可能会有一定的精度损失。

&emsp;&emsp;参数：无。

&emsp;&emsp;异常：无。

## vector2.hpp

&emsp;&emsp;本文件提供平面向量。

### namespace

```c++
namespace cirno
{
   // here
}
```

### struct CompactVector2

#### 成员

```c++
struct CompactVector2
{
    float32 x, y;
};
```

### class Vector2

&emsp;&emsp;该类提供平面向量封装，这个类不可继承。请注意，这个类不保证sizeof(\*this) == sizeof(float32) \* 2。

#### 类成员

##### 类成员声明

&emsp;&emsp;声明表示向量![](http://latex.codecogs.com/svg.latex?\left(x, y\right))

```c++
union
{
    struct
    {
        float32 x, y;
    };
    float32 buff[4];
    #if defined(_MATHLIB_USE_SSE)
        __m128 val;
    #endif // _MATHLIB_USE_SSE
};
```

##### 内存布局

![Memory layout](images/vector2_memorylayout.png)

#### 成员函数

&emsp;&emsp;若无特殊说明，我们默认\*this的变量名是![](http://latex.codecogs.com/svg.latex?x)，返回值为![](http://latex.codecogs.com/svg.latex?ret)，且方法不抛出任何异常。

&emsp;&emsp;另外，通常情况下GetXXX与SetXXX是一致的，区别在于Set开头的方法修改自身，Get开头的方法不修改自身。

##### Vector2::Vector2()

![](http://latex.codecogs.com/svg.latex?\vec{x}=\vec{0})

##### Vector2::Vector2(float32  \_v)

![](http://latex.codecogs.com/svg.latex?\vec{x}=\left(\_ v, \_ v\right))

##### Vector2::Vector2(float32 \_x, float32 \_y)

![](http://latex.codecogs.com/svg.latex?\vec{x}=\left(\_ x, \_ y\right))

##### Vector2::Vector2(const CompactVector2 &vec2)

![](http://latex.codecogs.com/svg.latex?\vec{x}=\left(vec2.x, vec2.y\right))

##### Vector2::GetNormL2Square()

&emsp;&emsp;该方法计算如下式子：

![](http://latex.codecogs.com/svg.latex?\text{ret}=\Vert x \Vert ^2)

##### Vector2::GetNormL2()

![](http://latex.codecogs.com/svg.latex?\text{ret}=\Vert x \Vert)


##### Vector2::Length()

&emsp;&emsp;该方法与Vector2::GetNormL2()功能相同。

##### Vector2::SetNormalize()

&emsp;&emsp;该方法对\*this执行归一化，即：

![](http://latex.codecogs.com/svg.latex?\vec{x}=\frac{1}{\Vert x \Vert} \cdot \vec{x})

##### Vector2::GetNormalize()

&emsp;&emsp;该方法返回\*this的归一化结果，即：

![](http://latex.codecogs.com/svg.latex?\text{ret}=\frac{1}{\Vert x \Vert} \cdot \vec{x})

##### Vector2::DotMul(const Vector2 b)

&emsp;&emsp;该方法返回\*this与向量b的点乘结果，即：

![](http://latex.codecogs.com/svg.latex?\text{ret}=\vec{x} \cdot \vec{b})

##### Vector2::operator+=(const Vector2 b)

![](http://latex.codecogs.com/svg.latex?\vec{x}=\vec{x} + \vec{b})

##### Vector2::operator+(const Vector2 b) const

![](http://latex.codecogs.com/svg.latex?\text{ret}=\vec{x} + \vec{b})

##### Vector2::operator-=(const Vector2 b)

![](http://latex.codecogs.com/svg.latex?\vec{x}=\vec{x} - \vec{b})

##### Vector2::operator-(const Vector2 b) const

![](http://latex.codecogs.com/svg.latex?\text{ret}=\vec{x} - \vec{b})

##### Vector2::operator\*=(const float32 v)

&emsp;&emsp;该方法计算v与\*this的数乘，并赋值给\*this：

![](http://latex.codecogs.com/svg.latex?\vec{x}=v\cdot\vec{x})

##### operator\*(const Vector2 vec, const float32 v)

&emsp;&emsp;该方法返回v与\*this的数乘：

![](http://latex.codecogs.com/svg.latex?\vec{x}=v\cdot\vec{x})

##### Vector2::operator-()

![](http://latex.codecogs.com/svg.latex?\text{ret}=-\vec{x})

##### Vector2::operator\[\](unsigned int i)

&emsp;&emsp;该重载函数返回向量![](http://latex.codecogs.com/svg.latex?\left(x, y\right))的元素，下标与元素的关系如下：

| 下标i | 元素 |
| ----- | ---- |
| 0     | x    |
| 1     | y    |

&emsp;&emsp;超过允许返回的下标不被允许：

```c++
assert(i < 2);
```

##### Vector2::X()

&emsp;&emsp;该方法返回向量![](http://latex.codecogs.com/svg.latex?\left(x, y\right))的元素x的值。

##### Vector2::Y()

&emsp;&emsp;该方法返回向量![](http://latex.codecogs.com/svg.latex?\left(x, y\right))的元素y的值。

##### Vector2::GetPtr()

&emsp;&emsp;该方法返回一个指针，与向量![](http://latex.codecogs.com/svg.latex?\left(x, y\right))使用共同的内存区域。

&emsp;&emsp;外部访问应该限定在两个元素，避免出现内存问题。

##### Vector2::operator CompactVector2()

&emsp;&emsp;该方法返回一个更紧凑的结构CompactVector2，表示相同的向量。

## vector3.hpp

&emsp;&emsp;本文件提供空间向量。

### namespace

```c++
namespace cirno
{
   // here
}
```


## vector4.hpp

&emsp;&emsp;本文件提供四维向量。

### namespace

```c++
namespace cirno
{
   // here
}
```


## quater.hpp

&emsp;&emsp;本文件提供四元数。

### namespace

```c++
namespace cirno
{
   // here
}
```


## matrix4.hpp

&emsp;&emsp;本文件提供4x4大小的方阵，和一些生成该大小矩阵的方法。

### namespace

```c++
namespace cirno
{
   // here
}
```

