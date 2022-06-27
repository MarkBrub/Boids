#include "Util.hpp"

std::random_device   RandomNumberGenerator::m_rd;
std::mt19937         RandomNumberGenerator::m_rng(RandomNumberGenerator::m_rd());

//between start and end
//incluseive of start but not end
double RandomNumberGenerator::getRandomNumber(const double& rangeStart, const double& rangeEnd) {
	std::uniform_real_distribution<> randomizer(rangeStart, rangeEnd);
	return randomizer(m_rng);
}

uint32_t Color::toUint32(const uint32_t red, const uint32_t green, const uint32_t blue) {
	uint32_t out = 0xFF000000;

	out |= blue << 16;
	out |= green << 8;
	out |= red;

	return out;
}
