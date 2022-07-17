#include "flightControl.hpp"

void FlightControl::Resize(const int width, const int height) {
	m_Width = width;
	m_Height = height;
}

void FlightControl::ChangePopulationSize(const int newCount) {
	if (newCount > m_Boids.size()) {
		for (int x = m_Boids.size(); x < newCount; x++) {
			m_Boids.emplace_back(m_Height, m_Width);
			m_Boids[x].id = x;
			m_Boids[x].color = Util::toUint32(m_Colors[x % 4]);
		}
	} else {
		m_Boids.erase(m_Boids.begin() + newCount, m_Boids.end());
	}

}

void FlightControl::AdvanceFrame() {
	// Updated cached values
	m_VisionRadiusSquared = visionRadius * visionRadius;
	m_AvoidanceRadiusSquared = avoidanceRadius * avoidanceRadius;
	m_fieldOfViewRange = (360 - fieldOfView) / 180 - 1; // Convert field of view to a [-1, 1] range

	for (auto& boid : m_Boids) {

		boid.velocity += Acceleration(boid);
		if (boid.velocity.length() > maxSpeed) {
			boid.velocity = (boid.velocity / boid.velocity.length()) / (static_cast<double>(1) / maxSpeed);
		}

		boid.position += boid.velocity;
		Bound(boid);
	}
}

void FlightControl::Render(uint32_t* canvas) {
	for (const auto& boid : m_Boids) {
		Vector2 top;
		Vector2 left;
		Vector2 right;

		double angle = boid.velocity.angle();
		if (boid.velocity.y >= 0) {
			angle += std::numbers::pi;
		}

		top.x = m_BasePoints[0].x * std::cos(angle) - m_BasePoints[0].y * std::sin(angle);
		top.y = m_BasePoints[0].y * std::cos(angle) + m_BasePoints[0].x * std::sin(angle);
		left.x = m_BasePoints[1].x * std::cos(angle) - m_BasePoints[1].y * std::sin(angle);
		left.y = m_BasePoints[1].y * std::cos(angle) + m_BasePoints[1].x * std::sin(angle);
		right.x = m_BasePoints[2].x * std::cos(angle) - m_BasePoints[2].y * std::sin(angle);
		right.y = m_BasePoints[2].y * std::cos(angle) + m_BasePoints[2].x * std::sin(angle);

		top *= scale;
		left *= scale;
		right *= scale;

		top.x *= -1;
		left.x *= -1;
		right.x *= -1;

		top += boid.position;
		left += boid.position;
		right += boid.position;

		auto xPair = std::minmax({ top.x, left.x, right.x });
		auto yPair = std::minmax({ top.y, left.y, right.y });

		int32_t position = 0;

		for (int y = yPair.first; y < yPair.second; y++) {
			for (int x = xPair.first; x < xPair.second; x++) {
				if (!inTriangle(Vector2(x, y), top, left, right)) continue;
				position = x + (m_Width * y);
				if (position >= m_Width * m_Height) position -= (m_Width * m_Height);
				if (position <= 0) position += (m_Width * m_Height);
				canvas[position] = boid.color;
			}
		}

		/*
		uint32_t position = (int)boid.position.x + (m_Width * (int)boid.position.y);

		if (position >= m_Width * m_Height - m_Width - 1 || position <= m_Width + 1) continue;

		canvas[position - 1 - m_Width] = Color::toUint32(boid.color[0], boid.color[1], boid.color[2]);
		canvas[position - m_Width] = Color::toUint32(boid.color[0], boid.color[1], boid.color[2]);
		canvas[position + 1 - m_Width] = Color::toUint32(boid.color[0], boid.color[1], boid.color[2]);
		canvas[position - 1] = Color::toUint32(boid.color[0], boid.color[1], boid.color[2]);
		canvas[position] = Color::toUint32(boid.color[0], boid.color[1], boid.color[2]);
		canvas[position + 1] = Color::toUint32(boid.color[0], boid.color[1], boid.color[2]);
		canvas[position - 1 + m_Width] = Color::toUint32(boid.color[0], boid.color[1], boid.color[2]);
		canvas[position + m_Width] = Color::toUint32(boid.color[0], boid.color[1], boid.color[2]);
		canvas[position + 1 + m_Width] = Color::toUint32(boid.color[0], boid.color[1], boid.color[2]);
		*/
	}
}

unsigned int FlightControl::GetBoidCount() {
	return m_Boids.size();
}

Vector2 FlightControl::Acceleration(const Boid& self) {
	Vector2 seperationVec;
	Vector2 alignmentVec;
	Vector2 cohesionVec;
	Vector2 trace;

	int closeBoids = 0;
	int reallyCloseBoids = 0;

	for (const auto& boid : m_Boids) {
		// For all other boids in perception range
		if (boid != self && distSquared(boid.position, self.position) < m_VisionRadiusSquared) {
			if ((boid.position - self.position).normalize() * self.velocity.normalize() < m_fieldOfViewRange) continue;
			if (distSquared(boid.position, self.position) < m_AvoidanceRadiusSquared) {
				seperationVec -= boid.position - self.position;
				reallyCloseBoids++;
			}

			if (self.id % 4 != boid.id % 4) continue;
			//if (self.id == 0) continue;

			alignmentVec += boid.velocity;
			cohesionVec += boid.position;
			closeBoids++;
		}
	}

	if (closeBoids > 0) {
		if (reallyCloseBoids > 0) seperationVec /= reallyCloseBoids * seperation;
		alignmentVec -= self.velocity;
		alignmentVec /= closeBoids * alignment;
		cohesionVec /= closeBoids;
		cohesionVec -= self.position;
		cohesionVec /= cohesion;
	}

	return seperationVec + alignmentVec + cohesionVec + trace;
}

void FlightControl::Bound(Boid& boid) {
	if (boid.position.x < 0) {
		boid.position.x = m_Width;
	} else if (boid.position.x > m_Width) {
		boid.position.x = 0;
	}

	if (boid.position.y < 0) {
		boid.position.y = m_Height;
	} else if (boid.position.y > m_Height) {
		boid.position.y = 0;
	}
}
