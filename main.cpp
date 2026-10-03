#include "raylib.h"
#include "CS3113/cs3113.h"
#include <math.h>


// Global Constants
constexpr int SCREEN_WIDTH  = 1200,
              SCREEN_HEIGHT = 675,
              FPS           = 60;

// Global Variables
AppStatus gAppStatus = RUNNING;

//Ghost Image
constexpr char GHOST[] = "assets/ghost_pic.png";

Texture2D gTexture;

constexpr char BG_COLOUR[] = "#B2AAC6";

constexpr Vector2 ORIGIN    = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
constexpr Vector2 BASE_SIZE = { 1000.0f, 1000.0f };

Vector2 gPosition = ORIGIN;
Vector2 gScale    = BASE_SIZE;

// Function Declarations
void initialise();
void processInput();
void update();
void render();
void shutdown();

// Function Definitions
void initialise()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Textures");
    gTexture = LoadTexture(GHOST);
    SetTargetFPS(FPS);
}

void processInput()
{
    if (WindowShouldClose()) gAppStatus = TERMINATED;
}

void update() {}

void render()
{
   BeginDrawing();
    ClearBackground(ColorFromHex(BG_COLOUR));

    //so that the image does not strech
    float scale = fminf((float) SCREEN_WIDTH  / gTexture.width,
                    (float) SCREEN_HEIGHT / gTexture.height);
                    
    Rectangle source = { 0.0f, 0.0f, (float) gTexture.width, (float) gTexture.height };
    Rectangle dest = {
    (SCREEN_WIDTH  - gTexture.width  * scale) / 2.0f,
    (SCREEN_HEIGHT - gTexture.height * scale) / 2.0f,
    gTexture.width  * scale,
    gTexture.height * scale
    };
    Vector2 origin   = { 0.0f, 0.0f };

    DrawTexturePro(gTexture, source, dest, origin, 0.0f, WHITE);

    EndDrawing();
}

void shutdown()
{
    CloseWindow(); // Close window and OpenGL context
}

int main(void)
{
    initialise();

    while (gAppStatus == RUNNING)
    {
        processInput();
        update();
        render();
    }

    shutdown();

    return 0;
}