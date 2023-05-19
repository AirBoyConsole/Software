#pragma once

#include <iostream>
#include <math.h>

namespace airboy {

template <class T>
class Vector2 {
    public:
        Vector2();
        Vector2(const T x, const T y);

        template <class U>
        Vector2(Vector2<U> arg) { x = arg.x, y = arg.y; };

        Vector2 abs() const;
        float lenght() const;
        void normalize();

        Vector2 operator+(Vector2 const &vec) const;
        Vector2 operator-(Vector2 const &vec) const;
        void operator+=(Vector2 const &vec);
        void operator-=(Vector2 const &vec);

        Vector2 operator*(Vector2 const &vec) const;
        Vector2 operator*(const T &val) const;
        void operator*=(Vector2 const &vec);
        void operator*=(const T &val);

        Vector2 operator/(Vector2 const &vec) const;
        Vector2 operator/(const T &val) const;
        void operator/=(Vector2 const &vec);
        void operator/=(const T &val);
        
        Vector2 operator-() const;
        Vector2 operator++();
        Vector2 operator++(int);
        Vector2 operator--();
        Vector2 operator--(int);

        bool operator==(const Vector2 &p_vec2) const;
        bool operator!=(const Vector2 &p_vec2) const;
        bool operator<(const Vector2 &p_vec2) const;
        bool operator>(const Vector2 &p_vec2) const;
        bool operator<=(const Vector2 &p_vec2) const;
        bool operator>=(const Vector2 &p_vec2) const;

        static const Vector2 UP;
        static const Vector2 DOWN;
        static const Vector2 LEFT;
        static const Vector2 RIGHT;
        static const Vector2 ZERO;
        static const Vector2 ONE;

        T x;
        T y; 
};

template <class T>
inline Vector2<T> Vector2<T>::operator+(Vector2<T> const& vec) const
{
    return Vector2(this->x + vec.x, this->y + vec.y);
}

template <class T>
inline void Vector2<T>::operator+=(Vector2<T> const& vec)
{
    this->x += vec.x;
    this->y += vec.y;
}

template <class T>
inline Vector2<T> Vector2<T>::operator-(Vector2<T> const& vec) const
{
    return Vector2(this->x - vec.x, this->y - vec.y);
}

template <class T>
inline void Vector2<T>::operator-=(Vector2<T> const& vec)
{
    this->x -= vec.x;
    this->y -= vec.y;
}

template <class T>
inline Vector2<T> Vector2<T>::operator*(const Vector2<T> &vec) const 
{
	return Vector2(x * vec.x, y * vec.y);
}

template <class T>
inline Vector2<T> Vector2<T>::operator*(const T &val) const 
{
	return Vector2(x * val, y * val);
}

template <class T>
inline void Vector2<T>::operator*=(const T &val) 
{
	x *= val;
	y *= val;
}

template <class T>
inline void Vector2<T>::operator*=(const Vector2<T> &val) 
{ 
    *this = *this * val; 
}

template <class T>
inline Vector2<T> Vector2<T>::operator/(const Vector2<T> &vec) const 
{
	return Vector2(x / vec.x, y / vec.y);
}

template <class T>
inline Vector2<T> Vector2<T>::operator/(const T &val) const 
{
	return Vector2(x / val, y / val);
}

template <class T>
inline void Vector2<T>::operator/=(const T &val) {
	x /= val;
	y /= val;
}

template <class T>
inline void Vector2<T>::operator/=(const Vector2<T> &vec)
{ 
    *this = *this / vec; 
}

template <class T>
inline Vector2<T> Vector2<T>::operator-() const 
{
	return Vector2(-x, -y);
}

template <class T>
inline bool Vector2<T>::operator==(const Vector2<T> &vec) const 
{
	return x == vec.x && y == vec.y;
}

template <class T>
inline bool Vector2<T>::operator!=(const Vector2<T> &vec) const 
{
	return x != vec.x || y != vec.y;
}

template <class T>
bool Vector2<T>::operator<(const Vector2<T> &vec) const 
{ 
    return x == vec.x ? (y < vec.y) : (x < vec.x); 
}

template <class T>
bool Vector2<T>::operator>(const Vector2<T> &vec) const 
{ 
    return x == vec.x ? (y > vec.y) : (x > vec.x); 
}

template <class T>
bool Vector2<T>::operator<=(const Vector2<T> &vec) const
{ 
    return x == vec.x ? (y <= vec.y) : (x < vec.x); 
}

template <class T>
bool Vector2<T>::operator>=(const Vector2<T> &vec) const 
{ 
    return x == vec.x ? (y >= vec.y) : (x > vec.x); 
}

template <class T>
inline Vector2<T> Vector2<T>::operator++()
{
    return Vector2(++x, ++y);
}

template <class T>
inline Vector2<T> Vector2<T>::operator++(int)
{
    return Vector2(x++, y++);
}

template <class T>
inline Vector2<T> Vector2<T>::operator--()
{
    return Vector2(--x, --y);
}

template <class T>
inline Vector2<T> Vector2<T>::operator--(int)
{
    return Vector2(x--, y--);
}

template <typename T>
const Vector2<T> Vector2<T>::ZERO = Vector2(0, 0);
template <typename T>
const Vector2<T> Vector2<T>::ONE = Vector2(1, 1);
template <typename T>
const Vector2<T> Vector2<T>::UP = Vector2(0, 1);
template <typename T>
const Vector2<T> Vector2<T>::DOWN = Vector2(0, -1);
template <typename T>
const Vector2<T> Vector2<T>::LEFT = Vector2(-1, 0);
template <typename T>
const Vector2<T> Vector2<T>::RIGHT = Vector2(1, 0);

typedef Vector2<float> Vector2f;
typedef Vector2<int>   Vector2i;

}

