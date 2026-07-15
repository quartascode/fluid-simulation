#include <raylib.h>

struct Particle {
	Vector2 position;
	Vector2 velocity;

	float radius;
};

int main(void) {
	InitWindow(600, 400, "sim");
	SetTargetFPS(60);
	
	while (!WindowShouldClose()) {
	
		BeginDrawing();
		EndDrawing();
	}

}
