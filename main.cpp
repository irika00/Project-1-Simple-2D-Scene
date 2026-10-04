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
constexpr char FIRE[] = "assets/new_fire.png";
constexpr char BG[] = "assets/newbg.jpg";


enum Direction { LEFT, RIGHT };
Direction gDirection = RIGHT; 

float gPreviousTicks = 0.0f;
Texture2D gTexture;
Texture2D gTexture_fire;
Texture2D gTexture_bg;

constexpr char BG_COLOUR[] = "#B2AAC6";

constexpr Vector2 ORIGIN    = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
constexpr Vector2 BASE_SIZE = { 1000.0f, 1000.0f };

Vector2 ghostPosition = {SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4 };
Vector2 gFirePosition = ORIGIN;
Vector2 gBgPosition = ORIGIN;
Vector2 gScale    = BASE_SIZE;


constexpr float LIMIT_ANGLE = 10.0f;

constexpr float TWO_PI_F    = 6.28318530718f;
constexpr float ORBIT_SPEED = 1.0f;         

constexpr float ORBIT_RADIUS_X = 400; //424.3f;   
constexpr float ORBIT_RADIUS_Y = 250 ; //238.6f;   

float gAngle = 0;  
float fireAngle = 0;

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
    gTexture_bg = LoadTexture(BG);
    SetTargetFPS(FPS);
}

void processInput()
{
    if (WindowShouldClose()) gAppStatus = TERMINATED;
}

void update()
{
    float ticks = static_cast<float>(GetTime()); 
    float deltaTime = ticks - gPreviousTicks; 
    gPreviousTicks = ticks;                   

    gAngle += ORBIT_SPEED * deltaTime;
    if (gAngle >= TWO_PI_F) gAngle -= TWO_PI_F;

    ghostPosition.x = gFirePosition.x + ORBIT_RADIUS_X * cosf(gAngle);
    ghostPosition.y = gFirePosition.y + ORBIT_RADIUS_Y * sinf(gAngle);

    fireAngle += ((gDirection == RIGHT) ? 1.0f : -1.0f)*deltaTime*10;
    if      (fireAngle >  LIMIT_ANGLE) gDirection = LEFT;
    else if (fireAngle < -LIMIT_ANGLE) gDirection = RIGHT;
}

void render()
{
    BeginDrawing();
    ClearBackground(ColorFromHex(BG_COLOUR));

    //background
    float bg_width = static_cast<float>(gTexture_bg.width);    
    float bg_height = static_cast<float>(gTexture_bg.height);   



    Rectangle bgTextureArea = { 0.0f, 0.0f, bg_width, bg_height };
    Rectangle bgDestinationArea   = { gBgPosition.x, gBgPosition.y, bg_width, bg_height};
    Vector2   bgOrigin = { bg_width / 2.0f, bg_height / 2.0f };
    DrawTexturePro(gTexture_bg, bgTextureArea, bgDestinationArea, bgOrigin, 0.0f, WHITE);


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


    //DrawTexturePro(gTexture_bg, )

    float ghost_width = static_cast<float>(gTexture.width);    // 216
    float ghost_height = static_cast<float>(gTexture.height);   // 236
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

    DrawTexturePro(gTexture_fire, fireTextureArea, fireDestinationArea, fireOrigin, fireAngle, WHITE);
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