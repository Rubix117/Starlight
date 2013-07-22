#ifndef STARLIGHT_VECTOR2_H_
#define STARLIGHT_VECTOR2_H_

#include <ostream>

namespace starlight
{
	template<typename T>
	class Vector2_
	{
	public:
		explicit Vector2_<T>(T x = 0, T y = 0);
		~Vector2_<T>();

		const Vector2_<int> toV2() const;

		const Vector2_<float> toV2F() const;

		const bool operator==(const Vector2_<T> &rhs) const;

		const Vector2_<T> operator+(const Vector2_<T> &rhs) const;
		const Vector2_<T> operator-(const Vector2_<T> &rhs) const;
		const Vector2_<T> operator*(const Vector2_<T> &rhs) const;
		const Vector2_<T> operator/(const Vector2_<T> &rhs) const;

		const Vector2_<T> operator/(const int &rhs) const;
		const Vector2_<T> operator*(const int &rhs) const;
		const Vector2_<float> operator/(const float &rhs) const;
		const Vector2_<float> operator*(const float &rhs) const;

		Vector2_<T>& operator+=(const Vector2_<T> &rhs);
		Vector2_<T>& operator-=(const Vector2_<T> &rhs);
		Vector2_<T>& operator*=(const Vector2_<T> &rhs);

		const float DistanceTo(const Vector2_<T> &vecB) const;

		template<typename T>
		friend std::ostream& operator<<(std::ostream &out, const Vector2_<T> &rhs);

		T x, y;
	};
	typedef Vector2_<float> Vector2F;
	typedef Vector2_<int> Vector2;

	template<typename T>
	Vector2_<T>::Vector2_(T x, T y) :
		x(x), y(y)
	{
	}

	template<typename T>
	Vector2_<T>::~Vector2_()
	{
	}

	template<typename T>
	const Vector2_<int> Vector2_<T>::toV2() const
	{
		return Vector2_<int>(int(x), int(y));
	}

	template<typename T>
	const Vector2_<float> Vector2_<T>::toV2F() const
	{
		return Vector2_<float>(float(x), float(y));
	}

	template<typename T>
	const bool Vector2_<T>::operator==(const Vector2_<T> &rhs) const
	{
		return (x == rhs.x && y == rhs.y);
	}

	template<typename T>
	const Vector2_<T> Vector2_<T>::operator+(const Vector2_<T> &rhs) const
	{
		return Vector2_<T>(x + rhs.x, y + rhs.y);
	}

	template<typename T>
	const Vector2_<T> Vector2_<T>::operator-(const Vector2_<T> &rhs) const
	{
		return Vector2_<T>(x - rhs.x, y - rhs.y);
	}

	template<typename T>
	const Vector2_<T> Vector2_<T>::operator*(const Vector2_<T> &rhs) const
	{
		return Vector2_<T>(x * rhs.x, y * rhs.y);
	}

	template<typename T>
	const Vector2_<T> Vector2_<T>::operator/(const Vector2_<T> &rhs) const
	{
		return Vector2_<T>(x / rhs.x, y / rhs.y);
	}

	template<typename T>
	const Vector2_<T> Vector2_<T>::operator/(const int &rhs) const
	{
		return Vector2_<T>(x / rhs, y / rhs);
	}

	template<typename T>
	const Vector2_<T> Vector2_<T>::operator*(const int &rhs) const
	{
		return Vector2_<T>(x * rhs, y * rhs);
	}

	template<typename T>
	const Vector2_<float> Vector2_<T>::operator/(const float &rhs) const
	{
		return Vector2_<float>(x / rhs, y / rhs);
	}

	template<typename T>
	const Vector2_<float> Vector2_<T>::operator*(const float &rhs) const
	{
		return Vector2_<float>(x * rhs, y * rhs);
	}

	template<typename T>
	Vector2_<T>& Vector2_<T>::operator+=(const Vector2_<T> &rhs)
	{
		x += rhs.x;
		y += rhs.y;
		return *this;
	}

	template<typename T>
	Vector2_<T>& Vector2_<T>::operator-=(const Vector2_<T> &rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		return *this;
	}

	template<typename T>
	Vector2_<T>& Vector2_<T>::operator*=(const Vector2_<T> &rhs)
	{
		x *= rhs.x;
		y *= rhs.y;
		return *this;
	}

	template<typename T>
	const float Vector2_<T>::DistanceTo(const Vector2_<T> &vecB) const
	{
		float deltaX = x - vecB.x;
		float deltaY = y - vecB.y;

		return sqrt(deltaX*deltaX + deltaY*deltaY);
	}

	template<typename T>
	std::ostream& operator<<(std::ostream &out, const Vector2_<T> &rhs)
	{
		return out << rhs.x <<", "<< rhs.y;
	}
}

#endif // STARLIGHT_VECTOR2_H_