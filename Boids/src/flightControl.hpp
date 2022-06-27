#pragma once

#include <vector>

#include "display.hpp"
#include "vector2.hpp"
#include "util.hpp"
#include "boid.hpp"

class FlightControl {
public:
	double cohesion = 350;
	double alignment = 250;
	double seperation = 16;
	int maxSpeed = 5;

	void Resize(const int width, const int height);
	void ChangePopulationSize(const int newCount);
	void AdvanceFrame();
	void Render(uint32_t* canvas);
	unsigned int GetBoidCount();

private:
	int m_Height = 0;
	int m_Width = 0;
	const std::vector<std::vector<uint8_t>> m_Colors = { {198, 46, 101}, {141, 169, 196}, {220, 133, 31}, {129, 244, 149} };
	std::vector<Boid> m_Boids;

	Vector2 Acceleration(const Boid& self);
	void Bound(Boid& boid);
};