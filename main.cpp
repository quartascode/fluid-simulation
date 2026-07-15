#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <ctime>
#include <raylib.h>
#include <raymath.h>
#include <vector>

struct Particle {
	Particle() : radius(0.05), position({0, 0}), velocity({0, 0}), acceleration({0, 0}), collisionDamping(0.8), color(BLUE) {}

	Vector2 position;
	Vector2 velocity;
	Vector2 acceleration;

	float density;

	float collisionDamping;

	float radius;

	Color color;
};

class Renderer {
	public:
		Renderer(float scale) : scale(scale) {};

		Vector2 WorldToScreen(const Vector2& v) {
			return {v.x * scale + GetScreenWidth() / 2.0f, -v.y * scale + GetScreenHeight() / 2.0f};
		}

		void Draw(const Particle& p) {
			Vector2 screenPos = WorldToScreen(p.position);
			
			DrawCircle(screenPos.x, screenPos.y, p.radius * scale, p.color);
		}

	private:
		float scale;
};

class Simulation {
	public:
		const int PARTICLE_COUNT;

		Simulation(int pCount, float smoothingRadius, float targetDensity, Vector2 bounds) : PARTICLE_COUNT(pCount), smoothingRadius(smoothingRadius), targetDensity(targetDensity), bounds(bounds) {
			for (int i = 0; i < PARTICLE_COUNT; i++) {
				Particle p;

				float x = (2 * ((float)rand() / RAND_MAX) - 1) * (bounds.x - 1.0f) * 0.5f;
				float y = (2 * ((float)rand() / RAND_MAX) - 1) * (bounds.y - 1.0f) * 0.5f;

				p.position = {x, y};

				particles.push_back(p);
			}
		}

		const std::vector<Particle>& GetParticles() const { return particles; }

		void Update(float dt) {
			for (auto& p : particles) {
				Integrate(p, dt);
				HandleBorderCollision(p);
				p.density = CalculateDensity(p.position);
			}
		}

	private:
		std::vector<Particle> particles;

		Vector2 bounds;

		float smoothingRadius;
		float targetDensity;

		void Integrate(Particle& p, float dt) {
			p.acceleration = {0, -9.81};

			p.velocity += p.acceleration * dt;
			p.position += p.velocity * dt;
		}

		void HandleBorderCollision(Particle& p) {
			// bounding box centered at 0, 0
			float r = p.radius;
			
			float right  = bounds.x / 2;
			float left   = -right;
			float top    = bounds.y / 2;
			float bottom = -top;

			if (p.position.x + r >= right) {
				p.position.x = right - r;		
				p.velocity.x = -p.velocity.x * p.collisionDamping;
			}
			if (p.position.x - r <= left) {
				p.position.x = left + r;		
				p.velocity.x = -p.velocity.x * p.collisionDamping;
			}
			if (p.position.y + r >= top) {
				p.position.y = top - r;
				p.velocity.y = -p.velocity.y * p.collisionDamping;
			}
			if (p.position.y - r <= bottom) {
				p.position.y = bottom + r;
				p.velocity.y = -p.velocity.y * p.collisionDamping;
			}
		}

		float SmoothingKernel(float radius, float dst) {
			float volume = PI * pow(radius, 8) / 4.0f;

			float value = std::max(0.0f, radius * radius - dst * dst);

			return value * value * value / volume;
		}

		float SmoothingKernelDerivative(float radius, float dst) {
			if (dst >= radius) { return 0; }
			float f = radius * radius - dst * dst;
			float scale = -24 / (PI * pow(radius, 8));
			return scale * dst * f * f;
		}

		float CalculateDensity(Vector2 samplePoint) {
			float density = 0;

			for (auto& p : particles) {
				float dst = Vector2Length((p.position - samplePoint));
				float influence = SmoothingKernel(smoothingRadius, dst);
				density += influence;
			}
			return density;
		}

		float ConvertDensityToPressure(float density) {
			float densityError = density - targetDensity;
			float pressure = densityError * 5;
			return pressure;
		}

		Vector2 CalculatePressureForce(Vector2 samplePoint) {
			Vector2 pressureForce {};

			for (auto& p : particles) {
				float dst = Vector2Length((p.position - samplePoint));
				Vector2 dir = (p.position - samplePoint) / dst;
				float slope = SmoothingKernelDerivative(smoothingRadius, dst);
				float influence = SmoothingKernel(smoothingRadius, dst);
				pressureForce += dir * -ConvertDensityToPressure(p.density) * slope / p.density;
			}
			return pressureForce;
		}
};

int main(void) {
	srand(time(NULL));

	InitWindow(600, 400, "sim");
	SetTargetFPS(180);

	Renderer renderer(100.0f);

	Simulation sim(2000, 1.5f, 10.0f, {6.0f, 4.0f});

	while (!WindowShouldClose()) {
		sim.Update(GetFrameTime());

		BeginDrawing();
		ClearBackground(BLACK);

		for (auto& p : sim.GetParticles()) {
			renderer.Draw(p);
		}
		EndDrawing();
	}
}
