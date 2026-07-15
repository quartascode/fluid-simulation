#include <raylib.h>

struct Particle {
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
			
			BeginDrawing();
				DrawCircle(screenPos.x, screenPos.y, p.radius * scale, p.color);
			EndDrawing();
		}

	private:
		float scale;
};

int main(void) {
	InitWindow(600, 400, "sim");
	SetTargetFPS(60);

	Renderer renderer(100.0f);

	Particle part;
	part.position = {1, 1};
	part.color = BLUE;
	part.radius = 0.05;
	
	while (!WindowShouldClose()) {

		renderer.Draw(part);
	}

}
