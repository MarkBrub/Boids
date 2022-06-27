#pragma once

#include <cmath>
#include <numbers>

struct Vector2 {
	double x = 0;
	double y = 0;

	Vector2() = default;
	Vector2(const Vector2& v) = default;
	Vector2(double a, double b) : x(a), y(b) {};

	double length() {
		return sqrt((x * x) + (y * y));
	}

	double angle() {
		return std::atan(x / y);
	}

	Vector2& operator+=(const Vector2& rhs) {
		x += rhs.x;
		y += rhs.y;
		return *this;
	}

	Vector2& operator-=(const Vector2& rhs) {
		x -= rhs.x;
		y -= rhs.y;
		return *this;
	}

	Vector2& operator*=(const double& rhs) {
		x *= rhs;
		y *= rhs;
		return *this;
	}

	Vector2& operator/=(const double& rhs) {
		x /= rhs;
		y /= rhs;
		return *this;
	}

	auto operator<=>(const Vector2&) const = default;

	friend Vector2 operator+(Vector2 lhs, const Vector2& rhs) {
		return lhs += rhs;
	}

	friend Vector2 operator-(Vector2 lhs, const Vector2& rhs) {
		return lhs -= rhs;
	}

	friend Vector2 operator/(Vector2 lhs, const double& rhs) {
		return lhs /= rhs;
	}

	friend Vector2 operator*(Vector2 lhs, const double& rhs) {
		return lhs *= rhs;
	}

	friend double dist(const Vector2& lhs, const Vector2& rhs) {
		return sqrt(((lhs.x - rhs.x) * (lhs.x - rhs.x)) + ((lhs.y - rhs.y) * (lhs.y - rhs.y)));
	}
};