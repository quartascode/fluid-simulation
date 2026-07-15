#include <cstddef>
#include <cstdlib>
#include <ctime>
#include <raylib.h>
#include <vector>

struct Particle {
	Particle() : radius(0.05), position({0, 0}), velocity({0, 0}), color(BLUE) {}

	Vector2 position;
	Vector2 velocity;

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

		Simulation(int pCount) : PARTICLE_COUNT(pCount) {
			for (int i = 0; i < PARTICLE_COUNT; i++) {
				Particle p;

				float x = (2 * ((float)rand() / RAND_MAX) - 1);
				float y = (2 * ((float)rand() / RAND_MAX) - 1);

				p.position = {x, y};

				particles.push_back(p);
			}
		}

		const std::vector<Particle>& GetParticles() const { return particles; }

		void HandleBorderCollision(float width, float height) {
			// bounding box centered at 0, 0

		}

	private:
		std::vector<Particle> particles;
};

int main(void) {
	srand(time(NULL));

	InitWindow(600, 400, "sim");
	SetTargetFPS(60);

	Renderer renderer(100.0f);

	Simulation sim(500);

	while (!WindowShouldClose()) {


		BeginDrawing();
			ClearBackground(BLACK);

			for (auto& p : sim.GetParticles()) {
				renderer.Draw(p);
			}
		EndDrawing();
	}
}
