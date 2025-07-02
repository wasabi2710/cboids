#include <stdlib.h>
#include <math.h>
#include <raylib.h>
#include <stdio.h>
#include <time.h>

//rules applications
//cohesion, separation, alignment

#define WIDTH 1600
#define HEIGHT 900

#define MAX_BIRDS 200
#define MAX_SPEED 600.0f
#define MAX_FORCE 20.0f

#define COHESION_WEIGHT 0.5f
#define SEPARATION_WEIGHT 0.5f
#define ALIGNMENT_WEIGHT 0.3f

#define SEPARATION_DIST 50.f
#define ALIGNMENT_DIST 100.f
#define COHESION_DIST 150.f

typedef struct {
    Vector2 startPos, endPos; 
    Vector2 cPos, dPos;       
    Vector2 velocity;
} Bird;

//vector2 helpers
float vecLength(Vector2 v) {
    return sqrtf(v.x * v.x + v.y * v.y);
}
Vector2 vecNormalize(Vector2 v) {
    float len = vecLength(v);
    if (len > 0.0001f) {
        return (Vector2){ v.x / len, v.y / len };
    }
    return (Vector2){0,0};
}
Vector2 vecLimit(Vector2 v, float max) {
    float len = vecLength(v);
    if (len > max) {
        float scale = max / len;
        return (Vector2){v.x * scale, v.y * scale};
    }
    return v;
}

//rules rule: 1flock, rest
//cohesion
void cohesion(Bird birds[MAX_BIRDS], Bird *mbird) {
    Vector2 center = {0, 0};
    int count = 0;

    for (int i = 0; i < MAX_BIRDS; i++) {
        if (&birds[i] != mbird) {
            float dx = birds[i].endPos.x - mbird->endPos.x;
            float dy = birds[i].endPos.y - mbird->endPos.y;
            float dist = sqrtf(dx*dx + dy*dy);
            
            if (dist > 0 && dist < COHESION_DIST) {
                center.x += birds[i].endPos.x;
                center.y += birds[i].endPos.y;
                count++;
            }
        }
    }

    if (count > 0) {
        center.x /= count;
        center.y /= count;

        Vector2 desired = (Vector2){ center.x - mbird->endPos.x, center.y - mbird->endPos.y };
        desired = vecNormalize(desired);
        desired.x *= MAX_SPEED;
        desired.y *= MAX_SPEED;

        Vector2 steer = (Vector2){ desired.x - mbird->velocity.x, desired.y - mbird->velocity.y };
        steer = vecLimit(steer, MAX_FORCE);

        mbird->velocity.x += steer.x * COHESION_WEIGHT;
        mbird->velocity.y += steer.y * COHESION_WEIGHT;
    }
}
//separation
void separation(Bird birds[MAX_BIRDS], Bird *mbird) {
    Vector2 steer = {0, 0};
    int count = 0;

    for (int i = 0; i < MAX_BIRDS; i++) {
        if (&birds[i] != mbird) {
            float dx = mbird->endPos.x - birds[i].endPos.x;
            float dy = mbird->endPos.y - birds[i].endPos.y;
            float dist = sqrtf(dx*dx + dy*dy);

            if (dist > 0 && dist < SEPARATION_DIST) {
                Vector2 diff = {dx / dist, dy / dist};
                diff.x /= dist; 
                diff.y /= dist;
                steer.x += diff.x;
                steer.y += diff.y;
                count++;
            }
        }
    }

    if (count > 0) {
        steer.x /= count;
        steer.y /= count;
    }

    if (vecLength(steer) > 0) {
        steer = vecNormalize(steer);
        steer.x *= MAX_SPEED;
        steer.y *= MAX_SPEED;

        Vector2 force = (Vector2){ steer.x - mbird->velocity.x, steer.y - mbird->velocity.y };
        force = vecLimit(force, MAX_FORCE);

        mbird->velocity.x += force.x * SEPARATION_WEIGHT;
        mbird->velocity.y += force.y * SEPARATION_WEIGHT;
    }
}
//alignment
void alignment(Bird birds[MAX_BIRDS], Bird *mbird) {
    Vector2 avgVelocity = {0, 0};
    int count = 0;

    for (int i = 0; i < MAX_BIRDS; i++) {
        if (&birds[i] != mbird) {
            float dx = birds[i].endPos.x - mbird->endPos.x;
            float dy = birds[i].endPos.y - mbird->endPos.y;
            float dist = sqrtf(dx*dx + dy*dy);
            if (dist > 0 && dist < ALIGNMENT_DIST) {
                avgVelocity.x += birds[i].velocity.x;
                avgVelocity.y += birds[i].velocity.y;
                count++;
            }
        }
    }

    if (count > 0) {
        avgVelocity.x /= count;
        avgVelocity.y /= count;

        avgVelocity = vecNormalize(avgVelocity);
        avgVelocity.x *= MAX_SPEED;
        avgVelocity.y *= MAX_SPEED;

        Vector2 steer = (Vector2){avgVelocity.x - mbird->velocity.x, avgVelocity.y - mbird->velocity.y};
        steer = vecLimit(steer, MAX_FORCE);

        mbird->velocity.x += steer.x * ALIGNMENT_WEIGHT;
        mbird->velocity.y += steer.y * ALIGNMENT_WEIGHT;
    }
}

