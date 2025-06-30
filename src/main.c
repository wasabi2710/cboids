#include <math.h>
#include <raylib.h>
#include <stdio.h>

typedef struct {
    Vector2 startPos;
    Vector2 endPos;
} Line;

//todo
//reapply center mass rotation first
//apply rotation around y axis => mpos target rotation
int main() {
    InitWindow(1000,500,"CBOIDS");
    SetTargetFPS(60);

    Line line = {.startPos={400, 20}, .endPos={400, 200}};

    Vector2 mpos = {0};

    while(!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        mpos = GetMousePosition();
        
        //y axis rotation
        //apply rotation matrix
        //for a given radian
        //=> x' = x * cos0 - x * sin0
        //=> y' = y * sin0 + y * cos0        
        float rad = 3.14 / 4;    

        float x = line.startPos.x;
        float y = line.startPos.y;

        float xrad = x * cosf(rad) - y * sinf(rad);
        float yrad = x * sinf(rad) + y * cosf(rad);

        //finalize rendering
        DrawLine(line.startPos.x, line.startPos.y, line.endPos.x, line.endPos.y, RAYWHITE);
        DrawLine(xrad, yrad, line.endPos.x, line.endPos.y, RAYWHITE);

        //debug 
        char log[100];
        snprintf(log, sizeof(log), "MPOS.X: %f, MPOS.Y: %f\n", mpos.x, mpos.y);
        DrawText(log,20,20,20,GREEN);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
