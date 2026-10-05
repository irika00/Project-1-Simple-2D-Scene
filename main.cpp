#include "raylib.h"
#include "CS3113/cs3113.h"
#include <math.h>


// Global Constants
constexpr int SCREEN_WIDTH  = 1200,
              SCREEN_HEIGHT = 675,
              FPS           = 60;

            

// Global Variables
AppStatus gAppStatus = RUNNING;

//Images
constexpr char GHOST[] = "assets/ghost_pic_resized.png";
constexpr char FIRE[] = "assets/new_fire.png";
constexpr char BG[] = "assets/newbg.jpg";
constexpr char PUMPKIN[] = "assets/ne1.png";

//direction
enum Direction { LEFT, RIGHT };
Direction gDirection = RIGHT; 

//for delta time
float gPreviousTicks = 0.0f;

//textures
Texture2D gTexture;
Texture2D gTexture_fire;
Texture2D gTexture_bg;
Texture2D gTexture_pumpkin;

constexpr char BG_COLOUR[] = "#B2AAC6";

constexpr Vector2 ORIGIN    = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
constexpr Vector2 BASE_SIZE = { 1000.0f, 1000.0f };
constexpr Vector2 FIRE_BASE = ORIGIN;


//postions
Vector2 ghostPosition = {SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4 };
Vector2 gFirePosition = ORIGIN;
Vector2 gBgPosition = ORIGIN;
Vector2 gScale    = BASE_SIZE;


//constexpr float LIMIT_ANGLE = 6.28319f;
constexpr float LIMIT_ANGLE = 360.0f;
constexpr float ORBIT_SPEED = 1.0f;         
constexpr float ORBIT_RADIUS_X = 400; 
constexpr float ORBIT_RADIUS_Y = 250 ; 
constexpr float  FIRE_BASE_SIZE   = 1.0f;
float gPulseScale = 1.0f;
float gPulseTime  = 0.0f;
constexpr float PULSE_AMOUNT = 0.1f; 
constexpr float PULSE_SPEED  = 6.0f; 

float gGhostAngle = 0;  
float fireAngle = 0;
float pumpkinAngle = 0;
constexpr float PUMPKIN_SPEED = 120.0f; 
constexpr float FLICKER_X_AMOUNT = 15.0f;
constexpr float FLICKER_Y_AMOUNT = 10.0f;
constexpr float FLICKER_SPEED    = 10.0f;
constexpr float PUMPKIN_MOVE_SPEED = 400.0f; 



//for pumpkin movement
constexpr Vector2 POINTS[4] = {
    { SCREEN_WIDTH / 2.0f,  50.0f                 },   // top
    { SCREEN_WIDTH - 50.0f, SCREEN_HEIGHT - 300.0f },   // right
    { SCREEN_WIDTH / 2.0f,  SCREEN_HEIGHT - 50.0f  },   // bottom
    { 50.0f,                SCREEN_HEIGHT - 300.0f }    // left
};


int   gPoint = 0;      
float gPointTime  = 0.0f; 
Vector2 pumpkinPosition = POINTS[0];

// Function Declarations
void initialise();
void processInput();
void update();
void render();
void shutdown();

// Function Definitions
void initialise()
{
    
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Happy October!");
    gTexture = LoadTexture(GHOST);
    gTexture_fire = LoadTexture(FIRE);
    gTexture_bg = LoadTexture(BG);
    gTexture_pumpkin = LoadTexture(PUMPKIN);
    SetTargetFPS(FPS);
}

void processInput()
{
    if (WindowShouldClose()) gAppStatus = TERMINATED;
}