//isosceles bird production tehehe
void isoProductions(Bird* bird) {
    Vector2 velNorm = vecNormalize(bird->velocity);
    if (vecLength(velNorm) < 0.01f) {
        return;
    }
    float rad = atan2f(velNorm.y, velNorm.x);
    float lineLength = 15.0f; // Smaller arrow size
    bird->startPos.x = bird->endPos.x + lineLength * cosf(rad);
    bird->startPos.y = bird->endPos.y + lineLength * sinf(rad);

    float ux = bird->startPos.x - bird->endPos.x;
    float uy = bird->startPos.y - bird->endPos.y;
    float len = sqrtf(ux*ux + uy*uy);

    if (len > 0.001f) {
        float scale = 5.0f / len; // Smaller wings
        bird->cPos.x = bird->endPos.x - uy * scale;
        bird->cPos.y = bird->endPos.y + ux * scale;
        bird->dPos.x = bird->endPos.x + uy * scale;
        bird->dPos.y = bird->endPos.y - ux * scale;
    }
}

void drawBirdLines(const Bird* bird) {
    DrawLineV(bird->startPos, bird->cPos, RAYWHITE);
    DrawLineV(bird->startPos, bird->dPos, RAYWHITE);
    DrawLineV(bird->cPos, bird->dPos, RAYWHITE);
}

int main() {
    InitWindow(WIDTH, HEIGHT, "CBOIDS");
    SetTargetFPS(60);

    srand(time(NULL));
    Bird birds[MAX_BIRDS] = {0};
    for (int i = 0; i < MAX_BIRDS; i++) {
        birds[i].endPos.x = (float)(rand() % WIDTH);
        birds[i].endPos.y = (float)(rand() % HEIGHT);

        //init rands velocs
        birds[i].velocity = (Vector2){
            (float)(rand() % (int)MAX_SPEED - MAX_SPEED/2),
            (float)(rand() % (int)MAX_SPEED - MAX_SPEED/2)
        };
        birds[i].velocity = vecLimit(birds[i].velocity, MAX_SPEED/2);
    }

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        for (int i = 0; i < MAX_BIRDS; i++) {
            separation(birds, &birds[i]);
            alignment(birds, &birds[i]);
            cohesion(birds, &birds[i]);
        }

        const float bounceFactor = -1.5f;

        for (int i = 0; i < MAX_BIRDS; i++) {
            birds[i].velocity = vecLimit(birds[i].velocity, MAX_SPEED);
            birds[i].endPos.x += birds[i].velocity.x * dt;
            birds[i].endPos.y += birds[i].velocity.y * dt;

            //bounce force must be stronger
            if (birds[i].endPos.x < 0) {
                birds[i].endPos.x = 0;
                birds[i].velocity.x *= bounceFactor;
            } 
            else if (birds[i].endPos.x > WIDTH) {
                birds[i].endPos.x = WIDTH;
                birds[i].velocity.x *= bounceFactor;
            }

            if (birds[i].endPos.y < 0) {
                birds[i].endPos.y = 0;
                birds[i].velocity.y *= bounceFactor;
            } 
            else if (birds[i].endPos.y > HEIGHT) {
                birds[i].endPos.y = HEIGHT;
                birds[i].velocity.y *= bounceFactor;
            }

            isoProductions(&birds[i]);
        }

        BeginDrawing();
        ClearBackground(BLACK);

        for (int i = 0; i < MAX_BIRDS; i++) {
            drawBirdLines(&birds[i]);
        }

        DrawFPS(10, 10);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
