#include "raylib.h"
#include "imgui.h"
#include "rlImGui.h"

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1600;
    const int screenHeight = 850;

    InitWindow(screenWidth, screenHeight, "spring");

    // Initialize rlImGui
    rlImGuiSetup(true);

    // Enable docking
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    // Define the camera to look into our 3d world
    Camera3D camera = { 0 };
    camera.position = { 10.0f, 10.0f, 10.0f }; // Camera position
    camera.target = { 0.0f, 0.0f, 0.0f };      // Camera looking at point
    camera.up = { 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                       // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;    // Camera projection type

    Vector3 cubePosition = { 0.0f, 0.0f, 0.0f };
    Vector3 otherPosition = { 0.0f, 0.0f, 5.0f };

    EnableCursor();                    // Enable cursor for interaction with ImGui

    SetTargetFPS(60);                   // Set our game to run at 60 frames-per-second

    // Create a framebuffer
    RenderTexture2D target = LoadRenderTexture(screenWidth, screenHeight);

    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())        // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
            DisableCursor(); 
        }
        if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT)) {
            EnableCursor(); 
        }

        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
            UpdateCamera(&camera, CAMERA_FREE);
        }

        if (IsKeyPressed(KEY_Z)) camera.target = { 0.0f, 0.0f, 0.0f };


        BeginDrawing();
        ClearBackground(RAYWHITE);

        rlImGuiBegin();

        ImGui::DockSpace(ImGui::GetID("DockSpace"));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::SetNextWindowSizeConstraints(ImVec2(400, 400), ImVec2((float)GetScreenWidth(), (float)GetScreenHeight()));

        if (ImGui::Begin("3D Scene", nullptr, ImGuiWindowFlags_NoScrollbar))
        {
            ImVec2 availableSize = ImGui::GetContentRegionAvail();

            if (availableSize.x != target.texture.width || availableSize.y != target.texture.height)
            {
                UnloadRenderTexture(target);
                target = LoadRenderTexture((int)availableSize.x, (int)availableSize.y);
            }

            BeginTextureMode(target);
            ClearBackground(RAYWHITE);

            BeginMode3D(camera);
            
            DrawCube(otherPosition, 2.0f, 2.0f, 2.0f, BLUE);
            DrawCube(cubePosition, 2.0f, 2.0f, 2.0f, RED);

            DrawGrid(20, 1.0f);

            EndMode3D();
            EndTextureMode();

            rlImGuiImageRenderTextureFit(&target, true);
        }
        ImGui::End();
        ImGui::PopStyleVar();

        rlImGuiEnd();

        EndDrawing();
        
    }


    UnloadRenderTexture(target); 

    rlImGuiShutdown();
    CloseWindow();        
   

    return 0;
}
