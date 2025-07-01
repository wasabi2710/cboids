#include <stdlib.h>
#include <math.h>
#include <raylib.h>
#include <stdio.h>
#include <time.h>

//boids rules: cohesion, alignment, separation
//without applying, birds are stationary

#define WIDTH 1000
#define HEIGHT 500
#define MAX_BIRDS 10
#define SPEED 100.f

typedef struct {
    Vector2 startPos, endPos; //vectors for center masses
    Vector2 cPos, dPos;       //perpendicular points
} Bird;

//isosceles productions
void isoProductions(Bird birds[MAX_BIRDS], Vector2 mousePos, float dt) {
    for (int i = 0; i < MAX_BIRDS; i++) {
        Bird* bird = &birds[i];

        float dx = mousePos.x - bird->endPos.x;
        float dy = mousePos.y - bird->endPos.y;
        float distance = sqrtf(dx*dx + dy*dy);

        if (distance > 0.1f) {
            float uv_x = dx / distance;
            float uv_y = dy / distance;
            bird->endPos.x += uv_x * 20 * dt;
            bird->endPos.y += uv_y * 20 * dt;

            float rad = atan2f(dy, dx);
            float lineLength = 50.0f;
            bird->startPos.x = bird->endPos.x + lineLength * cosf(rad);
            bird->startPos.y = bird->endPos.y + lineLength * sinf(rad);
        }

        float ux = bird->startPos.x - bird->endPos.x;
        float uy = bird->startPos.y - bird->endPos.y;
        float len = sqrtf(ux*ux + uy*uy);

        if (len > 0.001f) {
            float scale = 10.0f / len;
            bird->cPos.x = bird->endPos.x - uy * scale;
            bird->cPos.y = bird->endPos.y + ux * scale;
            bird->dPos.x = bird->endPos.x + uy * scale;
            bird->dPos.y = bird->endPos.y - ux * scale;
        }
    }
}

//iso renderer
void drawBirdLines(const Bird* bird) {
    DrawLineV(bird->startPos, bird->cPos, RAYWHITE);
    DrawLineV(bird->startPos, bird->dPos, RAYWHITE);
    DrawLineV(bird->cPos, bird->dPos, RAYWHITE);
}

//todo:
//factorized the birds?? 
//wtf
int main() {
    InitWindow(WIDTH,HEIGHT, "CBOIDS");
    SetTargetFPS(60);

    //endPos randomizer
    srand(time(NULL));
    Bird birds[10] = {0};
    for (int i = 0; i < 10; i++) {
        int randWidth = rand() / ((RAND_MAX + 1u) / 900);
        int randHeight = rand() / ((RAND_MAX + 1u) / 400); 
        birds[i].endPos.x = randWidth;
        birds[i].endPos.y = randHeight;
    }

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        Vector2 mousePos = GetMousePosition();

        isoProductions(birds, mousePos, dt);
        
        BeginDrawing();
        ClearBackground(BLACK);

        for (int i = 0; i < MAX_BIRDS; i++) {
            drawBirdLines(&birds[i]);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
