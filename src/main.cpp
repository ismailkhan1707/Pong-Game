#include <raylib.h>
#include <iostream>

Color Navy  = Color{15, 20, 30, 255};
Color Gray  = Color{45, 50, 65, 255};
Color Volt  = Color{50, 255, 130, 255};
Color White = Color{255, 255, 255, 255};

int playerScore = 0;
int cpuScore = 0;
int winner = 0;

enum GameState {
    PLAYING,
    PAUSED,
    GAME_OVER
};

GameState state = PAUSED;

enum GameMode {
    VS_CPU,
    VS_Player

};

GameMode mode = VS_Player;

class Ball{
    public:
    float x,y;
    int speed_x, speed_y;
    int radius;
    
    void Draw(){
        DrawCircle(x, y, radius, White);
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
        DrawRectangleRounded( Rectangle{x, y, width, height}, 0.8, 0, Volt);
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
        
     void UpdatePlayer2() {
        if (IsKeyDown(KEY_W)) {
            y -= speed;
        }            
        if (IsKeyDown(KEY_S)) {
               y += speed;
        }
        LimitMovement();
        }
};
Ball ball;
Paddle player;
CpuPaddle cpu;

void DrawPauseScreen(int screenwidth, int screenheight){
    DrawRectangle(0, 0, screenwidth, screenheight, Color{0,0,0,150});

    DrawText("PAUSED", screenwidth/2 , screenheight/2 - 140, 50, WHITE);

    DrawText("Press P to Resume",
             screenwidth/2 , screenheight/2 - 60, 30, WHITE);

    DrawText("Press 1 : VS CPU",
             screenwidth/2 , screenheight/2, 30,
             mode == VS_CPU ? Volt : WHITE);

    DrawText("Press 2 : VS Player",
             screenwidth/2 , screenheight/2 + 50, 30,
             mode == VS_Player ? Volt : WHITE);

    DrawText("Press ESC to Exit",
             screenwidth/2 , screenheight/2 + 120, 30, WHITE);
}

void DrawGameOverScreen(int screenwidth, int screenheight){
    DrawRectangle(0, 0, screenwidth, screenheight, Color{0,0,0,180});

    if (winner == 1){
        DrawText("RIGHT PLAYER WINS",
                 screenwidth/2 , screenheight/2 - 40, 50, Volt);
    } else {
        DrawText("LEFT PLAYER WINS",
                 screenwidth/2 , screenheight/2 - 40, 50, Volt);
    }

    DrawText("Press P to Restart",
             screenwidth/2 , screenheight/2 + 40, 30, WHITE);

    DrawText("Press ESC to Exit",
             screenwidth/2 , screenheight/2 + 90, 30, WHITE);
}



void ResetGame() {
    playerScore = 0;
    cpuScore = 0;
    ball.ResetBall();
}

int main(){
    const int screenwidth = 1280;
    const int screenheight = 720;

    InitWindow(screenwidth, screenheight, "Pong");
    SetWindowState(FLAG_FULLSCREEN_MODE);    
    SetTargetFPS(60);

    ball.radius = 20;
    ball.x = screenwidth /2;
    ball.y = screenheight/2;
    ball.speed_x = 7;
    ball.speed_y = 7;

    player.width = 25;
    player.height = 100;
    player.x = screenwidth - player.width - 10;
    player.y = screenheight/2 - player.height/2;
    player.speed = 6;
    
    cpu.width = 25;
    cpu.height = 100;
    cpu.x = 10;
    cpu.y = screenheight/2 - cpu.height/2;
    cpu.speed = 6;
    
    // Game loop
    while( WindowShouldClose() == false ){
        if ( IsKeyPressed(KEY_P)){
            if (state == PLAYING){
                state = PAUSED;
            } else{
                state = PLAYING;
            }
        }
        if (state == PAUSED) {
            if (IsKeyPressed(KEY_ONE)) {
                mode = VS_CPU;
                ResetGame();
            }
            if (IsKeyPressed(KEY_TWO)) {
                mode = VS_Player;
                ResetGame();
            }
}

        if (state == GAME_OVER && IsKeyPressed(KEY_P)){
            ResetGame();
            state = PAUSED;
        }

        //Event handling
        
        //updating postions

        if (state == PLAYING){
            ball.update();
            player.Update();

            if (mode == VS_CPU ){
                cpu.Update(ball.y);
            }
            else{
                cpu.UpdatePlayer2();
            }

            if (playerScore == 7) {
                winner = 1;
                state = GAME_OVER;
            }

            if (cpuScore == 7) {
                winner = 2;
                state = GAME_OVER;
            }

            if (CheckCollisionCircleRec( Vector2{ball.x, ball.y}, ball.radius, Rectangle{player.x, player.y, player.width, player.height })){
                ball.speed_x *= -1;
            }
            if (CheckCollisionCircleRec( Vector2{ball.x, ball.y}, ball.radius, Rectangle{cpu.x, cpu.y, cpu.width, cpu.height})){
                ball.speed_x *= -1;
            }
        }
        //drawing
        BeginDrawing();

        ClearBackground(Navy);

        DrawRectangle( screenwidth/2, 0, screenwidth/2, screenheight ,Navy);
        DrawCircle(screenwidth/2, screenheight/2, 150, Gray);
        DrawLine(screenwidth/2, 0, screenwidth/2, screenheight, WHITE);
        DrawCircle(0, 0, 150, Gray);
        DrawCircle(screenwidth, 0, 150, Gray);
        DrawCircle(0, screenheight, 150, Gray);
        DrawCircle(screenwidth, screenheight, 150, Gray);

        ball.Draw();
        player.Draw();
        cpu.Draw();
        
        DrawText(TextFormat("%i",cpuScore), screenwidth/4 -20, 20, 80,  Volt);
        DrawText(TextFormat("%i",playerScore), 3* screenwidth/4 -20, 20, 80,  Volt);

        if (state == PAUSED){
            DrawPauseScreen(screenwidth, screenheight);
        }

        if (state == GAME_OVER){
            DrawGameOverScreen(screenwidth, screenheight);
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}