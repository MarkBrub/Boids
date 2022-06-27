#pragma once

#include <random>
#include <numbers>


//https://stackoverflow.com/questions/32071721/error-in-using-mt19937-with-random-device-as-static-class-members
struct RandomNumberGenerator {
	static std::random_device   m_rd;
	static std::mt19937         m_rng;
public:
	static double getRandomNumber(const double& rangeStart, const double& rangeEnd);
};

struct Color {
	static uint32_t toUint32(const uint32_t red, const uint32_t green, const uint32_t blue);
};