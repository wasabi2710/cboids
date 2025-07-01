#include <math.h>
#include <raylib.h>
#include <stdio.h>

typedef struct {
    Vector2 startPos, endPos; //vectors for center masses
    Vector2 cPos, dPos;       //perpendicular points
} Bird;

int main() {
    InitWindow(1000, 500, "CBOIDS");
    SetTargetFPS(60);

    Bird bird = { .startPos = {0, 0}, .endPos = {400, 200} };

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        Vector2 mousePos = GetMousePosition();

        float dx = mousePos.x - bird.endPos.x;
        float dy = mousePos.y - bird.endPos.y;
        float distance = sqrtf(dx*dx + dy*dy);

        if (distance > 0.1f) {
            float uv_x = dx / distance;
            float uv_y = dy / distance; 

            bird.endPos.x += uv_x * 20 * dt;
            bird.endPos.y += uv_y * 20 * dt;

            float rad = atan2f(dy, dx);
            float lineLength = 50.0f;
            bird.startPos.x = bird.endPos.x + lineLength * cosf(rad);
            bird.startPos.y = bird.endPos.y + lineLength * sinf(rad);
        }

        float ux = bird.startPos.x - bird.endPos.x;
        float uy = bird.startPos.y - bird.endPos.y;
        float len = sqrtf(ux*ux + uy*uy);

        if (len > 0.001f) {
            float scale = 10.0f / len;
            bird.cPos.x = bird.endPos.x - uy * scale;
            bird.cPos.y = bird.endPos.y + ux * scale;

            bird.dPos.x = bird.endPos.x + uy * scale;
            bird.dPos.y = bird.endPos.y - ux * scale;
        }

        BeginDrawing();
        ClearBackground(BLACK);

        DrawLineV(bird.startPos, bird.cPos, RAYWHITE);
        DrawLineV(bird.startPos, bird.dPos, RAYWHITE);
        DrawLineV(bird.cPos, bird.dPos, RAYWHITE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
