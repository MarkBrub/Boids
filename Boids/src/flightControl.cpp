#include "flightControl.hpp"

/*

Vector2 acceleration(double seperation, double alignment, double cohesion, const Boid& self, const std::vector<Boid>& boids);
void bound(Boid& boid);

int NOTmain(int argc, char** args) {
	const std::vector<std::vector<uint8_t>> colors = { {198, 46, 101}, {141, 169, 196}, {220, 133, 31}, {129, 244, 149} };
	std::vector<Boid> boids(450);

	for (int x = 0; x < boids.size(); x++) {
		boids[x].id = x;
		boids[x].color = colors[x % 4];
	}

	//boids[0].color = { 237, 255, 0 };

	const int WIDTH = 1280;
	const int HEIGHT = 720;
	double cohesion = 16;
	double alignment = 4;
	double seperation = 250;
	int maxSpeed = 5;
	Renderer renderer(WIDTH, HEIGHT);

	int xInput = 0;
	int yInput = 0;

	bool running = true;

	while (running) {
		renderer.frameStart();

		//Boids update
		renderer.drawBoids(boids);

		for (auto& boid : boids) {

			//SDL_GetMouseState(&xInput, &yInput);
			//cohesion = ((double)xInput / WIDTH * 60) + .01;
			//alignment = ((double)yInput / HEIGHT * 120) + .01;
			
			boid.velocity += acceleration(cohesion, seperation, seperation, boid, boids);
			if (boid.velocity.length() > maxSpeed) {
				boid.velocity = (boid.velocity / boid.velocity.length()) / (static_cast<double>(1) / maxSpeed);
			}

			boid.position += boid.velocity;
			bound(boid);
		}

		renderer.frameEnd(running);
	}

	renderer.kill();

	return 0;
}

Vector2 acceleration(double seperation, double alignment, double cohesion, const Boid& self, const std::vector<Boid>& boids) {
	Vector2 seperationVec;
	Vector2 alignmentVec;
	Vector2 cohesionVec;
	Vector2 trace;

	int closeBoids = 0;
	int reallyCloseBoids = 0;

	if (false && self.id % 4 == 1) {
		// Follow cursor
		int cursorX = 0;
		int cursorY = 0;
		SDL_GetMouseState(&cursorX, &cursorY);
		trace = (Vector2(cursorX, cursorY) - self.position) / 250;
	}

	for (auto& boid : boids) {
		// For all other boids in perception range
		if (boid != self && dist(boid.position, self.position) < 100) {
			if (dist(boid.position, self.position) < 20) {
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
	}
	cohesionVec -= self.position;
	cohesionVec /= cohesion;

	return seperationVec + alignmentVec + cohesionVec + trace;
}

void bound(Boid& boid) {
	int xmin = 0;
	int xmax = 1280;
	int ymin = 0;
	int ymax = 720;

	Vector2 vec;

	if (boid.position.x < xmin) {
		boid.position.x = xmax;
	}
	else if (boid.position.x > xmax) {
		boid.position.x = 0;
	}

	if (boid.position.y < ymin) {
		boid.position.y = ymax;
	}
	else if (boid.position.y > ymax) {
		boid.position.y = 0;
	}
}

*/

void FlightControl::Resize(const int width, const int height) {
	m_Width = width;
	m_Height = height;
}

void FlightControl::ChangePopulationSize(const int newCount) {
	if (newCount > m_Boids.size()) {
		for (int x = m_Boids.size(); x < newCount; x++) {
			m_Boids.emplace_back(m_Height, m_Width);
			m_Boids[x].id = x;
			m_Boids[x].color = m_Colors[x % 4];
		}
	} else {
		m_Boids.erase(m_Boids.begin() + newCount, m_Boids.end());
	}

}

void FlightControl::AdvanceFrame() {
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

	for (auto& boid : m_Boids) {
		// For all other boids in perception range
		if (boid != self && dist(boid.position, self.position) < 100) {
			if (dist(boid.position, self.position) < 20) {
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
