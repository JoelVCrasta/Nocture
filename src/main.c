#include <raylib.h>
#include <rcamera.h>

#include "rlgl.h"
#include "raymath.h"

#define RLIGHTS_IMPLEMENTATION

#if defined(PLATFORM_DESKTOP)
    #define GLSL_VERSION            330
#else   // PLATFORM_ANDROID, PLATFORM_WEB
    #define GLSL_VERSION            100
#endif

#include "shaders/lights.h"
#include "world/skybox.h"
#include "world/ground.h"

int main(void)
{
    const int screenWidth = 1600;
    const int screenHeight = 900;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT | FLAG_MSAA_4X_HINT);
    InitWindow(screenWidth, screenHeight, "ARTEMIS");

    Camera camera = {0};
    camera.position = (Vector3){ -15.0f, 2.0f, 2.0f };
    camera.target = (Vector3){ 0.185f, 1.5f, -1.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 60.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    int cameraMode = CAMERA_FIRST_PERSON;

    // Model ground = Ground();
    Model skybox = SkyBox();

    // Light above camera
    Shader light = SetLights();

    // Plane below the player
    Model ground = Ground();

    DisableCursor();
    SetTargetFPS(60);

    while (!WindowShouldClose()) 
    {
        if (IsKeyPressed(KEY_F))
        {
            ToggleFullscreen();
        }

        UpdateCamera(&camera, cameraMode);
        lightShaderUpdate(camera, light);

        BeginDrawing();
            ClearBackground(BLACK);
            BeginMode3D(camera);

                BeginShaderMode(light);
                rlDisableBackfaceCulling();

                rlDisableDepthMask();
                   DrawModel(skybox, (Vector3){0,0,0},20.0f,BLACK);
                rlEnableDepthMask();

                rlEnableBackfaceCulling();
                DrawModel(ground, (Vector3){0.0f, 0.0f, 0.0f}, 1.0f, (Color){255, 255, 255, 255});

            EndMode3D();
        DrawFPS(5,5);

        EndDrawing();
    }
    UnloadModel(skybox);
    UnloadShader(skybox.materials[0].shader);
    UnloadTexture(skybox.materials[0].maps[MATERIAL_MAP_CUBEMAP].texture);


    CloseWindow();

    return 0;
}