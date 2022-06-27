#pragma once

#include "util.hpp"
#include "vector2.hpp"

struct Boid {
	int id = 0;
	Vector2 position;
	Vector2 velocity;
	std::vector<uint8_t> color = { 0, 0, 0 };
	Boid(const int maxHeight, const int maxWidth) {
		position.x = RandomNumberGenerator::getRandomNumber(maxWidth * .05, maxWidth * .95);
		position.y = RandomNumberGenerator::getRandomNumber(maxHeight * .05, maxHeight * .95);
		velocity.x = RandomNumberGenerator::getRandomNumber(-3, 3);
		velocity.y = RandomNumberGenerator::getRandomNumber(-3, 3);
	}

	auto operator<=>(const Boid&) const = default;

	auto operator==(const Boid& rhs) const {
		return id == rhs.id;
	}

	auto operator!=(const Boid& rhs) const {
		return !(*this == rhs);
	}
};