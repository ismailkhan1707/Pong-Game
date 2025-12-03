#include <raylib.h>
#include <iostream>

Color Green = Color{38, 185, 154, 255};
Color DarkGreen = Color{20, 160, 133, 255};
Color LightBrown = Color{176, 186, 136, 255};
Color Yellow = Color{243, 213, 91, 255};

int playerScore = 0;
int cpuScore = 0;

class Ball{
    public:
    float x,y;
    int speed_x, speed_y;
    int radius;
    
    void Draw(){
        DrawCircle(x, y, radius, Yellow);
    }
    void ResetBall(){
        x = GetScreenWidth()/2;
        y = GetScreenHeight()/2;

        int speedChoices[2] = {-1,1};
        
        speed_x *= speedChoices[GetRandomValue(0,1)];
        speed_y *= speedChoices[GetRandomValue(0,1)];
    }

    void update(){
        x += speed_x;
        y += speed_y;

        if ( y + radius >= GetScreenHeight() || y - radius <= 0 ){
            speed_y *= -1;
        }
        if ( x + radius >= GetScreenWidth()){
            cpuScore++;
            ResetBall();
        }
        if ( x - radius <= 0 ){
            playerScore++;
            ResetBall();
        }
    }    
};

class Paddle{
    protected:
    void LimitMovement(){
        if ( y <= 0 ){
            y = 0;
        }
        if ( y + height >= GetScreenHeight() ){
            y = GetScreenHeight() - height;
        }
    }
    public:
    float x,y;
    float width, height;
    int speed;

    void Draw(){
        DrawRectangleRounded( Rectangle{x, y, width, height}, 0.8, 0, WHITE);
    }

    void Update(){
        if ( IsKeyDown(KEY_UP)){
            y = y - speed;
        }
        if ( IsKeyDown(KEY_DOWN)){
            y = y + speed;
        }
    LimitMovement();
        
    }
};
class CpuPaddle : public Paddle{
    public:

    void Update( int ball_y ){
        if ( y + height/2 > ball_y ){
            y = y - speed;
        }
        if ( y + height/2 < ball_y ){
            y = y + speed;
        }
    LimitMovement();
    }
};
Ball ball;
Paddle player;
CpuPaddle cpu;

int main(){
    //SetConfigFlags(FLAG_WINDOW_RESIZABLE); 
    const int width = 1280;
    const int height = 680;

    InitWindow(width, height, "Pong");
    SetTargetFPS(60);

    ball.radius = 20;
    ball.x = width /2;
    ball.y = height/2;
    ball.speed_x = 7;
    ball.speed_y = 7;

    player.width = 25;
    player.height = 100;
    player.x = width - player.width - 10;
    player.y = height/2 - player.height/2;
    player.speed = 6;
    
    cpu.width = 25;
    cpu.height = 100;
    cpu.x = 10;
    cpu.y = height/2 - cpu.height/2;
    cpu.speed = 6;
    
    // Game loop
    while( WindowShouldClose() == false ){
        BeginDrawing();
        //Event handling
        
        //updating postions
        ball.update();
        player.Update();
        cpu.Update(ball.y);

        if (CheckCollisionCircleRec( Vector2{ball.x, ball.y}, ball.radius, Rectangle{player.x, player.y, player.width, player.height })){
            ball.speed_x *= -1;
        }
        if (CheckCollisionCircleRec( Vector2{ball.x, ball.y}, ball.radius, Rectangle{cpu.x, cpu.y, cpu.width, cpu.height})){
            ball.speed_x *= -1;
        }
        //drawing
        if (IsKeyPressed(KEY_F)) SetWindowState(FLAG_FULLSCREEN_MODE);
        ClearBackground(DarkGreen);
        DrawRectangle( width/2, 0, width/2, height ,Green);
        DrawCircle(width/2, height/2, 150, LightBrown);
        DrawLine(width/2, 0, width/2, height, WHITE);
        ball.Draw();
        player.Draw();
        cpu.Draw();
        
        DrawText(TextFormat("%i",cpuScore), width/4 -20, 20, 80,  WHITE);
        DrawText(TextFormat("%i",playerScore), 3* width/4 -20, 20, 80,  WHITE);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}