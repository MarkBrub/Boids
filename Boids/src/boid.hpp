#pragma once

#include "util.hpp"
#include "vector2.hpp"

struct Boid {
	int id = 0;
	Vector2 position;
	Vector2 velocity;
	uint32_t color = 0;
	Boid(const int maxHeight, const int maxWidth) {
		position.x = Util::getRandomNumber(maxWidth * .05, maxWidth * .95);
		position.y = Util::getRandomNumber(maxHeight * .05, maxHeight * .95);
		velocity.x = Util::getRandomNumber(-3, 3);
		velocity.y = Util::getRandomNumber(-3, 3);
	}

	auto operator<=>(const Boid&) const = default;

	auto operator==(const Boid& rhs) const {
		return id == rhs.id;
	}

	auto operator!=(const Boid& rhs) const {
		return !(*this == rhs);
	}
};