#pragma once

#include <cmath>
#include <string>

namespace arb {

/**
 * @brief Represents a two-dimensional vector.
 *
 * The Vector2 class is a template, allowing the use of various numeric types
 * (e.g., int, float, double) for the vector's coordinates.
 * It includes arithmetic operators and helper methods for vector manipulation.
 *
 * @tparam T The numeric type of the vector's coordinates (must be an arithmetic type).
 */
template <typename T> class Vector2
{
    static_assert(std::is_arithmetic<T>::value, "arb::Vector2<type> must be numeric");

public:
    /**
     * @brief Default constructor.
     *
     * Initializes the vector with zero coordinates (0, 0).
     */
    inline constexpr Vector2();
    /**
     * @brief Constructor initializing the vector with specified coordinates.
     *
     * @param x The X-coordinate of the vector.
     * @param y The Y-coordinate of the vector.
     */
    inline constexpr Vector2(T x, T y);
    /**
     * @brief Conversion constructor from another Vector2 type.
     *
     * Allows conversion of a vector from one numeric type to another
     * (e.g., Vector2<float> to Vector2<int>).
     * @tparam U The numeric type of the other vector.
     * @param other The other vector to convert from.
     */
    template <typename U>
    inline constexpr explicit Vector2(const Vector2<U>& other);

    /**
     * @brief Calculates the length (magnitude) of the vector.
     * @return The length of the vector.
     */
    inline auto length() const;
    /**
     * @brief Calculates the squared length (magnitude squared) of the vector.
     *
     * This method is often used instead of length() for comparisons
     * to avoid expensive square root operations.
     * @return The squared length of the vector.
     */
    inline constexpr T length_squared() const;
    /**
     * @brief Returns the normalized vector (unit vector).
     *
     * A unit vector has a length of 1 and the same direction as the original vector.
     * If the vector has zero length, a zero vector is returned.
     * @return The normalized vector.
     */
    inline Vector2<T> normalized() const;

    /**
     * @brief Calculates the dot product with second vector.
     * @param second The second vector.
     * @return The dot product.
     */
    inline constexpr T dot(const Vector2& second) const;

    /**
     * @brief Calculates the cross product with second vector.
     *
     * For 2D vectors, the cross product is a scalar value representing
     * the 'depth' (Z) of the resulting vector if the operation were in 3D
     * on the XY plane. It can be used to determine orientation.
     * @param second The second vector.
     * @return The cross product (scalar).
     */
    inline constexpr T cross(const Vector2& second) const;

    /**
     * @brief Converts the vector to a string representation.
     * @return A string representing the vector (e.g., "(1.0, 2.0)").
     */
    inline std::string to_string() const;

    /**
     * @brief Conversion operator to another Vector2 type.
     *
     * Allows implicit conversions between different Vector2 types.
     * @tparam U The numeric type of the target vector.
     * @return A new vector converted to type U.
     */
    template<class U>
    inline constexpr operator Vector2<U>() const;
    /**
     * @brief Vector addition operator.
     * @param second The second vector to add.
     * @return A new vector representing the sum of the two vectors.
     */
    inline constexpr Vector2<T> operator+ (const Vector2<T> &second) const;
    /**
     * @brief Vector subtraction operator.
     * @param second The second vector to subtract.
     * @return A new vector representing the difference of the two vectors.
     */
    inline constexpr Vector2<T> operator- (const Vector2<T> &second) const;
    /**
     * @brief Vector multiplication operator (component-wise).
     * @param second The second vector to multiply by.
     * @return A new vector with component-wise multiplication results.
     */
    inline constexpr Vector2<T> operator* (const Vector2<T> &second) const;
    /**
     * @brief Vector division operator (component-wise).
     * @param second The second vector to divide by.
     * @return A new vector with component-wise division results.
     */
    inline constexpr Vector2<T> operator/ (const Vector2<T> &second) const;
    /**
     * @brief Unary negation operator (reverses the vector's direction).
     * @return A new vector with negated coordinates.
     */
    inline constexpr Vector2<T> operator-() const;
    /**
     * @brief Scalar multiplication operator (vector * scalar).
     * @param value The scalar value.
     * @return A new vector with scaled coordinates.
     */
    inline constexpr Vector2<T> operator* (const T &value) const;
    /**
     * @brief Scalar division operator (vector / scalar).
     * @param value The scalar value.
     * @return A new vector with divided coordinates.
     */
    inline constexpr Vector2<T> operator/ (const T &value) const;
    /**
     * @brief Compound assignment operator for addition.
     * @param second The vector to add.
     * @return A reference to the modified vector.
     */
    inline constexpr Vector2<T>& operator+= (const Vector2<T>& second);
    /**
     * @brief Compound assignment operator for subtraction.
     * @param second The vector to subtract.
     * @return A reference to the modified vector.
     */
    inline constexpr Vector2<T>& operator-= (const Vector2<T>& second);
    /**
     * @brief Compound assignment operator for multiplication (component-wise).
     * @param second The vector to multiply by.
     * @return A reference to the modified vector.
     */
    inline constexpr Vector2<T>& operator*= (const Vector2<T>& second);
    /**
     * @brief Compound assignment operator for division (component-wise).
     * @param second The vector to divide by.
     * @return A reference to the modified vector.
     */
    inline constexpr Vector2<T>& operator/= (const Vector2<T>& second);
    /**
     * @brief Compound assignment operator for scalar multiplication.
     * @param value The scalar value.
     * @return A reference to the modified vector.
     */
    inline constexpr Vector2<T>& operator*= (const T& value);
    /**
     * @brief Compound assignment operator for scalar division.
     * @param value The scalar value.
     * @return A reference to the modified vector.
     */
    inline constexpr Vector2<T>& operator/= (const T& value);
    /**
     * @brief Equality comparison operator for vectors.
     *
     * @param second The other vector to compare with.
     * @return True if the vectors are equal, false otherwise.
     */
    inline constexpr bool operator == (const Vector2<T>& second) const;
    /**
     * @brief Inequality comparison operator for vectors.
     * @param second The other vector to compare with.
     * @return True if the vectors are not equal, false otherwise.
     */
    inline constexpr bool operator != (const Vector2<T>& second) const;

    /**
     * @brief Static constant zero vector (0,0).
     */
    static constexpr Vector2<T> ZERO{T{0}, T{0}};
    /**
     * @brief Static constant one vector (1,1).
     */
    static constexpr Vector2<T> ONE{T(1), T(1)};
    /**
     * @brief Static constant left direction vector (-1,0).
     */
    static constexpr Vector2<T> LEFT{T(-1), T(0)};
    /**
     * @brief Static constant right direction vector (1,0).
     */
    static constexpr Vector2<T> RIGHT{T(1), T(0)};
    /**
     * @brief Static constant up direction vector (0,-1).
     */
    static constexpr Vector2<T> UP{T(0), T(-1)};
    /**
     * @brief Static constant down direction vector (0,1).
     */
    static constexpr Vector2<T> DOWN{T(0), T(1)};

    /**
     * @brief The X-coordinate of the vector.
     */
    T x;
    /**
     * @brief The X-coordinate of the vector.
     */
    T y;
};

template <typename T>
constexpr Vector2<T>::Vector2() : x{0}, y{0} {};

template <typename T>
constexpr Vector2<T>::Vector2(T x, T y) : x{x}, y{y} {};

template <typename T>
template <typename U>
inline constexpr Vector2<T>::Vector2(const Vector2<U>& other) : x{static_cast<T>(other.x)}, y{static_cast<T>(other.y)} {};



template <typename T>
inline auto Vector2<T>::length() const
{
    // TO DO: if fixed point added and vector type is fixed cast to fixed point
    //if constexpr (std::is_same_v<T, char>) 
    return static_cast<float>(std::sqrtf(x * x + y * y));
}

template <typename T>
inline constexpr T Vector2<T>::length_squared() const
{
    return (x * x + y * y);
}

template <typename T>
inline Vector2<T> Vector2<T>::normalized() const
{
    if (x == 0 && y == 0) return Vector2<T>::ZERO;

    auto il = 1 / length();
    return Vector2(x * il, y * il);
}

template <typename T>
inline constexpr T Vector2<T>::dot(const Vector2& second) const
{
    return x * second.x + y * second.y;
}

template <typename T>
inline constexpr T Vector2<T>::cross(const Vector2& second) const
{
    return x * second.y - y * second.x;
}

template <typename T>
inline std::string Vector2<T>::to_string() const
{
    return std::string("(") + std::to_string(x) + ", " + std::to_string(y) + ")";
}

template <typename T>
template<class U>
inline constexpr Vector2<T>::operator Vector2<U>() const
{
    return { static_cast<U>(x), static_cast<U>(y) };
}

template <typename T>
inline constexpr Vector2<T> Vector2<T>::operator+ (const Vector2<T> &second) const
{
    return Vector2<T>(x + second.x, y + second.y);
}

template <typename T>
inline constexpr Vector2<T> Vector2<T>::operator- (const Vector2<T> &second) const
{
    return Vector2<T>(x - second.x, y - second.y);
}

template <typename T>
inline constexpr Vector2<T> Vector2<T>::operator* (const Vector2<T> &second) const
{
    return Vector2<T>(x * second.x, y * second.y);
}

template <typename T>
inline constexpr Vector2<T> Vector2<T>::operator/ (const Vector2<T> &second) const
{
    return Vector2<T>(x / second.x, y / second.y);
}

template <typename T>
inline constexpr Vector2<T> Vector2<T>::operator-() const
{
    return Vector2<T>(-x, -y);
}

template <typename T>
inline constexpr Vector2<T> Vector2<T>::operator* (const T &value) const
{
    return Vector2<T>(x * value, y * value);
}

template <typename T>
inline constexpr Vector2<T> Vector2<T>::operator/ (const T &value) const
{
    return Vector2<T>(x / value, y / value);
}

template <typename T>
inline constexpr Vector2<T>& Vector2<T>::operator+= (const Vector2<T>& second)
{
    x += second.x;
    y += second.y;

    return *this;
}

template <typename T>
inline constexpr Vector2<T>& Vector2<T>::operator-= (const Vector2<T>& second)
{
    x -= second.x;
    y -= second.y;

    return *this;
}

template <typename T>
inline constexpr Vector2<T>& Vector2<T>::operator*= (const Vector2<T>& second)
{
    x *= second.x;
    y *= second.y;

    return *this;
}

template <typename T>
inline constexpr Vector2<T>& Vector2<T>::operator/= (const Vector2<T>& second)
{
    x /= second.x;
    y /= second.y;

    return *this;
}

template <typename T>
inline constexpr Vector2<T>& Vector2<T>::operator*= (const T& value)
{
    x *= value;
    y *= value;

    return *this;
}

template <typename T>
inline constexpr Vector2<T>& Vector2<T>::operator/= (const T& value)
{
    x /= value;
    y /= value;

    return *this;
}

template <typename T>
inline constexpr bool Vector2<T>::operator == (const Vector2<T>& second) const
{
    return (x == second.x && y == second.y);
}

template <typename T>
inline constexpr bool Vector2<T>::operator != (const Vector2<T>& second) const
{
    return (x != second.x || y != second.y);
}

/**
 * @brief Alias for Vector2 with int type.
 */
using Vector2i = Vector2<int>;
/**
 * @brief Alias for Vector2 with uint32_t type.
 */
using Vector2u = Vector2<uint32_t>;
/**
 * @brief Alias for Vector2 with float type.
 */
using Vector2f = Vector2<float>;
/**
 * @brief Alias for Vector2 with double type.
 *
 * This type due to double software emulation should be avoided at all costs
 */
using Vector2d = Vector2<double>; 

} // namespace arb