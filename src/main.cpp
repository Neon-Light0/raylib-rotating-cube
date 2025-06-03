#include <fmt/core.h>
#include <fmt/ranges.h>
#include <raylib.h>
#include <vector>
#include <raymath.h>

const int targetFPS = 30;
int screenWidth = 800;
int screenHeight = 800;
const int fontSize = 10;
int worldWidth = screenWidth / fontSize;
int worldHeight = screenHeight / fontSize;
int worldCenterX = 0.5 * worldWidth;
int worldCenterY = 0.5 * worldHeight;
int cubeWidth = 20;

std::vector<std::vector<char>> pixelBuffer(worldHeight, std::vector<char>(worldWidth, ' '));
std::vector<std::vector<int>> zBuffer(worldHeight, std::vector<int>(worldWidth, INT_MAX));

void clearBuffers() { 
  for (int i = 0; i < worldHeight; i++){
    for (int j = 0; j < worldWidth; j++){
      pixelBuffer[i][j] = ' ';
      zBuffer[i][j] = INT_MAX;
    }
  }
}

void drawBuffer(){
  for (int i = 0; i < worldHeight; i += 1) {
    for (int j = 0; j < worldWidth; j += 1) {
      if (pixelBuffer[i][j] != ' '){
        char letter[2] = {pixelBuffer[i][j], '\0'};
        DrawText(letter, j * fontSize, i * fontSize, fontSize, WHITE);
      }
    }
  }
}

void run() {
  InitWindow(screenWidth, screenHeight, "Rotating Cube");
  SetTargetFPS(targetFPS);

  // Main loop
  while (!WindowShouldClose())  // Detect window close button or ESC key
  {
    BeginDrawing();

    ClearBackground(BLACK);
    clearBuffers();
    drawBuffer();

    EndDrawing();
  }

  CloseWindow();  // Close window and OpenGL context
}

int main(int argc, char* argv[]) {
  for (int i = 1; i < argc; ++i) {
    if (argv[i] == "--screenWidth"){
      screenWidth = std::stoi(argv[++i]);
    } else if (argv[i] == "--screenHeight"){
      screenHeight = std::stoi(argv[++i]);
    } else {
      fmt::print("Unknown argument: {}\n", argv[i]);
      return 1;
    }
  }

  run();
  return 0;
}