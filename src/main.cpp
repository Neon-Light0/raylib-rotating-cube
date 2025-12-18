#include <fmt/core.h>
#include <fmt/ranges.h>
#include <raylib.h>
#include <vector>
#include <raymath.h>
#include <thread>
#include <array>

const int targetFPS = 30;
int screenWidth = 800;
int screenHeight = 800;
const int fontSize = 10;
int worldWidth = screenWidth / fontSize;
int worldHeight = screenHeight / fontSize;
int worldCenterX = 0.5 * worldWidth;
int worldCenterY = 0.5 * worldHeight;
int cubeWidth = 30;
std::array<int, 3> cameraPos = {0, 0, -50};
std::array<double, 3> cubeRotation = {0.0, 0.0, 0.0};

std::vector<std::vector<char>> pixelBuffer(worldHeight, std::vector<char>(worldWidth, ' '));
std::vector<std::vector<int>> zBuffer(worldHeight, std::vector<int>(worldWidth, INT_MAX));

void rotate(int x, int y, int z, float angleX, float angleY, float angleZ, int &outX, int &outY, int &outZ){
  // Rotation around X axis
  float cosX = cos(angleX);
  float sinX = sin(angleX);
  int y1 = static_cast<int>(y * cosX - z * sinX);
  int z1 = static_cast<int>(y * sinX + z * cosX);

  // Rotation around Y axis
  float cosY = cos(angleY);
  float sinY = sin(angleY);
  int x2 = static_cast<int>(x * cosY + z1 * sinY);
  int z2 = static_cast<int>(-x * sinY + z1 * cosY);

  // Rotation around Z axis
  float cosZ = cos(angleZ);
  float sinZ = sin(angleZ);
  outX = static_cast<int>(x2 * cosZ - y1 * sinZ);
  outY = static_cast<int>(x2 * sinZ + y1 * cosZ);
  outZ = z2;
}

void drawCube(){
  for (int x = -cubeWidth/2; x < cubeWidth/2; x++){
    for (int y = -cubeWidth/2; y < cubeWidth/2; y++){
      for (int z = -cubeWidth/2; z < cubeWidth/2; z++){
        z = cubeWidth/2 - 1;  // Draw only front face for simplicity
        int rotatedX, rotatedY, rotatedZ;
        rotate(x, y, z, cubeRotation[0], cubeRotation[1], cubeRotation[2], rotatedX, rotatedY, rotatedZ);
        int screenX = worldCenterX + rotatedX;
        int screenY = worldCenterY + rotatedY;
        if (screenX >= 0 && screenX < worldWidth && screenY >= 0 && screenY < worldHeight){
          pixelBuffer[screenY][screenX] = '#';
        }
      }
    }
  }
}

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
  while (!WindowShouldClose()){  // Detect window close button or ESC key

    BeginDrawing();

    ClearBackground(ORANGE);
    clearBuffers();
    cubeRotation[0] = fmod(cubeRotation[0] + 0.03,  2 * M_PI);
    cubeRotation[1] = fmod(cubeRotation[1] + 0.00,  2 * M_PI);
    cubeRotation[2] = fmod(cubeRotation[2] + 0.00,  2 * M_PI);

    drawCube();
    drawBuffer();

    std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<std::size_t>(1000/60.0)));
    EndDrawing();
  }

  CloseWindow();  // Close window and OpenGL context
}

int main(int argc, char* argv[]) {
  for (int i = 1; i < argc; ++i) {
    if (std::string(argv[i]) ==  "--screenWidth"){
      screenWidth = std::stoi(argv[++i]);
    } else if (std::string(argv[i]) == "--screenHeight"){
      screenHeight = std::stoi(argv[++i]);
    } else {
      fmt::print("Unknown argument: {}\n", argv[i]);
      return 1;
    }
  }

  run();
  return 0;
}