void update()
{
    //delta time
    float ticks = static_cast<float>(GetTime()); 
    float deltaTime = ticks - gPreviousTicks; 
    gPreviousTicks = ticks;                   

    //ghost movement
    gGhostAngle += ORBIT_SPEED * deltaTime;
    if (gGhostAngle >= LIMIT_ANGLE) gGhostAngle -= LIMIT_ANGLE;
    //ghost translation  relative to fire
    ghostPosition.x = gFirePosition.x + ORBIT_RADIUS_X * cosf(gGhostAngle); 
    ghostPosition.y = gFirePosition.y + ORBIT_RADIUS_Y * sinf(gGhostAngle);

    
    //pumpkin movement (rotation)
    pumpkinAngle += PUMPKIN_SPEED * deltaTime; 
    if (pumpkinAngle >= LIMIT_ANGLE) pumpkinAngle -= LIMIT_ANGLE;


    //pumpkin movement translation
    gPointTime += deltaTime;

    Vector2 target = POINTS[gPoint];

    // distance from target
    float distance_remaining_x = target.x - pumpkinPosition.x;
    float distance_remaining_y = target.y - pumpkinPosition.y;

    // straight-line distance
    float distance = sqrtf(distance_remaining_x * distance_remaining_x + distance_remaining_y * distance_remaining_y);

    if (distance < 5.0f)
    {
        gPoint = (gPoint + 1) % 4;      
    }
    else
    {
        pumpkinPosition.x += (distance_remaining_x / distance) * PUMPKIN_MOVE_SPEED * deltaTime;
        pumpkinPosition.y += (distance_remaining_y / distance) * PUMPKIN_MOVE_SPEED * deltaTime;
    }

    
    //fire flickering translation
    gPulseTime += deltaTime;
    gPulseScale = FIRE_BASE_SIZE + PULSE_AMOUNT * sinf(gPulseTime * PULSE_SPEED);
    gFirePosition.x = FIRE_BASE.x + FLICKER_X_AMOUNT * sinf(deltaTime * FLICKER_SPEED);
    gFirePosition.y = FIRE_BASE.y + FLICKER_Y_AMOUNT * sinf(deltaTime * FLICKER_SPEED*100);


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



    float ghost_width = static_cast<float>(gTexture.width);    
    float ghost_height = static_cast<float>(gTexture.height);  

    Rectangle ghostTextureArea = { 0.0f, 0.0f, ghost_width, ghost_height };
    Rectangle ghostDestinationArea   = { ghostPosition.x, ghostPosition.y, ghost_width, ghost_height };
    Vector2   ghostOrigin = { ghost_width / 2.0f, ghost_height / 2.0f };
    DrawTexturePro(gTexture, ghostTextureArea, ghostDestinationArea, ghostOrigin, 0.0f, WHITE);


    float fire_width = static_cast<float>(gTexture_fire.width)*gPulseScale;    
    float fire_height = static_cast<float>(gTexture_fire.height)*gPulseScale;  

    Rectangle fireTextureArea = { 0.0f, 0.0f, static_cast<float>(gTexture_fire.width), static_cast<float>(gTexture_fire.height) };
    Rectangle fireDestinationArea   = { gFirePosition.x, gFirePosition.y, fire_width, fire_height };
    Vector2   fireOrigin = { fire_width / 2.0f, fire_height / 2.0f };

    DrawTexturePro(gTexture_fire, fireTextureArea, fireDestinationArea, fireOrigin, fireAngle, WHITE);



    float pumpkin_width = static_cast<float>(gTexture_pumpkin.width);    
    float pumpkin_height = static_cast<float>(gTexture_pumpkin.height);
    Rectangle pumpkinTextureArea = {0.0f, 0.0f,  static_cast<float>(gTexture_pumpkin.width), static_cast<float>(gTexture_pumpkin.width)};
    Rectangle pumpkinDestinationArea = {pumpkinPosition.x, pumpkinPosition.y, pumpkin_width, pumpkin_height};
    Vector2 pumpkinOrigin = {pumpkin_width/ 2.0f, pumpkin_height / 2.0f};

    DrawTexturePro(gTexture_pumpkin, pumpkinTextureArea, pumpkinDestinationArea, pumpkinOrigin, pumpkinAngle, WHITE);

    EndDrawing();
}

void shutdown()
{
    UnloadTexture(gTexture_pumpkin);
    UnloadTexture(gTexture_bg);
    UnloadTexture(gTexture);
    UnloadTexture(gTexture_fire);
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