#include "vector2.hpp"

namespace airboy 
{
    template <class T>
    Vector2<T>::Vector2() {}

    template <class T>
    Vector2<T>::Vector2(T x, T y)
    {
        this->x = x;
        this->y = y;
    }

    template <class T>
    Vector2<T> Vector2<T>::abs() const
    {
        Vector2<T> out = Vector2(std::abs(this->x), std::abs(this->y));
        return out;
    }

    template <class T>
    float Vector2<T>::lenght() const
    {
        return std::sqrt(x * x + y * y);
    }

    template <class T>
    void Vector2<T>::normalize()
    {
        T l = x * x + y * y;
        if (l != 0) 
        {
            l = std::sqrt(l);
            x /= l;
            y /= l;
        }
    }

    template class Vector2<float>;
    template class Vector2<int>;
}
