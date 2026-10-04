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
constexpr char GHOST[] = "assets/ghost_pic_resized.png";
constexpr char FIRE[] = "assets/fire_pic_resized.png";


Texture2D gTexture;
Texture2D gTexture_fire;

constexpr char BG_COLOUR[] = "#B2AAC6";

constexpr Vector2 ORIGIN    = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
constexpr Vector2 BASE_SIZE = { 1000.0f, 1000.0f };

Vector2 ghostPosition = {SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4 };
Vector2 gFirePosition = ORIGIN;
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
    gTexture_fire = LoadTexture(FIRE);
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
    float ghost_width = static_cast<float>(gTexture.width);    // 216
    float ghost_height = static_cast<float>(gTexture.height);   // 236

    // Rectangle textureArea = {
    //     0.0f, 0.0f,


    //     static_cast<float>(gTexture.width), // width
    //     static_cast<float>(gTexture.height) // height
    // };

    // Rectangle destinationArea = {

    //     gFirePosition.x,
    //     gFirePosition.y,


    //     static_cast<float>(gScale.x),
    //     static_cast<float>(gScale.y)
    // };

    Vector2 originOffset = {
        static_cast<float>(gScale.x) / 2.0f,
        static_cast<float>(gScale.y) / 2.0f
    };
    Rectangle ghostTextureArea = { 0.0f, 0.0f, ghost_width, ghost_height };
    Rectangle ghostDestinationArea   = { ghostPosition.x, ghostPosition.y, ghost_width, ghost_height };
    Vector2   ghostOrigin = { ghost_width / 2.0f, ghost_height / 2.0f };

    DrawTexturePro(gTexture, ghostTextureArea, ghostDestinationArea, ghostOrigin, 0.0f, WHITE);


    float fire_width = static_cast<float>(gTexture_fire.width);    // 216
    float fire_height = static_cast<float>(gTexture_fire.height);   // 236

    Rectangle fireTextureArea = { 0.0f, 0.0f, fire_width, fire_height };
    Rectangle fireDestinationArea   = { gFirePosition.x, gFirePosition.y, fire_width, fire_height };
    Vector2   fireOrigin = { fire_width / 2.0f, fire_height / 2.0f };

    DrawTexturePro(gTexture_fire, fireTextureArea, fireDestinationArea, fireOrigin, 0.0f, WHITE);
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