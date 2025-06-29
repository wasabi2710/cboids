#include <raylib.h>

int main() {
    InitWindow(1000,500,"CBOIDS");
    SetTargetFPS(60);

    while(!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

            

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
