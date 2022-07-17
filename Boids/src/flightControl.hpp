#pragma once

#include <vector>

#include "display.hpp"
#include "vector2.hpp"
#include "util.hpp"
#include "boid.hpp"

class FlightControl {
public:
	int cohesion = 350;
	int alignment = 250;
	int seperation = 16;
	int maxSpeed = 5;
	int visionRadius = 100;
	int avoidanceRadius = 20;
	int fieldOfView = 270;
	double scale = 1;

	void Resize(const int width, const int height);
	void ChangePopulationSize(const int newCount);
	void AdvanceFrame();
	void Render(uint32_t* canvas);
	unsigned int GetBoidCount();

private:
	int m_Height = 0;
	int m_Width = 0;
	const std::vector<std::vector<uint32_t>> m_Colors = { {255, 255, 255}, {141, 169, 196}, {220, 133, 31}, {129, 244, 149} };
	// Order: Top, Bottom left, Bottom right
	const std::vector<Vector2> m_BasePoints = { {0, -6}, {-3, 6}, {3, 6} };
	std::vector<Boid> m_Boids;

	Vector2 Acceleration(const Boid& self);
	void Bound(Boid& boid);
};